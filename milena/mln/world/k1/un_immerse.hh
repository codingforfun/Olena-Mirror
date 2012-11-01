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
/// \brief Un-immerse a 2D image from K1 to K0.


#ifndef MLN_WORLD_K1_UN_IMMERSE_HH
# define MLN_WORLD_K1_UN_IMMERSE_HH

# include <mln/core/concept/image.hh>
# include <mln/world/kn/un_immerse.hh>


namespace mln
{

  namespace world
  {

    namespace k1
    {

      /*! \brief Un-immerse a 2D image from K1 to K0.

	\verbatim

	  -1 0 1 2 3
	-1 . - . - .            0 1
	 0 | a | b |          0 a b
	 1 . - . - .      ->  1 c d
	 2 | c | d |
	 3 . - . - .

	\endverbatim

       */
      template <typename I, typename V>
      mln_ch_value(I,V)
      un_immerse(const Image<I>& ima, const V& new_value_type);

      /// \overload
      template <typename I>
      mln_concrete(I)
      un_immerse(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename V>
      mln_ch_value(I,V)
      un_immerse(const Image<I>& ima_, const V& new_value_type)
      {
	trace::entering("mln::world::k1::un_immerse");
	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	mln_ch_value(I,V) output = kn::un_immerse(ima, 2, new_value_type);

	trace::exiting("mln::world::k1::un_immerse");
	return output;
      }


      template <typename I>
      mln_concrete(I)
      un_immerse(const Image<I>& ima_)
      {
	trace::entering("mln::world::k1::un_immerse");
	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	mln_concrete(I) output = kn::un_immerse(ima, 1);

	trace::exiting("mln::world::k1::un_immerse");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_UN_IMMERSE_HH

