// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_MAGICK_GET_HEADER_HH
# define MLN_IO_MAGICK_GET_HEADER_HH

/// \file
///
/// \brief Routines reading image headers based on Magick++.

# include <cstdlib>

# include <Magick++.h>

# include <mln/core/image/image2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>

# include <mln/io/magick/internal/init_magick.hh>


namespace mln
{

  namespace io
  {

    namespace magick
    {

      /*!
	\brief Store magick file header.
	\ingroup iomagick
      */
      struct magick_header
      {
	int height;
	int width;
	unsigned depth;
	Magick::ImageType image_type;
      };


      /*! \brief Get image header from a file using Magick++.

	  \param[in]  filename  The name of the input file.

	  \return A structure with several header information.

	  \ingroup iomagick
      */
      magick_header get_header(const std::string& filename);


# ifndef MLN_INCLUDE_ONLY


      inline
      magick_header get_header(const std::string& filename)
      {
	mln_trace("mln::io::magick::get_header");

	// Initialize GraphicsMagick only once.
	static internal::init_magick init;
	(void) init;

	// FIXME: Handle Magick++'s exceptions (see either
	// ImageMagick++'s or GraphicsMagick++'s documentation).
	Magick::Image magick_ima(filename);
	magick_ima.ping(filename);

	magick_header header;
	header.height = magick_ima.rows();
	header.width = magick_ima.columns();
	header.depth = magick_ima.depth();
	header.image_type = magick_ima.type();

	return header;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_GET_HEADER_HH
