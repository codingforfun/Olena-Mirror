#!/usr/bin/env ruby

## Copyright (C) 2006  EPITA Research and Development Laboratory
##
## This file is part of the Olena Library.  This library is free
## software; you can redistribute it and/or modify it under the terms
## of the GNU General Public License version 2 as published by the
## Free Software Foundation.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this library; see the file COPYING.  If not, write to
## the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
## Boston, MA 02110-1301, USA.
##
## As a special exception, you may use this file as part of a free
## software library without restriction.  Specifically, if other files
## instantiate templates or use macros or inline functions from this
## file, or you compile this file and link it with other files to
## produce an executable, this file does not by itself cause the
## resulting executable to be covered by the GNU General Public
## License.  This exception does not however invalidate any other
## reasons why the executable file might be covered by the GNU General
## Public License.


# Sanity check: include all Ruby modules, but do nothing.

# Set swilena_path to the location where swilena ruby modules are.
if ENV.has_key? "SWILENA_PATH"
  $:.concat ENV["SWILENA_PATH"].split(":")
end

require "ltrequire"

# Integre types.
require "swilena_ntg"

# Olena types.
require "swilena_point1d"
require "swilena_point2d"
require "swilena_point3d"

require "swilena_structelt1d"
require "swilena_structelt2d"
require "swilena_structelt3d"

require "swilena_w_win1d"
require "swilena_w_win2d"
require "swilena_w_win3d"

require "swilena_imagesize1d"
require "swilena_imagesize2d"
require "swilena_imagesize3d"

# Image types.
require "swilena_image1d"
require "swilena_image2d"
require "swilena_image3d"

# Algorithms.
require "swilena_arith1d"
require "swilena_arith2d"
require "swilena_arith3d"

require "swilena_conversions1d"
require "swilena_conversions2d"
require "swilena_conversions3d"

require "swilena_morpho1d_ntg_bin"
require "swilena_morpho1d_ntg_float"
require "swilena_morpho1d_ntg_int_s32"
require "swilena_morpho1d_ntg_int_s8"
require "swilena_morpho1d_ntg_int_u32"
require "swilena_morpho1d_ntg_int_u8"

require "swilena_morpho2d_ntg_bin"
require "swilena_morpho2d_ntg_float"
require "swilena_morpho2d_ntg_int_s32"
require "swilena_morpho2d_ntg_int_s8"
require "swilena_morpho2d_ntg_int_u32"
require "swilena_morpho2d_ntg_int_u8"

require "swilena_morpho3d_ntg_bin"
require "swilena_morpho3d_ntg_float"
require "swilena_morpho3d_ntg_int_s32"
require "swilena_morpho3d_ntg_int_s8"
require "swilena_morpho3d_ntg_int_u32"
require "swilena_morpho3d_ntg_int_u8"
