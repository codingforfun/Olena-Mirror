AM_CXXFLAGS = -Wall -Werror -ansi -pedantic -ggdb -O3 -DNDEBUG 
AM_LDFLAGS = $(ZLIB_LDFLAGS)

check_PROGRAMS =					\
  basic_maxtree basic_najman basic_salembier		\
  fiorio-1 fiorio-2 fiorio-3				\
  hdc_maxtree hpc_maxtree r1ic_maxtree rpc_maxtree

basic_maxtree_SOURCES = basic_maxtree.cc
basic_najman_SOURCES = basic_najman.cc
basic_salembier_SOURCES = basic_salembier.cc
fiorio_1_SOURCES = fiorio.cc
fiorio_2_SOURCES = fiorio.cc
fiorio_2_CPPFLAGS = -DFIORIO_VERSION=2
fiorio_3_SOURCES = fiorio.cc
fiorio_3_CPPFLAGS = -DFIORIO_VERSION=3
hdc_maxtree_SOURCES = hdc_maxtree.cc
hpc_maxtree_SOURCES = hpc_maxtree.cc
r1ic_maxtree_SOURCES = r1ic_maxtree.cc
rpc_maxtree_SOURCES = rpc_maxtree.cc

# FIXME: Write (runtime) tests!
