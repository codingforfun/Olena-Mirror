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

#ifndef MLN_WORLD_KN_IMMERSE_WITH_INNER_BORDER_HH
# define MLN_WORLD_KN_IMMERSE_WITH_INNER_BORDER_HH

/// \file
///
/// \brief Immerse a 2D image into Kn with additional border pixels.

# include <mln/core/concept/image.hh>
# include <mln/data/fill.hh>
# include <mln/world/kn/internal/domain_from_k0.hh>
# include <mln/world/kn/internal/fill_primary_2_faces_from_input.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {
      /*! \brief Immerse a 2D image into Kn with additional border
      ///  pixels.

      \param[in] ima A 2D image.
      \param[in] n   Set in which space K\p n to immerse \p ima.
      \param[in] new_value_type An instance of the output value type.
      \param[in] inner_border_thickness Define the inner border size.

      By default, the output image is not initialized except the
      primary 2-faces.

      \verbatim
            0 1

	 0  X Y
      \endverbatim

        immersion with n = 2 and inner_border_thickness = 1 -->

      \verbatim
           0       4       8       12
         + - + - + - + - + - + - + - +
       0 | b |   | b |   | b |   | b |
         + - + - + - + - + - + - + - +
         |   |   |   |   |   |   |   |
         + - + - + - + - + - + - + - +
       4 | b |   | X |   | Y |   | b |
         + - + - + - + - + - + - + - +
         |   |   |   |   |   |   |   |
         + - + - + - + - + - + - + - +
       8 | b |   | b |   | b |   | b |
         + - + - + - + - + - + - + - +
      \endverbatim
      */
      template <typename I, typename V>
      mln_ch_value(I,V)
      immerse_with_inner_border(const Image<I>& ima,
				unsigned n,
				const V& new_value_type,
				unsigned inner_border_thickness);

      /// \overload
      /// This overload initializes the output image with \p
      /// default_value.
      template <typename I, typename V>
      mln_ch_value(I,V)
      immerse_with_inner_border(const Image<I>& ima,
				unsigned n,
				const V& new_value_type,
				const V& default_value,
				unsigned inner_border_thickness);

      /// \overload
      /// The output image has the same type as \p ima.
      template <typename I>
      mln_concrete(I)
      immerse_with_inner_border(const Image<I>& ima,
				unsigned n,
				unsigned inner_border_thickness);


# ifndef MLN_INCLUDE_ONLY

      template <typename I, typename V>
      mln_ch_value(I,V)
      immerse_with_inner_border(const Image<I>& ima_,
				unsigned n,
				const V& new_value_type,
				unsigned inner_border_thickness)
      {
	trace::entering("mln::world::kn::immerse_with_inner_border");
	mln_precondition(exact(ima_).is_valid());
	(void) new_value_type;

	const I& ima = exact(ima_);

	mln_ch_value(I,V)
	  output(internal::domain_from_k0(ima.domain(), n,
					  inner_border_thickness));

	internal::fill_primary_2_faces_from_input(output, n, ima,
						  inner_border_thickness);

	trace::exiting("mln::world::kn::immerse_with_inner_border");
	return output;
      }

      template <typename I, typename V>
      mln_ch_value(I,V)
      immerse_with_inner_border(const Image<I>& ima_,
				unsigned n,
				const V& new_value_type,
				const V& default_value,
				unsigned inner_border_thickness)
      {
	trace::entering("mln::world::kn::immerse_with_inner_border");
	mln_precondition(exact(ima_).is_valid());
	(void) new_value_type;

	const I& ima = exact(ima_);

	mln_ch_value(I,V)
	  output(internal::domain_from_k0(ima.domain(), n,
					  inner_border_thickness));
	data::fill(output, default_value);

	internal::fill_primary_2_faces_from_input(output, n, ima,
						  inner_border_thickness);

	trace::exiting("mln::world::kn::immerse_with_inner_border");
	return output;
      }

      template <typename I>
      mln_concrete(I)
      immerse_with_inner_border(const Image<I>& ima,
				unsigned n,
				unsigned inner_border_thickness)
      {
	typedef mln_value(I) V;
	return immerse_with_inner_border(ima, n, V(), inner_border_thickness);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln


#endif // ! MLN_WORLD_KN_IMMERSE_WITH_INNER_BORDER_HH