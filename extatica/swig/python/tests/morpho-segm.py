#! /usr/bin/env python

# Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE).
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
# \brief Test on watershed transform-based segmentation.

import data

import ltihooks

import xtc

import mln
import mln.io.pgm
import mln.morpho
import mln.morpho.closing
import mln.morpho.watershed

# Note that Python built-in types must be wrapped explicitly as
# Extatica objects using `xtc.data', because automatica conversion
# won't work from Python.

ima = mln.mk_image2d_int_u8()
mln.io.pgm.load(ima, xtc.data(data.lena))

# Gradient.
gradient = mln.morpho.gradient(ima, mln.win_c4p())
mln.io.pgm.save(gradient, xtc.data("morpho-segm-gradient.pgm"))
# Area closing of the gradient.
closed_gradient = mln.morpho.closing.area(ima, mln.c4(), xtc.data(50))
# Watershed transform.
nbasins = mln.value.mk_int_u8();
ws = mln.morpho.watershed.flooding(closed_gradient, mln.c4(), nbasins)
# FIXME: How to print nbasins?
mln.io.pgm.save(ws, xtc.data("morpho-segm-ws.pgm"))
