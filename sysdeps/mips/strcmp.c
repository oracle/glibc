/* Copyright (C) 2013 Free Software Foundation, Inc.
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
   License along with the GNU C Library.  If not, see
   <http://www.gnu.org/licenses/>.  */

#include <stdint.h>

#ifdef _LIBC
# include <string.h>
# include <memcopy.h>
# undef strcmp
#endif

#ifdef __mips64
# define ENABLE64
#endif

/* The UNALIGNED* macros returns 0 if both addresses are aligned on
   a 2, 4, or 8 byte boundry.  The DETECTNULL* macros return nonzero
   if the 2, 4, or 8 byte value has a null byte in it.  If ENABLE64
   is not set, we set UNALIGNED8 to '1' so that it always returns
   TRUE and we never try to use 8 byte operations.  */

#ifdef ENABLE64
# define UNALIGNED8(X, Y) (((uintptr_t) X & (sizeof (uint64_t) - 1)) \
			   | ((uintptr_t) Y & (sizeof (uint64_t) - 1)))
# define DETECTNULL8(X) (((X) - 0x0101010101010101) \
			 & ~((X) | 0x7f7f7f7f7f7f7f7f))
#else
# define UNALIGNED8(X, Y) (1)
# define DETECTNULL8(X) (0)
#endif


typedef struct bits8
{
  unsigned int B0:8, B1:8, B2:8, B3:8, B4:8, B5:8, B6:8, B7:8;
} bits8_t;
typedef union
{
  uint64_t v;
  bits8_t b;
} bitfields8_t;


#define UNALIGNED4(X, Y) (((uintptr_t) X & (sizeof (uint32_t) - 1)) \
			  | ((uintptr_t) Y & (sizeof (uint32_t) - 1)))
#define DETECTNULL4(X) (((X) - 0x01010101) & ~((X) | 0x7f7f7f7f))

typedef struct bits4
{
  unsigned int B0:8, B1:8, B2:8, B3:8;
} bits4_t;
typedef union
{
  uint32_t v;
  bits4_t b;
} bitfields4_t;


#define UNALIGNED2(X, Y) (((uintptr_t) X & (sizeof (uint16_t) - 1)) \
			  | ((uintptr_t) Y & (sizeof (uint16_t) - 1)))
#define DETECTNULL2(X) (((X) - 0x0101) & ~((X) | 0x7f7f))
typedef struct bits2
{
  unsigned int B0:8, B1:8;
} bits2_t;
typedef union
{
  uint16_t v;
  bits2_t b;
} bitfields2_t;


#ifndef STRCMP_NAME
# define STRCMP_NAME strcmp
#endif

int
STRCMP_NAME(const char *s1, const char *s2)
{ 
  if (__builtin_expect (UNALIGNED2 (s1, s2), 1))
    {
      /* s1 and s2 are unaligned, compare them one byte at a time.  */
      while (*s1 != '\0' && *s1 == *s2)
	{
	  s1++;
	  s2++;
	}
      return (*(unsigned char *) s1) - (*(unsigned char *) s2);
    }

  if (__builtin_expect (UNALIGNED4 (s1, s2), 1))
    {
      /* s1 and s2 are 2 byte aligned, compare them 2 bytes at a time.  */
      uint16_t *a1 = (uint16_t *) s1;
      uint16_t *a2 = (uint16_t *) s2;
      uint16_t x, y;
      bitfields2_t bx, by;
      a1 = (uint16_t *) s1;
      a2 = (uint16_t *) s2;
      x = *(a1++);
      y = *(a2++);
      while (1)
	{
	  if (x != y)
	    break;
	  if (DETECTNULL2 (x))
	    return 0;
	  x = *(a1++);
	  y = *(a2++);
	}
      bx.v = x; by.v = y;
      if (bx.b.B0 - by.b.B0 != 0 || bx.b.B0 == 0)
	return bx.b.B0 - by.b.B0;
      return bx.b.B1 - by.b.B1;
    }

  if (__builtin_expect (UNALIGNED8 (s1, s2), 1))
    {
      /* s1 and s2 are 4 byte aligned, compare them 4 bytes at a time. */
      uint32_t *a1 = (uint32_t *) s1;
      uint32_t *a2 = (uint32_t *) s2;
      uint32_t x, y;
      bitfields4_t bx, by;
      x = *(a1++);
      y = *(a2++);
      while (1)
	{
	  if (x != y)
	    break;
	  if (DETECTNULL4 (x))
	    return 0;
	  x = *(a1++);
	  y = *(a2++);
	}
      bx.v = x; by.v = y;
      if (bx.b.B0 - by.b.B0 != 0 || bx.b.B0 == 0)
	return bx.b.B0 - by.b.B0;
      if (bx.b.B1 - by.b.B1 != 0 || bx.b.B1 == 0)
	return bx.b.B1 - by.b.B1;
      if (bx.b.B2 - by.b.B2 != 0 || bx.b.B2 == 0)
	return bx.b.B2 - by.b.B2;
      return bx.b.B3 - by.b.B3;
    }

  /* s1 and s2 are 8 byte aligned, compare them 8 bytes at a time. */
  uint64_t *a1 = (uint64_t *) s1;
  uint64_t *a2 = (uint64_t *) s2;
  uint64_t x, y;
  bitfields8_t bx, by;
  x = *(a1++);
  y = *(a2++);
  while (1)
    {
      if (x != y)
	break;
      if (DETECTNULL8 (x))
	return 0;
      x = *(a1++);
      y = *(a2++);
    }
  bx.v = x; by.v = y;
  if (bx.b.B0 - by.b.B0 != 0 || bx.b.B0 == 0)
    return bx.b.B0 - by.b.B0;
  if (bx.b.B1 - by.b.B1 != 0 || bx.b.B1 == 0)
    return bx.b.B1 - by.b.B1;
  if (bx.b.B2 - by.b.B2 != 0 || bx.b.B2 == 0)
    return bx.b.B2 - by.b.B2;
  if (bx.b.B3 - by.b.B3 != 0 || bx.b.B3 == 0)
    return bx.b.B3 - by.b.B3;
  if (bx.b.B4 - by.b.B4 != 0 || bx.b.B4 == 0)
    return bx.b.B4 - by.b.B4;
  if (bx.b.B5 - by.b.B5 != 0 || bx.b.B5 == 0)
    return bx.b.B5 - by.b.B5;
  if (bx.b.B6 - by.b.B6 != 0 || bx.b.B6 == 0)
    return bx.b.B6 - by.b.B6;
  return bx.b.B7 - by.b.B7;
}
#ifdef _LIBC
libc_hidden_builtin_def (STRCMP_NAME)
#endif
