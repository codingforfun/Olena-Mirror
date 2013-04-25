// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_FUN_V2V_MAHALANOBIS_HH
# define MLN_FUN_V2V_MAHALANOBIS_HH

/// \file
///
/// \brief Define a function computing the Mahalanobis distance.

# include <cmath>
# include <mln/core/concept/function.hh>
# include <mln/algebra/vec.hh>
# include <mln/algebra/mat.hh>


namespace mln
{

  namespace fun
  {

    namespace v2v
    {

      /* \brief Compute the Mahalanobis distance.

 	\sa data::transform
	\ingroup funv2v
      */
      template <typename V, typename R = float>
      struct mahalanobis
	: public Function_v2v< mahalanobis<V,R> >,
	  private metal::equal< V, algebra::vec<V::dim,R> >::check_t
      {
	enum { n = V::dim };
	typedef R result;

	mahalanobis(const algebra::mat<n,n,R>& var,
		    const algebra::vec<n,R>&   mean);
	// Tech. note: using n (instead of V::dim) above helps g++-2.95.

	R operator()(const V& v) const;

	typedef algebra::vec<n,R> mean_t;

	mean_t mean() const;

      protected:
	algebra::mat<n,n,R> var_1_;
	algebra::vec<n,R>   mean_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename V, typename R>
      inline
      mahalanobis<V,R>::mahalanobis(const algebra::mat<n,n,R>& var,
				    const algebra::vec<n,R>&   mean)
      {
	var_1_ = var._1();
	mean_  = mean;
      }

      template <typename V, typename R>
      inline
      R
      mahalanobis<V,R>::operator()(const V& v) const
      {
	return std::sqrt((v - mean_).t() * var_1_ * (v - mean_));
      }

      template <typename V, typename R>
      inline
      typename mahalanobis<V,R>::mean_t
      mahalanobis<V,R>::mean() const
      {
	return mean_;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::v2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_V2V_MAHALANOBIS_HH
