// Copyright (C) 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_OPTRAITS_VEC_HH
# define OLENA_VALUE_OPTRAITS_VEC_HH

# include <mlc/basics.hh>
# include <mlc/type.hh>

# include <oln/types/predecls.hh>
# include <oln/types/builtins_properties.hh>
# include <oln/types/to_oln.hh>
# include <oln/types/optraits.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/behaviour.hh>
# include <oln/types/global_ops_traits.hh>
# include <oln/types/vec.hh>

# include <mlc/cmp.hh>


# define ASSIGN_VECTOR_VECTOR_OPERATOR(Name, Op)	\
template <class T1, class T2> inline			\
static T1& Name(T1& lhs, const T2& rhs)			\
{							\
  is_a(optraits<T1>, oln::optraits_vector)::ensure();	\
  is_a(optraits<T2>, oln::optraits_vector)::ensure();	\
  precondition(lhs.size() == rhs.size());		\
  unsigned s = lhs.size();				\
  for (unsigned i = 0; i < s; ++i)			\
    lhs[i] Op rhs[i];					\
  return lhs;						\
}


# define ASSIGN_VECTOR_SCALAR_OPERATOR(Name, Op)	\
template <class T1, class T2> inline			\
static T1& Name(T1& lhs, const T2& rhs)			\
{							\
  is_a(optraits<T1>, oln::optraits_vector)::ensure();	\
  is_a(optraits<T2>, oln::optraits_scalar)::ensure();	\
  unsigned s = lhs.size();				\
  for (unsigned i = 0; i < s; ++i)			\
    lhs[i] Op rhs;					\
  return lhs;						\
}


# define ARITH_VECTOR_VECTOR_OPERATOR(Name, Op) 			\
template <class T1, class T2>          					\
inline static typename                 					\
internal::deduce_from_traits<internal::operator_##Name##_traits, 	\
                             T1, T2>::ret   				\
Name(const T1& lhs, const T2& rhs)					\
{									\
  is_a(optraits<T1>, oln::optraits_vector)::ensure();			\
  is_a(optraits<T2>, oln::optraits_vector)::ensure();			\
  typedef typename                          				\
    internal::deduce_from_traits<internal::operator_##Name##_traits,   	\
    T1, T2>::ret return_type; 						\
  return_type result(lhs); 						\
  result Op rhs;            						\
  return result;							\
}

namespace oln
{
  //
  //  optraits for vec<N,T>
  //
  //////////////////////////////////////

  template <unsigned N, class T, class Self>
  class optraits<vec<N, T, Self> >: public optraits_vector<vec<N, T, Self> >
  {
    typedef vec<N, T, Self> self;
    typedef typename typetraits<self>::storage_type storage_type;
  public:

    static self zero ()
    {
      // A vectorial type MUST return a zero initialized value.
      return self();
    }

    // debug
    static std::string name() {
      std::ostringstream out;
      out << "vec<" << N << ", " << optraits<T>::name() << ">"<< std::ends;
      return out.str();
    }

    // No unit() for vector.
    // static storage_type unit ();

    ASSIGN_VECTOR_VECTOR_OPERATOR(plus_equal,  +=);
    ASSIGN_VECTOR_VECTOR_OPERATOR(minus_equal, -=);
    ASSIGN_VECTOR_SCALAR_OPERATOR(times_equal, *=);
    ASSIGN_VECTOR_SCALAR_OPERATOR(div_equal,   /=);
    ASSIGN_VECTOR_SCALAR_OPERATOR(mod_equal,   %=);

    ARITH_VECTOR_VECTOR_OPERATOR(plus, +=);
    ARITH_VECTOR_VECTOR_OPERATOR(minus, -=);

    // division

    template <class T1, class T2>
    inline static typename
    internal::deduce_from_traits<internal::operator_div_traits,
				 T1, T2>::ret
    div(const T1& lhs, const T2& rhs)
    {
      is_a(optraits<T1>, oln::optraits_vector)::ensure();
      is_a(optraits<T2>, oln::optraits_scalar)::ensure();
      typedef typename
	internal::deduce_from_traits<internal::operator_div_traits,
	T1, T2>::ret return_type;
      return_type result(lhs);
      result /= rhs;
      return result;
    }

    // modulo

    template <class T1, class T2>
    inline static typename
    internal::deduce_from_traits<internal::operator_mod_traits,
				 T1, T2>::ret
    mod(const T1& lhs, const T2& rhs)
    {
      is_a(optraits<T1>, oln::optraits_vector)::ensure();
      is_a(optraits<T2>, oln::optraits_scalar)::ensure();
      typedef typename
	internal::deduce_from_traits<internal::operator_mod_traits,
	T1, T2>::ret return_type;
      return_type result(lhs);
      result %= rhs;
      return result;
    }

    // multiplication

    // dot-product
    template <unsigned M, class T1, class T2>
    inline static typename
    internal::deduce_from_traits<internal::operator_times_traits,
				 vec<M, T1>, vec<M, T2> >::ret
    times(const vec<M, T1>& lhs, const vec<M, T2>& rhs)
    {
      typedef vec<M, T1> vec1;
      typedef vec<M, T2> vec2;
      is_a(optraits<vec1>, oln::optraits_vector)::ensure();
      is_a(optraits<vec2>, oln::optraits_vector)::ensure();
      typedef typename
	internal::deduce_from_traits<internal::operator_times_traits,
	vec<M, T1>, vec<M, T2> >::ret return_type;
      precondition(lhs.size() == rhs.size());

      return_type result = optraits<return_type>::zero();
      unsigned s = lhs.size();
      for (unsigned i = 0; i < s; ++i)
	result += lhs[i] * rhs[i];

      return result;
    }

    // vector * scalar
    template <class T1, class T2>
    inline static typename
    internal::deduce_from_traits<internal::operator_times_traits,
				 T1, T2>::ret
    times(const rec_vector<T1>& lhs, const T2& rhs)
    {
      is_a(optraits<T1>, oln::optraits_vector)::ensure();
      is_a(optraits<T2>, oln::optraits_scalar)::ensure();
      typedef typename
	internal::deduce_from_traits<internal::operator_times_traits,
	T1, T2>::ret return_type;
      return_type result(lhs.self());
      result *= rhs;
      return result;
    }

    // scalar * vector
    template <class T1, class T2>
    inline static typename
    internal::deduce_from_traits<internal::operator_times_traits,
				 T1, T2>::ret
    times(const T1& lhs, const rec_vector<T2>& rhs)
    {
      return times(rhs, lhs);
    }


    template <class T1, class T2>
    inline static bool cmp_eq (const T1& lhs, const T2& rhs)
    {
      is_a(optraits<T1>, oln::optraits_vector)::ensure();
      is_a(optraits<T2>, oln::optraits_vector)::ensure();
      precondition(lhs.size() == rhs.size());

      typedef typename
	internal::deduce_from_traits<internal::operator_cmp_traits,
	T1, T2>::ret tmp_type;

      unsigned s = lhs.size();
      for (unsigned i = 0; i < s; ++i)
	if (lhs[i] != rhs[i])
	  return false;
      return true;
    }
  };


  namespace internal {

    //
    // Operators traits
    //
    ////////////////////


    //
    // plus
    //

    // vec + vec

    template<unsigned N, class T1, class T2, class S1, class S2>
    struct operator_plus_traits<vec<N, T1, S1>, vec<N, T2, S2> >
    {
      enum { commutative = true };
      typedef vec<N, typename deduce_from_traits<internal::operator_plus_traits, T1, T2>::ret> ret;
      typedef vec<N, T1> impl;
    };


    //
    // minus
    //

    // vec - vec

    template<unsigned N, class T1, class T2>
    struct operator_minus_traits<vec<N, T1>, vec<N, T2> >
    {
      enum { commutative = true };
      typedef vec<N, typename deduce_from_traits<internal::operator_minus_traits, T1, T2>::ret> ret;
      typedef vec<N, T1> impl;
    };

    //
    // times
    //

    // vec * s; s * vec

    template<unsigned N, class T1, class T2>
    struct operator_times_traits<vec<N, T1>, T2>
    {
      enum { commutative = true };
      typedef vec<N, typename deduce_from_traits<internal::operator_times_traits, T1, T2>::ret> ret;
      typedef vec<N, T1> impl;
    };

    // vec * vec

    template<unsigned N, class T1, class T2>
    struct operator_times_traits<vec<N, T1>, vec<N, T2> >
    {
      enum { commutative = true };
      typedef typename deduce_from_traits<internal::operator_times_traits,T1,T2>::ret t;
      typedef typename typetraits<t>::cumul_type ret;
      typedef vec<N, T1> impl;
    };

    //
    // div
    //

    // vec / s

    template<unsigned N, class T1, class T2>
    struct operator_div_traits<vec<N, T1>, T2>
    {
      enum { commutative = false };
      typedef vec<N, typename deduce_from_traits<internal::operator_div_traits, T1, T2>::ret> ret;
      typedef vec<N, T1> impl;
    };

    //
    // mod
    //

    // vec % s

    template<unsigned N, class T1, class T2>
    struct operator_mod_traits<vec<N, T1>, T2>
    {
      enum { commutative = false };
      typedef vec<N, typename deduce_from_traits<internal::operator_mod_traits, T1, T2>::ret> ret;
      typedef vec<N, T1> impl;
    };

    //
    // Comparison operator
    //

    // vec compared with vec

    template<unsigned N, class T1, class T2, class S1, class S2>
    struct operator_cmp_traits<vec<N, T1, S1>, vec<N, T2, S2> >
    {
      enum { commutative = true };
      typedef vec<N, typename deduce_from_traits<internal::operator_cmp_traits, T1, T2>::ret> ret;
      typedef vec<N, T1> impl;
    };

  } // end of namespace internal
} // namespace oln

#endif // ndef OLENA_VALUE_OPTRAITS_VEC_HH
