/* bug 17197: check for redundant shift character at block boundary.
   Copyright (C) 2015 Free Software Foundation, Inc.
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

#include <iconv.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static int
do_test (void)
{
  iconv_t cd = iconv_open ("IBM930", "UTF-8");
  if (cd == (iconv_t) -1)
    {
      puts ("iconv_open failed");
      return 1;
    }

  char instr1[] = "\xc2\xa6.";
  const char expstr1[4] = "\016Bj\017";
  const char expstr2[] = "K";
  char outstr[4];
  size_t inlen = sizeof (instr1);
  size_t outlen = sizeof (outstr);
  char *inptr = instr1;
  char *outptr = outstr;
  size_t r = iconv (cd, &inptr, &inlen, &outptr, &outlen);
  if (r != -1
      || errno != E2BIG
      || inlen != sizeof (instr1) - 2
      || inptr != instr1 + 2
      || outlen != 0
      || memcmp (outstr, expstr1, sizeof (expstr1)) != 0)
    {
      puts ("wrong first conversion");
      return 1;
    }

  outlen = sizeof (outstr);
  outptr = outstr;
  r = iconv (cd, &inptr, &inlen, &outptr, &outlen);
  if (r != 0
      || inlen != 0
      || outlen != sizeof (outstr) - sizeof (expstr2)
      || memcmp (outstr, expstr2, sizeof (expstr2)) != 0)
    {
      puts ("wrong second conversion");
      return 1;
    }

  if (iconv_close (cd) != 0)
    {
      puts ("iconv_close failed");
      return 1;
    }
  return 0;
}

#define TEST_FUNCTION do_test ()
#include "../test-skeleton.c"
