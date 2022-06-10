#ifndef _SYS_CDEFS_H

#include <misc/sys/cdefs.h>

#ifndef _ISOMAC
/* The compiler will optimize based on the knowledge the parameter is
   not NULL.  This will omit tests.  A robust implementation cannot allow
   this so when compiling glibc itself we ignore this attribute.  */
# undef __nonnull
# define __nonnull(params)

extern void __chk_fail (void) __attribute__ ((__noreturn__));
libc_hidden_proto (__chk_fail)
rtld_hidden_proto (__chk_fail)


# define __attribute_alloc_size(...) __attribute__ ((alloc_size (__VA_ARGS__)))

/* This mirrors the C11/C++11 max_align_t type provided by GCC, but it
   is also available in C99 mode.  The aligned attributes are required
   because some ABIs have reduced alignment requirements for struct and
   union members.  */
#if __STDC_VERSION__ < 201112L && __cplusplus < 201103L
typedef struct {
  long long ll __attribute__ ((__aligned__ (__alignof__ (long long))));
  long double ld __attribute__ ((__aligned__ (__alignof__ (long double))));
} max_align_t;
#endif /* __STDC_VERSION__ < 201112 && __cplusplus < 201103L */

#endif

#endif
