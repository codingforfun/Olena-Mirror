## Process this file through Automake to create Makefile.in -*- Makefile -*-

## Copyright (C) 2002, 2003, 2004  EPITA Research and Development
## Laboratory
##
## This file is part of the Olena Library.  This library is free
## software; you can redistribute it and/or modify it under the terms
## of the GNU General Public License version 2 as published by the
## Free Software Foundation.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this library; see the file COPYING.  If not, write to
## the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
## Boston, MA 02110-1301, USA.
##
## As a special exception, you may use this file as part of a free
## software library without restriction.  Specifically, if other files
## instantiate templates or use macros or inline functions from this
## file, or you compile this file and link it with other files to
## produce an executable, this file does not by itself cause the
## resulting executable to be covered by the GNU General Public
## License.  This exception does not however invalidate any other
## reasons why the executable file might be covered by the GNU General
## Public License.


include ../check/Makefile.flags
include ../check/Makefile.check

check_PROGRAMS = \
  8-rgb-hsi \
  8-rgb-hsl \
  8-rgb-hsv \
  8-rgb-xyz \
  8-rgb-yiq \
  8-rgb-yuv \
  8-rgb-nrgb \
  8-rgb-xyz  \
  f-8-rgb-hsi \
  f-8-rgb-hsl \
  f-8-rgb-hsv \
  f-8-rgb-xyz \
  f-8-rgb-yiq \
  f-8-rgb-yuv \
  f-8-rgb-nrgb \
  f-8-rgb-xyz


# XFAIL_TESTS = 8-rgb-nrgb$(EXEEXT)

## FIXME: Ideally, each _SOURCES should be defined as $(CONVSOURCES), with
## CONVSOURCES = convcomp.cc convcomp.hh
## but this triggers a bug in Automake 1.6.

8_rgb_hsi_SOURCES = conv.cc conv.hh
8_rgb_hsi_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=hsi -DEM=5

8_rgb_hsl_SOURCES = conv.cc conv.hh
8_rgb_hsl_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=hsl -DEM=5

8_rgb_hsv_SOURCES = conv.cc conv.hh
8_rgb_hsv_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=hsv -DEM=3

8_rgb_xyz_SOURCES = conv.cc conv.hh
8_rgb_xyz_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=xyz -DEM=5

8_rgb_yiq_SOURCES = conv.cc conv.hh
8_rgb_yiq_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=yiq -DEM=6

8_rgb_yuv_SOURCES = conv.cc conv.hh
8_rgb_yuv_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=yuv -DEM=6

8_rgb_nrgb_SOURCES = conv.cc conv.hh
8_rgb_nrgb_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=nrgb -DEM=6

# 8_rgb_xyz_SOURCES = conv.cc conv.hh
# 8_rgb_xyz_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=xyz -DEM=4

f_8_rgb_hsi_SOURCES = convcomp.cc convcomp.hh
f_8_rgb_hsi_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=hsi -DEM=17.8723 -DEV=1099.78

f_8_rgb_hsl_SOURCES = convcomp.cc convcomp.hh
f_8_rgb_hsl_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=hsl -DEM=5.90563 -DEV=152.583

f_8_rgb_hsv_SOURCES = convcomp.cc convcomp.hh
f_8_rgb_hsv_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=hsv -DEM=1.10193 -DEV=74.3924

# f_8_nrgb_xyz_SOURCES = convcomp.cc convcomp.hh
# f_8_nrgb_xyz_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=nrgb -DDEST=xyz -DEM=0.952448 -DEV=4.04194

f_8_rgb_yiq_SOURCES = convcomp.cc convcomp.hh
f_8_rgb_yiq_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=yiq -DEM=2.11554 -DEV=18.7004

f_8_rgb_yuv_SOURCES = convcomp.cc convcomp.hh
f_8_rgb_yuv_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=yuv -DEM=5.20401 -DEV=1.36447

f_8_rgb_nrgb_SOURCES = convcomp.cc convcomp.hh
f_8_rgb_nrgb_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=nrgb -DEM=0.692094 -DEV=0.329154

f_8_rgb_xyz_SOURCES = convcomp.cc convcomp.hh
f_8_rgb_xyz_CPPFLAGS = $(AM_CPPFLAGS) -DSRC=rgb -DDEST=xyz -DEM=0.865949 -DEV=0.356157
