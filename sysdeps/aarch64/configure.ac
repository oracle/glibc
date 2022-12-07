GLIBC_PROVIDES dnl See aclocal.m4 in the top level source directory.
# Local configure fragment for sysdeps/aarch64.

# We check to see if the compiler and flags are
# selecting the big endian ABI and if they are then
# we set libc_cv_aarch64_be to yes which causes
# HAVE_AARCH64_BE to be defined in config.h and
# in include/libc-symbols.h and thus available to
# shlib-versions to select the appropriate name for
# the dynamic linker via %ifdef.
AC_CACHE_CHECK([for big endian],
  [libc_cv_aarch64_be],
  [AC_EGREP_CPP(yes,[#ifdef __AARCH64EB__
                      yes
                     #endif
  ], libc_cv_aarch64_be=yes, libc_cv_aarch64_be=no)])
if test $libc_cv_aarch64_be = yes; then
  AC_DEFINE(HAVE_AARCH64_BE)
  LIBC_CONFIG_VAR([default-abi], [lp64_be])
else
  LIBC_CONFIG_VAR([default-abi], [lp64])
fi

# Check if binutils supports variant PCS symbols.
AC_CACHE_CHECK([for variant PCS support], [libc_cv_aarch64_variant_pcs], [dnl
  cat > conftest.S <<EOF
.global foo
.type foo, %function
.variant_pcs foo
foo:
	ret
.global bar
.type bar, %function
bar:
	b foo
EOF
  libc_cv_aarch64_variant_pcs=no
  if AC_TRY_COMMAND([${CC-cc} $CFLAGS $CPPFLAGS $LDFLAGS -nostdlib -nostartfiles $no_ssp -shared -fPIC -o conftest.so conftest.S]) \
     && AC_TRY_COMMAND([$READELF -dW conftest.so | grep -q AARCH64_VARIANT_PCS])
  then
    libc_cv_aarch64_variant_pcs=yes
  fi
  rm -rf conftest.*])
LIBC_CONFIG_VAR([aarch64-variant-pcs], [$libc_cv_aarch64_variant_pcs])
