#if IS_IN (libc)
# include <string.h>

extern int __strcasecmp_nonascii (const char *__s1, const char *__s2);

# define __strcasecmp __strcasecmp_nonascii
# include <string/strcasecmp.c>

/* Used by the assembler IFUNC selectors.  Presence of this file
   indicates that the C implementation shall be used.  */
const char __sysconfig_strcasecmp_nonascii[] attribute_hidden =
  "/etc/sysconfig/strcasecmp-nonascii";
#endif
