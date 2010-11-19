// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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


#ifndef XTC_MLN_VALUE_SET
# define XTC_MLN_VALUE_SET

# include <mln/core/concept/image.hh>
# include <mln/core/def/coord.hh>

/// Additional Milena routines for Extatica.
namespace xtc_mln
{
  /// Assign the value \a v to image \a ima at site \a p.
  template <typename I, typename V>
  inline
  void
  value_set(mln::Image<I>& ima_, const mln_psite(I)& p, const V& v)
  {
    I& ima = mln::exact(ima_);
    ima(p) = v;
  }

  /// Assign the value \a v to image \a ima at site (\a row, \a col).
  ///
  /// Useful for mln::image2d.
  template <typename I, typename V>
  inline
  void
  value_set_(mln::Image<I>& ima_, mln::def::coord row, mln::def::coord col,
	     const V& v)
  {
    I& ima = mln::exact(ima_);
    ima.at_(row, col) = v;
  }

} // end of namespace xtc_mln

#endif // ! XTC_MLN_VALUE_SET
