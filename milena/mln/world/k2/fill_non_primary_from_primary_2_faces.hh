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

#ifndef MLN_WORLD_K2_FILL_NON_PRIMARY_FROM_PRIMARY_2_FACES_HH
# define MLN_WORLD_K2_FILL_NON_PRIMARY_FROM_PRIMARY_2_FACES_HH

# include <mln/core/alias/point2d.hh>
# include <mln/world/k2/is_non_primary_2_face_vertical.hh>
# include <mln/world/k2/is_non_primary_2_face_horizontal.hh>
# include <mln/world/k2/is_non_primary_2_face_center.hh>
# include <mln/world/kn/safe_cast.hh>
# include <mln/world/kn/border/adjust_duplicate_2_faces.hh>

namespace mln
{

  namespace world
  {

    namespace k2
    {

      /*! \brief Fill non-primary 2-faces in a K2 2D image using
       *  primary 2-faces.

	\param[in,out] ima A 2D image immersed in K2.
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

	* In case of center non-primary 2-face:

	\verbatim
	      x   x
		o
	      x   x
	\endverbatim

       */
      template <typename I, typename A>
      void fill_non_primary_from_primary_2_faces(Image<I>& ima,
						 const Accumulator<A>& accu);


      /// \overload
      /*!

	\param[in] f_intermediate The function used with non_primary
	                          2-faces located between two primary
	                          2-faces.

        \param[in] f_center The function used with non_primary 2-faces
	                    located the middle of four other
	                    non-primary 2-faces.
       */
      template <typename I, typename F2, typename F4>
      void fill_non_primary_from_primary_2_faces(Image<I>& ima,
						 const Function_vv2v<F2>& f_intermediate,
						 const Function_vvvv2v<F4>& f_center);



# ifndef MLN_INCLUDE_ONLY


      // Facade

      template <typename I, typename F2, typename F4>
      void fill_non_primary_from_primary_2_faces(Image<I>& ima_,
						 const Function_vv2v<F2>& f_intermediate_,
						 const Function_vvvv2v<F4>& f_center_)
      {
	trace::entering("mln::world::k2::fill_non_primary_from_primary_2_faces");

	I& ima = exact(ima_);
	const F2& f_intermediate = exact(f_intermediate_);
	const F4& f_center = exact(f_center_);

	mln_precondition(ima.is_valid());

	mln_piter(I) p(ima.domain());
	for_all(p)
	  if (is_non_primary_2_face_vertical(p))
	  {
	    ima(p) = kn::safe_cast(
	      f_intermediate(kn::safe_cast(ima(p + 2 * left)),
			     kn::safe_cast(ima(p + 2 * right))));
	  }
	  else if (is_non_primary_2_face_horizontal(p))
	  {
	    ima(p) = kn::safe_cast(
	      f_intermediate(kn::safe_cast(ima(p + 2 * up)),
			     kn::safe_cast(ima(p + 2 * down))));
	  }
	  else if (is_non_primary_2_face_center(p))
	  {
	    ima(p) = kn::safe_cast(
	      f_center(kn::safe_cast(ima(p + 2 * up_left)),
		       kn::safe_cast(ima(p + 2 * up_right)),
		       kn::safe_cast(ima(p + 2 * down_left)),
		       kn::safe_cast(ima(p + 2 * down_right))));
	  }

	// Initialize border.
	kn::border::adjust_duplicate_2_faces(ima, 1);

	trace::exiting("mln::world::k2::fill_non_primary_from_primary_2_faces");
      }


      template <typename I, typename A>
      void fill_non_primary_from_primary_2_faces(Image<I>& ima_,
						 const Accumulator<A>& accu_)
      {
	trace::entering("mln::world::k2::fill_non_primary_from_primary_2_faces");
	I& ima = exact(ima_);
	A accu = exact(accu_);

	mln_precondition(ima.is_valid());

	typedef mln_argument(A) arg;
	mln_piter(I) p(ima.domain());
	for_all(p)
	  if (is_non_primary_2_face_vertical(p))
	  {
	    accu.init();
	    if (ima.domain().has(p + 2 * left))
	      accu.take(kn::safe_cast_to<arg>(ima(p + 2 * left)));
	    if (ima.domain().has(p + 2 * right))
	      accu.take(kn::safe_cast_to<arg>(ima(p + 2 * right)));
	    ima(p) = kn::safe_cast(accu.to_result());
	  }
	  else if (is_non_primary_2_face_horizontal(p))
	  {
	    accu.init();
	    if (ima.domain().has(p + 2 * up))
	      accu.take(kn::safe_cast_to<arg>(ima(p + 2 * up)));
	    if (ima.domain().has(p + 2 * down))
	      accu.take(kn::safe_cast_to<arg>(ima(p + 2 * down)));
	    ima(p) = kn::safe_cast(accu.to_result());
	  }
	  else if (is_non_primary_2_face_center(p))
	  {
	    accu.init();
	    if (ima.domain().has(p + 2 * up_left))
	      accu.take(kn::safe_cast_to<arg>(ima(p + 2 * up_left)));
	    if (ima.domain().has(p + 2 * up_right))
	      accu.take(kn::safe_cast_to<arg>(ima(p + 2 * up_right)));
	    if (ima.domain().has(p + 2 * down_left))
	      accu.take(kn::safe_cast_to<arg>(ima(p + 2 * down_left)));
	    if (ima.domain().has(p + 2 * down_right))
	      accu.take(kn::safe_cast_to<arg>(ima(p + 2 * down_right)));
	    ima(p) = kn::safe_cast(accu.to_result());
	  }

	trace::exiting("mln::world::k2::fill_non_primary_from_primary_2_faces");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::k2

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_K2_FILL_NON_PRIMARY_FROM_PRIMARY_2_FACES_HH
