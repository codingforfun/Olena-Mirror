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

#ifndef NTG_CORE_INT_S_HH
# define NTG_CORE_INT_S_HH

# include <ntg/basics.hh>
# include <ntg/real/optraits_real.hh>
# include <ntg/real/real_value.hh>
# include <ntg/bin.hh>

# include <mlc/bool.hh>
# include <mlc/cmp.hh>
# include <mlc/is_a.hh>

# include <string>
# include <sstream>

/*-------.
| macros |
`-------*/

// FIXME: add bits number comparison to avoid useless checks.

# define INT_S_CTOR_FROM_BUILTIN(Builtin)       \
int_s (const Builtin rhs)			\
{						\
  this->val_ = optraits_type::check(rhs);	\
}						\
self& operator=(const Builtin rhs)	        \
{						\
  this->val_ = optraits_type::check(rhs);	\
  return *this;					\
}

namespace ntg {

  namespace internal {

    /*------------------.
    | typetraits<int_s> |
    `------------------*/

    template <unsigned nbits, class behavior>
    struct typetraits<int_s<nbits, behavior> >
    {
      typedef int_s<nbits, behavior>		self;
      typedef signed_integer			abstract_type;
      typedef self				ntg_type;

      ntg_build_value_type(sint_value<E>);

      typedef optraits<self>				optraits_type;
      typedef typename behavior::template get<self>	behavior_type;

      typedef self				base_type;
      typedef typename C_for_int_s<nbits>::type	storage_type;
      typedef self				signed_type;
      typedef int_u<nbits-1, behavior>		unsigned_type;
      // FIXME: calculate it more precisely
      typedef int_s<32, behavior>		cumul_type;
      typedef int_s<32, behavior>		largest_type;
      typedef int_s<32, behavior>		signed_largest_type;
      typedef int_s<32, behavior>		signed_cumul_type;
      typedef int_u<32, behavior>		unsigned_largest_type;
      typedef int_u<32, behavior>		unsigned_cumul_type;
      typedef signed int			integer_type;
    };

  } // end of internal.

  /*-----------------------.
  | int_s<nbits, behavior> |
  `-----------------------*/

  template <unsigned nbits, class behavior>
  class int_s : public sint_value<int_s<nbits, behavior> >
  {
    typedef int_s<nbits, behavior> self;
    typedef ntgi_storage_type(self) storage_type;
    typedef ntgi_optraits_type(self) optraits_type;

  public:

    int_s () { this->val_ = 0; }

    // We define ctor for each builtin to avoid implicit builtin
    // promotion.

    INT_S_CTOR_FROM_BUILTIN(unsigned long);
    INT_S_CTOR_FROM_BUILTIN(signed   long);

    INT_S_CTOR_FROM_BUILTIN(unsigned int);
    INT_S_CTOR_FROM_BUILTIN(signed   int);

    INT_S_CTOR_FROM_BUILTIN(unsigned short);
    INT_S_CTOR_FROM_BUILTIN(signed   short);

    INT_S_CTOR_FROM_BUILTIN(unsigned char);
    INT_S_CTOR_FROM_BUILTIN(signed   char);
    INT_S_CTOR_FROM_BUILTIN(char);

    template <unsigned mbits, class B2>
    int_s (const int_s<mbits, B2>& rhs)
    {
      if (mbits <= nbits)
	this->val_ = rhs.val();
      else
	this->val_ = optraits_type::check(rhs.val());
    }
    template <unsigned mbits, class B2>
    self&
    operator=(const int_s<mbits, B2>& rhs)
    {
      if (mbits <= nbits)
	this->val_ = rhs.val();
      else
	this->val_ = optraits_type::check(rhs.val());
      return *this;
    }

    template <class T>
    int_s (const real_value<T>& rhs)
    {
      this->val_ = optraits_type::check(rhs.val());
    }
    template <class T>
    self&
    operator=(const real_value<T>& rhs)
    {
      this->val_ = optraits_type::check(rhs.val());
      return *this;
    }

    // FIXME: do we really want float to int conversions ?

    int_s (const float_s rhs)
    {
      this->val_ = optraits_type::check(roundf(rhs));
    }
    self& operator=(const float_s rhs)
    {
      this->val_ = optraits_type::check(roundf(rhs));
      return *this;
    }

    int_s (const float_d rhs)
    {
      this->val_ = optraits_type::check(round(rhs));
    }
    self& operator=(const float_d rhs)
    {
      this->val_ = optraits_type::check(round(rhs));
      return *this;
    }

    // bin is allowed since it has defined values 0 or 1.
    int_s (bin b)
    {
      this->val_ = b.val();
    }
    self& operator=(bin b)
    {
      this->val_ = b.val();
      return *this;
    }

    operator storage_type () const { return this->val_; }

  private:
    // We want to prevent this
    int_s(bool);
  };

  template<unsigned nbits, class behavior>
  inline std::ostream&
  operator<<(std::ostream& stream, const int_s<nbits, behavior>& rhs)
  {
    stream << signed(rhs.val());
    return stream;
  }

  namespace internal
  {

    /*----------------.
    | optraits<int_s> |
    `----------------*/

    template <unsigned nbits, class behavior>
    struct optraits<int_s<nbits, behavior> > :
      public optraits_int_s<int_s<nbits, behavior> >
    {
    public:
      typedef int_s<nbits, behavior> self;

    private:
      typedef typename typetraits<self>::base_type	base_type_;
      typedef typename typetraits<self>::storage_type	storage_type_;
      typedef typename behavior::template get<self>	behavior_type_;

    public:
      template <class P>
      static storage_type_
      check(const P& rhs)
      { return behavior_type_::check(rhs); }

      static storage_type_ max()
      { return C_for_int_s<nbits>::max(); }

      static storage_type_ min()
      { return C_for_int_s<nbits>::min(); }

      //
      //  Comparison operators
      //
      /////////////////////////

      //  As int_x32 cannot grow, there is a problem with comparisons when a
      //  int_u32 is present, as we cannot convert it to a signed type safely.
      //

      //
      // cmp_eq
      //

      // int_sN == int_u32; int_u32 == int_sN

      template <class B2>
      static bool
      cmp_eq(const self& lhs, const int_u<32, B2>& rhs)
      {
	if (lhs.val() < 0)
	  return false;

	return static_cast<int_u<32, B2> >(lhs).val() == rhs.val();
      }
      template <class B1>
      static bool cmp_eq(const int_u<32, B1>& lhs, const self& rhs)
      { return cmp_eq(rhs, lhs); }

      // <T1> == <T2>

      template <class T1, class T2>
      static bool
      cmp_eq(const T1& lhs, const T2& rhs)
      { return optraits_scalar<self>::cmp_eq(lhs, rhs); }

      //
      // cmp_lt
      //

      // int_sN < int_u32; int_u32 < int_sN

      template <class B2>
      static bool
      cmp_lt(const self& lhs, const int_u<32, B2>& rhs)
      {
	if (lhs.val() < 0)
	  return true;

	return static_cast<int_u<32, B2> >(lhs).val() < rhs.val();
      }
      template <class B1>
      static bool cmp_lt(const int_u<32, B1>& lhs, const self& rhs)
      {
	if (rhs.val() < 0)
	  return false;

	return lhs.val() < static_cast<int_u<32, B1> >(rhs.val());
      }

      // <T1> < <T2>

      template <class T1, class T2>
      static bool
      cmp_lt(const T1& lhs, const T2& rhs)
      { return optraits_scalar<self>::cmp_lt(lhs, rhs); }

      // debug
      static std::string name() {
	std::ostringstream out;
	out << "int_s<" << int(nbits) << ", " << behavior::name() << ">"<< std::ends;
	return out.str();
      }
    };


    /*----------------.
    | operator traits |
    `----------------*/

    //
    // plus
    //

    // int_s + int_s

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_plus, int_s<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned)mlc::maxN<nbits + 1,mbits + 1, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };


    // int_s + int_u ; int_u + int_s

    template <unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_plus, int_s<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned)mlc::maxN<nbits + 1,mbits + 2, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };


    //
    // minus
    //

    // int_s - int_s

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_minus, int_s<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned)mlc::maxN<nbits + 1, mbits + 1, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    // int_s - int_u ; int_u - int_s

    template <unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_minus, int_s<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned)mlc::maxN<nbits + 1, mbits + 2, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };


    //
    // times
    //

    // int_s * int_s

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_times, int_s<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned)mlc::saturateN<nbits + mbits, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };


    // int_s * int_u ; int_u * int_s

    template <unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_times, int_s<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned)mlc::saturateN<nbits + mbits+1, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    //
    // div
    //

    // int_s / int_s

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_div, int_s<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<nbits, typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    // int_s / int_u ; int_u / int_s

    template <unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_div, int_s<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = false };
      typedef int_s<nbits, typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    template <unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_div, int_u<mbits, B2>, int_s<nbits, B1> >
    {
      enum { commutative = false };
      typedef int_s<mlc::saturateN<mbits + 1, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    //
    // mod
    //

    // int_s % int_s

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_mod, int_s<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = false };
      typedef int_s<mbits, typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    // int_s % int_u ; int_u % int_s

    template <unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_mod, int_s<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = false };
      typedef int_s<(unsigned)mlc::saturateN<mbits + 1, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    template <unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_mod, int_u<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = false };
      typedef int_u<mbits, typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };


    //
    // Min
    //

    // min(int_s, int_s)

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_min, int_s<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned) mlc::min<nbits, mbits>::ret, 
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    //
    // Max
    //

    // max(int_s, int_s)

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_max, int_s<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned) mlc::max<nbits, mbits>::ret, 
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_s<nbits, B1> impl;
    };

    //
    // Comparison operator
    //

    // int_s compared with int_s

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_cmp, int_s<nbits, B1>, int_s<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned)mlc::maxN<nbits,mbits,32>::ret, unsafe> ret;
      typedef int_s<nbits, B1> impl;
    };


    // int_s CMP int_u ; int_u CMP int_s

    template <unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_cmp, int_s<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned)mlc::maxN<nbits,mbits+1, 32>::ret, unsafe> ret;
      typedef int_s<nbits, B1> impl;
    };

  } // end of internal.

} // end of ntg.

#endif // !NTG_CORE_INT_S_HH
