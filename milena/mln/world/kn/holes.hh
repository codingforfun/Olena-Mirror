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
/// \brief

#ifndef MLN_WORLD_KN_SATURATE_HH
# define MLN_WORLD_KN_SATURATE_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/labeling/background.hh>
# include <mln/data/fill.hh>
# include <mln/pw/all.hh>
# include <mln/world/kn/internal/get_background_label.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {


      template <typename I>
      mln_concrete(I)
      holes(const Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      mln_concrete(I)
      holes(const Image<I>& input_)
      {
	trace::entering("mln::world::kn::holes");
	mlc_equal(mln_trait_image_kind(I),
		  mln::trait::image::kind::binary)::check();
	mln_precondition(exact(input_).is_valid());

	const I& input = exact(input_);

	unsigned n_labels;
	mln_ch_value(I,unsigned)
	  lab = labeling::background(input, c4(), n_labels);

	bool has_bg_label;
	unsigned bg = internal::get_background_label(lab, has_bg_label);

	mln_concrete(I) output(input.domain());

	if (! has_bg_label)
	{
	  data::fill(output, false);
	  return output;
	}

	data::fill(output, pw::value(lab) != pw::cst(bg)
		   && pw::value(lab) != pw::cst(literal::zero));

	trace::exiting("mln::world::kn::holes");
	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_SATURATE_HH
