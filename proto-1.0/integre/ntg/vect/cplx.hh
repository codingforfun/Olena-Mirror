// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef NTG_VECT_CPLX_HH
# define NTG_VECT_CPLX_HH

# include <ntg/basics.hh>
# include <ntg/vect/cplx_representation.hh>
# include <ntg/vect/vec.hh>

# include <mlc/cmp.hh>
# include <mlc/is_a.hh>

/*------------------------------.
| Assignements operators macros |
`------------------------------*/

# define ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(Rep, Name, Op)	\
template <class T1, class T2> inline				\
static cplx<Rep, T1>&						\
Name(cplx<Rep, T1>& lhs, const T2& rhs)				\
{								\
  ntg_is_a(T2, real)::ensure();					\
  lhs.first() Op rhs;						\
  return lhs;							\
}

# define ASSIGN_CPLX_SCALAR_OPERATOR_BOTH(Rep, Name, Op)	\
template <class T1, class T2> inline				\
static cplx<Rep, T1>&						\
Name(cplx<Rep, T1>& lhs, const T2& rhs)				\
{								\
  ntg_is_a(T2, real)::ensure();					\
  lhs.first() Op rhs;						\
  lhs.second() Op rhs;						\
  return lhs;							\
}

# define ASSIGN_CPLX_POLAR_SCALAR_OPERATOR(Name, Op)	\
template <class T1, class T2> inline			\
static cplx<polar, T1>&					\
Name(cplx<polar, T1>& lhs, const T2& rhs)		\
{							\
  ntg_is_a(T2, real)::ensure();				\
  cplx<rect, float_d> tmp(lhs);				\
  tmp.real() Op rhs;					\
  lhs = tmp;						\
  return lhs;						\
}

# define ASSIGN_CPLX_RECT_CPLX_OPERATOR_MULT(Name, Op1, Op2)	\
template <class T1, cplx_representation R2, class T2> inline	\
static cplx<rect, T1>&						\
Name(cplx<rect, T1>& lhs, const cplx<R2, T2>& rhs)		\
{								\
  cplx<polar, float_d> tmp(lhs);				\
  tmp.magn() Op1 (float_d)rhs.magn();				\
  tmp.angle() Op2 (float_d)rhs.angle();				\
  lhs = tmp;							\
  return lhs;							\
}

# define ASSIGN_CPLX_RECT_CPLX_OPERATOR_ADD(Name, Op1, Op2)	\
template <cplx_representation R, class T1, class T2> inline	\
static cplx<rect, T1>& 						\
Name(cplx<rect, T1>& lhs, const cplx<R, T2>& rhs)		\
{								\
  lhs.first() Op1 rhs.real();					\
  lhs.second() Op2 rhs.imag();					\
  return lhs;							\
}

# define ASSIGN_CPLX_POLAR_CPLX_OPERATOR_MULT(Name, Op1, Op2)	\
template <cplx_representation R, class T1, class T2> inline	\
static cplx<polar, T1>&						\
Name(cplx<polar, T1>& lhs, const cplx<R, T2>& rhs)		\
{								\
  lhs.magn() Op1 rhs.magn();					\
  lhs.angle() Op2 rhs.angle();					\
  return lhs;							\
}

# define ASSIGN_CPLX_POLAR_CPLX_OPERATOR_ADD(Name, Op)		\
template <cplx_representation R, class T1, class T2> inline	\
static cplx<polar, T1>&						\
Name(cplx<polar, T1>& lhs, const cplx<R, T2>& rhs)		\
{								\
  cplx<rect, float_d> tmp(lhs);					\
  tmp.real() Op (float_d)rhs.real();				\
  tmp.imag() Op (float_d)rhs.imag();				\
  lhs = tmp;							\
  return lhs;							\
}

# define ASSIGN_CPLX_VECTOR_OPERATOR(Rep, Name, Op)	\
template <class T1, class T2> inline			\
static cplx<Rep, T1>&					\
Name(cplx<Rep, T1>& lhs, const vec<2, T2>& rhs)		\
{							\
  lhs.first() Op rhs[0];				\
  lhs.second() Op rhs[1];				\
  return lhs;						\
}

/*-----------------------------.
| Arithemetic operators macros |
`-----------------------------*/

# define ARITH_CPLX_CPLX_OPERATOR(Rep1, Rep2, Name, Op)			\
template <class T1, class T2>						\
inline static								\
cplx<Rep1, ntg_return_type(Name, T1, T2)>				\
Name(const cplx<Rep1, T1>& lhs, const cplx<Rep2, T2>& rhs)		\
{									\
  typedef cplx<Rep1, ntg_return_type(Name, T1, T2)> return_type;	\
  return_type result(lhs);						\
  result Op rhs;							\
  return result;							\
}

# define ARITH_CPLX_SCALAR_OPERATOR(Rep, Name, Op)		\
template <class T1, class T2>					\
inline static							\
cplx<Rep, ntg_return_type(Name, T1, T2)>			\
Name(const cplx<Rep, T1>& lhs, const T2& rhs)			\
{								\
  ntg_is_a(T2, real)::ensure();					\
  typedef cplx<Rep, ntg_return_type(Name, T1, T2)> return_type;	\
  return_type result(lhs);					\
  result Op rhs;						\
  return result;						\
}

# define ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(Rep, Name, Op)	\
template <class T1, class T2>					\
inline static							\
cplx<Rep, ntg_return_type(Name, T1, T2)>			\
Name(const T1& lhs, const cplx<Rep, T2>& rhs)			\
{								\
  return Name(rhs, lhs);					\
}

# define ARITH_CPLX_VECTOR_OPERATOR(Rep, Name, Op)		\
template <class T1, class T2>					\
inline static							\
cplx<Rep, ntg_return_type(Name, T1, T2)>			\
Name(const cplx<Rep, T1>& lhs, const vec<2, T2>& rhs)		\
{								\
  typedef cplx<Rep, ntg_return_type(Name, T1, T2)> return_type;	\
  return_type result(lhs);					\
  result Op rhs;						\
  return result;						\
}

# define ARITH_CPLX_VECTOR_OPERATOR_COMMUTE_PLUS(Rep, Name, Op)		\
template <class T1, class T2>						\
inline static								\
cplx<Rep, ntg_return_type(Name, T1, T2)>				\
Name(const vec<2, T1>& lhs, const cplx<Rep, T2>& rhs)			\
{									\
  return Name(rhs, lhs);						\
}

# define ARITH_CPLX_VECTOR_OPERATOR_COMMUTE_MINUS(Rep, Name, Op)	\
template <class T1, class T2>						\
inline static								\
cplx<Rep, ntg_return_type(Name, T1, T2)>				\
Name(const vec<2, T1>& lhs, const cplx<Rep, T2>& rhs)			\
{									\
  return Name(rhs, lhs).invert();					\
}

namespace ntg {

  namespace internal {

    /*----------------------.
    | typetraits<cplx<R, T> |
    `----------------------*/

    // FIXME: why cplx<polar> does not inherit from vec<2, ...> ?

    template <cplx_representation R, class T>
    struct typetraits<cplx<R, T> > : 
      public typetraits<vec<2, T, cplx<R, T> > >
    {
      typedef cplx<R, T>	self;
      typedef self		ntg_type;
      typedef optraits<self>	optraits_type;
    };

  } // end of internal.

  /*-----------.
  | cplx<R, T> |
  `-----------*/

  //! No default implementation.
  template <cplx_representation R, class T>
  class cplx;

  /*--------------.
  | cplx<rect, T> |
  `--------------*/

  template <class T>
  class cplx<rect, T> : public vec<2, T, cplx<rect, T> >
  {
  public:

    cplx(const T& re = 0, const T& im = 0)
    {
      this->val_[0] = re;
      this->val_[1] = im;
    }

    cplx<rect, T>&
    operator=(const T& r)
    {
      this->val_[0] = r;
      this->val_[1] = 0;
      return *this;
    }

    template<cplx_representation R, class T2>
    cplx(const cplx<R, T2>& rhs)
    {
      this->val_[0] = (T)rhs.real();
      this->val_[1] = (T)rhs.imag();
    }

    template<cplx_representation R, class T2>
    cplx<rect, T>&
    operator=(const cplx<R, T2>& rhs)
    {
      this->val_[0] = (T)rhs.real();
      this->val_[1] = (T)rhs.imag();
      return *this;
    }

    template<class T2>
    cplx(const vec<2, T2>& rhs)
    {
      this->val_[0] = (T)rhs[0];
      this->val_[1] = (T)rhs[1];
    }

    template<class T2>
    cplx<rect, T>&
    operator=(const vec<2, T2>& rhs)
    {
      this->val_[0] = (T)rhs[0];
      this->val_[1] = (T)rhs[1];
      return *this;
    }

    ~cplx()
    {
      ntg_is_a(T, ntg::real)::ensure();
    }

    // accessors

    const T real() const { return this->val_[0]; }
    T& real() { return this->val_[0]; }

    const T imag() const { return this->val_[1]; }
    T& imag() { return this->val_[1]; }

    const T first() const { return this->val_[0]; }
    T& first() { return this->val_[0]; }

    const T second() const { return this->val_[1]; }
    T& second() { return this->val_[1]; }

    // methods

    const float_d
    magn() const
    {
      return sqrt(this->val_[0] * this->val_[0] + this->val_[1] * this->val_[1]);
    }

    const float_d
    angle() const
    {
      return atan2(this->val_[1], this->val_[0]);
    }

    const cplx<rect, T>
    conj() const
    {
      return cplx<rect, T>(this->val_[0], -this->val_[1]);
    }

    const cplx<rect, T>
    invert() const
    {
      return cplx<rect, T>(-this->val_[0], -this->val_[1]);
    }

    // Declaration of to_polar (definition is given after
    // cplx<polar, T> definition.
    const cplx<polar, float_d>
    to_polar() const;

  };

  /*---------------.
  | cplx<polar, T> |
  `---------------*/

  template <class T>
  class cplx<polar, T> : public vect_value<cplx<polar, T> >
  {
  public:

    cplx(const T& ma = 0, const T& an = 0)
    {
      ntg_assert(ma >= 0);
      this->val_[0] = ma;
      this->val_[1] = an;
    }

    cplx<polar, T>& operator=(const T& r)
    {
      this->val_[0] = r;
      this->val_[1] = 0;
      return *this;
    }

    template<cplx_representation R, class T2>
    cplx(const cplx<R, T2>& rhs)
    {
      this->val_[0] = (T)rhs.magn();
      this->val_[1] = (T)rhs.angle();
    }

    template<cplx_representation R, class T2>
    cplx<polar, T>& operator=(const cplx<R, T2>& rhs)
    {
      this->val_[0] = (T)rhs.magn();
      this->val_[1] = (T)rhs.angle();
      return *this;
    }

    ~cplx()
    {
      ntg_is_a(T, ntg::real)::ensure();
    }

    // accessors

    const T magn() const { return this->val_[0]; }
    T& magn() { return this->val_[0]; }

    const T angle() const { return this->val_[1]; }
    T& angle() { return this->val_[1]; }

    const T first() const { return this->val_[0]; }
    T& first() { return this->val_[0]; }

    const T second() const { return this->val_[1]; }
    T& second() { return this->val_[1]; }

    // methods

    const float_d
    real() const
    {
      return this->val_[0] * cos(this->val_[1]);
    }

    const float_d
    imag() const
    {
      return this->val_[0] * sin(this->val_[1]);
    }

    const cplx<polar, T>
    conj() const
    {
      return cplx<polar, T>(this->val_[0], -this->val_[1]);
    }

    const cplx<rect, T>
    invert() const
    {
      return cplx<rect, T>(this->val_[0], this->val_[1] + M_PI);
    }

    const cplx<rect, float_d>
    to_rect() const
    {
      return cplx<rect, float_d>(real(), imag());
    }

  };

  /// Definition of cplx<rect, T>::to_polar.
  template <class T>
  const cplx<polar, float_d>
  cplx<rect, T>::to_polar() const
  {
    return cplx<polar, float_d>(magn(), angle());
  }

  template<class T>
  inline std::ostream&
  operator<<(std::ostream& ostr, const cplx<rect, T>& rhs)
  {
    // Using an ostringstream is necessary to pretty print.
    std::ostringstream str;
    str << rhs.real() << " + " << rhs.imag() << "i";
    return ostr << str.str();
  }

  template<class T>
  inline std::ostream&
  operator<<(std::ostream& ostr, const cplx<polar, T>& rhs)
  {
    // Using an ostringstream is necessary to pretty print.
    std::ostringstream str;
    str << rhs.magn() << " * exp(" << rhs.angle() << "i)";
    return ostr << str.str();
  }

  namespace internal 
  {

    /*-------------------------.
    | optraits<cplx<rect, T> > |
    `-------------------------*/
  
    template <class T>
    class optraits<cplx<rect, T> > : public optraits<vec<2, T> >
    {
      typedef cplx<rect, T> self;
      typedef typename typetraits<self>::storage_type storage_type_;
    
    public:
      static self zero () { return self(); }
      static self unit () { return self(1); }

      static unsigned max_print_width() 
      {
	return 2 * ntg_max_print_width(T) + 4;
      }
    
      static std::string
      name()
      {
	std::ostringstream out;
	out << "cplx<rect, " <<  optraits<T>::name() << ">" << std::ends;
	return out.str();
      }

      ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(rect, plus_equal, +=)
      ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(rect, minus_equal, -=)
      ASSIGN_CPLX_SCALAR_OPERATOR_BOTH(rect, times_equal, *=)
      ASSIGN_CPLX_SCALAR_OPERATOR_BOTH(rect, div_equal, /=)
    
      ASSIGN_CPLX_RECT_CPLX_OPERATOR_ADD(plus_equal, +=, +=)
      ASSIGN_CPLX_RECT_CPLX_OPERATOR_ADD(minus_equal, -=, -=)
      ASSIGN_CPLX_RECT_CPLX_OPERATOR_MULT(times_equal, *=, +=)
      ASSIGN_CPLX_RECT_CPLX_OPERATOR_MULT(div_equal, /=, -=)

      ASSIGN_CPLX_VECTOR_OPERATOR(rect, plus_equal, +=)
      ASSIGN_CPLX_VECTOR_OPERATOR(rect, minus_equal, -=)

      ARITH_CPLX_SCALAR_OPERATOR(rect, plus, +=)
      ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(rect, plus, +=)
      ARITH_CPLX_SCALAR_OPERATOR(rect, minus, -=)
      ARITH_CPLX_SCALAR_OPERATOR(rect, times, *=)
      ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(rect, times, *=)
      ARITH_CPLX_SCALAR_OPERATOR(rect, div, /=)

      ARITH_CPLX_CPLX_OPERATOR(rect, rect, plus, +=)
      ARITH_CPLX_CPLX_OPERATOR(rect, rect, minus, -=)
      ARITH_CPLX_CPLX_OPERATOR(rect, rect, times, *=)
      ARITH_CPLX_CPLX_OPERATOR(rect, rect, div, /=)

      ARITH_CPLX_CPLX_OPERATOR(rect, polar, plus, +=)
      ARITH_CPLX_CPLX_OPERATOR(rect, polar, minus, -=)
      ARITH_CPLX_CPLX_OPERATOR(rect, polar, times, *=)
      ARITH_CPLX_CPLX_OPERATOR(rect, polar, div, /=)

      ARITH_CPLX_VECTOR_OPERATOR(rect, plus, +=)
      ARITH_CPLX_VECTOR_OPERATOR_COMMUTE_PLUS(rect, plus, +=)
      ARITH_CPLX_VECTOR_OPERATOR(rect, minus, -=)
      ARITH_CPLX_VECTOR_OPERATOR_COMMUTE_MINUS(rect, minus, -=)

      template <class T1, cplx_representation R2, class T2>
      inline static bool
      cmp_eq (const cplx<rect, T1>& lhs, const cplx<R2, T2>& rhs)
      {
	if (lhs.real() != rhs.real() || lhs.imag() != rhs.imag())
	  return false;
	return true;
      }

    };

    /*--------------------------.
    | optraits<cplx<polar, T> > |
    `--------------------------*/

    template <class T>
    class optraits<cplx<polar, T> >
    {
      typedef cplx<polar, T> self;
      typedef ntgi_storage_type(self) storage_type_;
    
    public:
      static self zero () { return self(); }
    
      static self unit () { return self(1); }
    
      static unsigned max_print_width() 
      {
	return 2 * ntg_max_print_width(T) + 4;
      }

      static std::string
      name()
      {
	std::ostringstream out;
	out << "cplx<polar, " <<  optraits<T>::name() << ">" << std::ends;
	return out.str();
      }

      ASSIGN_CPLX_POLAR_SCALAR_OPERATOR(plus_equal, +=)
      ASSIGN_CPLX_POLAR_SCALAR_OPERATOR(minus_equal, -=)
      ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(polar, times_equal, *=)
      ASSIGN_CPLX_SCALAR_OPERATOR_SINGLE(polar, div_equal, /=)

      ASSIGN_CPLX_POLAR_CPLX_OPERATOR_ADD(plus_equal, +=)
      ASSIGN_CPLX_POLAR_CPLX_OPERATOR_ADD(minus_equal, -=)
      ASSIGN_CPLX_POLAR_CPLX_OPERATOR_MULT(times_equal, *=, +=)
      ASSIGN_CPLX_POLAR_CPLX_OPERATOR_MULT(div_equal, /=, -=)

      ARITH_CPLX_SCALAR_OPERATOR(polar, plus, +=)
      ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(polar, plus, +=)
      ARITH_CPLX_SCALAR_OPERATOR(polar, minus, -=)
      ARITH_CPLX_SCALAR_OPERATOR(polar, times, *=)
      ARITH_CPLX_SCALAR_OPERATOR_COMMUTE(polar, times, *=)
      ARITH_CPLX_SCALAR_OPERATOR(polar, div, /=)

      ARITH_CPLX_CPLX_OPERATOR(polar, polar, plus, +=)
      ARITH_CPLX_CPLX_OPERATOR(polar, polar, minus, -=)
      ARITH_CPLX_CPLX_OPERATOR(polar, polar, times, *=)
      ARITH_CPLX_CPLX_OPERATOR(polar, polar, div, /=)

      ARITH_CPLX_CPLX_OPERATOR(polar, rect, plus, +=)
      ARITH_CPLX_CPLX_OPERATOR(polar, rect, minus, -=)
      ARITH_CPLX_CPLX_OPERATOR(polar, rect, times, *=)
      ARITH_CPLX_CPLX_OPERATOR(polar, rect, div, /=)

      template <class T1, cplx_representation R2, class T2>
      inline static bool
      cmp_eq (const cplx<polar, T1>& lhs, const cplx<R2, T2>& rhs)
      {
	if (lhs.magn() != rhs.magn() || lhs.angle() != rhs.angle())
	  return false;
	return true;
      }

    };
    
    /*-----------------------.
    | operator traits macros |
    `-----------------------*/

    // FIXME: I think there should be an easy way to simplify this. 
    // -- nes

# define CPLX_SCALAR_OPERATORS_TRAITS(Name, CommuteBool)	\
    template <cplx_representation R1, class T1, class T2>	\
    struct operator_traits<operator_##Name, cplx<R1, T1>, T2>	\
    {								\
      enum { commutative = CommuteBool };			\
      typedef cplx<R1, ntg_return_type(Name, T1, T2)> ret;	\
      typedef cplx<R1, T1> impl;				\
    }

# define CPLX_CPLX_OPERATORS_TRAITS(Name, CommuteBool)		\
    template <cplx_representation R1,				\
              class T1,						\
	      cplx_representation R2, class T2>			\
    struct operator_traits<operator_##Name, 			\
                           cplx<R1, T1>, cplx<R2, T2> >		\
    {								\
      enum { commutative = CommuteBool };			\
      typedef cplx<R1, ntg_return_type(Name, T1, T2)> ret;	\
      typedef cplx<R1, T1> impl;				\
    }

# define CPLX_VECTOR_OPERATORS_TRAITS(Rep, Name, CommuteBool)	\
    template <class T1, class T2>				\
    struct operator_traits<operator_##Name,			\
                           cplx<Rep, T1>, vec<2, T2> >		\
    {								\
      enum { commutative = CommuteBool };			\
      typedef cplx<Rep, ntg_return_type(Name, T1, T2)> ret;	\
      typedef cplx<Rep, T1> impl;				\
    }

    /*----------------.
    | operator traits |
    `----------------*/
 
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

    template<cplx_representation R1, class T1, 
	     cplx_representation R2, class T2>
    struct operator_traits<operator_cmp, cplx<R1, T1>, cplx<R2, T2> >
    {
      enum { commutative = true };
      typedef cplx<R1, ntg_return_type(cmp, T1, T2)> ret;
      typedef cplx<R1, T1> impl;
    };

  } // end of internal.

} // end of ntg.

#endif // !NTG_VECT_CPLX_HH
