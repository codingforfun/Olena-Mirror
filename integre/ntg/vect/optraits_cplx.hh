// Copyright (C) 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_OPTRAITS_CPLX_HH
# define NTG_OPTRAITS_CPLX_HH

# include <mlc/bool.hh>
# include <mlc/is_a.hh>

# include <ntg/core/predecls.hh>
# include <ntg/utils/to_oln.hh>
# include <ntg/core/optraits.hh>
# include <ntg/core/typetraits_builtins.hh>
# include <ntg/core/behaviour.hh>
# include <ntg/core/global_ops_traits.hh>
# include <ntg/vect/cplx.hh>

// Assignement operators macros

# define ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(Rep, Name, Op)			\
template <class T1, class T2> inline						\
static cplx<Rep, T1>& Name(cplx<Rep, T1>& lhs, const T2& rhs)			\
{										\
  is_a(optraits<T2>, ntg::optraits_scalar)::ensure();				\
  lhs.first() Op rhs;								\
  return lhs;									\
}

# define ASSIGN_CPLX_SCALAR_OPERATOR_BOTH(Rep, Name, Op)			\
template <class T1, class T2> inline						\
static cplx<Rep, T1>& Name(cplx<Rep, T1>& lhs, const T2& rhs)			\
{										\
  is_a(optraits<T2>, ntg::optraits_scalar)::ensure();				\
  lhs.first() Op rhs;								\
  lhs.second() Op rhs;								\
  return lhs;									\
}

# define ASSIGN_CPLX_POLAR_SCALAR_OPERATOR(Name, Op)				\
template <class T1, class T2> inline						\
static cplx<polar, T1>& Name(cplx<polar, T1>& lhs, const T2& rhs)		\
{										\
  is_a(optraits<T2>, ntg::optraits_scalar)::ensure();				\
  cplx<rect, dfloat> tmp(lhs);							\
  tmp.real() Op rhs;								\
  lhs = tmp;									\
  return lhs;									\
}

# define ASSIGN_CPLX_RECT_CPLX_OPERATOR_MULT(Name, Op1, Op2)			\
template <class T1, cplx_representation R2, class T2> inline			\
static cplx<rect, T1>& Name(cplx<rect, T1>& lhs, const cplx<R2, T2>& rhs)	\
{										\
  cplx<polar, dfloat> tmp(lhs);							\
  tmp.magn() Op1 (dfloat)rhs.magn();						\
  tmp.angle() Op2 (dfloat)rhs.angle();						\
  lhs = tmp;									\
  return lhs;									\
}

# define ASSIGN_CPLX_RECT_CPLX_OPERATOR_ADD(Name, Op1, Op2)			\
template <cplx_representation R, class T1, class T2> inline			\
static cplx<rect, T1>& Name(cplx<rect, T1>& lhs, const cplx<R, T2>& rhs)	\
{										\
  lhs.first() Op1 rhs.real();							\
  lhs.second() Op2 rhs.imag();							\
  return lhs;									\
}

# define ASSIGN_CPLX_POLAR_CPLX_OPERATOR_MULT(Name, Op1, Op2)			\
template <cplx_representation R, class T1, class T2> inline			\
static cplx<polar, T1>& Name(cplx<polar, T1>& lhs, const cplx<R, T2>& rhs)	\
{										\
  lhs.magn() Op1 rhs.magn();							\
  lhs.angle() Op2 rhs.angle();							\
  return lhs;									\
}

# define ASSIGN_CPLX_POLAR_CPLX_OPERATOR_ADD(Name, Op)				\
template <cplx_representation R, class T1, class T2> inline			\
static cplx<polar, T1>& Name(cplx<polar, T1>& lhs, const cplx<R, T2>& rhs)	\
{										\
  cplx<rect, dfloat> tmp(lhs);							\
  tmp.real() Op (dfloat)rhs.real();						\
  tmp.imag() Op (dfloat)rhs.imag();						\
  lhs = tmp;									\
  return lhs;									\
}

# define ASSIGN_CPLX_VECTOR_OPERATOR(Rep, Name, Op)				\
template <class T1, class T2> inline						\
static cplx<Rep, T1>& Name(cplx<Rep, T1>& lhs, const vec<2, T2>& rhs)		\
{										\
  lhs.first() Op rhs[0];							\
  lhs.second() Op rhs[1];							\
  return lhs;									\
}

// Arithmetic operators macros

# define ARITH_CPLX_CPLX_OPERATOR(Rep1, Rep2, Name, Op)						\
template <class T1, class T2>									\
inline static											\
cplx<Rep1, typename internal::deduce_from_traits<internal::operator_##Name##_traits,		\
                             T1, T2>::ret>							\
Name(const cplx<Rep1, T1>& lhs, const cplx<Rep2, T2>& rhs)					\
{												\
  typedef											\
    cplx<Rep1, typename internal::deduce_from_traits<internal::operator_##Name##_traits,	\
    T1, T2>::ret> return_type;									\
  return_type result(lhs);									\
  result Op rhs;										\
  return result;										\
}

# define ARITH_CPLX_SCALAR_OPERATOR(Rep, Name, Op)						\
template <class T1, class T2>									\
inline static											\
cplx<Rep, typename internal::deduce_from_traits<internal::operator_##Name##_traits,		\
                             T1, T2>::ret>							\
Name(const cplx<Rep, T1>& lhs, const T2& rhs)							\
{												\
  is_a(optraits<T2>, ntg::optraits_scalar)::ensure();						\
  typedef											\
    cplx<Rep, typename internal::deduce_from_traits<internal::operator_##Name##_traits,		\
    T1, T2>::ret> return_type;									\
  return_type result(lhs);									\
  result Op rhs;										\
  return result;										\
}

# define ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(Rep, Name, Op)					\
template <class T1, class T2>									\
inline static											\
cplx<Rep, typename internal::deduce_from_traits<internal::operator_##Name##_traits,		\
                             T1, T2>::ret>							\
Name(const T1& lhs, const cplx<Rep, T2>& rhs)							\
{												\
  return Name(rhs, lhs);									\
}

# define ARITH_CPLX_VECTOR_OPERATOR(Rep, Name, Op);						\
template <class T1, class T2>									\
inline static											\
cplx<Rep, typename internal::deduce_from_traits<internal::operator_##Name##_traits,		\
                             T1, T2>::ret>							\
Name(const cplx<Rep, T1>& lhs, const vec<2, T2>& rhs)						\
{												\
  typedef											\
    cplx<Rep, typename internal::deduce_from_traits<internal::operator_##Name##_traits,		\
    T1, T2>::ret> return_type;									\
  return_type result(lhs);									\
  result Op rhs;										\
  return result;										\
}

# define ARITH_CPLX_VECTOR_OPERATOR_COMMUTE_PLUS(Rep, Name, Op);				\
template <class T1, class T2>									\
inline static											\
cplx<Rep, typename internal::deduce_from_traits<internal::operator_##Name##_traits,		\
                             T1, T2>::ret>							\
Name(const vec<2, T1>& lhs, const cplx<Rep, T2>& rhs)						\
{												\
  return Name(rhs, lhs);									\
}

# define ARITH_CPLX_VECTOR_OPERATOR_COMMUTE_MINUS(Rep, Name, Op);				\
template <class T1, class T2>									\
inline static											\
cplx<Rep, typename internal::deduce_from_traits<internal::operator_##Name##_traits,		\
                             T1, T2>::ret>							\
Name(const vec<2, T1>& lhs, const cplx<Rep, T2>& rhs)						\
{												\
  return Name(rhs, lhs).invert();								\
}

namespace ntg
{
  //
  //  optraits for cplx<rect, T>
  //
  //////////////////////////////////////

  template <class T>
  class optraits<cplx<rect, T> >: public optraits<vec<2, T> >
  {
    typedef cplx<rect, T> self;
    typedef typename typetraits<self>::storage_type storage_type;
    
  public:
    
    static self zero ()
    {
      return self();
    }
    
    static self unit ()
    {
      return self(1);
    }
    
    // debug
    static std::string name() {
      std::ostringstream out;
      out << "cplx<rect, " <<  optraits<T>::name() << ">" << std::ends;
      return out.str();
    }

    ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(rect, plus_equal, +=);
    ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(rect, minus_equal, -=);
    ASSIGN_CPLX_SCALAR_OPERATOR_BOTH(rect, times_equal, *=);
    ASSIGN_CPLX_SCALAR_OPERATOR_BOTH(rect, div_equal, /=);
    
    ASSIGN_CPLX_RECT_CPLX_OPERATOR_ADD(plus_equal, +=, +=);
    ASSIGN_CPLX_RECT_CPLX_OPERATOR_ADD(minus_equal, -=, -=);
    ASSIGN_CPLX_RECT_CPLX_OPERATOR_MULT(times_equal, *=, +=);
    ASSIGN_CPLX_RECT_CPLX_OPERATOR_MULT(div_equal, /=, -=);

    ASSIGN_CPLX_VECTOR_OPERATOR(rect, plus_equal, +=);
    ASSIGN_CPLX_VECTOR_OPERATOR(rect, minus_equal, -=);

    ARITH_CPLX_SCALAR_OPERATOR(rect, plus, +=);
    ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(rect, plus, +=);
    ARITH_CPLX_SCALAR_OPERATOR(rect, minus, -=);
    ARITH_CPLX_SCALAR_OPERATOR(rect, times, *=);
    ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(rect, times, *=);
    ARITH_CPLX_SCALAR_OPERATOR(rect, div, /=);

    ARITH_CPLX_CPLX_OPERATOR(rect, rect, plus, +=);
    ARITH_CPLX_CPLX_OPERATOR(rect, rect, minus, -=);
    ARITH_CPLX_CPLX_OPERATOR(rect, rect, times, *=);
    ARITH_CPLX_CPLX_OPERATOR(rect, rect, div, /=);

    ARITH_CPLX_CPLX_OPERATOR(rect, polar, plus, +=);
    ARITH_CPLX_CPLX_OPERATOR(rect, polar, minus, -=);
    ARITH_CPLX_CPLX_OPERATOR(rect, polar, times, *=);
    ARITH_CPLX_CPLX_OPERATOR(rect, polar, div, /=);

    ARITH_CPLX_VECTOR_OPERATOR(rect, plus, +=);
    ARITH_CPLX_VECTOR_OPERATOR_COMMUTE_PLUS(rect, plus, +=);
    ARITH_CPLX_VECTOR_OPERATOR(rect, minus, -=);
    ARITH_CPLX_VECTOR_OPERATOR_COMMUTE_MINUS(rect, minus, -=);

    template <class T1, cplx_representation R2, class T2>
    inline static bool cmp_eq (const cplx<rect, T1>& lhs,
			       const cplx<R2, T2>& rhs)
    {
      if (lhs.real() != rhs.real() || lhs.imag() != rhs.imag())
	return false;
      return true;
    }

  };

  //
  //  optraits for cplx<polar, T>
  //
  //////////////////////////////////////

  template <class T>
  class optraits<cplx<polar, T> >: public optraits_top<cplx<polar, T> >
  {
    typedef cplx<polar, T> self;
    typedef typename typetraits<self>::storage_type storage_type;
    
  public:
    
    static self zero ()
    {
      return self();
    }
    
    static self unit ()
    {
      return self(1);
    }
    
    // debug
    static std::string name() {
      std::ostringstream out;
      out << "cplx<polar, " <<  optraits<T>::name() << ">" << std::ends;
      return out.str();
    }

    ASSIGN_CPLX_POLAR_SCALAR_OPERATOR(plus_equal, +=);
    ASSIGN_CPLX_POLAR_SCALAR_OPERATOR(minus_equal, -=);
    ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(polar, times_equal, *=);
    ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(polar, div_equal, /=);

    ASSIGN_CPLX_POLAR_CPLX_OPERATOR_ADD(plus_equal, +=);
    ASSIGN_CPLX_POLAR_CPLX_OPERATOR_ADD(minus_equal, -=);
    ASSIGN_CPLX_POLAR_CPLX_OPERATOR_MULT(times_equal, *=, +=);
    ASSIGN_CPLX_POLAR_CPLX_OPERATOR_MULT(div_equal, /=, -=);

    ARITH_CPLX_SCALAR_OPERATOR(polar, plus, +=);
    ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(polar, plus, +=);
    ARITH_CPLX_SCALAR_OPERATOR(polar, minus, -=);
    ARITH_CPLX_SCALAR_OPERATOR(polar, times, *=);
    ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(polar, times, *=);
    ARITH_CPLX_SCALAR_OPERATOR(polar, div, /=);

    ARITH_CPLX_CPLX_OPERATOR(polar, polar, plus, +=);
    ARITH_CPLX_CPLX_OPERATOR(polar, polar, minus, -=);
    ARITH_CPLX_CPLX_OPERATOR(polar, polar, times, *=);
    ARITH_CPLX_CPLX_OPERATOR(polar, polar, div, /=);

    ARITH_CPLX_CPLX_OPERATOR(polar, rect, plus, +=);
    ARITH_CPLX_CPLX_OPERATOR(polar, rect, minus, -=);
    ARITH_CPLX_CPLX_OPERATOR(polar, rect, times, *=);
    ARITH_CPLX_CPLX_OPERATOR(polar, rect, div, /=);

    template <class T1, cplx_representation R2, class T2>
    inline static bool cmp_eq (const cplx<polar, T1>& lhs,
			       const cplx<R2, T2>& rhs)
    {
      if (lhs.magn() != rhs.magn() || lhs.angle() != rhs.angle())
	return false;
      return true;
    }

  };

  namespace internal 
  {
    
    // Operators traits macros

# define CPLX_SCALAR_OPERATORS_TRAITS(Name, CommuteBool)							\
    template <cplx_representation R1, class T1, class T2>							\
    struct operator_##Name##_traits<cplx<R1, T1>, T2>								\
    {														\
      enum { commutative = CommuteBool };									\
      typedef cplx<R1, typename deduce_from_traits<internal::operator_##Name##_traits, T1, T2>::ret> ret;	\
      typedef cplx<R1, T1> impl;										\
    }

# define CPLX_CPLX_OPERATORS_TRAITS(Name, CommuteBool)								\
    template <cplx_representation R1, class T1, cplx_representation R2, class T2>				\
    struct operator_##Name##_traits<cplx<R1, T1>, cplx<R2, T2> >						\
    {														\
      enum { commutative = CommuteBool };									\
      typedef cplx<R1, typename deduce_from_traits<internal::operator_##Name##_traits, T1, T2>::ret> ret;	\
      typedef cplx<R1, T1> impl;										\
    }

# define CPLX_VECTOR_OPERATORS_TRAITS(Rep, Name, CommuteBool)							\
    template <class T1, class T2>										\
    struct operator_##Name##_traits<cplx<Rep, T1>, vec<2, T2> >							\
    {														\
      enum { commutative = CommuteBool };									\
      typedef cplx<Rep, typename deduce_from_traits<internal::operator_##Name##_traits, T1, T2>::ret> ret;	\
      typedef cplx<Rep, T1> impl;										\
    }

    //
    // Operators traits
    //
    ////////////////////
    
    CPLX_SCALAR_OPERATORS_TRAITS(plus, true);
    CPLX_SCALAR_OPERATORS_TRAITS(minus, true);
    CPLX_SCALAR_OPERATORS_TRAITS(times, true);
    CPLX_SCALAR_OPERATORS_TRAITS(div, true);
    
    CPLX_CPLX_OPERATORS_TRAITS(plus, true);
    CPLX_CPLX_OPERATORS_TRAITS(minus, true);
    CPLX_CPLX_OPERATORS_TRAITS(times, true);
    CPLX_CPLX_OPERATORS_TRAITS(div, true);

    CPLX_VECTOR_OPERATORS_TRAITS(rect, plus, true);
    CPLX_VECTOR_OPERATORS_TRAITS(rect, minus, true);

    template<cplx_representation R1, class T1, cplx_representation R2, class T2>
    struct operator_cmp_traits<cplx<R1, T1>, cplx<R2, T2> >
    {
      enum { commutative = true };
      typedef cplx<R1, typename deduce_from_traits<internal::operator_cmp_traits, T1, T2>::ret> ret;
      typedef cplx<R1, T1> impl;
    };

  } // end of internal

} // end of ntg

#endif // ndef NTG_OPTRAITS_CPLX_HH
