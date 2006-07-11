#!/usr/bin/env python

import os
import sys
import ltihooks
from swilena_all2d import *

imgdir = os.environ['IMGDIR']

lena = image2d_u8(load(imgdir + "/lena.pgm"))
lenas32 = cast_to_int_s32(lena)
closing(lenas32, win_c4p())
#watershed_seg(lena, neighb_c4())
