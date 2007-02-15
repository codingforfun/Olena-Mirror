## Process this file with Automake to produce Makefile.in.

AM_CXXFLAGS = \
  $(CXXFLAGS_OPTIMIZE) $(CXXFLAGS_STRICT_ERRORS) $(ZLIB_CXXFLAGS) -DNDEBUG
AM_LDFLAGS = $(ZLIB_LDFLAGS)

check_PROGRAMS =							    \
  basic_maxtree basic_najman basic_salembier				    \
  generic_salembier_int_u8 generic_salembier_float generic_salembier_float2 \
  generic_simplified_salembier_float					    \
  naive_generic_salembier_float						    \
  basic_salembier_float_via_16b						    \
  generic_salembier_float_via_16b					    \
  fiorio-1 fiorio-2 fiorio-3						    \
  hdc_maxtree hpc_maxtree r1ic_maxtree rpc_maxtree

basic_maxtree_SOURCES = basic_maxtree.cc
basic_najman_SOURCES = basic_najman.cc

# Salembier - Unsigned integer versions.
basic_salembier_SOURCES = basic_salembier.cc
generic_salembier_int_u8_SOURCES = generic_salembier_int_u8.cc

# Salembier - Float versions.
generic_salembier_float_SOURCES = generic_salembier_float.cc
generic_salembier_float_LDFLAGS = -lcfitsio $(ZLIB_LDFLAGS)
generic_salembier_float2_SOURCES = generic_salembier_float2.cc
generic_salembier_float2_LDFLAGS = -lcfitsio $(ZLIB_LDFLAGS)
generic_simplified_salembier_float_SOURCES = generic_simplified_salembier_float.cc
generic_simplified_salembier_float_LDFLAGS = -lcfitsio $(ZLIB_LDFLAGS)
naive_generic_salembier_float_SOURCES = naive_generic_salembier_float.cc
naive_generic_salembier_float_LDFLAGS = -lcfitsio $(ZLIB_LDFLAGS)

# Salembier - Float versions, using a 16-bits equalization step.
basic_salembier_float_via_16b_SOURCES = basic_salembier_float_via_16b.cc
basic_salembier_float_via_16b_LDFLAGS = -lcfitsio $(ZLIB_LDFLAGS)
generic_salembier_float_via_16b_SOURCES = generic_salembier_float_via_16b.cc
generic_salembier_float_via_16b_LDFLAGS = -lcfitsio $(ZLIB_LDFLAGS)


fiorio_1_SOURCES = fiorio.cc
fiorio_2_SOURCES = fiorio.cc
fiorio_2_CPPFLAGS = -DFIORIO_VERSION=2
fiorio_3_SOURCES = fiorio.cc
fiorio_3_CPPFLAGS = -DFIORIO_VERSION=3

hdc_maxtree_SOURCES = hdc_maxtree.cc
hpc_maxtree_SOURCES = hpc_maxtree.cc
r1ic_maxtree_SOURCES = r1ic_maxtree.cc
rpc_maxtree_SOURCES = rpc_maxtree.cc

# FIXME: Write runtime tests (using input/output data)!
