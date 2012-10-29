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
  fill_0_from_1_faces 				\
  fill_1_from_2_faces				\
  fill_0_from_primary_faces			\
  immerse					\
  immerse_with					\
  immerse_with_inner_border			\
  immerse_and_duplicate_2_to_1_faces		\
  immerse_and_duplicate_2_to_0_1_faces		\
  is_primary_face				\
  un_immerse

fill_0_from_1_faces_SOURCES = fill_0_from_1_faces.cc
fill_1_from_2_faces_SOURCES = fill_1_from_2_faces.cc
fill_0_from_primary_faces_SOURCES = fill_0_from_primary_faces.cc
immerse_SOURCES = immerse.cc
immerse_with_SOURCES = immerse_with.cc
immerse_with_inner_border_SOURCES = immerse_with_inner_border.cc
immerse_and_duplicate_2_to_1_faces_SOURCES = immerse_and_duplicate_2_to_1_faces.cc
immerse_and_duplicate_2_to_0_1_faces_SOURCES = immerse_and_duplicate_2_to_0_1_faces.cc
is_primary_face_SOURCES = is_primary_face.cc
un_immerse_SOURCES = un_immerse.cc

TESTS = $(check_PROGRAMS)
