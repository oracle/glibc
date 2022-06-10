/* strcmp.c: Optimized for Octeon. derived from both the generic version in 
 * newlib/libc/string/strcmp.c and the mips version in 
 * newlib/libc/machine/mips/strcmp.c.
 *
 * Copyright (c) 2004, 2005, 2006 Cavium Networks.
 *
 * The authors hereby grant permission to use, copy, modify, distribute,
 * and license this software and its documentation for any purpose, provided
 * that existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions. No written agreement,
 * license, or royalty fee is required for any of the authorized uses.
 * Modifications to this software may be copyrighted by their authors
 * and need not follow the licensing terms described here, provided that
 * the new terms are clearly indicated on the first page of each file where
 * they apply.  */

/* File version info: $Id: strcmp.c 58646 2011-03-31 20:00:02Z apinski $  */

#include <string.h>
#include <limits.h>

/* Nonzero if either X or Y is not aligned on a "long" boundary.  */
#define UNALIGNED(X, Y, type) \
  (((size_t)X & (sizeof (type) - 1)) | ((size_t)Y & (sizeof (type) - 1)))


#define DETECTNULL_LL(X) (((X) - 0x0101010101010101LL) & ~(X) & 0x8080808080808080LL)
#define DETECTNULL_I(X) (((X) - 0x01010101) & ~(X) & 0x80808080)
#define DETECTNULL_H(X) (((X) - 0x0101) & ~(X) & 0x8080)

#ifndef STRCMP_NAME
# define STRCMP_NAME strcmp
#endif


#undef strcmp
int STRCMP_NAME(const char *s1, const char *s2)
{ 
    /* If s1 or s2 are unaligned, then compare bytes. */
    if (!UNALIGNED (s1, s2, long long)) {

        unsigned long long *a1;
        unsigned long long *a2;

        /* If s1 and s2 are double word-aligned, compare them a double word at a time. */
        a1 = (unsigned long long*)s1;
        a2 = (unsigned long long*)s2;
        while (*a1 == *a2)
        {
            /* To get here, *a1 == *a2, thus if we find a null in *a1,
               then the strings must be equal, so return zero.  */
            if (DETECTNULL_LL (*a1))
                return 0;

            a1++;
            a2++;
        }

        /* A difference was detected in last few bytes of s1, so search bytewise */
        s1 = (char*)a1;
        s2 = (char*)a2;
    }
    /* If s1 or s2 are unaligned, then compare bytes. */
    else if (!UNALIGNED (s1, s2, int)) {

        unsigned int *a1;
        unsigned int *a2;

        /* If s1 and s2 are double word-aligned, compare them a double word at a time. */
        a1 = (unsigned int*)s1;
        a2 = (unsigned int*)s2;
        while (*a1 == *a2)
        {
            /* To get here, *a1 == *a2, thus if we find a null in *a1,
               then the strings must be equal, so return zero.  */
            if (DETECTNULL_I (*a1))
                return 0;

            a1++;
            a2++;
        }

        /* A difference was detected in last few bytes of s1, so search bytewise */
        s1 = (char*)a1;
        s2 = (char*)a2;
    }
    /* If s1 or s2 are unaligned, then compare bytes. */
    else if (!UNALIGNED (s1, s2, short)) {

        unsigned short *a1;
        unsigned short *a2;

        /* If s1 and s2 are double word-aligned, compare them a double word at a time. */
        a1 = (unsigned short*)s1;
        a2 = (unsigned short*)s2;
        while (*a1 == *a2)
        {
            /* To get here, *a1 == *a2, thus if we find a null in *a1,
               then the strings must be equal, so return zero.  */
            if (DETECTNULL_H (*a1))
                return 0;

            a1++;
            a2++;
        }

        /* A difference was detected in last few bytes of s1, so search bytewise */
        s1 = (char*)a1;
        s2 = (char*)a2;
    }
    while (*s1 != '\0' && *s1 == *s2)
    {
      s1++;
      s2++;
    }
  return (*(unsigned char *) s1) - (*(unsigned char *) s2); 
}
#ifdef _LIBC
libc_hidden_builtin_def (STRCMP_NAME)
#endif
