################################################################################
# Only enabled if we are not building for ppc64le.
ifeq (,$(filter %le,$(config-machine)))
# Enable bind-now behaviour by default for POWER. This is a downstream specific
# change that is required due to a toolchain limitation in handling COPY
# relocations and BIND_NOW (see rhbz#1790475).
LDCFLAGS-c.so += -Wl,-z,now
# Likewise. Take care that this is carefully selected to turn BIND_NOW back on
# for ld.so without turning it back on for libpthread.so which has the
# problematic OPD that generates a COPY relocation. Enable these only for the
# elf subdir which is also used to build libc.so.6. This avoids enabling
# BIND_NOW for libpthread.
ifeq ($(subdir),elf)
z-now-no = -Wl,-z,now
LDFLAGS-lib.so += -Wl,-z,now
link-extra-flags += -Wl,-z,now
endif
# Likewise. Take care that this is carefully selected to turn BIND_NOW
# back on for iconv modules to ensure the data-only DSOs have consistently the
# correct expected flags for DSOs (even if they don't really need them).
ifeq ($(subdir),iconvdata)
LDFLAGS.so += -Wl,-z,now
endif
endif
################################################################################

ifeq ($(subdir),string)
CFLAGS-memcmp.c += -Wno-uninitialized
endif

ifeq ($(subdir),elf)
# extra shared linker files to link into dl-allobjs.so and libc
sysdep-dl-routines += dl-machine
sysdep_routines += dl-machine
# extra shared linker files to link only into dl-allobjs.so
sysdep-rtld-routines += dl-machine
endif

ifeq ($(subdir),setjmp)
ifeq (yes,$(build-shared))
sysdep_routines += novmx-longjmp novmx-sigjmp
endif
endif

ifeq ($(subdir),csu)
# get offset to rtld_global._dl_hwcap and rtld_global._dl_hwcap2
gen-as-const-headers += rtld-global-offsets.sym
# get offset to __locale_struct.__ctype_tolower
gen-as-const-headers += locale-defines.sym
endif

ifeq ($(subdir),misc)
sysdep_headers += sys/platform/ppc.h
tests += test-gettimebase
endif

ifneq (,$(filter %le,$(config-machine)))
abilist-pattern = %-le.abilist
endif
