/* Copyright (C) 2003 Free Software Foundation, Inc.
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
#include <fcntl.h>
#include <sysdep.h>

/* Copied from sysdeps/unix/sysv/linux directory. The kernel expects 4 
   parameters for fadvise64 system call. Applies to both n32 and n64 ABI.
   This fixes posix_fadvise tests from LTP 20080131 release. */

/* This file is a modified version of the generic file to invoke the 
   system call with four parameters. Refer to posix_fadvise64.c
   for more details.

   In the mainline, the posix_fadvise() is added to 
   mips/mips64/n32/syscalls.list and posix_fadvise.c is not present. In 
   such case, the posix_fadvise.c gets generated while building glibc. 
   The posix_fadvise() returns -1 in case of error, instead the function 
   should return errno. */

int
posix_fadvise (int fd, off_t offset, off_t len, int advise)
{
#ifdef __NR_fadvise64
  INTERNAL_SYSCALL_DECL (err);
  int ret = INTERNAL_SYSCALL (fadvise64, err, 4, fd, offset, len, advise);
  if (INTERNAL_SYSCALL_ERROR_P (ret, err))
    return INTERNAL_SYSCALL_ERRNO (ret, err);
  return 0;
#else
  return ENOSYS;
#endif
}
