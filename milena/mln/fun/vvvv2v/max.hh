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

#ifndef MLN_FUN_VVVV2V_MAX_HH
# define MLN_FUN_VVVV2V_MAX_HH

/// \file
///
/// Functor that computes the max of four values.

# include <mln/core/concept/function.hh>
# include <mln/math/max.hh>


namespace mln
{

  namespace fun
  {

    namespace vvvv2v
    {

      /// \brief A functor computing the max of four values.
      template <typename T, typename R=T>
      struct max : public Function_vvvv2v< max<T> >
      {
	typedef R result;
	typedef T argument;
	R operator()(const T& v1, const T& v2, const T& v3, const T& v4) const;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename T, typename R>
      inline
      R
      max<T,R>::operator()(const T& v1, const T& v2, const T& v3, const T& v4) const
      {
	return R(mln::math::max(v1, v2, v3, v4));
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::vvvv2v

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_VVVV2V_MAX_HH
