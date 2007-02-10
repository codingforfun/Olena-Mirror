#!/usr/bin/env python

# Copyright (C) 2003, 2006  EPITA Research and Development Laboratory
#
# This file is part of the Olena Library.  This library is free
# software; you can redistribute it and/or modify it under the terms
# of the GNU General Public License version 2 as published by the
# Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# As a special exception, you may use this file as part of a free
# software library without restriction.  Specifically, if other files
# instantiate templates or use macros or inline functions from this
# file, or you compile this file and link it with other files to
# produce an executable, this file does not by itself cause the
# resulting executable to be covered by the GNU General Public
# License.  This exception does not however invalidate any other
# reasons why the executable file might be covered by the GNU General
# Public License.


import sys, os
import copyright

years = "2003, 2004, 2006"
files = []
dict_type_num = {1: "ntg_bin", 2: "ntg_int_u8", 3: "ntg_int_u32",
                 4: "ntg_int_s8", 5: "ntg_int_s32", 6: "ntg_float"}
dict_num_type = {"ntg_bin": 1, "ntg_int_u8": 2, "ntg_int_u32": 3,
                 "ntg_int_s8": 4, "ntg_int_s32": 5, "ntg_float": 6}

def close_files():
    for file in files:
	file.close()

def open_files(path):
    for dim in range(1, 4):
        for type in range(1, 7):
            str_type = dict_type_num[type]
            files.append(open(path + "/swilena_morpho%(dim)sd_%(str_type)s.i" % vars(), 'w'))

def write_headers(copyright_header):
    for dim in range(1, 4):
        for type in range(1, 7):
            str_type = dict_type_num[type]
            file = files[(dim - 1)* 6 + type - 1]
            file.write("""// Note: this file was generated by generate_morpho.py.

""")
            file.writelines(copyright_header)
            file.write("""

%%module swilena_morpho%(dim)sd_%(str_type)s

%%include swilena_config.i

%%include swilena_exception.i

%%include swilena_ntg_macros.i
%%import swilena_ntg.i

%%include swilena_morpho.i

%%include swilena_image%(dim)sd.i

""" % vars())

def instantiate(idx, algorithm, *args):
    file = files[idx - 1]
    swilena_algorithm = "my_" + algorithm
    params = args[0]
    for param in args[1:]:
	params += ", " + param
    file.write(
	"%%template(%(algorithm)s) %(swilena_algorithm)s< %(params)s >;\n"
	% vars())

def write_algorithms():
    # Algorithms with all types and all dims
    for dim in range(1, 4):
	for type in [ "ntg_bin",
		      "ntg_int_u8", "ntg_int_u32",
		      "ntg_int_s8", "ntg_int_s32",
		      "ntg_float" ]:
            # FIXME: SWIG 1.3.29 Bug.  We used to refer to `oln::image' from
            # the global (top-level) namespace, i.e. `::oln::image',
            # but it makes swig 1.3.29 generate invalid C++ code when
            # used ad a template argument: the space between `<' and
            # `::' is eaten by swig, and `<:' is understood as a
            # trigraph (for `[') by the C++ compiler.
	    img_type = "oln::image%(dim)sd< %(type)s >" % vars()
	    win_type = "::oln::window%(dim)sd" % vars()
	    neighb_type = "::oln::neighborhood%(dim)sd" % vars()

            idx = (dim - 1) * 6 + dict_num_type[type]

	    # FIXME: these algorithms do not work with floats
            if type != "ntg_float":
		instantiate(idx, "erosion", img_type, img_type, win_type)
		instantiate(idx, "fast_erosion", img_type, img_type, win_type)

		instantiate(idx, "dilation", img_type, img_type, win_type)
		instantiate(idx, "fast_dilation", img_type, img_type, win_type)

		instantiate(idx, "opening", img_type, img_type, win_type)
		instantiate(idx, "fast_opening", img_type, img_type, win_type)

		instantiate(idx, "closing", img_type, img_type, win_type)
		instantiate(idx, "fast_closing", img_type, img_type, win_type)

		instantiate(idx, "thickening", img_type, img_type, win_type, win_type)
		instantiate(idx, "fast_thickening", img_type, img_type, win_type, win_type)

		instantiate(idx, "thinning", img_type, img_type, win_type, win_type)
		instantiate(idx, "fast_thinning", img_type, img_type, win_type, win_type)

		instantiate(idx, "geodesic_erosion", img_type, img_type, img_type, neighb_type)
		instantiate(idx, "sure_geodesic_erosion", img_type, img_type, img_type, neighb_type)

		instantiate(idx, "geodesic_dilation", img_type, img_type, img_type, neighb_type)
                instantiate(idx, "sure_geodesic_dilation", img_type, img_type, img_type, neighb_type)

                instantiate(idx, "sure_geodesic_reconstruction_dilation", img_type, img_type, img_type, neighb_type)
                instantiate(idx, "sequential_geodesic_reconstruction_dilation", img_type, img_type, img_type, neighb_type)
                instantiate(idx, "hybrid_geodesic_reconstruction_dilation", img_type, img_type, img_type, neighb_type)

                instantiate(idx, "sure_geodesic_reconstruction_erosion", img_type, img_type, img_type, neighb_type)
                instantiate(idx, "sequential_geodesic_reconstruction_erosion", img_type, img_type, img_type, neighb_type)
                instantiate(idx, "hybrid_geodesic_reconstruction_erosion", img_type, img_type, img_type, neighb_type)

		instantiate(idx, "hit_or_miss", img_type, img_type, win_type, win_type)
                instantiate(idx, "fast_hit_or_miss", img_type, img_type, win_type, win_type)

		instantiate(idx, "hit_or_miss_opening", img_type, img_type, win_type, win_type)
                instantiate(idx, "fast_hit_or_miss_opening", img_type, img_type, win_type, win_type)

		instantiate(idx, "hit_or_miss_opening_bg", img_type, img_type, win_type, win_type)
		instantiate(idx, "fast_hit_or_miss_opening_bg", img_type, img_type, win_type, win_type)

		instantiate(idx, "hit_or_miss_closing", img_type, img_type, win_type, win_type)
		instantiate(idx, "fast_hit_or_miss_closing", img_type, img_type, win_type, win_type)

		instantiate(idx, "hit_or_miss_closing_bg", img_type, img_type, win_type, win_type)
		instantiate(idx, "fast_hit_or_miss_closing_bg", img_type, img_type, win_type, win_type)

                # FIXME: This should work with other types.
                if type != "ntg_bin":
                    instantiate(idx, "beucher_gradient", img_type, img_type, win_type)
                    instantiate(idx, "fast_beucher_gradient", img_type, img_type, win_type)

                    instantiate(idx, "internal_gradient", img_type, img_type, win_type)
                    instantiate(idx, "fast_internal_gradient", img_type, img_type, win_type)

                    instantiate(idx, "external_gradient", img_type, img_type, win_type)
                    instantiate(idx, "fast_external_gradient", img_type, img_type, win_type)

                    instantiate(idx, "white_top_hat", img_type, img_type, win_type)
                    instantiate(idx, "fast_white_top_hat", img_type, img_type, win_type)

                    instantiate(idx, "black_top_hat", img_type, img_type, win_type)
                    instantiate(idx, "fast_black_top_hat", img_type, img_type, win_type)

                    instantiate(idx, "self_complementary_top_hat", img_type, img_type, win_type)
                    instantiate(idx, "fast_self_complementary_top_hat", img_type, img_type, win_type)

                    instantiate(idx, "top_hat_contrast_op", img_type, img_type, win_type)
                    instantiate(idx, "fast_top_hat_contrast_op", img_type, img_type, win_type)
                    # Watershed
                    # FIXME: SWIG 1.3.29 Bug (same as above).
                    img_ret_type = "oln::image%(dim)sd< ntg_int_u32 >" % vars()
                    instantiate(idx, "watershed_seg", img_ret_type, img_type, neighb_type)
                    instantiate(idx, "watershed_con", img_ret_type, img_type, neighb_type)
                    # FIXME: SWIG 1.3.29 Bug (same as above).
                    bin_img_type = "oln::image%(dim)sd< ntg_bin >" % vars()
                    instantiate(idx, "sure_minima_imposition", img_type, img_type, bin_img_type, neighb_type)
                    instantiate(idx, "sequential_minima_imposition", img_type, img_type, bin_img_type, neighb_type)
                    instantiate(idx, "hybrid_minima_imposition", img_type, img_type, bin_img_type, neighb_type)

                    instantiate(idx, "sure_regional_minima", bin_img_type, img_type, neighb_type)
                    instantiate(idx, "sequential_regional_minima", bin_img_type, img_type, neighb_type)
                    instantiate(idx, "hybrid_regional_minima", bin_img_type, img_type, neighb_type)

                    # Extrema killers
                    if dim == 2:
                        instantiate(idx, "fast_maxima_killer", img_type, neighb_type)
                        instantiate(idx, "fast_minima_killer", img_type, neighb_type)
                        # FIXME: This should work with other types.
                        if type == "ntg_int_u8":
                            instantiate(idx, "sure_maxima_killer", img_type, neighb_type)
                            instantiate(idx, "sure_minima_killer", img_type, neighb_type)

                #FIXME: Does not work due to the return type:
                #instantiate(idx, "watershed_seg_or", img_type, img_type, img_ret_type, neighb_type)
                #instantiate(idx, "laplacian", img_type, img_type, win_type)
	        #instantiate(idx, "fast_laplacian", img_type, img_type, win_type)

def main():
    if len(sys.argv) != 2:
	sys.stderr.write ("Usage: generate_morpho_instanciations.py path\n")
	sys.exit (1)
    else:
	path = sys.argv[1]
    open_files(path)
    copyright_header = \
      copyright.notice(os.path.join(path, "copyright.template"), years)
    write_headers(copyright_header)
    write_algorithms()
    close_files()

if __name__ == "__main__":
    main()
