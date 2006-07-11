#!/usr/bin/env python

import os
import sys
import ltihooks
from swilena_all2d import *

imgdir = os.environ['IMGDIR']

lena = image2d_u8(load(imgdir + "/lena.pgm"))
lena32 = cast_to_int_u32(lena)

lena32.set(0, 0, 65536)
assert(lena32.at(0, 0) == 65536)

lena8 = bound_to_int_u8(lena32)
assert(lena8.at(0, 0) == 255)
