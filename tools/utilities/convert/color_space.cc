//                                                                  -*- c++ -*-
// Copyright (C) 2004  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

//
// How to add a new color space (ex:hsi)?
//
// 1) Add header	"#include <oln/convert/rgbhsi.hh>"
// 2) Add		"FROM_X_TO_RGB(hsi);"
//			"FROM_RGB_TO_X(hsi);"
// 3) Add		"else if (cs_in == "hsi")
//			     r = from_x_to_rgb(copy_to_x<hsi_8>(i));"
// 4) Add		"else if (cs_out == "hsi")
//			     save(from_rgb_to_x<hsi_8>(r),  f_out);"
// 3) update help	usage()
//

#include <oln/basics2d.hh>
#include <ntg/color.hh>
#include <oln/convert/rgbhsi.hh>
#include <oln/convert/rgbhsl.hh>
#include <oln/convert/rgbhsv.hh>
#include <oln/convert/rgbnrgb.hh>
#include <oln/convert/rgbxyz.hh>
#include <oln/convert/rgbyiq.hh>
#include <oln/convert/rgbyuv.hh>
#include <getopt.hh>

using namespace oln;
using namespace ntg;

static struct option long_options[] =
  {
    { "help", no_argument, 0, 'h' },
    { "version", no_argument, 0, 'V' },
    { 0, 0, 0, 0 }
  };



//! Help and exit
void
usage(const char *pg)
{
  std::cout << "Usage      : " << pg
	    << " FileIn.ppm FileOut.ppm Color_space_in Color_space_out"
	    << std::endl
	    << "Example    : `" << pg
	    << " ../../../olena/img/lena.ppm lena_hsl.ppm rgb hsl'"
	    << std::endl
	    << "Color space: hsi hsl hsv nrgb xyz yiq yuv rgb"
	    << std::endl
	    << "Note       : every conversion goes through the rgb"
	    << "color space."
    	    << std::endl
	    <<  "Report bugs to <" OLN_PACKAGE_BUGREPORT ">." << std::endl;
  exit(1);
}

void
version(const char *pg)
{
  std::cout << pg << " for Olena " << OLN_PACKAGE_VERSION << std::endl
	    << std::endl
	    << "Copyright (C) 2004  EPITA Research and Development Laboratory."
	    << std::endl
	    << "This is free software; see the source for copying conditions."
	    << "  There is NO"
	    << std::endl
	    << "warranty; not even for MERCHANTABILITY or FITNESS FOR A"
	    <<" PARTICULAR PURPOSE."
	    << std::endl << std::endl
	    << "Written by Niels Van Vliet <niels@lrde.epita.fr>" << std::endl;
  exit(0);
}


//! cast an rgb_8 image to a x image
template<class D, class I>
typename oln::mute<I, D>::ret
copy_to_x(const oln::abstract::vectorial_image<I>	&i)
{
  //Not very clean.
  assert(ntg_nb_comp(oln_value_type(I)) == 3);
  typename oln::mute<I, D>::ret				out(i.size());

  oln_iter_type(I)					it(i);
  for_all(it)
    for (unsigned c = 0; c < 3; ++c)
      out[it][c] = i[it][c];
  return out;
}



template<class T>
oln::image2d<ntg::rgb_8>
from_x_to_rgb(const oln::image2d<T>			&in)
{
  assert(0);
}

#define FROM_X_TO_RGB(T)						\
  template<>								\
  oln::image2d<ntg::rgb_8>						\
  from_x_to_rgb<ntg::T##_8>(const oln::image2d<ntg::T##_8>&in)		\
  {									\
    return apply(oln::convert::f_##T##_to_rgb<8, 8>(), in);		\
  }


template<class T>
oln::image2d<T>
from_rgb_to_x(const oln::image2d<ntg::rgb_8>		&in)
{
  assert(0);
}

#define FROM_RGB_TO_X(T)						\
  template<>								\
  oln::image2d<ntg::T##_8>						\
  from_rgb_to_x<ntg::T##_8>(const oln::image2d<ntg::rgb_8>&in)		\
  {									\
    return apply(oln::convert::f_rgb_to_##T<8, 8>(), in);		\
  }


FROM_X_TO_RGB(hsi)
FROM_RGB_TO_X(hsi)
FROM_X_TO_RGB(hsl)
FROM_RGB_TO_X(hsl)
FROM_X_TO_RGB(hsv)
FROM_RGB_TO_X(hsv)
FROM_X_TO_RGB(nrgb)
FROM_RGB_TO_X(nrgb)
FROM_X_TO_RGB(xyz)
FROM_RGB_TO_X(xyz)
FROM_X_TO_RGB(yiq)
FROM_RGB_TO_X(yiq)
FROM_X_TO_RGB(yuv)
FROM_RGB_TO_X(yuv)




using namespace oln;
using namespace ntg;
int
main(int		argc,
     char		**argv)
{

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hV:", long_options, &opt_index)) != -1)
    switch (c)
      {
      case ':':
      case '?':
        std::cerr << "Try `" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'V':
	version(argv[0]);

      default:
        opt_index = 0;
        break;
      }


  if ((optind + 4) != argc)
      usage(argv[0]);

  const std::string	f_in	(argv[optind]);
  const std::string	f_out	(argv[optind + 1]);
  const std::string	cs_in	(argv[optind + 2]);
  const std::string	cs_out	(argv[optind + 3]);

  image2d<rgb_8>	i	(f_in);
  if (!i.has_impl())
    {
      std::cout << "File not found: "<< f_in << std::endl;
      usage(argv[0]);
    }

  if (cs_in == cs_out)
    {
      save(i, f_out);
      exit(0);
    }

  // First the image is cast into the input type given by the user, and
  // converted to rgb.
  image2d<rgb_8>	r;
  if (cs_in == "hsi")
    r = from_x_to_rgb(copy_to_x<hsi_8>(i));
  else if (cs_in == "hsl")
    r = from_x_to_rgb(copy_to_x<hsl_8>(i));
  else if (cs_in == "hsv")
    r = from_x_to_rgb(copy_to_x<hsv_8>(i));
  else if (cs_in == "nrgb")
    r = from_x_to_rgb(copy_to_x<nrgb_8>(i));
  else if (cs_in == "xyz")
    r = from_x_to_rgb(copy_to_x<xyz_8>(i));
  else if (cs_in == "yiq")
    r = from_x_to_rgb(copy_to_x<yiq_8>(i));
  else if (cs_in == "yuv")
    r = from_x_to_rgb(copy_to_x<yuv_8>(i));
  else if (cs_in == "rgb")
    r = i;
  else
    {
      std::cout << "Format not supported: " << cs_in << std::endl;
      usage(argv[0]);
    }

  // Then the rgb image is converted to the output type given by the user.
  if (cs_out == "hsi")
    save(from_rgb_to_x<hsi_8>(r),  f_out);
  else if (cs_out == "hsl")
    save(from_rgb_to_x<hsl_8>(r),  f_out);
  else if (cs_out == "hsv")
    save(from_rgb_to_x<hsv_8>(r),  f_out);
  else if (cs_out == "nrgb")
    save(from_rgb_to_x<nrgb_8>(r),  f_out);
  else if (cs_out == "xyz")
    save(from_rgb_to_x<xyz_8>(r),  f_out);
  else if (cs_out == "yiq")
    save(from_rgb_to_x<yiq_8>(r),  f_out);
  else if (cs_out == "yuv")
    save(from_rgb_to_x<yuv_8>(r),  f_out);
  else if (cs_out == "rgb")
    save(r, f_out);
  else
    {
      std::cout << "Format not supported: " << cs_out << std::endl;
      usage(argv[0]);
    }
}
