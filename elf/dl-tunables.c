/* The tunable framework.  See the README.tunables to know how to use the
   tunable in a glibc module.

   Copyright (C) 2016 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <libc-internal.h>
#include <sysdep.h>
#include <fcntl.h>
#include <ldsodefs.h>

#define TUNABLES_INTERNAL 1
#include "dl-tunables.h"

#define GLIBC_TUNABLES "GLIBC_TUNABLES"

/* Compare environment or tunable names, bounded by the name hardcoded in
   glibc.  */
static bool
is_name (const char *orig, const char *envname)
{
  for (;*orig != '\0' && *envname != '\0'; envname++, orig++)
    if (*orig != *envname)
      break;

  /* The ENVNAME is immediately followed by a value.  */
  if (*orig == '\0' && *envname == '=')
    return true;
  else
    return false;
}

static char *
tunables_strdup (const char *in)
{
  size_t i = 0;

  while (in[i++] != '\0');
  char *out = __sbrk (i);

  /* FIXME: In reality if the allocation fails, __sbrk will crash attempting to
     set the thread-local errno since the TCB has not yet been set up.  This
     needs to be fixed with an __sbrk implementation that does not set
     errno.  */
  if (out == (void *)-1)
    return NULL;

  i--;

  while (i-- > 0)
    out[i] = in[i];

  return out;
}

static char **
get_next_env (char **envp, char **name, size_t *namelen, char **val)
{
  while (envp != NULL && *envp != NULL)
    {
      char *envline = *envp;
      int len = 0;

      while (envline[len] != '\0' && envline[len] != '=')
	len++;

      /* Just the name and no value, go to the next one.  */
      if (envline[len] == '\0')
	continue;

      *name = envline;
      *namelen = len;
      *val = &envline[len + 1];

      return ++envp;
    }

  return NULL;
}

static int
tunables_unsetenv (char **ep, const char *name)
{
  while (*ep != NULL)
    {
      size_t cnt = 0;

      while ((*ep)[cnt] == name[cnt] && name[cnt] != '\0')
	++cnt;

      if (name[cnt] == '\0' && (*ep)[cnt] == '=')
	{
	  /* Found it.  Remove this pointer by moving later ones to
	     the front.  */
	  char **dp = ep;

	  do
	    dp[0] = dp[1];
	  while (*dp++);
	  /* Continue the loop in case NAME appears again.  */
	}
      else
	++ep;
    }

  return 0;
}

/* A stripped down strtoul-like implementation for very early use.  It does not
   set errno if the result is outside bounds because it gets called before
   errno may have been set up.  */
static unsigned long int
tunables_strtoul (const char *nptr)
{
  unsigned long int result = 0;
  long int sign = 1;
  unsigned max_digit;

  while (*nptr == ' ' || *nptr == '\t')
    ++nptr;

  if (*nptr == '-')
    {
      sign = -1;
      ++nptr;
    }
  else if (*nptr == '+')
    ++nptr;

  if (*nptr < '0' || *nptr > '9')
    return 0UL;

  int base = 10;
  max_digit = 9;
  if (*nptr == '0')
    {
      if (nptr[1] == 'x' || nptr[1] == 'X')
	{
	  base = 16;
	  nptr += 2;
	}
      else
	{
	  base = 8;
	  max_digit = 7;
	}
    }

  while (1)
    {
      unsigned long int digval;
      if (*nptr >= '0' && *nptr <= '0' + max_digit)
        digval = *nptr - '0';
      else if (base == 16)
        {
	  if (*nptr >= 'a' && *nptr <= 'f')
	    digval = *nptr - 'a' + 10;
	  else if (*nptr >= 'A' && *nptr <= 'F')
	    digval = *nptr - 'A' + 10;
	  else
	    break;
	}
      else
        break;

      if (result > ULONG_MAX / base
	  || (result == ULONG_MAX / base && digval > ULONG_MAX % base))
	return ULONG_MAX;
      result *= base;
      result += digval;
      ++nptr;
    }

  return result * sign;
}

/* Initialize the internal type if the value validates either using the
   explicit constraints of the tunable or with the implicit constraints of its
   type.  */
static void
tunable_set_val_if_valid_range (tunable_t *cur, const char *strval,
				int64_t default_min, int64_t default_max)
{
  int64_t val = tunables_strtoul (strval);

  int64_t min = cur->type.min;
  int64_t max = cur->type.max;

  if (min == max)
    {
      min = default_min;
      max = default_max;
    }

  if (val >= min && val <= max)
    {
      cur->val.numval = val;
      cur->strval = strval;
    }
}

/* Validate range of the input value and initialize the tunable CUR if it looks
   good.  */
static void
tunable_initialize (tunable_t *cur, const char *strval)
{
  switch (cur->type.type_code)
    {
    case TUNABLE_TYPE_INT_32:
	{
	  tunable_set_val_if_valid_range (cur, strval, INT32_MIN, INT32_MAX);
	  break;
	}
    case TUNABLE_TYPE_SIZE_T:
	{
	  tunable_set_val_if_valid_range (cur, strval, 0, SIZE_MAX);
	  break;
	}
    case TUNABLE_TYPE_STRING:
	{
	  cur->val.strval = cur->strval = strval;
	  break;
	}
    default:
      __builtin_unreachable ();
    }
}

static void
parse_tunables (char *tunestr)
{
  if (tunestr == NULL || *tunestr == '\0')
    return;

  char *p = tunestr;

  while (true)
    {
      char *name = p;
      size_t len = 0;

      /* First, find where the name ends.  */
      while (p[len] != '=' && p[len] != ':' && p[len] != '\0')
	len++;

      /* If we reach the end of the string before getting a valid name-value
	 pair, bail out.  */
      if (p[len] == '\0')
	return;

      /* We did not find a valid name-value pair before encountering the
	 colon.  */
      if (p[len]== ':')
	{
	  p += len + 1;
	  continue;
	}

      p += len + 1;

      char *value = p;
      len = 0;

      while (p[len] != ':' && p[len] != '\0')
	len++;

      char end = p[len];
      p[len] = '\0';

      /* Add the tunable if it exists.  */
      for (size_t i = 0; i < sizeof (tunable_list) / sizeof (tunable_t); i++)
	{
	  tunable_t *cur = &tunable_list[i];

	  /* If we are in a secure context (AT_SECURE) then ignore the tunable
	     unless it is explicitly marked as secure.  Tunable values take
	     precendence over their envvar aliases.  */
	  if (__libc_enable_secure && !cur->is_secure)
	    continue;

	  if (is_name (cur->name, name))
	    {
	      tunable_initialize (cur, value);
	      break;
	    }
	}

      if (end == ':')
	p += len + 1;
      else
	return;
    }
}

static size_t
min_strlen (const char *s)
{
  size_t i = 0;
  while (*s++ != '\0')
    i++;

  return i;
}

/* Disable a tunable if it is set.  */
static void
disable_tunable (tunable_id_t id, char **envp)
{
  const char *env_alias = tunable_list[id].env_alias;

  if (env_alias != NULL)
    tunables_unsetenv (envp, tunable_list[id].env_alias);

  char *tunable = getenv (GLIBC_TUNABLES);
  const char *cmp = tunable_list[id].name;
  const size_t len = min_strlen (cmp);

  while (tunable && *tunable != '\0' && *tunable != ':')
    {
      if (is_name (tunable, cmp))
	{
	  tunable += len;
	  /* Overwrite the = and the value with colons.  */
	  while (*tunable != '\0' && *tunable != ':')
	    *tunable++ = ':';
	  break;
	}
      tunable++;
    }
}

/* Disable the glibc.malloc.check tunable in SETUID/SETGID programs unless
   the system administrator overrides it by creating the /etc/suid-debug
   file.  This is a special case where we want to conditionally enable/disable
   a tunable even for setuid binaries.  We use the special version of access()
   to avoid setting ERRNO, which is a TLS variable since TLS has not yet been
   set up.  */
static inline void
__always_inline
maybe_disable_malloc_check (void)
{
  if (__libc_enable_secure && __access_noerrno ("/etc/suid-debug", F_OK) != 0)
    disable_tunable (TUNABLE_ENUM_NAME(glibc, malloc, check), __environ);
}

/* Initialize the tunables list from the environment.  For now we only use the
   ENV_ALIAS to find values.  Later we will also use the tunable names to find
   values.  */
void
__tunables_init (char **envp)
{
  char *envname = NULL;
  char *envval = NULL;
  size_t len = 0;

  maybe_disable_malloc_check ();

  while ((envp = get_next_env (envp, &envname, &len, &envval)) != NULL)
    {
      if (is_name (GLIBC_TUNABLES, envname))
	{
	  char *val = tunables_strdup (envval);
	  if (val != NULL)
	    parse_tunables (val);
	  continue;
	}

      for (int i = 0; i < sizeof (tunable_list) / sizeof (tunable_t); i++)
	{
	  tunable_t *cur = &tunable_list[i];

	  /* Skip over tunables that have either been set already or should be
	     skipped.  */
	  if (cur->strval != NULL || cur->env_alias == NULL
	      || (__libc_enable_secure && !cur->is_secure))
	    continue;

	  const char *name = cur->env_alias;

	  /* We have a match.  Initialize and move on to the next line.  */
	  if (is_name (name, envname))
	    {
	      tunable_initialize (cur, envval);
	      break;
	    }
	}
    }
}

/* Set the tunable value.  This is called by the module that the tunable exists
   in. */
void
__tunable_set_val (tunable_id_t id, void *valp, tunable_callback_t callback)
{
  tunable_t *cur = &tunable_list[id];

  /* Don't do anything if our tunable was not set during initialization or if
     it failed validation.  */
  if (cur->strval == NULL)
    return;

  if (valp == NULL)
    goto cb;

  switch (cur->type.type_code)
    {
    case TUNABLE_TYPE_INT_32:
	{
	  *((int32_t *) valp) = (int32_t) cur->val.numval;
	  break;
	}
    case TUNABLE_TYPE_SIZE_T:
	{
	  *((size_t *) valp) = (size_t) cur->val.numval;
	  break;
	}
    case TUNABLE_TYPE_STRING:
	{
	  *((const char **)valp) = cur->val.strval;
	  break;
	}
    default:
      __builtin_unreachable ();
    }

cb:
  if (callback)
    callback (&cur->val);
}
