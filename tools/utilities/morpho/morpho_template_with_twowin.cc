// Copyright (C) 2002,2003  EPITA Research and Development Laboratory
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

#include <oln/types/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/morpho/hit_or_miss.hh>
#include <oln/morpho/thinning.hh>
#include <oln/morpho/thickening.hh>
#include <iostream>

#include "getopt.hh"

using namespace oln;

#define _MAKE_STRING(S) # S
#define MAKE_STRING(S) _MAKE_STRING(S)

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
    { "help", no_argument, 0, 'h' },
    { "version", no_argument, 0, 'V' },
    { "win1", 1, 0, 'w' },
    { "win2", 1, 0, 'W' },
    { 0, 0, 0, 0 }
  };

void version(const char *progname)
{
  std::cout << progname << " for Olena " << OLN_PACKAGE_VERSION << std::endl
	    << std::endl
	    << "Copyright (C) 2002,2003  EPITA Research and Development Laboratory." << std::endl
	    << "This is free software; see the source for copying conditions.  There is NO" << std::endl
	    << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl
	    << std::endl
	    << "Written by Quôc Peyrot <chojin@lrde.epita.fr>" << std::endl;
  exit(0);
}  

void usage(const char * progname)
{
  std::cout << "'" << progname 
	    << "' calls Olena function morpho::" MAKE_STRING(FUNC) 
    "(" MAKE_STRING(IMAGE_TYPE<DATA_TYPE>) ", " MAKE_STRING(WINDOW_TYPE) ", " MAKE_STRING(WINDOW_TYPE) ")" << std::endl
	    << std::endl
	    << "Usage: " << progname << " [-w WINFILE1] [-W WINFILE2] INFILE OUTFILE" << std::endl
	    << std::endl
	    << " -w filename, --win1 filename     load a window from file [win_c8p]" << std::endl
	    << " -W filename, --win2 filename     load a window from file [win_c8p]" << std::endl
	    << std::endl
	    << "Example: " << progname << " -w my_win1.pbm -W my_win2.pbm lena.pgm output.pgm" << std::endl
	    << std::endl
	    << "Report bugs to <" OLN_PACKAGE_BUGREPORT ">." << std::endl;
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
        std::cerr << "Try `" << argv[0]
                  << " --help' for more information." << std::endl;
        exit (1);

      case 'h':
        usage(argv[0]);

      case 'V':
	version(argv[0]);

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

