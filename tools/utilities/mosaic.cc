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
#include <oln/level/fill.hh>
#include <iostream>
#include "getopt.hh"

using namespace oln;
using namespace ntg;

#ifndef TYPE
# define TYPE int_u16
#endif

/* The extra indirection to the _STR macro is required so that if the
   argument of STR() is itself macro, it will be expanded before being
   quoted.  */
#ifndef STR
# define _STR(arg)     #arg
# define STR(arg)      _STR(arg)
#endif

#define TYPE_STR STR(TYPE)

unsigned step = 1;		// Output a slice every `STEP' slices.
unsigned padding = 0;	    // Dimension of border between each slice.
coord cols = 0;		    // Number of columns of slices (0 = AUTO).
coord rows = 0;		       // number of rows of slices (0 = AUTO).
coord first = 0;		// First slice to output.
coord last = (~0U) >> 1;	// Last slice to output.
unsigned padding_color = 0;	// Color for padding.
bool padding_color_given = false; // whether padding_color has be set ot not.

const struct option long_options[] = {
  { "border",		required_argument,	NULL,	'b' },
  { "border-color",	required_argument,	NULL,	'B' },
  { "columns",		required_argument,	NULL,	'c' },
  { "first",		required_argument,	NULL,	'f' },
  { "help",		no_argument,		NULL,	'h' },
  { "last",		required_argument,	NULL,	'l' },
  { "rows",		required_argument,	NULL,	'r' },
  { "step",		required_argument,	NULL,	's' },
  { "version",          no_argument,            NULL,   'V' }
};

static void
version(const char *name)
{
  std::cout << name << " for Olena " << OLN_PACKAGE_VERSION << std::endl
	    << std::endl
	    << "Copyright (C) 2001,2002,2003  EPITA Research and Development Laboratory." << std::endl
	    << "This is free software; see the source for copying conditions.  There is NO" << std::endl
	    << "warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE." << std::endl
	    << std::endl
	    << "Written by Quôc Peyrot <chojin@lrde.epita.fr>" << std::endl;
  exit(0);
}

static void
usage(const char *name, int status)
{
  if (status) {
    std::cerr << "Try '" << name << " --help' for more information." << std::endl;
    exit(status);
  }

  std::cout << "'" << name << "' saves an image3d<" TYPE_STR "> as an image2d<" TYPE_STR "> mosaic of slices." << std::endl
	    << std::endl
	    << "Usage: " << name << "[OPTION]... 3DIMAGE" << std::endl
	    << std::endl
	    << "  -h, --help                  display this help" << std::endl
	    << "  -V, --version               display program version" << std::endl
	    << std::endl
	    << "  -b, --border=INTEGER	      size of padding between each slice [0]" << std::endl
	    << "  -B, --border-color=INTEGER  value used for padding" << std::endl
	    << std::endl
	    << "  -c, --columns=INTEGER	      number of slices per row" << std::endl
	    << "  -r, --rows=INTEGER          number of slices per columns" << std::endl
	    << std::endl
	    << "  -f, --first=INTEGER	      first slice to consider" << std::endl
	    << "  -l, --last=INTEGER	      last slice to consider" << std::endl
	    << "  -s, --step=INTEGER	      slice increment (-s3 will output 1 slice every" << std::endl
	    << "                                3 slices)" << std::endl
	    << std::endl
	    << "Mandatory arguments to long options are mandatory for short options too." << std::endl
	    << std::endl
	    << "Report bugs to <" OLN_PACKAGE_BUGREPORT ">." << std::endl;
  exit(status);
}

static void
decode_switches(int argc, char *argv[])
{
  int c;
  while ((c = getopt_long(argc, argv,
			  "b:B:c:f:hl:r:s:V", long_options, NULL)) != -1)
    {
      switch (c)
	{
	case 'b':
	  padding = atoi(optarg);
	  break;
	case 'B':
	  padding_color = atoi(optarg);
	  padding_color_given = true;
	  break;
	case 'c':
	  cols = atoi(optarg);
	  break;
	case 'f':
	  first = atoi(optarg);
	  break;
	case 'h':
	  usage (argv[0], 0);
	  break;
	case 'l':
	  last = atoi(optarg);
	  break;
	case 'r':
	  rows = atoi(optarg);
	  break;
	case 's':
	  step = atoi(optarg);
	  break;
	case 'V':
	  version (argv[0]);
	  break;
	default:
	  usage (argv[0], 1);
	}
    }
}

int
main(int argc, char *argv[])
{
  decode_switches(argc, argv);

  if (optind + 2 != argc)
    {
      std::cerr << ((optind + 2 < argc) ? "Too many filenames." : "Missing filename.") << std::endl;
      usage(argv[0], 1);
    }

  std::cout << "Loading " << argv[optind] << "..." << std::endl;
  image3d<TYPE> input = load(argv[optind]);
  std::cout << argv[optind] << " has "
	    << input.nslices() << " slices, "
	    << input.nrows() << " rows, "
	    << input.ncols() << " cols." << std::endl;

  if (last >= input.nslices())
    last = input.nslices() - 1;
  if (first > last)
    first = last;
  if (step == 0)
    step = 1;

  coord nslices_to_output = (last - first) / step + 1;

  if (rows == 0 && cols == 0)
    {
      rows = (unsigned)roundf(sqrtf((float)nslices_to_output /* * 3.0/4.0 */));
      cols = nslices_to_output / rows;
      // Due to rounding we may lack space for a few slices.  So add
      // another row or column (we prefer to add a column).
      if (rows * cols < nslices_to_output)
	if (rows * (cols + 1) >= nslices_to_output)
	  ++cols;
	else
	  ++rows;
    }
  else if (cols == 0)
    {
      cols = nslices_to_output / rows;
      if (rows * cols < nslices_to_output)
	++cols;
    }
  else if (rows == 0)
    {
      rows = nslices_to_output / cols;
      if (rows * cols < nslices_to_output)
	++rows;
    }
  else if (rows * cols < nslices_to_output)
    {
      nslices_to_output = rows * cols;
      last = first + step * (nslices_to_output - 1);
    }

  assert(rows * cols >= nslices_to_output);

  std::cout << "Formating " << nslices_to_output << " slices from "
	    << first << " to " << last
	    << ", using " << rows << " rows and " << cols << " columns..."
	    << std::endl;

  // Prepare output image.

  image2d<TYPE> output(rows * input.nrows() + (rows - 1) * padding,
		       cols * input.ncols() + (cols - 1) * padding);


  if (padding_color_given)
    {
      level::fill(output, padding_color);
    }
  else
    {
      image2d<TYPE>::iter_type p(output);
      for_all(p)
	{
	  if (((p.row() / 8) ^ (p.col() / 8)) & 1)
	    output[p] = ntg_min_val(TYPE);
	  else
	    output[p] = ntg_max_val(TYPE);
	}
    }

  // Fill in the mosaic.

  coord s = first;
  coord inr = input.nrows();
  coord inc = input.ncols();
  for (coord r = 0; r < rows; ++r)
    for (coord c = 0; c < cols; ++c)
      {
	coord orow = r * (inr + padding);
	coord ocol = c * (inc + padding);
	for (coord ir = 0; ir < inr; ++ir)
	  memcpy(&output.data()->at(orow + ir, ocol),
		 &input.data()->at(s, ir, 0),
		 inc * sizeof(TYPE));

	s += step;
	if (s > last)
	  goto exitloops;
      }
 exitloops:

  std::cout << "Saving " << argv[optind+1] << "..." << std::endl;
  save(output, argv[optind+1]);
}
