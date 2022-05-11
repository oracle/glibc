# sysdeps makefile fragment for all pthread-supporting configurations.
# Copyright (C) 2014-2018 Free Software Foundation, Inc.
# This file is part of the GNU C Library.

# The GNU C Library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.

# The GNU C Library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.

# You should have received a copy of the GNU Lesser General Public
# License along with the GNU C Library; if not, see
# <http://www.gnu.org/licenses/>.

ifeq ($(subdir),rt)

ifeq (yes,$(build-shared))
$(objpfx)tst-timer: $(objpfx)librt.so $(shared-thread-library)
else
$(objpfx)tst-timer: $(objpfx)librt.a $(static-thread-library)
endif

endif

ifneq (,$(filter $(subdir),htl nptl))
ifeq ($(build-shared),yes)
tests += tst-create1
endif

tst-create1mod.so-no-z-defs = yes

tests += tst-once1 tst-once2 tst-once3 tst-once4

tests += tst-oncex3 tst-oncex4

ifeq ($(build-shared),yes)
# Build all the modules even when not actually running test programs.
tests: $(test-modules)
endif

CFLAGS-tst-oncex3.c += -fexceptions
CFLAGS-tst-oncex4.c += -fexceptions

modules-names += tst-create1mod
test-modules = $(addprefix $(objpfx),$(addsuffix .so,$(modules-names)))

LDFLAGS-tst-create1 = -Wl,-export-dynamic
$(objpfx)tst-create1: $(libdl) $(shared-thread-library)
$(objpfx)tst-create1.out: $(objpfx)tst-create1mod.so

endif
