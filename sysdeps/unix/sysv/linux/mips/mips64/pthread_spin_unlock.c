/* pthread_spin_unlock -- unlock a spin lock.  MIPS64 version.
   Copyright (C) 2008 Free Software Foundation, Inc.
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

#include "pthreadP.h"
#include <atomic.h>

/* Add SYNCW for Octeon to flush out unlock in order to shorten hold-time.  */

int
pthread_spin_unlock (pthread_spinlock_t *lock)
{
  atomic_full_barrier ();
  *lock = 0;
#ifdef __OCTEON__
  asm (MIPS_SYNC_STR ::: "memory");
#endif
  return 0;
}
