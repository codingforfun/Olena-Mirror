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
/// \brief Fill 1 faces in a K1 2D image using 2 faces from an
/// auxiliary image.

#ifndef MLN_WORLD_K1_FILL_1_FROM_AUX_2_FACES_HH
# define MLN_WORLD_K1_FILL_1_FROM_AUX_2_FACES_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/k1/is_1_face_vertical.hh>
# include <mln/world/k1/is_1_face_horizontal.hh>


namespace mln
{

  namespace world
  {

    namespace k1
    {

      /*! \brief Fill 1 faces in a K1 2D image using 2 faces from an
	auxiliary image.

	\param[in,out] inout A 2D image immersed in K1.
	\param[in] aux A 2D image with the same domain as \p inout.
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
      template <typename I, typename J, typename F>
      void fill_1_from_aux_2_faces(Image<I>& inout, const Image<J>& aux,
					 Function_vv2v<F>& f);

      /// \overload
      template <typename I, typename J, typename A>
      void fill_1_from_aux_2_faces(Image<I>& inout, const Image<J>& aux,
					 const Accumulator<A>& accu);


# ifndef MLN_INCLUDE_ONLY


      // Facade


      template <typename I, typename J, typename F>
      void fill_1_from_aux_2_faces(Image<I>& inout_, const Image<J>& aux_,
					 Function_vv2v<F>& f_)
      {
	trace::entering("mln::world::k1::fill_1_from_aux_2_faces");

	mln_precondition(exact(inout_).is_valid());
	mln_precondition(exact(aux_).is_valid());
	mln_precondition(exact(inout_).domain() == exact(aux_).domain());
	I& inout = exact(inout_);
	const J& aux = exact(aux_);
	F& f = exact(f_);

	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (is_1_face_vertical(p))
	    inout(p) = f(aux(p + left), aux(p + right));
	  else if (is_1_face_horizontal(p))
	    inout(p) = f(aux(p + up), aux(p + down));

	trace::exiting("mln::world::k1::fill_1_from_aux_2_faces");
      }


      template <typename I, typename J, typename A>
      void fill_1_from_aux_2_faces(Image<I>& inout_, const Image<J>& aux_,
					 const Accumulator<A>& accu_)
      {
	trace::entering("mln::world::k1::fill_1_from_aux_2_faces");

	mln_precondition(exact(inout_).is_valid());
	mln_precondition(exact(aux_).is_valid());
	mln_precondition(exact(inout_).domain() == exact(aux_).domain());
	I& inout = exact(inout_);
	const J& aux = exact(aux_);
	(void) accu_;

	A accu = A();
	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (is_1_face_vertical(p))
	  {
	    accu.init();
	    accu.take(aux(p + left));
	    accu.take(aux(p + right));
	    inout(p) = accu.to_result();
	  }
	  else if (is_1_face_horizontal(p))
	  {
	    accu.init();
	    accu.take(aux(p + up));
	    accu.take(aux(p + down));
	    inout(p) = accu.to_result();
	  }

	trace::exiting("mln::world::k1::fill_1_from_aux_2_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k1

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K1_FILL_1_FROM_AUX_2_FACES_HH
