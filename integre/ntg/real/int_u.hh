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

#ifndef NTG_REAL_INT_U_HH
# define NTG_REAL_INT_U_HH

# include <ntg/basics.hh>
# include <ntg/real/optraits_scalar.hh>
# include <ntg/real/real_value.hh>

# include <mlc/cmp.hh>

# include <string>
# include <sstream>

// Macros //

# define INT_U_CTOR_FROM_BUILTIN_INT(Builtin)   \
int_u (const Builtin rhs)			\
{						\
  val_ = optraits_type::check(rhs);		\
}						\
self& operator=(const Builtin rhs)	        \
{						\
  val_ = optraits_type::check(rhs);		\
  return *this;					\
}

// --- //

namespace ntg {

  namespace internal {

    //
    //  Typetraits
    //
    ///////////////

    template <unsigned nbits, class behavior>
    struct typetraits<int_u<nbits, behavior> >
    {
      typedef int_u<nbits, behavior>		self;
      typedef unsigned_integer			abstract_type;
      typedef self				ntg_type;
      ntg_build_value_type(uint_value<E>);

      typedef optraits<self>			optraits;
      typedef typename behavior::get<self>	behavior_type;


      typedef self					base_type;
      typedef typename C_for_int_u<nbits>::type		storage_type;
      typedef int_s<mlc::saturateN<nbits+1, 32>::ret, 
		    behavior>				signed_type;
      typedef self					unsigned_type;
      // FIXME: calculate it more precisely
      typedef int_u<32, behavior>			cumul_type;
      typedef int_u<32, behavior>			largest_type;
      typedef int_s<32, behavior>			signed_largest_type;
      typedef int_s<32, behavior>			signed_cumul_type;
      typedef int_u<32, behavior>			unsigned_largest_type;
      typedef int_u<32, behavior>			unsigned_cumul_type;
      typedef unsigned int				integer_type;
    };

  } // end of internal.

  //
  //  Class int_u<Nbits, Behaviour>
  //
  //////////////////////////////////

  template <unsigned nbits, class behavior>
  class int_u : public uint_value<int_u<nbits, behavior> >
  {
    typedef int_u<nbits, behavior> self;
    typedef ntgi_storage_type(self) storage_type;
    // dev note : should be directly optraits<self_t>, but with g++ this
    // breaks inheritance in optraits herarchy ...
    typedef typename internal::typetraits<self>::optraits optraits_type;

  public:
    int_u () { val_ = 0; }

    // We define ctor for each builtin to avoid implicit builtin promotion

    INT_U_CTOR_FROM_BUILTIN_INT(unsigned long);
    INT_U_CTOR_FROM_BUILTIN_INT(signed   long);

    INT_U_CTOR_FROM_BUILTIN_INT(unsigned int);
    INT_U_CTOR_FROM_BUILTIN_INT(signed   int);

    INT_U_CTOR_FROM_BUILTIN_INT(unsigned short);
    INT_U_CTOR_FROM_BUILTIN_INT(signed   short);

    INT_U_CTOR_FROM_BUILTIN_INT(unsigned char);
    INT_U_CTOR_FROM_BUILTIN_INT(signed   char);

    int_u (const float_s rhs)
    {
      val_ = optraits_type::check(roundf(rhs));
    }
    self& operator=(const float rhs)
    {
      val_ = optraits_type::check(roundf(rhs));
      return *this;
    }

    int_u (const float_d rhs)
    {
      val_ = optraits_type::check(round(rhs));
    }
    self& operator=(const double rhs)
    {
      val_ = optraits_type::check(round(rhs));
      return *this;
    }

    template <unsigned mbits, class B2>
    int_u (const int_u<mbits, B2>& rhs)
    {
      if (mbits <= nbits)
	val_ = rhs.val();
      else
	val_ = optraits_type::check(rhs.val());
    }
    template <unsigned mbits, class B2>
    self& operator=(const int_u<mbits, B2>& rhs)
    {
      if (mbits <= nbits)
	val_ = rhs.val();
      else
	val_ = optraits_type::check(rhs.val());
      return *this;
    }

    template <class T>
    int_u (const real_value<T>& rhs)
    {
      val_ = optraits_type::check(rhs.val());
    }
    template <class T>
    self& operator=(const real_value<T>& rhs)
    {
      val_ = optraits_type::check(rhs.val());
      return *this;
    }

    // Cast
    operator storage_type () const { return val_; }

  private:
    // We want to prevent this
    int_u(bool);
  };

  template<unsigned nbits, class behavior>
  inline std::ostream&
  operator<<(std::ostream& stream, const int_u<nbits, behavior>& rhs)
  {
    stream << unsigned(rhs.val());
    return stream;
  }

  namespace internal
  {

    //
    //  optraits for int_u
    //
    /////////////////////////////////////////////////////

    template <unsigned nbits, class behavior>
    struct optraits<int_u<nbits, behavior> > :
      public optraits_int_u<int_u<nbits, behavior> >
    {
    private:
      // shortcuts
      typedef int_u<nbits, behavior> self;
      typedef typename typetraits<self>::storage_type storage_type_;
      typedef typename behavior::get<self> behavior_type_;

    public:
      // behavior's check
      template <class P>
      static storage_type_ check(const P& rhs)
      { return behavior_type_::apply(rhs); }

      //
      // Properties
      //

      static storage_type_ max()
      { return C_for_int_u<nbits>::max(); }

      // debug
      static std::string name() {
	std::ostringstream out;
	out << "int_u<" << int(nbits) << ", " << behavior::name() << ">"
	    << std::ends;
	return out.str();
      }
    };

    //
    // Operators traits
    //
    ////////////////////

    //
    // plus
    //

    // int_u + int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_plus, int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) mlc::maxN<nbits + 1, mbits + 1, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };

    //
    // minus
    //

    // int_u - int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_minus, int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned) mlc::maxN<nbits+1, mbits+1, 32>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };

    // int_u32 - int_u : we do not convert result to int_s because we
    // want to access (UINT_MAX - 1)

    template<class B1, unsigned mbits, class B2>
    struct operator_traits<operator_minus, int_u<32, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<32, typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_u<32, B1> impl;
    };

    //
    // times
    //

    // int_u * int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_times, int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) mlc::saturateN<nbits + mbits, 32>::ret,
	typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };

    //
    // div
    //

    // int_u / int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_div, int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<nbits, typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };

    //
    // modulo
    //

    // int_u % int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_mod, int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = false };
      typedef int_u<mbits, typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };


    //
    // Min
    //

    // MIN(int_u, int_u)

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_min, int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) mlc::min<nbits, mbits>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };


    //
    // Max
    //

    // MAX(int_u, int_u)

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_max, int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) mlc::max<nbits, mbits>::ret,
		    typename deduce_op_behavior<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };


    //
    // Comparison
    //

    // int_u CMP int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_traits<operator_cmp, int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) mlc::maxN<nbits, mbits, 32>::ret, unsafe> ret;
      typedef int_u<nbits, B1> impl;
    };

  } // end of internal.

} // end of ntg.

#endif // ndef NTG_REAL_INT_U_HH
