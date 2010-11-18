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
# \brief Python wrappers of some Milena routines.

from common import *

__all__ = ["config", "data", "debug", "io", "morpho", "value"]

# Get mln.value.mk_int_u8, etc.
import value

# -------------------------- #
# Wrappers of mln::image2d.  #
# -------------------------- #

xtc.include("mln/core/image/image2d.hh")

# mln::image2d<int>.
mk_image2d_int = xtc.ctor("mln::image2d<int>")

# mln::image2d<mln::value::int_u8>.
mk_image2d_int_u8 = xtc.ctor("mln::image2d<mln::value::int_u8>")

# --------------------------- #
# Wrappers of mln::window2d.  #
# --------------------------- #

xtc.include("mln/core/alias/window2d.hh")
win_c4p = xtc.fun("mln::win_c4p")

# --------------------------- #
# Wrappers of mln::neighb2d.  #
# --------------------------- #

xtc.include("mln/core/alias/neighb2d.hh")
c4 = xtc.fun("mln::c4")
