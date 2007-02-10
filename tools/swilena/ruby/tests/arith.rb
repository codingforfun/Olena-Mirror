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
