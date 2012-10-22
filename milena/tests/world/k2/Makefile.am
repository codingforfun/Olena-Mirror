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
  fill_non_primary_from_primary_2_faces		\
  is_non_primary_2_face				\
  is_non_primary_2_face_center			\
  is_non_primary_2_face_horizontal		\
  is_non_primary_2_face_vertical		\
  is_primary_2_face				\
  immerse					\
  un_immerse

fill_non_primary_from_primary_2_faces_SOURCES = fill_non_primary_from_primary_2_faces.cc
is_non_primary_2_face_SOURCES = is_non_primary_2_face.cc
is_non_primary_2_face_center_SOURCES = is_non_primary_2_face_center.cc
is_non_primary_2_face_horizontal_SOURCES = is_non_primary_2_face_horizontal.cc
is_non_primary_2_face_vertical_SOURCES = is_non_primary_2_face_vertical.cc
is_primary_2_face_SOURCES = is_primary_2_face.cc
immerse_SOURCES = immerse.cc
un_immerse_SOURCES = un_immerse.cc

TESTS = $(check_PROGRAMS)
