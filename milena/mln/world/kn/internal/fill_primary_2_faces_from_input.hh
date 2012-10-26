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
/// \brief Fill primary 2-faces from a Kn image using original pixel
/// as values.

#ifndef MLN_WORLD_KN_FILL_PRIMARY_2_FACES_FROM_INPUT_HH
# define MLN_WORLD_KN_FILL_PRIMARY_2_FACES_FROM_INPUT_HH

# include <mln/world/kn/internal/immerse_point.hh>
# include <mln/world/kn/safe_cast.hh>
# include <mln/world/kn/border/adjust_duplicate_2_faces.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace internal
      {

	/// \brief Fill primary 2-faces from a Kn image using original pixel
	/// as values.
	template <typename I, typename J>
	void
	fill_primary_2_faces_from_input(Image<I>& ima_kn,
					unsigned n,
					const Image<J>& ima,
					const unsigned inner_border_thickness);

	/// \overload
	/// \p inner_border_thickness is set to 0.
	template <typename I, typename J>
	void
	fill_primary_2_faces_from_input(Image<I>& ima_kn,
					unsigned n,
					const Image<J>& ima);

# ifndef MLN_INCLUDE_ONLY


	template <typename I, typename J>
	void
	fill_primary_2_faces_from_input(Image<I>& ima_kn_,
					unsigned n,
					const Image<J>& ima_,
					const unsigned inner_border_thickness)
	{
	  trace::entering("mln::world::kn::internal::fill_primary_2_faces_from_input");
	  mlc_equal(mln_site(I), mln_site(J))::check();
	  mln_precondition(exact(ima_).is_valid());
	  mln_precondition(exact(ima_kn_).is_valid());
	  mln_precondition(exact(ima_).domain() <= exact(ima_kn_).domain());

	  I& ima_kn = exact(ima_kn_);
	  const J& ima = exact(ima_);

	  // Filling Primary 2-Faces
	  typedef mln_value(I) V;
	  mln_piter(J) p(ima.domain());
	  for_all(p)
	  {
	    mln_site(I) pout = internal::immerse_point(p, n, inner_border_thickness);
	    ima_kn(pout) = safe_cast_to<V>(ima(p));
	  }

	  kn::border::adjust_duplicate_2_faces(ima_kn, 1);

	  trace::exiting("mln::world::kn::internal::fill_primary_2_faces_from_input");
	}


	template <typename I, typename J>
	void
	fill_primary_2_faces_from_input(Image<I>& ima_kn,
					unsigned n,
					const Image<J>& ima)
	{
	  return fill_primary_2_faces_from_input(ima_kn, n, ima, 0);
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::work::kn::internal

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_FILL_PRIMARY_2_FACES_FROM_INPUT_HH


