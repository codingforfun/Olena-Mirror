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
/// \brief Fill 0 faces in a KN 2D image using 2-faces values from an
/// auxiliary image.

#ifndef MLN_WORLD_KN_FILL_2_FROM_AUX_1_FACES_HH
# define MLN_WORLD_KN_FILL_2_FROM_AUX_1_FACES_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/kn/is_2_face.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      /*! \brief Fill 0 faces in a KN 2D image using 2-faces values
	  from an auxiliary image.

	\param[in,out] inout A 2D image immersed in KN.
	\param[in] aux An auxiliary image with the same domain as \p
	inout.
	\param[in] f A functor computing a result from four values.

	Warning: This function considers that 1-faces values in \p aux
	borders are initialized.

	This function use the following neighborhood:

	\verbatim
	         -
	        |o|
	         -
	\endverbatim


       */
      template <typename I, typename J, typename F>
      void fill_2_from_aux_1_faces(Image<I>& inout, const Image<J>& aux,
				   const Function_vvvv2v<F>& f);

      /// \overload
      template <typename I, typename J, typename A>
      void fill_2_from_aux_1_faces(Image<I>& inout, const Image<J>& aux,
				   const Accumulator<A>& accu);


# ifndef MLN_INCLUDE_ONLY


      // Facade


      template <typename I, typename J, typename F>
      void fill_2_from_aux_1_faces(Image<I>& inout_, const Image<J>& aux_,
				   const Function_vvvv2v<F>& f_)
      {
	trace::entering("mln::world::kn::fill_2_from_aux_1_faces");

	mln_precondition(exact(inout_).is_valid());
	mln_precondition(exact(inout_).domain() <= exact(aux_).domain());
	I& inout = exact(inout_);
	const J& aux = exact(aux_);
	const F& f = exact(f_);

	// FIXME: we cannot check if 1-faces in border are initialized
	// in aux image.

	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (kn::is_2_face(p))
	    inout(p) = f(aux(p + up), aux(p + left),
			 aux(p + right), aux(p + down));

	trace::exiting("mln::world::kn::fill_2_from_aux_1_faces");
      }


      template <typename I, typename J, typename A>
      void fill_2_from_aux_1_faces(Image<I>& inout_, const Image<J>& aux_,
				   const Accumulator<A>& accu_)
      {
	trace::entering("mln::world::kn::fill_2_from_aux_1_faces");

	mln_precondition(exact(inout_).is_valid());
	mln_precondition(exact(inout_).domain() <= exact(aux_).domain());
	I& inout = exact(inout_);
	const J& aux = exact(aux_);

	A accu(exact(accu_));
	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (kn::is_2_face(p))
	  {
	    accu.init();
	    if (inout.domain().has(p + up))
	      accu.take(aux(p + up));
	    if (inout.domain().has(p + left))
	      accu.take(aux(p + left));
	    if (inout.domain().has(p + right))
	      accu.take(aux(p + right));
	    if (inout.domain().has(p + down))
	      accu.take(aux(p + down));
	    inout(p) = accu.to_result();
	  }

	trace::exiting("mln::world::kn::fill_2_from_aux_1_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_FILL_2_FROM_AUX_1_FACES_HH
