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
# include <mln/world/kn/immerse.hh>

namespace mln
{

  namespace world
  {

    namespace k1
    {

      /*! \brief Immerse a 2D image into K1.

	\param[in] ima 2D Image in K0.
	\param[in] new_value_type Value type of the immersed image.

	\return A 2D image immersed in k1 of value type \tparam V.

	\verbatim
	           -1 0 1 2 3
	 0 1     -1 . - . - .
       0 o o      0 | o | o |
       1 o o  ->  1 . - . - .
	          2 | o | o |
	 	  3 . - . - .
	\endverbatim

       */
      template <typename I, typename V>
      mln_ch_value(I,V)
      immerse(const Image<I>& ima, const V& new_value_type);

      /// \overload
      /// new_type is set to mln_value(I).
      template <typename I>
      mln_concrete(I) immerse(const Image<I>& ima);

      /// \overload
      /// 0, 1 and non-primary 2-faces values are set to \p
      /// default_value.
      template <typename I, typename V>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima, const V& new_value_type,
	      const V& default_value);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename V>
      mln_ch_value(I,V)
      immerse(const Image<I>& ima, const V& new_value_type)
      {
	trace::entering("mln::world::k1::immerse");
	mln_precondition(exact(ima).is_valid());
	(void) new_value_type;

	mln_ch_value(I,V) output = kn::immerse(ima, 1, V());

	trace::exiting("mln::world::k1::immerse");
	return output;
      }


      template <typename I>
      mln_concrete(I)
      immerse(const Image<I>& ima)
      {
	typedef mln_value(I) V;
	return immerse(ima, 1);
      }


      template <typename I, typename V>
      mln_ch_value(I,V)
      immerse(const Image<I>& ima, const V& new_value_type,
	      const V& default_value)
      {
	trace::entering("mln::world::k2::immerse");
	mln_precondition(exact(ima).is_valid());

	mln_ch_value(I,V)
	  output = kn::immerse(ima, 1, new_value_type, default_value);

	trace::exiting("mln::world::k2::immerse");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_IMMERSE_HH
