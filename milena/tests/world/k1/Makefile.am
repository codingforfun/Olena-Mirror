# Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE).
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

include $(top_srcdir)/milena/tests/tests.mk

check_PROGRAMS =				\
  display_enlarged				\
  immerse					\
  immerse_and_duplicate_2_to_1_faces		\
  is_0_face					\
  is_0_or_1_face				\
  is_1_face					\
  is_1_face_vertical				\
  is_1_face_horizontal				\
  is_2_face					\
  is_primary_face

display_enlarged_SOURCES = display_enlarged.cc
immerse_SOURCES = immerse.cc
immerse_and_duplicate_2_to_1_faces_SOURCES = immerse_and_duplicate_2_to_1_faces.cc
is_0_face_SOURCES = is_0_face.cc
is_0_or_1_face_SOURCES = is_0_or_1_face.cc
is_1_face_SOURCES = is_1_face.cc
is_1_face_vertical_SOURCES = is_1_face_vertical.cc
is_1_face_horizontal_SOURCES = is_1_face_horizontal.cc
is_2_face_SOURCES = is_2_face.cc
is_primary_face_SOURCES = is_primary_face.cc

TESTS = $(check_PROGRAMS)
