#!/usr/bin/env python

import os
import sys
from swilena_all2d import *

imgdir = os.environ['IMGDIR']

lena = image2d_u8(load(imgdir + "/lena.pgm"))
closing(lena, win_c4p())
#watershed_seg(lena, neighb_c4())
