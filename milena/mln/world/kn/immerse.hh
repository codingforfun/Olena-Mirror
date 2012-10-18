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
/// \brief Immerse a 2D image into Kn.

#ifndef MLN_WORLD_Kn_IMMERSE_HH
# define MLN_WORLD_Kn_IMMERSE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/box.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/value/interval.hh>
# include <mln/fun/vv2v/span.hh>
# include <mln/fun/vvvv2v/span.hh>

# include <mln/world/k1/fill_0_from_1_faces.hh>
# include <mln/world/k1/fill_1_from_2_faces.hh>

# include <mln/world/k2/converters.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      /*! \brief Immerse a 2D image into Kn.

	\param[in] ima 2D Image in K0.
	\param[in] n Set in which space to immerse \p ima (ex: n=1 => k1).
	\param[in] new_type Value type of the immersed image.

	\return A 2D image immersed in k\p n of value type \tparam V.

	\verbatim

	Example with n=1 :

	           -1 0 1 2 3
	 0 1     -1 . - . - .
       0 o o      0 | o | o |
       1 o o  ->  1 . - . - .
	          2 | o | o |
	 	  3 . - . - .

	\endverbatim

       */
      template <typename I, typename V, typename F>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima_, const unsigned n, const V& new_type, const F& converter_);

      /// \overload
      template <typename I>
      mln_concrete(I)
      immerse(const Image<I>& ima, const unsigned n);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	/// Return the equivalent point in Kn from a point in K0.
	inline
	point2d immerse_point(const point2d& p, const unsigned n)
	{
	  point2d tmp(std::pow(2u, n) * p.row(), std::pow(2u, n) * p.col());
	  return tmp;
	}

	/// \brief Return the equivalent domain in Kn from a domain in
	/// K0.
	template <typename B>
	inline
	B domain_from_K0(const Box<B>& b_, const unsigned n)
	{
	  mln_precondition(exact(b_).is_valid());
	  const B& b = exact(b_);

	  mln_deduce(B, site, delta) one;
	  one.set_all(1);
	  return B(immerse_point(b.pmin(), n) - one, immerse_point(b.pmax(), n) + one);
	}

      } // end of namespace mln::world::kn::internal



      // Facade

      template <typename I, typename V, typename F>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima_, const unsigned n,
	      const V& new_type, const F& converter_)
      {
	trace::entering("mln::world::kn::immerse_with");
	mln_precondition(exact(ima_).is_valid());
	const I& ima = exact(ima_);
	const F& converter = exact(converter_);
	(void) new_type;

	mln_ch_value(I,V)
	  output(internal::domain_from_K0(ima.domain(), n));

	// Filling Primary 2-Faces
	mln_piter(I) p(ima.domain());
	for_all(p)
	{
	  V tmp;
	  tmp = converter(ima(p));
	  output(internal::immerse_point(p, n)) = tmp;
	}

	trace::exiting("mln::world::kn::immerse_with");
	return output;
      }

      template <typename I>
      mln_concrete(I)
      immerse(const Image<I>& ima, const unsigned n)
      {
	typedef mln_value(I) V;
	return immerse(ima, n, V(), k2::generic_converter<V,V>());
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_Kn_IMMERSE_HH
