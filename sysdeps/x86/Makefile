ifeq ($(subdir),elf)
CFLAGS-.os += $(if $(filter $(@F),$(patsubst %,%.os,$(all-rtld-routines))),\
		   -mno-sse -mno-mmx)

tests: $(objpfx)tst-xmmymmzmm.out
$(objpfx)tst-xmmymmzmm.out: ../sysdeps/x86/tst-xmmymmzmm.sh $(objpfx)ld.so
	@echo "Checking ld.so for SSE register use.  This will take a few seconds..."
	$(SHELL) $< $(objpfx) '$(NM)' '$(OBJDUMP)' '$(READELF)' > $@
endif

ifeq ($(subdir),csu)
gen-as-const-headers += cpu-features-offsets.sym rtld-global-offsets.sym
endif

ifeq ($(subdir),elf)
sysdep-dl-routines += dl-get-cpu-features

tests += tst-get-cpu-features
tests-static += tst-get-cpu-features-static
endif
