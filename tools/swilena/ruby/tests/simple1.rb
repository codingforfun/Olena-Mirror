#!/usr/bin/env ruby

# Set swilena_path to the location where swilena ruby modules are
$: << ENV["SWILENA_PATH"] if ENV.has_key? "SWILENA_PATH"

require "swilena_image2d"
require "swilena_ntg"

include Swilena_ntg
include Swilena_image2d

imgdir = ENV["IMGDIR"]

lena = Image2d_uint.new
lena.load(imgdir + "/lena.pgm")
# FIXME: uncomment when ready
#exit 1 unless lena.has_impl()

lena[5, 5] = Uint.new(51)
lena[6, 6] = Uint.new(42)

exit 1 unless lena[5, 5].value() == 51
exit 1 unless lena[6, 6].value() == 42
