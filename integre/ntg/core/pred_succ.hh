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

#ifndef NTG_CORE_PRED_SUCC_HH
# define NTG_CORE_PRED_SUCC_HH


#include <mlc/is_a.hh>
#include <ntg/real/int_u.hh>
#include <ntg/core/macros.hh>

namespace ntg {

  namespace internal {
    //! Return a type which supports inc and dec.
    template <typename T>
    struct with_arith
    {
      typedef typename ntg_is_a(T, non_vectorial)::ensure_type non_v;

      typedef int_u<1> bool_with_arith;
      typedef T non_vectorial_with_arith;

      typedef typename mlc::if_<ntg_is_a(T, ntg::binary)::ret,
				id_<bool_with_arith>,
				id_<non_vectorial_with_arith> >::ret::ret ret;
    };
  }

  /*! Return the successor of \a t.
  **
  ** \note The goal is to iterate on types such as ntg::bin.
  */
  template <typename T>
  T
  succ(const T &t)
  {
    return T(typename internal::with_arith<T>::ret(t) + 1);
  }

  /*! Return the predecessor of \a t.
  **
  ** \note The goal is to iterate on types such as ntg::bin.
  */
  template <typename T>
  T
  pred(const T&t)
  {
    return T(typename internal::with_arith<T>::ret(t) - 1);
  }

  namespace internal {
    /*! Default less.
    **
    ** Specialization of this class provides a less functor even for
    ** types that do not support the operator "<".
    */
    template <typename T>
    struct default_less
    {
      typedef T arg_type;
      bool operator()(const T& l,
		      const T& r) const
      {
	return l < r;
      }
    };
  }
}

#endif
