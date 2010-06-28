// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_IO_MAGICK_SAVE_HH
# define MLN_IO_MAGICK_SAVE_HH

/// \file
///
/// Define a function which saves an image of kind magick with
/// given path.
///
/// \todo At the moment it works; is it a miracle?

# include <mln/core/image/image2d.hh>
# include <mln/metal/equal.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/qt/rgb32.hh>
# include <Magick++.h>


namespace mln
{

  namespace io
  {

    namespace magick
    {

      /*! Save a Milena image in a magick image.
       *
       * \param[out] ima A reference to the image to save.
       * \param[in] filename The output.
       */
      template <typename I>
      void save(const Image<I>& ima,
		const std::string& filename);

      /*! Save a Milena tiled image in a magick image.
       *
       * \param[out] ima A reference to the image to save.
       * \param[in] filename The output.
       */
      /*template <typename T>
      void save(Image< tiled2d<T> >& ima,
		const std::string& filename);*/


# ifndef MLN_INCLUDE_ONLY

      inline
      Magick::Color get_color(bool value)
      {
	return Magick::ColorMono(value);
      }

      // Gray values must be between 0 and 1.
      inline
      Magick::Color get_color(const value::int_u8& value)
      {
	return Magick::ColorGray(value / 255.0f);
      }

      // Color values must be between 0 and 1.
      inline
      Magick::Color get_color(const value::rgb8& value)
      {
	return Magick::ColorRGB(value.red() / 255.0f,
				value.green() / 255.0f,
				value.blue() / 255.0f);
      }

      // Color values must be between 0 and 1.
      inline
      Magick::Color get_color(const value::qt::rgb32& value)
      {
	return Magick::ColorRGB(value.red() / 255.0f,
				value.green() / 255.0f,
				value.blue() / 255.0f);
      }

      template <typename I>
      inline
      void save(const Image<I>& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::save");

	mln_precondition(mln_site_(I)::dim == 2);
	const I& ima = exact(ima_);
	if (!(mln::metal::equal<mln_value(I), bool>::value ||
	    mln::metal::equal<mln_value(I), value::int_u8>::value ||
	    mln::metal::equal<mln_value(I), value::rgb8>::value ||
	      mln::metal::equal<mln_value(I), value::qt::rgb32>::value))
	{
	  std::cerr << "error: trying to save an unsupported format" << std::endl;
	  std::cerr << "supported formats: binary, 8bits grayscale (int_u8), 8bits truecolor (rgb8)" << std::endl;
	  abort();
	}

	Magick::Image im_file;
	im_file.size(Magick::Geometry(ima.ncols(), ima.nrows()));

	im_file.modifyImage();

        Magick::PixelPacket *
	  pp = im_file.getPixels(0,0, ima.ncols(), ima.nrows());

	mln_site(I) pmin = ima.domain().pmin();
	mln_piter(I) p(ima.domain());
	for_all(p)
	  *pp++ = get_color(ima(p));

	im_file.syncPixels();

	im_file.write(filename);

	trace::exiting("mln::io::magick::save");
      }

      /*template <typename T>
      void save(Image< tiled2d<T> >& ima_, const std::string& filename)
      {
	trace::entering("mln::io::magick::save");

	tiled2d<T>& ima = exact(ima_);

	ima.buffer().write(filename);

	trace::exiting("mln::io::magick::save");
      }*/


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_SAVE_HH
