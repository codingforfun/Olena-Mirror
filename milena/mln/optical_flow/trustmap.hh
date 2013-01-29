// Copyright (C) 2013 EPITA Research and Development Laboratory
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

#ifndef MLN_OPTICAL_FLOW_TRUSTMAP_HH_
# define MLN_OPTICAL_FLOW_TRUSTMAP_HH_

/// \file
///
/// \brief Useful functions to compute the trust map and to draw it.
/// See techrep 1211 for more details.

# include <mln/core/concept/image.hh>
# include <mln/value/int_u8.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/core/alias/point2d.hh>

namespace mln
{

  namespace optical_flow
  {

    /// \brief Compute the octogonal box (aka trust map) for the impact of
    /// diffusion process.
    ///
    /// @param ima Output
    /// @param min_i Minimum row for the unknown region
    /// @param max_i Maximum row for the unknown region
    /// @param min_j Minimum col for the unknown region
    /// @param max_j Maximum col for the unknown region
    /// @param max_value Number of iterations
    void
    computeBox (mln::image2d<mln::value::int_u8>& ima,
		unsigned min_i,
		unsigned max_i,
		unsigned min_j,
		unsigned max_j,
		unsigned max_value);

    /// \brief Compute the octogonal box (aka trust map) for the impact of
    /// diffusion process.
    ///
    /// @param ima Output
    /// @param box The unknown region
    /// @param max_value Number of iterations
    void
    computeBox (mln::image2d<mln::value::int_u8>& ima,
		mln::box2d& box,
		unsigned max_value);

    /// \brief Given the trust map, compute the mask assiocated to it
    ///
    /// @param ima The output
    /// @param box The trust map
    void
    computeMask (mln::image2d<bool>& ima,
		 mln::image2d<mln::value::int_u8>& box);

    /// \brief Compute the weightened trust map used in combination process
    ///
    /// @param box The trust map
    /// @param trust The output
    /// @param max_value Number of iterations
    void
    makeTrust (mln::image2d<mln::value::int_u8>& box,
	       mln::image2d<double>& trust,
	       unsigned max_value);

    /// \brief Draw the countours of the trust map.
    ///
    /// @param ima The image on which we want to draw
    /// @param box The trust map
    /// @param v The color used to draw
    /// @param max_value The number of iterations
    template<typename I>
    void
    drawMask (I& ima,
	      const mln::image2d<mln::value::int_u8>& box,
	      const mln_value(I)& v,
	      unsigned max_value);

    /// \brief Internal function
    void
    compute (mln::image2d<mln::value::int_u8>& ima, mln::point2d pt, unsigned value, unsigned max_value,
	     bool up, bool down, bool right, bool left);


# ifndef MLN_INCLUDE_ONLY

    void
    compute (mln::image2d<mln::value::int_u8>& ima, mln::point2d pt, unsigned value, unsigned max_value,
	     bool up, bool down, bool right, bool left)
    {
      if (ima (pt) > value && value < max_value)
	{
	  ima (pt) = value;
	  if (down)
	    compute (ima, pt + mln::dpoint2d (-1, 0), value + 1, max_value, up, down, right, left);
	  if (up)
	    compute (ima, pt + mln::dpoint2d (1, 0), value + 1, max_value, up, down, right, left);
	  if (left)
	    compute (ima, pt + mln::dpoint2d (0, -1), value + 1, max_value, up, down, right, left);
	  if (right)
	    compute (ima, pt + mln::dpoint2d (0, 1), value + 1, max_value, up, down, right, left);
	}
    }

    void
    computeBox (mln::image2d<mln::value::int_u8>& ima,
		unsigned min_i,
		unsigned max_i,
		unsigned min_j,
		unsigned max_j,
		unsigned max_value)

    {

      for (unsigned i = min_i; i < max_i; ++i)
	{
	  compute (ima, mln::point2d (i, min_j), 0, max_value, true, true, false, true);
	  compute (ima, mln::point2d (i, max_j), 0, max_value, true, true, true,  false);
	}

      for (unsigned j = min_j; j < max_j; ++j)
	{
	  compute (ima, mln::point2d (min_i, j), 0, max_value, false, true, true, true);
	  compute (ima, mln::point2d (max_i, j), 0, max_value, true, false, true, true);
	}

    }

    void
    computeBox (mln::image2d<mln::value::int_u8>& ima,
		mln::box2d& box,
		unsigned max_value)
    {
      mln::point2d pmin = box.pmin ();
      mln::point2d pmax = box.pmax ();

      computeBox (ima, pmin.row (), pmax.row (), pmin.col (), pmax.col (),
		  max_value);
    }

    void
    computeMask (mln::image2d<bool>& ima,
		 mln::image2d<mln::value::int_u8>& box)
    {
      mln_piter_(mln::image2d<bool>) p (ima.domain ());

      for_all (p)
      {
	if (box(p) < 255)
	  ima(p) = false;
	else
	  ima(p) = true;
      }
    }

    void
    makeTrust (mln::image2d<mln::value::int_u8>& box,
	       mln::image2d<double>& trust,
	       unsigned max_value)
    {
      mln_piter_ (mln::image2d<mln::value::int_u8>) p (box.domain ());

      for_all (p)
      {
	if (box (p) >= max_value - 1)
	  trust (p) = 1;
	else
	  {
	    trust (p) = (2 * (double)box (p)) / (3 * (double)max_value);
	  }
      }
    }

    void
    convertTrust (mln::image2d<double>& trust,
		  mln::image2d<mln::value::int_u8>& out)
    {
      mln_piter_ (mln::image2d<mln::value::int_u8>) p (trust.domain ());

      for_all (p)
      {
	out (p) = trust (p) * 255;
      }
    }

    template<typename I>
    void
    drawMask (I& ima,
	      const mln::image2d<mln::value::int_u8>& box,
	      const mln_value(I)& v,
	      unsigned max_value)
    {
      mln_piter (I) p (ima.domain ());

      for_all (p)
      {
	if (box(p) == max_value - 1)
	  ima(p) = v;
      }
    }


# endif // ! MLN_INCLUDE_ONLY

  } //end of namespace optical_flow

} //end of namespace mln


#endif // ! MLN_OPTICAL_FLOW_TRUSTMAP_HH_
