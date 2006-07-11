#!/usr/bin/env ruby

# Set swilena_path to the location where swilena ruby modules are.
if ENV.has_key? "SWILENA_PATH"
  $:.concat ENV["SWILENA_PATH"].split(":")
end

require "ltrequire"

require "swilena_image2d"
require "swilena_conversions2d"
require "swilena_structelt2d"
require "swilena_morpho2d_ntg_int_s32"
require "swilena_morpho2d_ntg_int_u8"

include Swilena_image2d
include Swilena_conversions2d
include Swilena_structelt2d
Morpho_s32 = Swilena_morpho2d_ntg_int_s32
Morpho_u8 = Swilena_morpho2d_ntg_int_u8

imgdir = ENV['IMGDIR']

lena = Image2d_u8.new(load(imgdir + "/lena.pgm"))
# FIXME: uncomment when ready
#exit 1 unless lena.has_impl()
lenas32 = cast_to_int_s32(lena)
Morpho_s32.closing(lenas32, win_c4p())

# FIXME: The watershed transform fails with this error:
#
# ../../../../../tools/swilena/ruby/tests/morpho.rb:29:in `watershed_seg': ../../../../integre/ntg/real/behavior.hh:238: Assertion `ntg::to_ntg(p) <= ntg::type_traits< T >::max()' failed. (RuntimeError)
# 	from ../../../../../tools/swilena/ruby/tests/morpho.rb:29

#Morpho_u8.watershed_seg(lena, neighb_c4())
