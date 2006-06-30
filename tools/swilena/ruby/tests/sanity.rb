#!/usr/bin/env ruby

# Sanity check: include all Ruby modules, but do nothing.


# Set swilena_path to the location where swilena ruby modules are.
$: << ENV["SWILENA_PATH"] if ENV.has_key? "SWILENA_PATH"


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
