#!/bin/sh
# generate_toolbox.sh
#
# $Id: generate_morpho.sh 1.2 Mon, 02 Dec 2002 20:22:25 +0100 chojin $

# Global variables
DESTDIR=morpho

# morpho_template.cc
FILE=$DESTDIR/morpho_template.cc
cat > $FILE <<EOF
#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/erosion.hh>
#include <oln/morpho/dilation.hh>
#include <oln/morpho/opening.hh>
#include <oln/morpho/closing.hh>
#include <oln/morpho/laplacian.hh>
#include <oln/morpho/gradient.hh>
#include <oln/morpho/top_hat.hh>
#include <oln/morpho/hit_or_miss.hh>
#include <oln/morpho/watershed.hh>
#include <oln/morpho/thinning.hh>
#include <oln/morpho/thickening.hh>
#include <oln/convol/fast_gaussian.hh>
#include <oln/level/fill.hh>
#include <iostream>

using namespace oln;

#ifndef DATA_TYPE
# error
#endif

#ifndef IMG_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " filename_in filename_out"
            << std::endl;
  std::cerr << "\tex: " << progname
            << " lena.pgm lena_ero.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  if (argc != 3)
    usage(argv[0]);
  IMG_TYPE<DATA_TYPE> img_in = load(argv[1]);
  save(morpho::FUNC(img_in, win_c8p()), argv[2]);
  return 0;
}

EOF

# Makefile.am
FILE=$DESTDIR/Makefile.am
cat > $FILE <<EOF

AM_CPPFLAGS = -I\$(top_builddir) -I\$(top_srcdir)
AM_CXXFLAGS = -Wall -W -ftemplate-depth-50 -ansi -pedantic

bin_PROGRAMS = \
    erosion2d_intu8 \
    dilation2d_intu8 \
    opening2d_intu8 \
    closing2d_intu8 \
    fast_erosion2d_intu8 \
    fast_dilation2d_intu8 \
    fast_opening2d_intu8 \
    fast_closing2d_intu8 \
    fast_beucher_gradient2d_intu8 \
    fast_internal_gradient2d_intu8 \
    fast_external_gradient2d_intu8 \
    white_top_hat2d_intu8 \
    black_top_hat2d_intu8 \
    self_complementary_top_hat2d_intu8 \
    fast_white_top_hat2d_intu8 \
    fast_black_top_hat2d_intu8 \
    fast_self_complementary_top_hat2d_intu8

erosion2d_intu8_SOURCES = morpho_template.cc
erosion2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC=erosion
dilation2d_intu8_SOURCES = morpho_template.cc
dilation2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC=dilation
opening2d_intu8_SOURCES = morpho_template.cc
opening2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC=opening
closing2d_intu8_SOURCES = morpho_template.cc
closing2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC=closing

fast_erosion2d_intu8_SOURCES = morpho_template.cc
fast_erosion2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::erosion'
fast_dilation2d_intu8_SOURCES = morpho_template.cc
fast_dilation2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::dilation'
fast_opening2d_intu8_SOURCES = morpho_template.cc
fast_opening2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::opening'
fast_closing2d_intu8_SOURCES = morpho_template.cc
fast_closing2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::closing'

fast_beucher_gradient2d_intu8_SOURCES = morpho_template.cc
fast_beucher_gradient2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::beucher_gradient'
fast_internal_gradient2d_intu8_SOURCES = morpho_template.cc
fast_internal_gradient2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::internal_gradient'
fast_external_gradient2d_intu8_SOURCES = morpho_template.cc
fast_external_gradient2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::external_gradient'

white_top_hat2d_intu8_SOURCES = morpho_template.cc
white_top_hat2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC=white_top_hat
black_top_hat2d_intu8_SOURCES = morpho_template.cc
black_top_hat2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC=black_top_hat
self_complementary_top_hat2d_intu8_SOURCES = morpho_template.cc
self_complementary_top_hat2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC=self_complementary_top_hat

fast_white_top_hat2d_intu8_SOURCES = morpho_template.cc
fast_white_top_hat2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::white_top_hat'
fast_black_top_hat2d_intu8_SOURCES = morpho_template.cc
fast_black_top_hat2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::black_top_hat'
fast_self_complementary_top_hat2d_intu8_SOURCES = morpho_template.cc
fast_self_complementary_top_hat2d_intu8_CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMG_TYPE=image2d -DFUNC='fast::self_complementary_top_hat'

EOF
