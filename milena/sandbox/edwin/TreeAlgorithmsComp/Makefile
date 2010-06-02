-include makefile.rules
#DEBUG = 1 #comment
WALL_CLOCK=1

MLN_DIR = ~/git/olena/milena
TBB_DIR = /home/edwin/Telechargement/tbb30_20100406oss
TBB_INCLUDE_DIR = $(TBB_DIR)/include
TBB_LIB_DIR = $(TBB_DIR)/lib/intel64/cc3.4.3_libc2.3.4_kernel2.6.9
CXXFLAGS += -W -Wall
CXXFLAGS += $(if $(DEBUG), -g -ggdb, -DNDEBUG $(if $(RELEASE), -O3, -O1))
CXXFLAGS += -I $(MLN_DIR)
CXXFLAGS += $(if $(WALL_CLOCK), -DWALL_CLOCK)
LDFLAGS = $(if $(WALL_CLOCK), -ltbb)

CC = g++

tbb_CXXFLAGS = $(CXXFLAGS) -I $(TBB_INCLUDE_DIR) -DWALL_CLOCK
tbb_LDFLAGS = -L $(TBB_LIB_DIR) -ltbb

TARGETS =			\
	tbb_union_find_8	\
	salembier_08		\
	salembier_12		\
	salembier_16		\
	max_tree_08		\
	max_tree_12		\
	max_tree_16		\
	max_tree_with_rank_08	\
	max_tree_with_rank_12	\
	max_tree_with_rank_16	\
	convert_8_to_12		\
	convert_8_to_16		\
	compute_histo_8		\
	compute_histo_12	\
	compute_histo_16


all: $(TARGETS)

tbb_union_find_8: tbb_union_find_8.cc
	$(CXX) $(tbb_CXXFLAGS) $(tbb_LDFLAGS)  $< -o $@

clean:
	rm -f *.o
	rm -f $(TARGETS)

convert_8_to_12: convert_8_to_.cc
	$(CXX) -DQ=int_u12 $(CXXFLAGS) $(LDFLAGS) $< -o $@

convert_8_to_16: convert_8_to_.cc
	$(CXX) -DQ=int_u16 $(CXXFLAGS) $(LDFLAGS) $< -o $@

compute_histo_8: compute_histo_.cc
	$(CXX) -DQ=int_u8 $(CXXFLAGS) $(LDFLAGS) $< -o $@

compute_histo_12: compute_histo_.cc
	$(CXX) -DQ=int_u12 $(CXXFLAGS) $(LDFLAGS) $< -o $@

compute_histo_16: compute_histo_.cc
	$(CXX) -DQ=int_u16 $(CXXFLAGS) $(LDFLAGS) $< -o $@

max_tree_with_rank_08: max_tree_with_rank.cc
	$(CXX) -DQ=int_u8 $(CXXFLAGS) $(LDFLAGS) $< -o $@

max_tree_with_rank_12: max_tree_with_rank.cc
	$(CXX) -DQ=int_u12 $(CXXFLAGS) $(LDFLAGS) $< -o $@

max_tree_with_rank_16: max_tree_with_rank.cc
	$(CXX) -DQ=int_u16 $(CXXFLAGS) $(LDFLAGS) $< -o $@

salembier_08: salembier_.cc
	$(CXX) -DQ=int_u8 $(CXXFLAGS) $(LDFLAGS) $< -o $@

salembier_12: salembier_.cc
	$(CXX) -DQ=int_u12 $(CXXFLAGS) $(LDFLAGS) $< -o $@

salembier_16: salembier_.cc
	$(CXX) -DQ=int_u16 $(CXXFLAGS) $(LDFLAGS) $< -o $@

max_tree_08: max_tree.cc
	$(CXX) -DQ=int_u8 $(CXXFLAGS) $(LDFLAGS) $< -o $@

max_tree_12: max_tree.cc
	$(CXX) -DQ=int_u12 $(CXXFLAGS) $(LDFLAGS) $< -o $@

max_tree_16: max_tree.cc
	$(CXX) -DQ=int_u16 $(CXXFLAGS) $(LDFLAGS) $< -o $@