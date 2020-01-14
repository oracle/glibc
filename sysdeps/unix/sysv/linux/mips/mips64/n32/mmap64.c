/* Copyright (C) 1999,2000,2001,2002,2006 Free Software Foundation, Inc.
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
#include <sysdep.h>
#include <sys/syscall.h>

/* The generic implementation of __mmap64() returns an error if the value 
   passed in for the "offset" parameter is greater than what can fit in a
   __off_t, which for n32 is 2^32. This prevents mmap64() from being used to 
   map file offsets greater than 2^32 bytes for n32 programs. The following 
   provides a separate definition of __mmap64() call for n32 programs to 
   support full 64 bit offsets. */

void *mmap(void* addr, size_t len, int prot, int flags, int fd, 
           off64_t offset);

void *
__mmap64 (void *addr, size_t len, int prot, int flags, int fd, off64_t offset)
{
  return (void *)INLINE_SYSCALL (mmap, 6, addr, len, prot, flags, fd, offset);
}
weak_alias (__mmap64, mmap64)
