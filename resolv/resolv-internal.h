/* libresolv interfaces for internal use across glibc.
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

#ifndef _RESOLV_INTERNAL_H
#define _RESOLV_INTERNAL_H 1

#include <resolv.h>
#include <stdbool.h>

/* Internal version of RES_USE_INET6 which does not trigger a
   deprecation warning.  */
#define DEPRECATED_RES_USE_INET6 0x00002000

static inline bool
res_use_inet6 (void)
{
  return _res.options & DEPRECATED_RES_USE_INET6;
}

/* Convert from presentation format (which usually means ASCII
   printable) to network format (which is usually some kind of binary
   format).  The input is in the range [SRC, SRC + SRCLEN).  The
   output is written to DST (which has to be 4 or 16 bytes long,
   depending on AF).  Return 0 for invalid input, 1 for success, -1
   for an invalid address family.  */
int __inet_pton_length (int af, const char *src, size_t srclen, void *);
libc_hidden_proto (__inet_pton_length)

#endif  /* _RESOLV_INTERNAL_H */
