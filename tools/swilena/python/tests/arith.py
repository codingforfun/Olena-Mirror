#!/usr/bin/env python

import os
import sys
from swilena_image2d import *
from swilena_arith2d import *

imgdir = os.environ['IMGDIR']

ima1 = image2d_u8(5, 5)
ima2 = image2d_u8(5, 5)

ima1.set(1, 1, 51)
ima1.set(2, 2, 42)

ima2.set(0, 0, 12)
ima2.set(2, 2, 9)

ima = plus(ima1, ima2)

assert(ima.at(0, 0) == 12)
assert(ima.at(1, 1) == 51)
assert(ima.at(2, 2) == 51)
