#include <string.h>

extern int __strncasecmp_nonascii (const char *__s1, const char *__s2,
                                   size_t __n);

#define __strncasecmp __strncasecmp_nonascii
#include <string/strncase.c>
