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

#ifndef OLENA_VALUE_OPTRAITS_INT_U_HH
# define OLENA_VALUE_OPTRAITS_INT_U_HH

# include <oln/config/system.hh>
# include <oln/types/builtins_properties.hh>

# include <oln/types/optraits_scalar.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/predecls.hh>
# include <oln/types/behaviour.hh>
# include <oln/types/global_ops.hh>
# include <oln/types/int_u.hh>
# include <oln/types/int_s.hh>

# include <mlc/cmp.hh>

# include <string>
# include <sstream>

namespace oln
{

  //
  //  optraits for int_u
  //
  /////////////////////////////////////////////////////

  template <unsigned nbits, class behaviour>
  struct optraits<int_u<nbits, behaviour> > :
    public optraits_int_u<int_u<nbits, behaviour> >
  {
  private:
    // shortcuts
    typedef int_u<nbits, behaviour> self;
    typedef typename typetraits<self>::storage_type storage_type;
    typedef typename behaviour::get<self> behaviour_type;

  public:
    // behaviour's check
    template <class P>
    static storage_type check(const P& rhs)
    { return behaviour_type::apply(rhs); }

    //
    // Properties
    //

    static storage_type max()
    { return C_for_int_u<nbits>::max(); }

    // debug
    static std::string name() {
      std::ostringstream out;
      out << "int_u<" << int(nbits) << ", " << behaviour::name() << ">"
	  << std::ends;
      return out.str();
    }
  };

  namespace internal
  {

    //
    // Operators traits
    //
    ////////////////////

    //
    // plus
    //

    // int_u + int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_plus_traits<int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) meta::maxN<nbits + 1, mbits + 1, 32>::ret,
		    typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };

    //
    // minus
    //

    // int_u - int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_minus_traits<int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_s<(unsigned) meta::maxN<nbits+1, mbits+1, 32>::ret,
		    typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };

    // int_u32 - int_u : we do not convert result to int_s because we
    // want to access (UINT_MAX - 1)

    template<class B1, unsigned mbits, class B2>
    struct operator_minus_traits<int_u<32, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<32, typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<32, B1> impl;
    };

    //
    // times
    //

    // int_u * int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_times_traits<int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) meta::saturateN<nbits + mbits, 32>::ret,
	typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };

    //
    // div
    //

    // int_u / int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_div_traits<int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<nbits, typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };

    //
    // modulo
    //

    // int_u % int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_mod_traits<int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = false };
      typedef int_u<mbits, typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };


    //
    // Min
    //

    // MIN(int_u, int_u)

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_min_traits<int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) meta::min<nbits, mbits>::ret,
		    typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };


    //
    // Max
    //

    // MAX(int_u, int_u)

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_max_traits<int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) meta::max<nbits, mbits>::ret,
		    typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
    };


    //
    // Comparison
    //

    // int_u CMP int_u

    template<unsigned nbits, class B1, unsigned mbits, class B2>
    struct operator_cmp_traits<int_u<nbits, B1>, int_u<mbits, B2> >
    {
      enum { commutative = true };
      typedef int_u<(unsigned) meta::maxN<nbits, mbits, 32>::ret, unsafe> ret;
      typedef int_u<nbits, B1> impl;
    };

  } // end of namespace internal
} // namespace oln

#endif // ndef OLENA_VALUE_OPTRAITS_INT_U_HH
