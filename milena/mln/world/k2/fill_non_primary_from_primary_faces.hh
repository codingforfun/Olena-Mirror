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
/// \brief Fill 1 faces in a K2 2D image using its 2 faces.

#ifndef MLN_WORLD_K2_FILL_NON_PRIMARY_FROM_PRIMARY_FACES_HH
# define MLN_WORLD_K2_FILL_NON_PRIMARY_FROM_PRIMARY_FACES_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/k2/is_non_primary_face_vertical.hh>
# include <mln/world/k2/is_non_primary_face_horizontal.hh>
# include <mln/world/k2/is_non_primary_face_cross.hh>
# include <mln/world/k2/convert_on_access.hh>

namespace mln
{

  namespace world
  {

    namespace k2
    {

      /*! \brief Fill non-primary 2-faces in a K2 2D image using
       *  primary 2-faces.

	\param[in,out] inout A 2D image immersed in K2.
	\param[in,out] accu An accumulator.

	This function use the following neighborhoods:

	* In case of vertical non-primary 2-faces:

	\verbatim
	      x o x
	\endverbatim

	* In case of horizontal non-primary 2-face:

	\verbatim
	        x
		o
		x
	\endverbatim

	* In case of cross non-primary 2-face:

	\verbatim
	      x   x
		o
	      x   x
	\endverbatim

       */
      /// \overload
      template <typename I, typename A>
      void fill_non_primary_from_primary_faces(Image<I>& inout,
					       const Accumulator<A>& accu);


# ifndef MLN_INCLUDE_ONLY


      // Facade

      template <typename I, typename A>
      void fill_non_primary_from_primary_faces(Image<I>& inout_,
					       F& functor_)
      {
	trace::entering("mln::world::k2::fill_non_primary_from_primary_faces");

	mln_precondition(exact(inout_).is_valid());
	I& inout = exact(inout_);
	(void) accu_;
	typedef typename A::argument V;

	A& accu = exact(accu_);

	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (is_non_primary_face_vertical(p))
	  {
	    accu.init();
	    V tmp1, tmp2;

	    convert_on_access(inout(p + 2 * left), tmp1);
	    convert_on_access(inout(p + 2 * right), tmp2);
	    convert_on_access(f(tmp1, tmp2), inout(p));
	  }
	  else if (is_non_primary_face_horizontal(p))
	  {
	    accu.init();
	    V tmp1, tmp2;

	    convert_on_access(inout(p + 2 * up), tmp1);
	    convert_on_access(inout(p + 2 * down), tmp2);
	    convert_on_access(f(tmp1, tmp2), inout(p));
	  }
	  else if (is_non_primary_face_cross(p))
	  {
	    accu.init();
	    V tmp1, tmp2, tmp3, tmp4;

	    convert_on_access(inout(p + 2 * up_left), tmp1);
	    convert_on_access(inout(p + 2 * up_right), tmp2);
	    convert_on_access(inout(p + 2 * down_left), tmp3);
	    convert_on_access(inout(p + 2 * down_right), tmp4);
	    convert_on_access(f(tmp1, tmp2, tmp3, tmp4), inout(p));
	  }

	trace::exiting("mln::world::k2::fill_non_primary_from_primary_faces");
      }


      template <typename I, typename A>
      void fill_non_primary_from_primary_faces(Image<I>& inout_,
					       Accumulator<A>& accu_)
      {
	trace::entering("mln::world::k2::fill_non_primary_from_primary_faces");

	mln_precondition(exact(inout_).is_valid());
	I& inout = exact(inout_);
	(void) accu_;
	typedef typename A::argument V;

	A& accu = exact(accu_);

	mln_piter(I) p(inout.domain());
	for_all(p)
	  if (is_non_primary_face_vertical(p))
	  {
	    accu.init();
	    V tmp;

	    convert_on_access(inout(p + 2 * left), tmp);
	    accu.take(tmp);

	    convert_on_access(inout(p + 2 * right), tmp);
	    accu.take(tmp);

	    convert_on_access(accu.to_result(), inout(p));
	  }
	  else if (is_non_primary_face_horizontal(p))
	  {
	    accu.init();
	    V tmp;

	    convert_on_access(inout(p + 2 * up), tmp);
	    accu.take(tmp);

	    convert_on_access(inout(p + 2 * down), tmp);
	    accu.take(tmp);

	    convert_on_access(accu.to_result(), inout(p));
	  }
	  else if (is_non_primary_face_cross(p))
	  {
	    accu.init();
	    V tmp;

	    convert_on_access(inout(p + 2 * up_left), tmp);
	    accu.take(tmp);

	    convert_on_access(inout(p + 2 * up_right), tmp);
	    accu.take(tmp);

	    convert_on_access(inout(p + 2 * down_left), tmp);
	    accu.take(tmp);

	    convert_on_access(inout(p + 2 * down_right), tmp);
	    accu.take(tmp);

	    convert_on_access(accu.to_result(), inout(p));
	  }

	trace::exiting("mln::world::k2::fill_non_primary_from_primary_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k2

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K2_FILL_NON_PRIMARY_FROM_PRIMARY_FACES_HH
