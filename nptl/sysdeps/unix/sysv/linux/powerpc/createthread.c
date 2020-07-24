/* Copyright (C) 2003 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Paul Mackerras <paulus@au.ibm.com>.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

/* RHEL 7-specific changes: The functions PREPARE_CREATE and TLS_VALUE
   are used by createthread.c to override thread setup.  In upstream
   they appear in TLS_DEFINE_INIT_TP.  */
# define PREPARE_CREATE \
  void *tp = (void *) (pd) + TLS_TCB_OFFSET + TLS_PRE_TCB_SIZE;            \
  (((tcbhead_t *) ((char *) tp - TLS_TCB_OFFSET))[-1].tm_capable) =        \
  THREAD_GET_TM_CAPABLE ();

/* Value passed to 'clone' for initialization of the thread register.  */
# define TLS_VALUE tp

/* Get the real implementation.	 */
#include <nptl/sysdeps/pthread/createthread.c>
