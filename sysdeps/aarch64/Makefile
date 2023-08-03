long-double-fcts = yes

ifeq ($(subdir),elf)
sysdep-dl-routines += tlsdesc dl-tlsdesc
gen-as-const-headers += dl-link.sym

tests += tst-audit26 \
	 tst-audit27

modules-names += \
    tst-audit26mod \
    tst-auditmod26 \
    tst-audit27mod \
    tst-auditmod27

$(objpfx)tst-audit26: $(objpfx)tst-audit26mod.so \
		      $(objpfx)tst-auditmod26.so
LDFLAGS-tst-audit26 += -Wl,-z,lazy
tst-audit26-ENV = LD_AUDIT=$(objpfx)tst-auditmod26.so

$(objpfx)tst-audit27: $(objpfx)tst-audit27mod.so \
		      $(objpfx)tst-auditmod27.so
$(objpfx)tst-audit27mod.so: $(libsupport)
LDFLAGS-tst-audit27 += -Wl,-z,lazy
tst-audit27-ENV = LD_AUDIT=$(objpfx)tst-auditmod27.so

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
