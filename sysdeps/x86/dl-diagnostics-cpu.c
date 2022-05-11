/* Print CPU diagnostics data in ld.so.  x86 version.
   Copyright (C) 2021 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <dl-diagnostics.h>
#include <ldsodefs.h>

static void
print_cpu_features_value (const char *label, uint64_t value)
{
  _dl_printf ("x86.cpu_features.");
  _dl_diagnostics_print_labeled_value (label, value);
}

static void
print_cpu_feature_internal (unsigned int index, const char *kind,
                            unsigned int reg, uint32_t value)
{
  _dl_printf ("x86.cpu_features.features[0x%x].%s[0x%x]=0x%x\n",
              index, kind, reg, value);
}

static void
print_cpu_feature_preferred (const char *label, unsigned int flag)
{
  _dl_printf("x86.cpu_features.preferred.%s=0x%x\n", label, flag);
}

void
_dl_diagnostics_cpu (void)
{
  const struct cpu_features *cpu_features = __get_cpu_features ();

  print_cpu_features_value ("basic.kind", cpu_features->basic.kind);
  print_cpu_features_value ("basic.max_cpuid", cpu_features->basic.max_cpuid);
  print_cpu_features_value ("basic.family", cpu_features->basic.family);
  print_cpu_features_value ("basic.model", cpu_features->basic.model);
  print_cpu_features_value ("basic.stepping", cpu_features->basic.stepping);

  for (unsigned int index = 0; index < COMMON_CPUID_INDEX_MAX; ++index)
    {
      /* Downstream, these constants are not part of the ABI yet, so
         analysis needs to take the precise glibc version into
         account.  */
      print_cpu_feature_internal
        (index, "cpuid", 0, cpu_features->features[index].cpuid.eax);
      print_cpu_feature_internal
        (index, "cpuid", 1, cpu_features->features[index].cpuid.ebx);
      print_cpu_feature_internal
        (index, "cpuid", 2, cpu_features->features[index].cpuid.ecx);
      print_cpu_feature_internal
        (index, "cpuid", 3, cpu_features->features[index].cpuid.edx);
      print_cpu_feature_internal
        (index, "usable", 0, cpu_features->features[index].usable.eax);
      print_cpu_feature_internal
        (index, "usable", 1, cpu_features->features[index].usable.ebx);
      print_cpu_feature_internal
        (index, "usable", 2, cpu_features->features[index].usable.ecx);
      print_cpu_feature_internal
        (index, "usable", 3, cpu_features->features[index].usable.edx);
    }

  /* The preferred indicators are not part of the ABI and need to be
     translated.  */
#define BIT(x) \
  print_cpu_feature_preferred (#x, CPU_FEATURE_PREFERRED_P (cpu_features, x));
#include "cpu-features-preferred_feature_index_1.def"
#undef BIT

  print_cpu_features_value ("xsave_state_size",
                            cpu_features->xsave_state_size);
  print_cpu_features_value ("xsave_state_full_size",
                            cpu_features->xsave_state_full_size);
  print_cpu_features_value ("data_cache_size", cpu_features->data_cache_size);
  print_cpu_features_value ("shared_cache_size",
                            cpu_features->shared_cache_size);
  print_cpu_features_value ("non_temporal_threshold",
                            cpu_features->non_temporal_threshold);
  print_cpu_features_value ("rep_movsb_threshold",
                            cpu_features->rep_movsb_threshold);
  print_cpu_features_value ("rep_stosb_threshold",
                            cpu_features->rep_stosb_threshold);
  _Static_assert (offsetof (struct cpu_features, rep_stosb_threshold)
                  + sizeof (cpu_features->rep_stosb_threshold)
                  == sizeof (*cpu_features),
                  "last cpu_features field has been printed");
}
