// Copyright (C) 2001  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef OLENA_CORE_COMPOSE_HH
# define OLENA_CORE_COMPOSE_HH

# include <functional>

namespace oln {

  namespace internal {

    template< class F1, class F2 >
    struct compose_uu_ :
      public std::unary_function <typename F2::argument_type,
				  typename F1::result_type>
    {
      typedef compose_uu_ self_type;

      typename self_type::result_type
      operator()(typename self_type::argument_type arg) const {
	return f1_(f2_(arg));
      }

      compose_uu_(const F1& f1, const F2& f2) : f1_(f1), f2_(f2) {}
      private:
      const F1 f1_;
      const F2 f2_;
    };

    template< class F1, class F2 >
    struct compose_ub_ :
      public std::binary_function <typename F2::first_argument_type,
				   typename F2::second_argument_type,
				   typename F1::result_type>
    {
      typedef compose_ub_ self_type;

      typename self_type::result_type
      operator()(typename self_type::first_argument_type arg1,
		 typename self_type::second_argument_type arg2) const {
	return f1_(f2_(arg1, arg2));
      }

      compose_ub_(const F1& f1, const F2& f2) : f1_(f1), f2_(f2) {}
      private:
      const F1 f1_;
      const F2 f2_;
    };

    template< class F1, class F2 >
    struct compose_bu_ :
      public std::binary_function <typename F2::argument_type,
				   typename F2::argument_type,
				   typename F1::result_type>
    {
      typedef compose_bu_ self_type;

      typename self_type::result_type
      operator()(typename self_type::first_argument_type arg1,
		 typename self_type::second_argument_type arg2) const {
	return f1_(f2_(arg1), f2_(arg2));
      }

      compose_bu_(const F1& f1, const F2& f2) : f1_(f1), f2_(f2) {}
      private:
      const F1 f1_;
      const F2 f2_;
    };

  }


  template<class UF1, class UF2>
  internal::compose_uu_<UF1, UF2>
  compose_uu(const UF1& f1, const UF2& f2)
  {
    return internal::compose_uu_<UF1, UF2>(f1, f2);
  }

  template<class UF1, class BF2>
  internal::compose_ub_<UF1, BF2>
  compose_ub(const UF1& f1, const BF2& f2)
  {
    return internal::compose_ub_<UF1, BF2>(f1, f2);
  }

  template<class BF1, class UF2>
  internal::compose_bu_<BF1, UF2>
  compose_bu(const BF1& f1, const UF2& f2)
  {
    return internal::compose_bu_<BF1, UF2>(f1, f2);
  }


  template<class T>
  struct f_identity : std::unary_function<T, T>
  {
    T operator()(T t) const
    {
      return t;
    }
  };

} // end of oln

#endif // OLENA_CORE_COMPOSE_HH
