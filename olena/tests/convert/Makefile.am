## Process this file through Automake to create Makefile.in -*- Makefile -*-

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
