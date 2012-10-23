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

#ifndef MLN_WORLD_KN_IMMERSE_HH
# define MLN_WORLD_KN_IMMERSE_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/box.hh>
# include <mln/world/kn/immerse_with_inner_border.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      /*! \brief Immerse a 2D image into Kn.

	\param[in] ima 2D Image in K0.
	\param[in] n Set in which space to immerse \p ima (ex: n=1 => k1).
	\param[in] new_value_type Value type of the immersed image.

	\return A 2D image immersed in k\p n of value type \tparam V.

	\verbatim

	Example with n=1 :

	           -1 0 1 2 3
	 0 1     -1 . - . - .
       0 a b      0 | a | b |
       1 c d  ->  1 . - . - .
	          2 | c | d |
	 	  3 . - . - .

	\endverbatim

       */
      template <typename I, typename V>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima, const unsigned n,
	      const V& new_value_type);

      /// \overload
      /// \param[in] default_value 0, 1 and non-primary 2-faces values
      /// are initialized with this value.
      template <typename I, typename V>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima, const unsigned n,
	      const V& new_value_type, const V& default_value);

      /// \overload
      template <typename I>
      mln_concrete(I)
      immerse(const Image<I>& ima, const unsigned n);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename V>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima_, const unsigned n,
	      const V& new_value_type)
      {
	trace::entering("mln::world::kn::immerse");
	mln_precondition(exact(ima_).is_valid());
	const I& ima = exact(ima_);
	(void) new_value_type;

	mln_ch_value(I,V)
	  output = immerse_with_inner_border(ima, n, new_value_type, 0);

	trace::exiting("mln::world::kn::immerse");
	return output;
      }


      template <typename I, typename V>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima_, const unsigned n,
	      const V& new_value_type, const V& default_value)
      {
	trace::entering("mln::world::kn::immerse");
	mln_precondition(exact(ima_).is_valid());
	const I& ima = exact(ima_);
	(void) new_value_type;

	mln_ch_value(I,V)
	  output = immerse_with_inner_border(ima, n, new_value_type,
					     default_value, 0);

	trace::exiting("mln::world::kn::immerse");
	return output;
      }


      template <typename I>
      mln_concrete(I)
      immerse(const Image<I>& ima, const unsigned n)
      {
	typedef mln_value(I) V;
	return immerse(ima, n, V());
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_IMMERSE_HH
