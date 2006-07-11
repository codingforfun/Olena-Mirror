#! /usr/bin/env python

import os
import sys
import ltihooks
from swilena_image2d import *

imgdir = os.environ['IMGDIR']

lena = image2d_u8()
lena.load(imgdir + "/lena.pgm")
# FIXME: uncomment when ready
#assert(lena.has_impl())

lena.set(5, 5, 51)
lena.ref(6, 6).value(42)

assert(lena.at(5, 5) == 51)
assert(lena.at(6, 6) == 42)
