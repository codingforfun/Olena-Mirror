#! /usr/bin/env python

# Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

# \file
# \brief Test on mathematical morphology on functions (scalar images).

import data

import ltihooks

import xtc

import mln
import mln.io.pgm
import mln.morpho
import mln.morpho.elementary
import mln.morpho.closing
import mln.morpho.opening

# Note that Python built-in types must be wrapped explicitly as
# Extatica objects using `xtc.data', because automatica conversion
# won't work from Python.

ima = mln.mk_image2d_int_u8()
mln.io.pgm.load(ima, xtc.data(data.lena))

dilated = mln.morpho.dilation(ima, mln.win_c4p())
mln.io.pgm.save(dilated, xtc.data("morpho-fun-dilation.pgm"))

eroded = mln.morpho.erosion(ima, mln.win_c4p())
mln.io.pgm.save(eroded, xtc.data("morpho-fun-erosion.pgm"))

mln.io.pgm.save(mln.morpho.elementary.opening(ima, mln.c4()),
                xtc.data("morpho-fun-opening.pgm"))
mln.io.pgm.save(mln.morpho.elementary.closing(ima, mln.c4()),
                xtc.data("morpho-fun-closing.pgm"))

mln.io.pgm.save(mln.morpho.gradient(ima, mln.win_c4p()),
                xtc.data("morpho-fun-gradient.pgm"))
mln.io.pgm.save(mln.morpho.gradient_internal(ima, mln.win_c4p()),
                xtc.data("morpho-fun-gradient_internal.pgm"))
mln.io.pgm.save(mln.morpho.gradient_external(ima, mln.win_c4p()),
                xtc.data("morpho-fun-gradient_external.pgm"))

mln.io.pgm.save(mln.morpho.closing.area(ima, mln.c4(), xtc.data(50)),
                xtc.data("morpho-fun-closing_area.pgm"))
mln.io.pgm.save(mln.morpho.opening.area(ima, mln.c4(), xtc.data(50)),
                xtc.data("morpho-fun-opening_area.pgm"))
