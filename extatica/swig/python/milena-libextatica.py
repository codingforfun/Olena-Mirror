#! /usr/bin/env python

# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory (LRDE).
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
# \brief A Python version of test/test_milena.cc using libextatica (directly).

import ltihooks

import xtc
import mln_config_local

# Set up paths.
xtc.include_path(mln_config_local.milena_include_path)
xtc.include("mln/core/image/image2d.hh")
xtc.include("mln/data/fill.hh")
xtc.include("mln/debug/iota.hh")
xtc.include("mln/debug/println.hh")

# Instantiate functors.
mk_image2d_int = xtc.ctor("mln::image2d<int>")
fill = xtc.fun("mln::data::fill")
iota = xtc.fun("mln::debug::iota")
println = xtc.fun("mln::debug::println")

# Use them (by compiling them on-the-fly).

# We'd like to be able to write this:
#
#  ima = mk_image2d_int(3, 3)
#
# but we just can't.  `mk_image2d_int' only accept `xtc.data' as
# arguments, so we have to encapsulate integers in `xtc.data' objects
# (likewise for strings).

ima = mk_image2d_int(xtc.data(3), xtc.data(3))

fill(ima, xtc.data(0))
println(xtc.data("ima (before) ="), ima)
iota(ima)
println(xtc.data("ima (after) ="), ima)
