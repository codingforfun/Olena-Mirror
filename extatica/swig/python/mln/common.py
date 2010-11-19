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
# \brief Common requirements for Extatica Python wrappers on Milena.

import os

import ltihooks
import xtc

# FIXME: We should use MILENA_INCLUDE_DIR instead of MILENA_DIR.
#
# By the way MILENA_DIR should point to the prefix of Milena, and
# MILENA_INCLUDE_DIR to the directory containing the (installed)
# `mln/' directory.

# Query the environment first for Milena's directory, then try to
# import `milena_dir' from `mln.config'.
if os.environ.has_key('MILENA_DIR'):
  milena_include_dir = os.environ['MILENA_DIR']
  extatica_include_dir = os.environ['EXTATICA_INCLUDE_DIR']
else:
  from mln.config import milena_include_dir, extatica_include_dir

# Set up paths.
xtc.include_dir(milena_include_dir)
xtc.include_dir(extatica_include_dir)
