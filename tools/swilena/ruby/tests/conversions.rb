#!/usr/bin/env ruby

# Set swilena_path to the location where swilena ruby modules are.
if ENV.has_key? "SWILENA_PATH"
  $:.concat ENV["SWILENA_PATH"].split(":")
end

require "ltrequire"
require "swilena_image2d"
require "swilena_conversions2d"

include Swilena_ntg
include Swilena_image2d
include Swilena_conversions2d

imgdir = ENV['IMGDIR']

lena = Image2d_u8.new(load(imgdir + "/lena.pgm"))
# FIXME: uncomment when ready
#exit 1 unless lena.has_impl()
lena32 = cast_to_int_u32(lena)

lena32[0, 0] = 65536
exit 1 unless lena32[0, 0] == 65536

lena8 = bound_to_int_u8(lena32)
exit 1 unless lena8[0, 0] == 255
