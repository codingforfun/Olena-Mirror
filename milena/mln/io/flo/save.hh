// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009, 2010, 2011, 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_IO_FLO_SAVE_HH
# define MLN_IO_FLO_SAVE_HH

/// \file
///
/// Define a function which saves an image of kind flo with given path.
/// Information about .flo can be found there :
/// http://vision.middlebury.edu/flow/data/

# define TAG_STRING "PIEH"

# include <stdio.h>
# include <stdlib.h>
# include <string>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/vec2d.hh>

namespace mln
{

  namespace io
  {

    namespace flo
    {

      /// Save vector field in a .flo file.
      ///
      /// \param[in] optical_flow A refernce to the image which contains the
      /// vector field
      /// \param[in] filename The destination.
      ///
      void
      save (const mln::image2d <mln::vec2d_f>& optical_flow,
	    const std::string& filename);

# ifndef MLN_INCLUDE_ONLY

      inline
      void
      save (const mln::image2d <mln::vec2d_f>& optical_flow,
	    const std::string& filename)
      {
	trace::entering("mln::io::flo::save");

	int width = optical_flow.ncols ();
	int height = optical_flow.nrows ();

	FILE *stream = fopen (filename.c_str (), "wb");
	if (!stream)
	  {
	    std::cerr << "error: can not write into '" << filename << "'.";
	    abort ();
	  }

	fprintf (stream, TAG_STRING);
	if ((fwrite (&width, sizeof(int), 1, stream) != 1)
	    || (fwrite (&height, sizeof(int), 1, stream) != 1))
	  {
	    std::cerr << "error: could not write header.";
	    abort ();
	  }

	mln::point2d p;
	for (p.row () = 0; p.row () < height; ++p.row ())
	  for (p.col () = 0; p.col () < width; ++p.col ())
	    {
	      float u = optical_flow (p)[0];
	      float v = optical_flow (p)[1];

	      if ((fwrite (&u, sizeof(float), 1, stream) != 1)
		  || (fwrite (&v, sizeof(float), 1, stream) != 1))
		{
		  std::cerr << "error while writing the " << p << "th point.";
		  abort ();
		}
	    }

	fclose (stream);

	trace::exiting("mln::io::flo::save");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::flo

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_FLO_SAVE_HH
