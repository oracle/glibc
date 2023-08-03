long-double-fcts = yes

ifeq ($(subdir),elf)
sysdep-dl-routines += tlsdesc dl-tlsdesc
gen-as-const-headers += dl-link.sym

ifeq (yes,$(aarch64-variant-pcs))
tests += tst-vpcs
modules-names += tst-vpcs-mod
LDFLAGS-tst-vpcs-mod.so = -Wl,-z,lazy
$(objpfx)tst-vpcs: $(objpfx)tst-vpcs-mod.so
endif
endif

ifeq ($(subdir),csu)
gen-as-const-headers += tlsdesc.sym
endif

ifeq ($(subdir),gmon)
CFLAGS-mcount.c += -mgeneral-regs-only
endif

ifeq ($(subdir),math)
CPPFLAGS += -I../soft-fp
endif
