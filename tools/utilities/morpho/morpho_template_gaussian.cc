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

#include <ntg/all.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/convol/fast_gaussian.hh>
#include <iostream>
#include <stdlib.h>

#include "getopt.hh"

using namespace oln;

#define _MAKE_STRING(S) # S
#define MAKE_STRING(S) _MAKE_STRING(S)

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
    { "help", no_argument, 0, 'h' },
    { "version", no_argument, 0, 'V' },
    { "threshold", 1, 0, 't' },
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
    "(" MAKE_STRING(IMAGE_TYPE<DATA_TYPE>) ", theshold)" << std::endl
	    << std::endl
	    << "Usage: " << progname << " [-t THRESHOLD] INFILE OUTFILE" << std::endl
	    << std::endl
	    << " -t value, --theshold value    set the theshold value [0.5]" << std::endl
	    << std::endl
	    << "Example: " << progname << " -t 0.7 lena.pgm output.pgm" << std::endl
	    << std::endl
	    << "Report bugs to <" OLN_PACKAGE_BUGREPORT ">." << std::endl;
  exit(1);
}

int main(int argc, char *argv[])
{
  dfloat thres(0.5);

  int c;
  int opt_index = 0;
  while ((c = getopt_long (argc, argv, "hVt:", long_options, &opt_index)) != -1)
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

      case 't':
        thres = dfloat(strtod(optarg, 0));

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

