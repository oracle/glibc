/* Copyright (C) 2003-2012 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License as
   published by the Free Software Foundation; either version 2.1 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If
   not, see <http://www.gnu.org/licenses/>.  */

#include <dlfcn.h>
#include <stdio.h>
#include <unwind.h>
#include <pthreadP.h>
#include <sysdep.h>
#include <gnu/lib-names.h>

static void *libgcc_s_handle;
static void (*libgcc_s_resume) (struct _Unwind_Exception *exc);
static _Unwind_Reason_Code (*libgcc_s_personality)
  (int, _Unwind_Action, _Unwind_Exception_Class, struct _Unwind_Exception *,
   struct _Unwind_Context *);

static void
init (void)
{
  void *resume, *personality;
  void *handle;

  handle = __libc_dlopen (LIBGCC_S_SO);

  if (handle == NULL
      || (resume = __libc_dlsym (handle, "_Unwind_Resume")) == NULL
      || (personality = __libc_dlsym (handle, "__gcc_personality_v0")) == NULL)
    __libc_fatal (LIBGCC_S_SO " must be installed for pthread_cancel to work\n");

  libgcc_s_resume = resume;
  libgcc_s_personality = personality;
  atomic_write_barrier ();
  /* At the point at which any thread writes the handle
     to libgcc_s_handle, the initialization is complete.
     The writing of libgcc_s_handle is atomic. All other
     threads reading libgcc_s_handle do so atomically. Any
     thread that does not execute this function must issue
     a read barrier to ensure that all of the above has
     actually completed and that the values of the
     function pointers are correct.   */
  libgcc_s_handle = handle;
}

static __always_inline void
_maybe_init (void)
{
  if (__builtin_expect (libgcc_s_handle == NULL, 0))
    init ();
  else
    atomic_read_barrier ();
}


void
_Unwind_Resume (struct _Unwind_Exception *exc)
{
  _maybe_init ();
  libgcc_s_resume (exc);
}

_Unwind_Reason_Code
__gcc_personality_v0 (int version, _Unwind_Action actions,
		      _Unwind_Exception_Class exception_class,
                      struct _Unwind_Exception *ue_header,
                      struct _Unwind_Context *context)
{
  _maybe_init ();
  return libgcc_s_personality (version, actions, exception_class,
			       ue_header, context);
}
