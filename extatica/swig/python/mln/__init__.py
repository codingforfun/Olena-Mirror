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

## Note: passing the headers as second argument like this:
##
##   ctor mk_down_int("down<int>", "my_lib/damien.hh");
##   ctor mk_boo("my_lib::boo", "my_lib/lib.hh");
##   meth print("print", "my_lib/lib.hh:my_lib/damien.hh");
##
## might be better (and probably lead to faster compilation times).
##
## There is a drawback though: when headers are passed as a second
## argument as in the above examples, it seems headers declared with
## `xtc.include' are ignored.  To be checked.
##
## Careful: we probably still want to inject headers of data types
## (images types, etc.) through `xtc.include', so that they are visible
## to routines wrapped later.

# -------------------------- #
# Wrappers of mln::image2d.  #
# -------------------------- #

xtc.include("mln/core/image/image2d.hh")

# mln::image2d<int>.
mk_image2d_int = xtc.ctor("mln::image2d<int>")

# mln::image2d<mln::value::int_u8>.
mk_image2d_int_u8 = xtc.ctor("mln::image2d<mln::value::int_u8>")

# Methods.
domain = xtc.meth("domain")
value_get = xtc.meth("operator()")
value_get_ = xtc.meth("at_")

# -------------------------- #
# Wrappers of mln::point2d.  #
# -------------------------- #

mk_point2d = xtc.ctor("mln::point2d", "mln/core/alias/point2d.hh")

# ------------------------------------------ #
# Wrappers of mln::box_piter<mln::point2d>.  #
# ------------------------------------------ #

mk_box_fwd_piter_2d = \
  xtc.ctor("mln::box_fwd_piter_<mln::point2d>",
           "mln/core/site_set/box_piter.hh:mln/core/alias/point2d.hh")

# Methods.
start = xtc.meth("start", "mln/core/site_set/box_piter.hh")
next = xtc.meth("next", "mln/core/site_set/box_piter.hh")
is_valid = xtc.meth("is_valid", "mln/core/site_set/box_piter.hh")

# --------------------------- #
# Wrappers of mln::window2d.  #
# --------------------------- #

xtc.include("mln/core/alias/window2d.hh")
mk_window2d = xtc.ctor("mln::window2d")
win_c4p = xtc.fun("mln::win_c4p")

# Methods.
insert = xtc.meth("insert", "mln/core/alias/window2d.hh")

# --------------------------- #
# Wrappers of mln::neighb2d.  #
# --------------------------- #

xtc.include("mln/core/alias/neighb2d.hh")
c4 = xtc.fun("mln::c4")

# --------- #
# xtc_mln.  #
# --------- #

# Additional Milena routines for Extatica, provided by the xtc_mln
# layer.
xtc.include("xtc_mln/value_set.hh")
value_set = xtc.fun("xtc_mln::value_set")
value_set_ = xtc.fun("xtc_mln::value_set_")
