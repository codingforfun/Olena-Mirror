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

/// \file
///
/// \brief Routines used in scribo::layout::smearing

#ifndef SCRIBO_LAYOUT_INTERNAL_SMEARING_CRLA_HH
# define SCRIBO_LAYOUT_INTERNAL_SMEARING_CRLA_HH

#include <mln/core/image/image2d.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace smearing
      {


	using namespace mln;


	template< typename V >
	void horizontal_CRLA(const image2d<V>& input,
			     image2d<V>& output,
			     const unsigned threshold,
			     const V bound);

	template< typename V >
	void vertical_CRLA(const image2d<V>& input,
			   image2d<V>& output,
			   const unsigned threshold,
			   const V bound);

	template< typename V >
	void CRLA(const image2d<V>& input,
		  image2d<V>& output,
		  const unsigned hthreshold,
		  const unsigned hthreshold2,
		  const unsigned vthreshold,
		  const V bound);


# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// Horizontal CRLA
	//---------------------------

	template< typename V >
	inline
	void horizontal_CRLA(const image2d<V>& input,
			     image2d<V>& output,
			     const unsigned threshold,
			     const V bound)
	{
	  unsigned count = 0;
	  unsigned nrows = input.nrows();
	  unsigned ncols = input.ncols();
	  V last_pixel_value = 0;

	  for (unsigned i = 0; i < nrows; ++i)
	  {
	    for (unsigned j = 0; j < ncols; ++j)
	    {
	      V current_pixel = input.at_(i, j);

	      if (current_pixel == 0)
	      {
		if (last_pixel_value)
		{
		  unsigned k = j + 1;
		  for (; input.at_(i, k) == 0 && (k < ncols); ++k);

		  count = k - j;
		  current_pixel = input.at_(i, k);

		  if (last_pixel_value == current_pixel &&
		      current_pixel <= bound &&
		      count < threshold)
		    for (unsigned l = j; l <= k; ++l)
		      output.at_(i, l) = last_pixel_value;

		  j = k;
		  output.at_(i, j) = current_pixel;
		  last_pixel_value = current_pixel;
		}
	      }
	      else
	      {
		output.at_(i, j) = current_pixel;
		last_pixel_value = current_pixel;
	      }
	    }
	  }
	}

	//---------------------------
	// Vertical CRLA
	//---------------------------

	template< typename V >
	inline
	void vertical_CRLA(const image2d<V>& input,
			   image2d<V>& output,
			   const unsigned threshold,
			   const V bound)
	{
	  unsigned count = 0;
	  unsigned nrows = input.nrows();
	  unsigned ncols = input.ncols();
	  V last_pixel_value = 0;

	  for (unsigned j = 0; j < ncols; ++j)
	  {
	    for (unsigned i = 0; i < nrows; ++i)
	    {
	      V current_pixel = input.at_(i, j);

	      if (current_pixel == 0)
	      {
		if (last_pixel_value)
		{
		  unsigned k = i + 1;
		  for (; !input.at_(k, j) && (k < nrows); ++k);

		  count = k - i;
		  current_pixel = input.at_(k, j);

		  if (last_pixel_value == current_pixel &&
		      current_pixel <= bound &&
		      count < threshold)
		    for (unsigned l = i; l <= k; ++l)
		      output.at_(l, j) = last_pixel_value;

		  i = k;
		  output.at_(i, j) = current_pixel;
		  last_pixel_value = current_pixel;
		}
	      }
	      else
	      {
		output.at_(i, j) = current_pixel;
		last_pixel_value = current_pixel;
	      }
	    }
	  }
	}

	//---------------------------
	// CRLA
	//---------------------------

	template< typename V >
	inline
	void CRLA(const image2d<V>& input,
		  image2d<V>& output,
		  const unsigned hthreshold,
		  const unsigned hthreshold2,
		  const unsigned vthreshold,
		  const V bound)
	{
	  const box2d& domain = input.domain();
	  image2d<V> himg(domain);
	  image2d<V> vimg(domain);
	  data::fill(himg, 0);
	  data::fill(vimg, 0);

	  horizontal_CRLA(input, himg, hthreshold, bound);
	  vertical_CRLA(input, vimg, vthreshold, bound);

	  data::fill((output |(pw::value(himg) == pw::value(vimg))).rw(), pw::value(himg));

	  horizontal_CRLA(output, output, hthreshold2, bound);
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::smearing

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_SMEARING_CRLA_HH

