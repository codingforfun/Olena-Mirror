include $(top_srcdir)/tests/check/Makefile.check

TESTS = $(check_PROGRAMS)

check_PROGRAMS = \
  8-nrgb-hsi \
  8-nrgb-hsl \
  8-nrgb-hsv \
  8-nrgb-xyz \
  8-nrgb-yiq \
  8-nrgb-yuv \
  8-rgb-nrgb \
  8-rgb-xyz

## Ideally, each _SOURCES should be defined as $(CONVSOURCES), with
## CONVSOURCES = convcomp.cc convcomp.hh
## but this triggers a bug in Automake 1.6.

8_nrgb_hsi_SOURCES = convcomp.cc convcomp.hh
8_nrgb_hsi_CXXFLAGS = -DSRC=nrgb -DDEST=hsi -DEM=17.8723 -DEV=1099.78

8_nrgb_hsl_SOURCES = convcomp.cc
8_nrgb_hsl_CXXFLAGS = -DSRC=nrgb -DDEST=hsl -DEM=5.90563 -DEV=152.583

8_nrgb_hsv_SOURCES = convcomp.cc
8_nrgb_hsv_CXXFLAGS = -DSRC=nrgb -DDEST=hsv -DEM=1.10193 -DEV=74.3924

8_nrgb_xyz_SOURCES = convcomp.cc
8_nrgb_xyz_CXXFLAGS = -DSRC=nrgb -DDEST=xyz -DEM=0.952448 -DEV=4.04194

8_nrgb_yiq_SOURCES = convcomp.cc
8_nrgb_yiq_CXXFLAGS = -DSRC=nrgb -DDEST=yiq -DEM=2.11554 -DEV=18.7004

8_nrgb_yuv_SOURCES = convcomp.cc
8_nrgb_yuv_CXXFLAGS = -DSRC=nrgb -DDEST=yuv -DEM=5.20401 -DEV=1.36447

8_rgb_nrgb_SOURCES = convcomp.cc
8_rgb_nrgb_CXXFLAGS = -DSRC=rgb -DDEST=nrgb -DEM=0.426287 -DEV=1.97667

8_rgb_xyz_SOURCES = convcomp.cc
8_rgb_xyz_CXXFLAGS = -DSRC=rgb -DDEST=xyz -DEM=0.770938 -DEV=0.34167
