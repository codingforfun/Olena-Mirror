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
/// \brief Immerse a 2D image into K1 and compute values of 0 and 1
/// faces from 2-faces'.

#ifndef MLN_WORLD_K1_IMMERSE_WITH_HH
# define MLN_WORLD_K1_IMMERSE_WITH_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/box.hh>
# include <mln/core/alias/point2d.hh>

# include <mln/world/k1/immerse.hh>
# include <mln/world/k1/fill_0_from_1_faces.hh>
# include <mln/world/k1/fill_1_from_2_faces.hh>

namespace mln
{

  namespace world
  {

    namespace k1
    {

      /*! \brief Immerse a 2D image into K1 and compute values of
       *  0 and 1 faces from 2-faces'.

	\verbatim
	           -1 0 1 2 3
	 0 1     -1 . - . - .
       0 o o      0 | o | o |
       1 o o  ->  1 . - . - .
	          2 | o | o |
	 	  3 . - . - .
	\endverbatim

	1-faces are valued with function \p f2, using left and right
	2-faces (for vertical 1-faces) or up and down 2-faces (for
	horizontal 1-faces).

	0-faces are valued with function \p f4, considering a
	4-connexity with its 1-face neighbors.

       */
      template <typename I, typename V, typename F2, typename F4>
      mln_ch_value(I, V)
      immerse_with(const Image<I>& ima, const V& new_value_type,
		   const Function_vv2v<F2>& f_1faces,
		   const Function_vvvv2v<F4>& f_0faces);


# ifndef MLN_INCLUDE_ONLY


      template <typename I, typename V, typename F2, typename F4>
      mln_ch_value(I, V)
      immerse_with(const Image<I>& ima_, const V& new_value_type,
		   const Function_vv2v<F2>& f_1faces_,
		   const Function_vvvv2v<F4>& f_0faces_)
      {
	trace::entering("mln::world::k1::immerse_with");
	mln_precondition(exact(ima_).is_valid());

	// FIXME: we cannot write that test because we rely on
	//        safe_convert. So, even though some types may not
	//        have conversions available by default, conversion
	//        may work...
	//mlc_converts_to(mln_result(F2), V)::check();
	//mlc_converts_to(mln_result(F4), V)::check();

	const I& ima = exact(ima_);
	const F2& f_1faces = exact(f_1faces_);
	const F4& f_0faces = exact(f_0faces_);

	mln_ch_value(I,V) output = k1::immerse(ima, new_value_type);
	k1::fill_1_from_2_faces(output, f_1faces);
	k1::fill_0_from_1_faces(output, f_0faces);

	trace::exiting("mln::world::k1::immerse_with");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_IMMERSE_WITH_HH
