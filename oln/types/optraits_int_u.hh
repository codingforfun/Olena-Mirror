// Copyright 2002  EPITA Research and Development Laboratory
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

# include <oln/types/builtins_properties.hh>

# include <oln/types/optraits_scalar.hh>
# include <oln/types/typetraits.hh>
# include <oln/types/behaviour.hh>
# include <oln/types/global_ops_traits.hh>

# include <oln/meta/cmp.hh>

namespace oln
{

  template <unsigned nbits, class behaviour> class int_u;

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
    typedef typename typetraits<self>::store store_type;
    typedef typename behaviour::get<self> behaviour_type;

  public:
    // behaviour's check
    template <class P>
    static store_type check(const P& rhs)
    { return behaviour_type::apply(rhs); }

    //
    // Properties
    //

    static store_type max()
    { return C_for_int_u<nbits>::max(); }
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
      typedef int_s<(unsigned) meta::maxN<nbits + 1, mbits + 1, 32>::ret,
		    typename deduce_op_behaviour<B1, B2>::ret> ret;
      typedef int_u<nbits, B1> impl;
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
