#!/usr/bin/env ruby

# Set swilena_path to the location where swilena ruby modules are.
if ENV.has_key? "SWILENA_PATH"
  $:.concat ENV["SWILENA_PATH"].split(":")
end

require "ltrequire"
require "swilena_image2d"
require "swilena_arith2d"

include Swilena_ntg
include Swilena_image2d
include Swilena_arith2d

ima1 = Image2d_u8.new(5, 5)
ima2 = Image2d_u8.new(5, 5)

ima1[1, 1] = 51
ima1[2, 2] = 42

ima2[0, 0] = 12
ima2[2, 2] = 9

ima = plus(ima1, ima2)

exit 1 unless ima[0, 0] == 12
exit 1 unless ima[1, 1] == 51
exit 1 unless ima[2, 2] == 51
