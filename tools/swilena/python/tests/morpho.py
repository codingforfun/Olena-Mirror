#! /usr/bin/env python

## Copyright (C) 2003, 2004, 2006  EPITA Research and Development Laboratory
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


import os
import sys
import ltihooks

from swilena_image2d import *
from swilena_conversions2d import *

from swilena_structelt2d import *
import swilena_morpho2d_ntg_int_s32 as morpho_s32
import swilena_morpho2d_ntg_int_u8 as morpho_u8

imgdir = os.environ['IMGDIR']

lena = image2d_u8(load(imgdir + "/lena.pgm"))
lena_s32 = cast_to_int_s32(lena)
morpho_s32.closing(lena_s32, win_c4p())

# FIXME: The watershed transform fails with this error:
#
#   Traceback (most recent call last):
#     File "../../../../../tools/swilena/python/tests/morpho.py", line 21, in ?
#       morpho_u8.watershed_seg(lena, neighb_c4())
#   RuntimeError: ../../../../integre/ntg/real/behavior.hh:238:
#     Assertion `ntg::to_ntg(p) <= ntg::type_traits< T >::max()' failed.

#morpho_u8.watershed_seg(lena, neighb_c4())
