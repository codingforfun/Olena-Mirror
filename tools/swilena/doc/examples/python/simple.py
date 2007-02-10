#!/usr/bin/env python

## Copyright (C) 2003  EPITA Research and Development Laboratory
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


# Add swilena modules path into the PYTHON_PATH variable

from swilena_image2d import *
from swilena_ntg import *
import swilena_display
from swilena_display import display_image, wait_all_displays

swilena_display.tmpdir = "/tmp/swilena"
swilena_display.display_command = "xv %{image_file}"

ima = image2d_u8(5,5)
print ima.at(5,5)

ima.set(2, 2, 2)
print ima

p = ima.ref(1, 1)
p.value(3)
print ima

lena = image2d_u8();
lena.load("lena.pgm")

display_image (ima, "lena")
display_image (lena, "lena2")

wait_all_displays()
