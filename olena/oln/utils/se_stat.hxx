// Copyright (C) 2004  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.


#ifndef OLN_UTILS_SE_STAT_HXX
# define OLN_UTILS_SE_STAT_HXX

namespace oln {
  namespace utils {

    /// Compute the mean and the variance.
    template <typename Sum, typename Var>
    template <class I, class S>
    inline se_stat<Sum, Var> &
    se_stat<Sum, Var>::compute(const oln::abstract::image<I> &im,
			       const oln_point_type(I) &p,
			       const oln::abstract::struct_elt<S> &s)
    {
      sum_ = ntg_zero_val(Sum);
      card_ = s.card();
      // Compute the mean
      oln_iter_type(S) it(s);
      {
	for_all(it)
	  sum_ += internal::f_to_float_<Sum,
	  oln_value_type(I)>::doit(im[p + it]);
	mean_ = sum_ / card_;
      }
      // Compute the variance
      {
	Var sum_2(ntg_zero_val(Var));
	for_all(it)
	  {
	    Sum tmp = mean_;
	    tmp -= im[p + it];
	    sum_2 += oln::math::dot_product<Var>(tmp, tmp);
	  }
	variance_ = sum_2 / card_;
      }
      return *this;
    }
  } // end namespace utils

} // end namespace oln

#endif //! OLN_UTILS_SE_STAT_HXX
