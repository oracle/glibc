/* Copyright (C) 1998, 2000, 2003, 2004, 2006 Free Software Foundation, Inc.
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
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <errno.h>
#include <unistd.h>
#include <setxid.h>

/* Arguments are passed as longs to system calls. Since the arguments of
   this function are of type "unsigned int" they get zero-extended during
   the coversion to long. The kernel does not convert them back to
   "unsigned int" i.e. does not truncate the values but assumes that they
   are already unsigned ints. By converting to "int" we avoid the
   zero-extension and thus the value remains in unsigned-int
   representation.  */
int
__setresgid (gid_t rgid, gid_t egid, gid_t sgid)
{
  return INLINE_SETXID_SYSCALL (setresgid, 3, (int) rgid,
				(int) egid, (int) sgid);
}
libc_hidden_def (__setresgid)
#ifndef __setresgid
weak_alias (__setresgid, setresgid)
#endif
