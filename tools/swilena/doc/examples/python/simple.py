#!/usr/bin/env python

# Add swilena modules path into the PYTHON_PATH variable

from swilena_image2d import *
from swilena_ntg import *
import swilena_display
from swilena_display import display_image, wait_all_displays

swilena_display.tmpdir = "/tmp/swilena"
swilena_display.display_command = "xv %{image_file}"

ima = image2d_uint(5,5)
print ima.at(5,5).value()

ima.set(2,2,uint(2))
print ima

p = ima.ref(1,1)
p.value(3)
print ima

lena = image2d_uint();
lena.load("lena.pgm")

display_image (ima, "lena")
display_image (lena, "lena2")

wait_all_displays()
