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

#ifndef MLN_INNER_BORDER_FILL_HH
# define MLN_INNER_BORDER_FILL_HH

/// \file
///
/// \brief Fill the inner border of an image.

# include <mln/core/image/image2d.hh>
# include <mln/data/paste.hh>
# include <mln/inner_border/internal/is_on_frontiere.hh>

namespace mln
{

  namespace inner_border
  {

    /// \brief Fill the inner border of an image.
    template <typename I>
    void
    fill(Image<I>& input, unsigned border_size,
	 const mln_value(I)& value);

    /// \overload
    /// The border_size is set to 1.
    template <typename I>
    void
    fill(Image<I>& input, const mln_value(I)& value);


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    void
    fill(Image<I>& input_, unsigned border_size,
	 const mln_value(I)& value)
    {
      trace::entering("mln::inner_border::fill");
      mln_precondition(exact(input_).is_valid());
      I& input = exact(input_);

      mln_piter(I) p(input.domain());
      for_all(p)
	if (internal::is_on_frontiere(p, input.domain(), border_size))
	  input(p) = value;

      trace::exiting("mln::inner_border::fill");
    }

    template <typename I>
    void
    fill(Image<I>& input, const mln_value(I)& value)
    {
      fill(input, 1, value);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::inner_border

} // end of namespace mln

#endif // ! MLN_INNER_BORDER_FILL_HH

