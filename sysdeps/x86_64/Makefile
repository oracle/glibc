# The i387 `long double' is a distinct type we support.
long-double-fcts = yes

ifeq ($(subdir),csu)
gen-as-const-headers += link-defines.sym
endif

ifeq ($(subdir),gmon)
sysdep_routines += _mcount
endif

ifeq ($(subdir),malloc)
tests += tst-mallocalign1
endif

ifeq ($(subdir),string)
sysdep_routines += cacheinfo strcasecmp_l-nonascii strncase_l-nonascii
gen-as-const-headers += locale-defines.sym
endif

ifeq ($(subdir),elf)
sysdep-dl-routines += tlsdesc dl-tlsdesc tls_get_addr

tests += ifuncmain8
modules-names += ifuncmod8

$(objpfx)ifuncmain8: $(objpfx)ifuncmod8.so

tests += tst-quad1 tst-quad2
modules-names += tst-quadmod1 tst-quadmod2

$(objpfx)tst-quad1: $(objpfx)tst-quadmod1.so
$(objpfx)tst-quad2: $(objpfx)tst-quadmod2.so

quad-pie-test += tst-quad1pie tst-quad2pie
tests += $(quad-pie-test)
tests-pie += $(quad-pie-test)

$(objpfx)tst-quad1pie: $(objpfx)tst-quadmod1pie.o
$(objpfx)tst-quad2pie: $(objpfx)tst-quadmod2pie.o

tests += tst-sse tst-avx tst-avx512
test-extras += tst-avx-aux tst-avx512-aux
extra-test-objs += tst-avx-aux.o tst-avx512-aux.o

tests += tst-audit10
modules-names += tst-auditmod10a tst-auditmod10b \
		 tst-ssemod tst-avxmod tst-avx512mod

$(objpfx)tst-audit10: $(objpfx)tst-auditmod10a.so
$(objpfx)tst-audit10.out: $(objpfx)tst-auditmod10b.so
tst-audit10-ENV = LD_AUDIT=$(objpfx)tst-auditmod10b.so

$(objpfx)tst-sse: $(objpfx)tst-ssemod.so
$(objpfx)tst-avx: $(objpfx)tst-avx-aux.o $(objpfx)tst-avxmod.so
$(objpfx)tst-avx512: $(objpfx)tst-avx512-aux.o $(objpfx)tst-avx512mod.so

CFLAGS-tst-avx-aux.c += $(AVX-CFLAGS)
CFLAGS-tst-avxmod.c += $(AVX-CFLAGS)

ifeq (yes,$(config-cflags-avx512))
AVX512-CFLAGS = -mavx512f
CFLAGS-tst-audit10.c += $(AVX512-CFLAGS)
CFLAGS-tst-auditmod10a.c += $(AVX512-CFLAGS)
CFLAGS-tst-auditmod10b.c += $(AVX512-CFLAGS)
CFLAGS-tst-avx512-aux.c += $(AVX512-CFLAGS)
CFLAGS-tst-avx512mod.c += $(AVX512-CFLAGS)
endif
endif

ifeq ($(subdir),csu)
gen-as-const-headers += tlsdesc.sym rtld-offsets.sym
endif
