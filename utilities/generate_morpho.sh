#!/bin/sh
# generate_toolbox.sh
#
# $Id: generate_morpho.sh 1.3.1.1 Wed, 04 Dec 2002 21:23:33 +0100 chojin $

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
#include <oln/morpho/gradient.hh>
#include <oln/morpho/top_hat.hh>
#include <iostream>
#include <getopt.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef WINDOW_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "win", 1, NULL, 'w' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-w filename]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-w filename, --win filename   : load a window from file"
            << ", if not specified use" << std::endl
            << "                                win_c8p by default" << std::endl;
  std::cerr << "filename_in                   : source file" << std::endl;
  std::cerr << "filename_out                  : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -w my_win.pbm"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  WINDOW_TYPE win = win_c8p();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hw:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try \`" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'w':
        win = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(morpho::FUNC(img_in, win), argv[optind + 1]);
  return 0;
}

EOF

# morpho_template_with_functor.cc
FILE=$DESTDIR/morpho_template_with_functor.cc
cat > $FILE <<EOF
#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/laplacian.hh>
#include <oln/morpho/top_hat.hh>
#include <iostream>
#include <getopt.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef WINDOW_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

#ifndef FUNCTOR
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "win", 1, NULL, 'w' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-w filename]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-w filename, --win filename   : load a window from file"
            << ", if not specified use" << std::endl
            << "                                win_c8p by default" << std::endl;
  std::cerr << "filename_in                   : source file" << std::endl;
  std::cerr << "filename_out                  : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -w my_win.pbm"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  WINDOW_TYPE win = win_c8p();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hw:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try \`" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'w':
        win = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(morpho::FUNC(FUNCTOR, img_in, win), argv[optind + 1]);
  return 0;
}

EOF

# morpho_template_with_twowin.cc
FILE=$DESTDIR/morpho_template_with_twowin.cc
cat > $FILE <<EOF
#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/hit_or_miss.hh>
#include <oln/morpho/thinning.hh>
#include <oln/morpho/thickening.hh>
#include <iostream>
#include <getopt.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef WINDOW_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "win1", 1, NULL, 'w' },
    { "win2", 1, NULL, 'W' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-w filename] [-W filename]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-w filename, --win1 filename   : load a window from file"
            << ", if not specified use" << std::endl
            << "                                win_c8p by default" << std::endl;
  std::cerr << "-W filename, --win2 filename   : load a window from file"
            << ", if not specified use" << std::endl
            << "                                win_c8p by default" << std::endl;
  std::cerr << "filename_in                    : source file" << std::endl;
  std::cerr << "filename_out                   : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -w my_win1.pbm -W my_win2.pbm"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  WINDOW_TYPE win1 = win_c8p();
  WINDOW_TYPE win2 = win_c8p();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hw:W:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try \`" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'w':
        win1 = load(optarg);

      case 'W':
        win2 = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(morpho::FUNC(img_in, win1, win2), argv[optind + 1]);
  return 0;
}

EOF

# morpho_template_watershed.cc
FILE=$DESTDIR/morpho_template_watershed.cc
cat > $FILE <<EOF
#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/watershed.hh>
#include <iostream>
#include <getopt.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef NEIGHBORHOOD_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "neighb", 1, NULL, 'n' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-n filename]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-n filename, --neighb filename : load a neighborhood from file,"
            << std::endl
            << "                                 if not specified use neighb_c4"
            << std::endl
            << "                                 by default" << std::endl;
  std::cerr << "filename_in                    : source file" << std::endl;
  std::cerr << "filename_out                   : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -n my_neighb.pbm"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  NEIGHBORHOOD_TYPE n = neighb_c4();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hn:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try \`" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'n':
        n = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(morpho::FUNC<DATA_TYPE>(img_in, n), argv[optind + 1]);
  return 0;
}

EOF

# morpho_template_gaussian.cc
FILE=$DESTDIR/morpho_template_gaussian.cc
cat > $FILE <<EOF
#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/convol/fast_gaussian.hh>
#include <iostream>
#include <getopt.h>
#include <stdlib.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "threshold", 1, NULL, 't' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-t threshold]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-t value, --threshold value : threshold value"
            << std::endl
            << "                              if not specified use 0.5"
            << std::endl
            << "                              by default" << std::endl;
  std::cerr << "filename_in                 : source file" << std::endl;
  std::cerr << "filename_out                : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -t 0.7"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  dfloat thres(0.5);

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "ht:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try \`" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 't':
        thres = dfloat(strtod(optarg, NULL));

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(convol::FUNC(img_in, thres), argv[optind + 1]);
  return 0;
}

EOF

# morpho_template_with_neighb.cc
FILE=$DESTDIR/morpho_template_with_neighb.cc
cat > $FILE <<EOF
#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/extrema.hh>
#include <iostream>
#include <getopt.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef NEIGHBORHOOD_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "neighb", 1, NULL, 'n' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-n filename]"
            << " filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-n filename, --neighb filename : load a neighborhood from file,"
            << std::endl
            << "                                 if not specified use neighb_c4"
            << std::endl
            << "                                 by default" << std::endl;
  std::cerr << "filename_in                    : source file" << std::endl;
  std::cerr << "filename_out                   : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -n my_neighb.pbm"
            << " lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  NEIGHBORHOOD_TYPE n = neighb_c4();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hn:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try \`" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'n':
        n = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 2) > argc)
    usage(argv[0]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind]);
  save(morpho::FUNC(img_in, n), argv[optind + 1]);
  return 0;
}

EOF

# morpho_template_extrema.cc
FILE=$DESTDIR/morpho_template_extrema.cc
cat > $FILE <<EOF
#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/extrema.hh>
#include <iostream>
#include <getopt.h>

using namespace oln;

#ifndef IMAGE_TYPE
# error
#endif

#ifndef NEIGHBORHOOD_TYPE
# error
#endif

#ifndef DATA_TYPE
# error
#endif

#ifndef FUNC
# error
#endif

static struct option long_options[] =
  {
    { "help", no_argument, NULL, 'h' },
    { "neighb", 1, NULL, 'n' },
    { NULL, 0, NULL, 0 }
  };


void usage(const char * progname)
{
  std::cerr << "usage:" << std::endl;
  std::cerr << progname
            << " [-n filename]"
            << " map filename_in filename_out"
            << std::endl << std::endl;
  std::cerr << "-n filename, --neighb filename : load a neighborhood from file,"
            << std::endl
            << "                                 if not specified use neighb_c4"
            << std::endl
            << "                                 by default" << std::endl;
  std::cerr << "map                            : extrema map file" << std::endl;
  std::cerr << "filename_in                    : source file" << std::endl;
  std::cerr << "filename_out                   : destination file" << std::endl;
  std::cerr << std::endl;
  std::cerr << "example: " << progname
            << " -n my_neighb.pbm"
            << " map.pbm lena.pgm lena_out.pgm"
            << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  NEIGHBORHOOD_TYPE n = neighb_c4();

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hn:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try \`" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'n':
        n = load(optarg);

      default:
        opt_index = 0;
        break;
      }

  if ((optind + 3) > argc)
    usage(argv[0]);
  IMAGE_TYPE<bin> img_map = load(argv[optind]);
  IMAGE_TYPE<DATA_TYPE> img_in = load(argv[optind+1]);
  save(morpho::FUNC(img_in, img_map, n), argv[optind + 2]);
  return 0;
}

EOF

DATA_TYPE_LIST="int_u8"
DIM_TYPE_LIST="2d"
FCT_LIST="erosion dilation opening closing\
          fast::erosion fast::dilation fast::opening fast::closing\
          fast::beucher_gradient fast::internal_gradient fast::external_gradient\
          white_top_hat black_top_hat self_complementary_top_hat\
          fast::white_top_hat fast::black_top_hat fast::self_complementary_top_hat"

FCT_LIST_TWOWIN="hit_or_miss fast::hit_or_miss\
                       thinning fast::thinning\
                       thickening fast::thickening"

FCT_LIST_WATERSHED="watershed_seg watershed_con"

FCT_LIST_GAUSSIAN="fast::gaussian\
                          fast::gaussian_derivative\
                          fast::gaussian_second_derivative"

FCT_LIST_NEIGHB="sure::regional_minima\
                       sequential::regional_minima\
                       hybrid::regional_minima"

FCT_LIST_EXTREMA="sure::minima_imposition\
                         sequential::minima_imposition\
                         hybrid::minima_imposition"

# Makefile.am
FILE=$DESTDIR/Makefile.am
cat > $FILE <<EOF

AM_CPPFLAGS = -I\$(top_builddir) -I\$(top_srcdir)
AM_CXXFLAGS = -Wall -W -ftemplate-depth-50 -ansi -pedantic

bin_PROGRAMS = \\
EOF



for dim in $DIM_TYPE_LIST; do
  for data in $DATA_TYPE_LIST; do
    for fct in $FCT_LIST; do
      echo -n "  ${fct/::/_}"_"$dim"_"$data" >> $FILE;
    done

    echo -n "  laplacian"_"$dim"_"$data" >> $FILE;
    echo -n "  fast_laplacian"_"$dim"_"$data" >> $FILE;
    echo -n "  top_hat_contrast_op"_"$dim"_"$data" >> $FILE;
    echo -n "  fast_top_hat_contrast_op"_"$dim"_"$data" >> $FILE;

    for fct in $FCT_LIST_TWOWIN; do
      echo -n "  ${fct/::/_}"_"$dim"_"$data" >> $FILE;
    done

    for fct in $FCT_LIST_WATERSHED; do
      echo -n "  ${fct/::/_}"_"$dim"_"$data" >> $FILE;
    done

    for fct in $FCT_LIST_GAUSSIAN; do
      echo -n "  ${fct/::/_}"_"$dim"_"$data" >> $FILE;
    done

    for fct in $FCT_LIST_NEIGHB; do
      echo -n "  ${fct/::/_}"_"$dim"_"$data" >> $FILE;
    done

    for fct in $FCT_LIST_EXTREMA; do
      echo -n "  ${fct/::/_}"_"$dim"_"$data" >> $FILE;
    done

  done
done

echo "" >> $FILE;
echo "" >> $FILE;

for dim in $DIM_TYPE_LIST; do
  for data in $DATA_TYPE_LIST; do
    for fct in $FCT_LIST; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template.cc" >> $FILE;
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='$fct'" >> $FILE;
      echo "" >> $FILE;
    done

    echo "laplacian"_"$dim"_"$data"_"SOURCES = morpho_template_with_functor.cc" >> $FILE;
    echo "laplacian"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='laplacian' -DFUNCTOR='convert::force<$data>()'" >> $FILE;
    echo "" >> $FILE;
    echo "fast_laplacian"_"$dim"_"$data"_"SOURCES = morpho_template_with_functor.cc" >> $FILE;
    echo "fast_laplacian"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='fast::laplacian' -DFUNCTOR='convert::force<$data>()'" >> $FILE;
    echo "" >> $FILE;

    echo "top_hat_contrast_op"_"$dim"_"$data"_"SOURCES = morpho_template_with_functor.cc" >> $FILE;
    echo "top_hat_contrast_op"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='top_hat_contrast_op' -DFUNCTOR='convert::bound<$data>()'" >> $FILE;
    echo "" >> $FILE;
    echo "fast_top_hat_contrast_op"_"$dim"_"$data"_"SOURCES = morpho_template_with_functor.cc" >> $FILE;
    echo "fast_top_hat_contrast_op"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='fast::top_hat_contrast_op' -DFUNCTOR='convert::bound<$data>()'" >> $FILE;
    echo "" >> $FILE;

    for fct in $FCT_LIST_TWOWIN; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_with_twowin.cc" >> $FILE;
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='$fct'" >> $FILE;
      echo "" >> $FILE;
    done

    for fct in $FCT_LIST_WATERSHED; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_watershed.cc" >> $FILE;
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DNEIGHBORHOOD_TYPE=neighborhood$dim -DFUNC='$fct'" >> $FILE;
      echo "" >> $FILE;
    done

    for fct in $FCT_LIST_GAUSSIAN; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_gaussian.cc" >> $FILE;
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DFUNC='$fct'" >> $FILE;
      echo "" >> $FILE;
    done

    for fct in $FCT_LIST_NEIGHB; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_with_neighb.cc" >> $FILE;
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DNEIGHBORHOOD_TYPE=neighborhood$dim -DFUNC='$fct'" >> $FILE;
      echo "" >> $FILE;
    done

    for fct in $FCT_LIST_EXTREMA; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_extrema.cc" >> $FILE;
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DNEIGHBORHOOD_TYPE=neighborhood$dim -DFUNC='$fct'" >> $FILE;
      echo "" >> $FILE;
    done

  done
done
