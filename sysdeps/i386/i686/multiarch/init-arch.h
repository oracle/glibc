#include <sysdeps/x86_64/multiarch/init-arch.h>

#ifdef __ASSEMBLER__
/* Performan an access system call to check whether the configuration
  file exists.  If it does, load SYMBOL into %eax and jump to LABEL.  */
# define CHECK_STRCASECMP_CONFIG_FILE(symbol, label) \
	pushl	%ebx; \
	cfi_adjust_cfa_offset (4); \
	cfi_rel_offset (%ebx, 0); \
	LOAD_PIC_REG (dx); \
	movl	$__NR_access, %eax; \
	lea	__sysconfig_strcasecmp_nonascii@GOTOFF(%edx), %ebx; \
	xor	%ecx, %ecx; \
	int	$0x80; \
	test	%eax, %eax; \
	lea	symbol@GOTOFF(%edx), %eax; \
	popl	%ebx; \
	cfi_adjust_cfa_offset (-4); \
	cfi_restore (%ebx); \
	jz	label
#endif
