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
# include <mln/world/kn/immerse.hh>
# include <mln/metal/converts_to.hh>

namespace mln
{

  namespace world
  {

    namespace k2
    {

      /*! \brief Immerse a 2D image into K2.

	\param[in] ima A 2D image.
	\param[in] new_value_type An instance of the value type used
	in the resulting immersed image.

	\return A 2D image immersed in K2 of type \tparam V.

	\verbatim
	           -1 0 1 2 3 4 5 6 7
	 0 1     -1 . - . - . - . - .
       0 a b      0 | a |   | b |   |
       1 c d  ->  1 . - . - . - . - .
                  2 |   |   |   |   |
		  3 . - . - . - . - .
	          4 | c |   | d |   |
	 	  5 . - . - . - . - .
		  6 |   |   |   |   |
		  7 . - . - . - . - .
	\endverbatim

       */
      template <typename I, typename V>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima, const V& new_value_type);

      /// \overload
      template <typename I>
      mln_concrete(I)
      immerse(const Image<I>& ima);

      /// \overload
      /// 0, 1 and non-primary 2-faces values are set to \p
      /// default_value.
      template <typename I, typename V, typename U>
      mln_ch_value(I, V)
      immerse(const Image<I>& ima, const V& new_value_type,
	      const U& default_value);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename V>
      mln_ch_value(I,V)
      immerse(const Image<I>& ima, const V& new_value_type)
      {
	trace::entering("mln::world::k2::immerse");
	mln_precondition(exact(ima).is_valid());

	mln_ch_value(I,V)
	  output = kn::immerse(ima, 2, new_value_type);

	trace::exiting("mln::world::k2::immerse");
	return output;
      }


      template <typename I>
      mln_concrete(I)
      immerse(const Image<I>& ima)
      {
	trace::entering("mln::world::k2::immerse");
	mln_precondition(exact(ima).is_valid());

	mln_concrete(I)
	  output = kn::immerse(ima, 2);

	trace::exiting("mln::world::k2::immerse");
	return output;
      }


      template <typename I, typename V, typename U>
      mln_ch_value(I,V)
      immerse(const Image<I>& ima, const V& new_value_type,
	      const U& default_value)
      {
	trace::entering("mln::world::k2::immerse");
	mln_precondition(exact(ima).is_valid());
	mlc_converts_to(U,V)::check();

	mln_ch_value(I,V)
	  output = kn::immerse(ima, 2, new_value_type, V(default_value));

	trace::exiting("mln::world::k2::immerse");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k2

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K2_IMMERSE_HH
