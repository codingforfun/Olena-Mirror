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

SUBDIRS = accu

check_PROGRAMS =				\
  fill_0_1_faces_internal_border		\
  fill_0_from_1_faces				\
  fill_0_from_2_faces				\
  fill_1_from_2_faces				\
  fill_1_from_aux_2_faces			\
  fill_2_from_1_faces				\
  holes						\
  immerse					\
  immerse_with_inner_border			\
  is_0_face					\
  is_0_or_1_face				\
  is_1_face					\
  is_1_face_vertical				\
  is_1_face_horizontal				\
  is_2_face					\
  level						\
  saturate					\
  un_immerse

fill_0_1_faces_internal_border_SOURCES = fill_0_1_faces_internal_border.cc
fill_0_from_1_faces_SOURCES = fill_0_from_1_faces.cc
fill_0_from_2_faces_SOURCES = fill_0_from_2_faces.cc
fill_1_from_2_faces_SOURCES = fill_1_from_2_faces.cc
fill_1_from_aux_2_faces_SOURCES = fill_1_from_aux_2_faces.cc
fill_2_from_1_faces_SOURCES = fill_2_from_1_faces.cc
holes_SOURCES = holes.cc
immerse_SOURCES = immerse.cc
immerse_with_inner_border_SOURCES = immerse_with_inner_border.cc
is_0_face_SOURCES = is_0_face.cc
is_0_or_1_face_SOURCES = is_0_or_1_face.cc
is_1_face_SOURCES = is_1_face.cc
is_1_face_vertical_SOURCES = is_1_face_vertical.cc
is_1_face_horizontal_SOURCES = is_1_face_horizontal.cc
is_2_face_SOURCES = is_2_face.cc
level_SOURCES = level.cc
saturate_SOURCES = saturate.cc
un_immerse_SOURCES = un_immerse.cc


TESTS = $(check_PROGRAMS)
