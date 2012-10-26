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
/// \brief Fill 1 faces in a K1 2D image using its 2 faces.

#ifndef MLN_WORLD_K1_FILL_1_FROM_2_FACES_HH
# define MLN_WORLD_K1_FILL_1_FROM_2_FACES_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/kn/is_1_face_vertical.hh>
# include <mln/world/kn/is_1_face_horizontal.hh>

namespace mln
{

  namespace world
  {

    namespace k1
    {

      /*! \brief Fill 1 faces in a K1 2D image using its 2 faces.

	\param[in,out] inout A 2D image immersed in K1.
	\param[in,out] f A functor computing a result from two values.

	This function use the following neighborhoods:

	* In case of vertical 1 faces:

	\verbatim
	      x | x
	\endverbatim

	* In case of horizontal 1 face:

	\verbatim
	        x
		-
		x
	\endverbatim

       */
      template <typename I, typename F>
      void fill_1_from_2_faces(Image<I>& inout, const Function_vv2v<F>& f);

      /// \overload
      /// This implementation do not read values outside of the image
      /// domain.
      template <typename I, typename A>
      void fill_1_from_2_faces(Image<I>& inout, const Accumulator<A>& accu);


# ifndef MLN_INCLUDE_ONLY


      // Facades


      template <typename I, typename F>
      void fill_1_from_2_faces(Image<I>& inout_, const Function_vv2v<F>& f_)
      {
	trace::entering("mln::world::k1::fill_1_from_2_faces");

	mln_precondition(exact(inout_).is_valid());
	I& inout = exact(inout_);
	const F& f = exact(f_);

	mln_box(I) b = inout.domain();
	mln_piter(I) p(b);
	for_all(p)
	  if (kn::is_1_face_vertical(p))
	    inout(p) = f(inout(p + left), inout(p + right));
	  else if (kn::is_1_face_horizontal(p))
	    inout(p) = f(inout(p + up), inout(p + down));

	trace::exiting("mln::world::k1::fill_1_from_2_faces");
      }

      template <typename I, typename A>
      void fill_1_from_2_faces(Image<I>& inout_, const Accumulator<A>& accu_)
      {
	trace::entering("mln::world::k1::fill_1_from_2_faces");

	mln_precondition(exact(inout_).is_valid());
	I& inout = exact(inout_);
	A accu(exact(accu_));

	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (kn::is_1_face_vertical(p))
	  {
	    accu.init();
	    if (inout.domain().has(p + left))
	      accu.take(inout(p + left));
	    if (inout.domain().has(p + right))
	      accu.take(inout(p + right));
	    inout(p) = accu.to_result();
	  }
	  else if (kn::is_1_face_horizontal(p))
	  {
	    accu.init();
	    if (inout.domain().has(p + up))
	      accu.take(inout(p + up));
	    if (inout.domain().has(p + down))
	      accu.take(inout(p + down));
	    inout(p) = accu.to_result();
	  }

	trace::exiting("mln::world::k1::fill_1_from_2_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_FILL_1_FROM_2_FACES_HH
