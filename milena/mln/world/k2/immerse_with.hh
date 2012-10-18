// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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
/// \brief Immerse a 2D image into K2.

#ifndef MLN_WORLD_K2_IMMERSE_HH
# define MLN_WORLD_K2_IMMERSE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/box.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/value/interval.hh>
# include <mln/fun/vv2v/span.hh>
# include <mln/fun/vvvv2v/span.hh>

# include <mln/world/k1/fill_0_from_1_faces.hh>
# include <mln/world/k1/fill_1_from_2_faces.hh>

namespace mln
{

  namespace world
  {

    namespace k2
    {

      /*! \brief Immerse a 2D image into K2.

	\verbatim

	           -1 0 1 2 3
	 0 1     -1 . - . - .
       0 o o      0 | o | o |
       1 o o  ->  1 . - . - .
	          2 | o | o |
	 	  3 . - . - .

	\endverbatim

       */
      template <typename I>
      mln_concrete(I) immerse(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	/// Return the equivalent point in K2 from a point in K0.
	inline
	point2d immerse_point(const point2d& p)
	{
	  point2d tmp(4 * p.row(), 4 * p.col());
	  return tmp;
	}

	/// \brief Return the equivalent domain in K2 from a domain in
	/// K0.
	template <typename B>
	inline
	B domain_from_K0(const Box<B>& b_)
	{
	  mln_precondition(exact(b_).is_valid());
	  const B& b = exact(b_);

	  mln_deduce(B, site, delta) one;
	  one.set_all(1);
	  return B(immerse_point(b.pmin()) - one, immerse_point(b.pmax()) + one);
	}

      } // end of namespace mln::world::k2::internal



      // Facade

      template <typename I, typename F2, typename F4>
      mln_ch_value(I, value::interval<mln_result(F4)>)
      immerse_with(const Image<I>& ima_,
		   Function_vv2v<F2>& f2_, Function_vvvv2v<F4>& f4_)
      {
	trace::entering("mln::world::k2::immerse_with");
	mln_precondition(exact(ima_).is_valid());
	const I& ima = exact(ima_);
	const F2& f2 = exact(f2_);
	const F4& f4 = exact(f4_);

	// FIXME: how to initialize the border... ?
	// border::fill(ima, 0);

	typedef mln_result(F4) V;
	typedef value::interval<V> VOUT;
	mln_ch_value(I,VOUT)
	  output(internal::domain_from_K0(ima.domain()));

	// Filling 2-Faces
	mln_piter(I) p(ima.domain());
	for_all(p)
	{
	  //       .-.-
	  // ab    |x|*| -> a
	  // cd -> .-.-.
	  //       |*|*|
	  //       .-.-.
	  output(internal::immerse_point(p)) = ima(p);

	  //       .-.-
	  // ab    |*|x| -> f2(a,b)
	  // cd -> .-.-.
	  //       |*|*|
	  //       .-.-.
	  output(internal::immerse_point(p) + 2 * right) = f2(ima(p), ima(p + right));

	  //       .-.-
	  // ab    |*|*|
	  // cd -> .-.-.
	  //       |x|*| -> f2(a,c)
	  //       .-.-.
	  output(internal::immerse_point(p) + 2 * down) = f2(ima(p), ima(p + down));

	  //       .-.-
	  // ab    |*|*|
	  // cd -> .-.-.
	  //       |*|x| -> f4(a,b,c,d)
	  //       .-.-.
	  output(internal::immerse_point(p) + 2 * down_right)
	    = f4(ima(p), ima(p + right), ima(p + down), ima(p + down_right));
	}

	fun::vv2v::span<V> span2;
	k1::fill_1_from_2_faces(output, span2);
	fun::vvvv2v::span<V> span4;
	k1::fill_0_from_1_faces(output, span4);

	trace::exiting("mln::world::k2::immerse_with");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k2

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K2_IMMERSE_HH
