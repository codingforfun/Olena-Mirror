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
# \brief Test on image2d_int.

import data

import ltihooks

import xtc

import mln
import mln.data
import mln.debug
import mln.morpho

# Note that Python built-in types must be wrapped explicitly as
# Extatica objects using `xtc.data', because automatica conversion
# won't work from Python.

ima = mln.mk_image2d_int(xtc.data(3), xtc.data(3))
mln.data.fill(ima, xtc.data(42))

d = mln.domain(ima)
# FIXME: Could we get P from D instead?
p = mln.mk_box_fwd_piter_2d(d)

# FIXME: Try to turn this into a for loop (how? by creating an
# intermediate iterator?).
mln.start(p)
while mln.is_valid(p).to_bool():
  print "ima(" + str(p) + ") = " + str(mln.value_get(ima, p).to_int())
  mln.next(p)

# To execute an assignment equivalent to the following C++ code
#
#   ima.at_(1, 1) = 51;
#
# we cannot use this:
#
#   mln.value_get_(ima, xtc.data(1), xtc.data(1)).assign(xtc.data(51))
#
# The `mln.value_get_' call returns a *copy* of the (1, 1) cell in
# `ima' indeed, because the const version of mln::image2d<int>::at_ is
# called.  We cannot easily change this, because all xtc_* wrapping
# routines get their argument as const refs.
#
# So the easiest solution is to rely on a `value_set' routine,
# fortunately provided by the `xtc_mln' layer.
mln.value_set_(ima, xtc.data(1), xtc.data(1), xtc.data(51))

p = mln.mk_point2d (xtc.data(0), xtc.data(0))
mln.value_set(ima, p, xtc.data(51))


# Elementary erosion in 4-connectivity using win_c4p.
eroded = mln.morpho.erosion(ima, mln.win_c4p())
mln.debug.println(eroded)

# Erosion using a custom window.
w = mln.mk_window2d()
mln.insert(w, xtc.data(-1), xtc.data(0))
mln.insert(w, xtc.data(0),  xtc.data(0))
mln.insert(w, xtc.data(+1), xtc.data(0))
eroded = mln.morpho.erosion (ima, w)
mln.debug.println(eroded)
