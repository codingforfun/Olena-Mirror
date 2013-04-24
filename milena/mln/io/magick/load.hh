// Copyright (C) 2009, 2010, 2012, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_IO_MAGICK_LOAD_HH
# define MLN_IO_MAGICK_LOAD_HH

/// \file
///
/// \brief Image intput routines based on Magick++.

# include <cstdlib>

# include <Magick++.h>

# include <mln/core/image/image2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/rgb16.hh>

# include <mln/io/magick/internal/init_magick.hh>


namespace mln
{

  namespace io
  {

    namespace magick
    {

      /*! \brief Load data from a file into a Milena image using
	  Magick++.

	  \param[out] ima       The image data are loaded into.
	  \param[in]  filename  The name of the input file.

	  \warning In order to use 16bits images, the version
	  installed on the system must be compiled with a 16 bits
	  Quantum (16 bits per channel). On most Linux distribution,
	  it is compiled with a 8 bits Quantum (8 bits per channel)
	  and converts automatically 16 bits images toward 8 bits
	  images.

	  \warning 32bits images are not supported by Milena for the
	  moment.

	  \ingroup iomagick
      */
      template <typename I>
      void load(Image<I>& ima, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	template <unsigned n>
	inline
	bool
	do_it(const value::rgb<n>& in, bool& out)
	{
	  if (in.red() != in.green() || in.green() != in.blue())
	    {
	      std::cerr <<
		"error: attempt to load what looks like a color\n"
		"(mln::value::rgb" << n << ") image into a Boolean (bool) image"
			<< std::endl;
	      return false;
	    }
	  if (in.red() != 0 &&
	      in.red() != mln_max(typename value::rgb<n>::red_t))
	    {
	      std::cerr <<
		"error: attempt to load what looks like a grayscale\n"
		"(mln::value::int_u" << n << ") image into a Boolean (bool) image"
			<< std::endl;
	      return false;
	    }

	  out = (in.red() != 0);
	  return true;
	}

	template <unsigned n, unsigned m>
	inline
	bool
	do_it(const value::rgb<n>& in, value::int_u<m>& out)
	{
	  if (n > m)
	  {
	    std::cerr << "error: attempt to load high quantification values ("
		      << n
		      << " bits) into low quantification values ("
		      << m
		      << ")." << std::endl;
	    return false;
	  }

	  if (in.red() != in.green() || in.green() != in.blue())
	    {
	      std::cerr <<
		"error: attempt to load what looks like a color\n"
		"(mln::value::rgb" << n << ") image into a grayscale\n"
		"(mln::int_u" << m << " values) image" << std::endl;
	      return false;
	    }

	  out = in.red();
	  return true;
	}

	template <unsigned n>
	bool
	do_it(const value::rgb<n>& in, value::rgb<n>& out)
	{
 	  out = in;
	  return true;
	}

	template <unsigned n, unsigned m>
	bool
	do_it(const value::rgb<n>& in, value::rgb<m>& out)
	{
	  if (n > m)
	  {
	    std::cerr << "error: attempt to load high quantification values ("
		      << n
		      << " bits) into low quantification values ("
		      << m
		      << ")." << std::endl;
	    return false;
	  }

	  convert::from_to(in, out);
	  return true;
	}


	template <typename V, typename I>
	void load(I& ima, const std::string& filename)
	{
	  // FIXME: Handle Magick++'s exceptions (see either
	  // ImageMagick++'s or GraphicsMagick++'s documentation).
	  Magick::Image magick_ima(filename);
	  magick_ima.read(filename);
	  magick_ima.type(Magick::TrueColorType);
	  int nrows = magick_ima.rows();
	  int ncols = magick_ima.columns();
	  mln_site(I) pmin(0, 0);
	  mln_site(I) pmax(nrows - 1, ncols - 1);

	  mln_concrete(I) result(box<mln_site(I)>(pmin, pmax));
	  initialize(ima, result);

	  def::coord
	    minrow = geom::min_row(ima),
	    mincol = geom::min_col(ima),
	    maxrow = geom::max_row(ima),
	    maxcol = geom::max_col(ima);

	  Magick::Pixels view(magick_ima);
	  // Note that `ncols' is passed before `nrows'.
	  Magick::PixelPacket* pixels = view.get(0, 0, ima.ncols(), ima.nrows());
	  mln_value(I) *ptr_ima = &ima(ima.domain().pmin());

	  unsigned row_offset = ima.delta_offset(dpoint2d(+1, - ncols));

	  for (def::coord row = minrow; row <= maxrow;
	       ++row, ptr_ima += row_offset)
	    for (def::coord col = mincol; col <= maxcol; ++col)
	    {
	      /* Each channel of a Magick++ image is coded on a
		 Magick::Quantum value, which can be an 8-, 16- or 32-bit
		 integer.  Load the most significant bits of each channel
		 into a component of an mln::value::rgb8 value (i.e., into
		 an mln::value::int_u8 value).  */
	      V c(pixels->red   >> 8 * (sizeof(Magick::Quantum)
					- sizeof(typename V::red_t)),
		  pixels->green >> 8 * (sizeof(Magick::Quantum)
					- sizeof(typename V::green_t)),
		  pixels->blue  >> 8 * (sizeof(Magick::Quantum)
					- sizeof(typename V::blue_t)));
	      mln_value(I) res;
	      if (!do_it(c, res))
	      {
		std::cerr << "while trying to load `" << filename << "'"
			  << std::endl;
		abort();
	      }

	      *ptr_ima++ = res;
	      ++pixels;
	    }
	}

      } // end of namespace mln::io::magick::impl


      template <typename I>
      inline
      void load(Image<I>& ima_, const std::string& filename)
      {
	mln_trace("mln::io::magick::load");

	I& ima = exact(ima_);

	// Initialize GraphicsMagick only once.
	static internal::init_magick init;
	(void) init;

	unsigned quantum_size = sizeof(Magick::Quantum);
	switch(quantum_size)
	{
	  case 1:
	    impl::load<value::rgb8>(ima, filename);
	    break;

	  case 2:
# if (QuantumDepth == 16) // Enable if the current version of
			  // GraphicsMagick is compatible.
	    impl::load<value::rgb16>(ima, filename);
	    break;
#endif

	  case 4:
	  default:
	    std::cerr << "Quantum size not supported! Values in each channel"
	      " must be encoded in 8 or 16 bits. (" << quantum_size * 8 << "bits here)"
		      << std::endl;
	    abort();
	}

      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::magick

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_MAGICK_LOAD_HH
