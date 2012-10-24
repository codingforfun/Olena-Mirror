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

#ifndef MLN_INNER_BORDER_EXTEND_AND_FILL_HH
# define MLN_INNER_BORDER_EXTEND_AND_FILL_HH

/// \file
///
/// \brief Extend the inner border of an image.

# include <mln/inner_border/extend.hh>
# include <mln/inner_border/fill.hh>


namespace mln
{

  namespace inner_border
  {

    /// \brief Extend the inner border of an image and fill it with a
    /// value.
    template <typename I>
    mln_concrete(I)
    extend_and_fill(const Image<I>& input, unsigned inner_border_size,
		    const mln_value(I)& value);

    /// \overload
    /// inner_border_size is set to 1.
    template <typename I>
    mln_concrete(I)
    extend_and_fill(const Image<I>& input, const mln_value(I)& value);

# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    mln_concrete(I)
    extend_and_fill(const Image<I>& input, unsigned inner_border_size,
		    const mln_value(I)& value)
    {
      trace::entering("mln::inner_border::extend_and_fill");
      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output = inner_border::extend(input, inner_border_size);
      inner_border::fill(output, inner_border_size, value);

      trace::exiting("mln::inner_border::extend_and_fill");
      return output;
    }

    template <typename I>
    mln_concrete(I)
    extend_and_fill(const Image<I>& input, const mln_value(I)& value)
    {
      return extend_and_fill(input, 1, value);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::inner_border

} // end of namespace mln

#endif // ! MLN_INNER_BORDER_EXTEND_AND_FILL_HH
