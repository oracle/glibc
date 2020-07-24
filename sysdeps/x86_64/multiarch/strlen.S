/* Multiple versions of strlen(str) -- determine the length of the string STR.
   All versions must be listed in ifunc-impl-list.c.
   Copyright (C) 2009-2012 Free Software Foundation, Inc.
   Contributed by Ulrich Drepper <drepper@redhat.com>.
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

#include <sysdep.h>
#include <init-arch.h>


/* Define multiple versions only for the definition in libc and for
   the DSO.  In static binaries we need strlen before the initialization
   happened.  */
#if defined SHARED && IS_IN (libc)
	.text
ENTRY(strlen)
	.type	strlen, @gnu_indirect_function
	LOAD_RTLD_GLOBAL_RO_RDX
	leaq	__strlen_sse2_pminub(%rip), %rax
	HAS_ARCH_FEATURE (Prefer_PMINUB_for_stringop)
	jnz	2f
	leaq	__strlen_sse2(%rip), %rax
	HAS_CPU_FEATURE (SSE4_2)
	jz	2f
	leaq	__strlen_sse42(%rip), %rax
	ret
2:	HAS_ARCH_FEATURE (Slow_BSF)
	jz	3f
	leaq    __strlen_sse2_no_bsf(%rip), %rax
3:	ret
END(strlen)

# undef ENTRY
# define ENTRY(name) \
	.type __strlen_sse2, @function; \
	.align 16; \
	.globl __strlen_sse2; \
	.hidden __strlen_sse2; \
	__strlen_sse2: cfi_startproc; \
	CALL_MCOUNT
# undef END
# define END(name) \
	cfi_endproc; .size __strlen_sse2, .-__strlen_sse2
# undef libc_hidden_builtin_def
/* It doesn't make sense to send libc-internal strlen calls through a PLT.
   The speedup we get from using SSE4.2 instruction is likely eaten away
   by the indirect call in the PLT.  */
# define libc_hidden_builtin_def(name) \
	.globl __GI_strlen; __GI_strlen = __strlen_sse2
#endif

#include "../strlen.S"
