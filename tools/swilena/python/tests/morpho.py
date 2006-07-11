#! /usr/bin/env python

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
