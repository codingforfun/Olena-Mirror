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
# \brief Python wrappers of Milena's mln::morpho::elementary.

from mln.common import *

xtc.include("mln/morpho/elementary/closing.hh")
closing = xtc.fun("mln::morpho::elementary::closing")

xtc.include("mln/morpho/elementary/opening.hh")
opening = xtc.fun("mln::morpho::elementary::opening")
