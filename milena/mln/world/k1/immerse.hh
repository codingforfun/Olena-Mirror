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
/// \brief Immerse a 2D image into K1.

#ifndef MLN_WORLD_K1_IMMERSE_HH
# define MLN_WORLD_K1_IMMERSE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/box.hh>
# include <mln/core/alias/point2d.hh>

namespace mln
{

  namespace world
  {

    namespace k1
    {

      /*! \brief Immerse a 2D image into K1.

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

	/// Return the equivalent point in K1 from a point in K0.
	inline
	point2d immerse_point(const point2d& p)
	{
	  point2d tmp(2 * p.row(), 2 * p.col());
	  return tmp;
	}

	/// \brief Return the equivalent domain in K1 from a domain in
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

      } // end of namespace mln::world::k1::internal



      // Facade

      template <typename I>
      mln_concrete(I) immerse(const Image<I>& ima_)
      {
	trace::entering("mln::world::k1::immerse");
	mln_precondition(exact(ima_).is_valid());
	const I& ima = exact(ima_);

	mln_concrete(I) output(internal::domain_from_K0(ima.domain()));

	mln_piter(I) p(ima.domain());
	for_all(p)
	  output(internal::immerse_point(p)) = ima(p);

	trace::exiting("mln::world::k1::immerse");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_IMMERSE_HH
