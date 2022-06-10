/* Multiple versions of strncasecmp_l
   All versions must be listed in ifunc-impl-list.c.  */
#define STRCMP __strncasecmp_l
#define STRCMP_NONASCII __strncasecmp_l_nonascii
#define USE_AS_STRNCASECMP_L
#include "strcmp.S"

weak_alias (__strncasecmp_l, strncasecmp_l)
