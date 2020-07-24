/* Verify that default stack size gets set correctly from the environment
   variable.

   Copyright (C) 2013 Free Software Foundation, Inc.
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

#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define RETURN_IF_FAIL(f, ...) \
  ({									      \
    int ret = f (__VA_ARGS__);						      \
    if (ret != 0)							      \
      {									      \
	printf ("%s:%d: %s returned %d (errno = %d)\n", __FILE__, __LINE__,   \
		#f, ret, errno);					      \
	return ret;							      \
      }									      \
  })

/* DEFAULT_STACKSIZE macro is defined in the Makefile.  */
static size_t stacksize = DEFAULT_STACKSIZE;
long int pagesize;

static int
verify_stacksize_result (pthread_attr_t *attr)
{
  size_t stack;

  RETURN_IF_FAIL (pthread_attr_getstacksize, attr, &stack);

  /* pthread_create perturbs the stack size by a page if it aligns to 64K to
     avoid the 64K aliasing conflict.  We cannot simply add 4K to the size in
     the Makefile because it breaks the test on powerpc since the page size
     there is 64K, resulting in a resize in __pthread_initialize_minimal.
     Hence, our check is to ensure that the stack size is not more than a page
     more than the requested size.  */
  if (stack < stacksize || stack > stacksize + pagesize)
    {
      printf ("failed to set default stacksize (%zu, %zu)\n", stacksize, stack);
      return 1;
    }

  printf ("Requested %zu and got %zu\n", stacksize, stack);

  return 0;
}

static void *
thr (void *unused __attribute__ ((unused)))
{
  pthread_attr_t attr;
  int ret;

  memset (&attr, 0xab, sizeof attr);
  /* To verify that the attributes actually got applied.  */
  if ((ret = pthread_getattr_np (pthread_self (), &attr)) != 0)
    {
      printf ("pthread_getattr_np failed: %s\n", strerror (ret));
      goto out;
    }

  ret = verify_stacksize_result (&attr);

out:
  return (void *) (uintptr_t) ret;
}

static int
run_threads (void)
{
  pthread_t t;
  void *tret = NULL;

  /* Run twice to ensure that the attributes do not get overwritten in the
     first run somehow.  */
  for (int i = 0; i < 2; i++)
    {
      RETURN_IF_FAIL (pthread_create, &t, NULL, thr, NULL);
      RETURN_IF_FAIL (pthread_join, t, &tret);

      if (tret != NULL)
	{
	  puts ("Thread failed");
	  return 1;
	}
    }

  return 0;
}

static int
do_test (void)
{
  pagesize = sysconf (_SC_PAGESIZE);
  if (pagesize < 0)
    {
      printf ("sysconf failed: %s\n", strerror (errno));
      return 1;
    }

  RETURN_IF_FAIL (run_threads);
  return 0;
}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
