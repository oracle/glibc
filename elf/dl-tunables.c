/* RHEL 7-specific rudimentary tunables handling.
   Copyright (C) 2017 Free Software Foundation, Inc.
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

#include <string.h>
#include <unistd.h>
#include <ldsodefs.h>
#include <elision-conf.h>

/* RHEL 7 specific change:
   Manually process RHEL 7-specific tunable entries.  */
void
internal_function
_dl_process_tunable_env_entries (void)
{
#if HAVE_ELISION
  char **ep;
  const char *envname = { "RHEL_GLIBC_TUNABLES" };
# define TUNABLE_ELISION 0
# define TUNABLE_MAX 1
  const char *tunables[TUNABLE_MAX] = { "glibc.elision.enable=1" };

  ep = __environ;
  while (*ep != NULL)
    {
      size_t cnt = 0;

      while ((*ep)[cnt] == envname[cnt] && envname[cnt] != '\0')
        ++cnt;

      if (envname[cnt] == '\0' && (*ep)[cnt] == '=')
        {
          /* Found it.  */
          char *found;
          found = strstr (*ep, tunables[TUNABLE_ELISION]);
          /* Process TUNABLE_ELISION:
             Note: elision-conf.c applies security checks.  */
          if (found != NULL)
	    GLRO(dl_elision_enabled) = true;
          /* Continue the loop in case NAME appears again.  */
        }

      ++ep;
    }
#endif
}
