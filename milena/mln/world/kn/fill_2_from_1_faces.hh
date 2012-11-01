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
/// \brief Fill 2 faces in a KN 2D image using its 1 faces.

#ifndef MLN_WORLD_KN_FILL_2_FROM_1_FACES_HH
# define MLN_WORLD_KN_FILL_2_FROM_1_FACES_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/kn/is_2_face.hh>
# include <mln/world/kn/safe_cast.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      /*! \brief Fill 0 faces in a KN 2D image using its 2 faces.

	\param[in,out] inout A 2D image immersed in KN.
	\param[in,out] f A functor computing a result from four values.

	This function use the following neighborhood:

	\verbatim
	         -
	        |o|
	         -
	\endverbatim


       */
      template <typename I, typename F>
      void fill_2_from_1_faces(Image<I>& inout, Function_vvvv2v<F>& f);

      /// \overload
      template <typename I, typename A>
      void fill_2_from_1_faces(Image<I>& inout, const Accumulator<A>& accu);


# ifndef MLN_INCLUDE_ONLY


      // Facade


      template <typename I, typename F>
      void fill_2_from_1_faces(Image<I>& inout_, Function_vvvv2v<F>& f_)
      {
	trace::entering("mln::world::kn::fill_2_from_1_faces");

	mln_precondition(exact(inout_).is_valid());
	I& inout = exact(inout_);
	F& f = exact(f_);

	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (kn::is_2_face(p))
	    inout(p) = safe_cast(f(safe_cast(inout(p + up)),
				   safe_cast(inout(p + left)),
				   safe_cast(inout(p + right)),
				   safe_cast(inout(p + down))));

	trace::exiting("mln::world::kn::fill_2_from_1_faces");
      }


      template <typename I, typename A>
      void fill_2_from_1_faces(Image<I>& inout_, const Accumulator<A>& accu_)
      {
	trace::entering("mln::world::kn::fill_2_from_1_faces");

	I& inout = exact(inout_);
	mln_precondition(inout.is_valid());

	A accu = exact(accu_);
	typedef mln_argument(A) arg;
	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (kn::is_2_face(p))
	  {
	    accu.init();
	    if (inout.domain().has(p + up))
	      accu.take(safe_cast_to<arg>(inout(p + up)));
	    if (inout.domain().has(p + left))
	      accu.take(safe_cast_to<arg>(inout(p + left)));
	    if (inout.domain().has(p + right))
	      accu.take(safe_cast_to<arg>(inout(p + right)));
	    if (inout.domain().has(p + down))
	      accu.take(safe_cast_to<arg>(inout(p + down)));
	    inout(p) = safe_cast(accu.to_result());
	  }

	trace::exiting("mln::world::kn::fill_2_from_1_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_FILL_2_FROM_1_FACES_HH
