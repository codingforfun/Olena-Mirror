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

#ifndef NTG_OPTRAITS_REAL_HH_
# define NTG_OPTRAITS_REAL_HH_

# include <cmath>

# include <mlc/is_a.hh>

# include <ntg/real/real_value.hh>
# include <ntg/core/type_traits.hh>
# include <ntg/core/abstract_hierarchy.hh>

// --

# include <ntg/core/internal/global_ops_traits.hh>
# include <ntg/core/macros.hh>

// macros definitions
# include <ntg/real/optraits_real_defs.hh>

namespace ntg {

  namespace internal {

    /*---------------------.
    | optraits<real_value> |
    `---------------------*/

    //! Implement common operators for scalars
    template <class E>
    class optraits<real_value<E> > : public optraits<value<E> >
    {
    private:
      typedef typename typetraits<E>::storage_type storage_type_;

    public:
      static storage_type_ zero () { return 0; }
      static storage_type_ unit () { return 1; }
      static storage_type_ default_val() { return zero(); }

      //
      // dev note : the aim of these default operators is to check the kind
      // of operands (value or not), and then call the good function.
      //
      // ASSIGN_SCALAR_OPERATOR includes default check_xxx_equal functions
      //

      ASSIGN_SCALAR_OPERATOR(plus)
      ASSIGN_SCALAR_OPERATOR(minus)
      ASSIGN_SCALAR_OPERATOR(times)
      ASSIGN_SCALAR_OPERATOR(div)

      ARITH_SCALAR_OPERATOR(plus)
      ARITH_SCALAR_OPERATOR(minus)
      ARITH_SCALAR_OPERATOR(times)
      ARITH_SCALAR_OPERATOR(div)

      CMP_SCALAR_OPERATOR(cmp_eq, ==)
      CMP_SCALAR_OPERATOR(cmp_lt, <)
    };

    /*----------------------.
    | optraits<float_value> |
    `----------------------*/

    template <class E>
    class optraits<float_value<E> > : public optraits<real_value<E> >
    {
    public:
      // This is the standard size for float on std::ostream.
      static unsigned max_print_width () { return 11U; }
    };

    /*--------------------.
    | optraits<int_value> |
    `--------------------*/

    template <class E>
    class optraits<int_value<E> > : public optraits<real_value<E> >
    {
      typedef typename typetraits<E>::storage_type storage_type_;

    public:
      static storage_type_ inf () { return optraits<E>::min(); }
      static storage_type_ sup () { return optraits<E>::max(); }

      static unsigned max_print_width ()
      { 
	return (unsigned) log10(double(optraits<E>::max())) + 1;
      }

      ASSIGN_INT_OPERATOR(mod_equal,  %)
      ARITH_INT_OPERATOR(mod,  %=)
    };

    /*---------------------.
    | optraits<uint_value> |
    `---------------------*/

    template <class E>
    class optraits<uint_value<E> > : public optraits<int_value<E> >
    {
    private:
      typedef typename typetraits<E>::storage_type storage_type_;

    public:
      static storage_type_ min () { return 0; }
    };

    /*---------------------.
    | optraits<sint_value> |
    `---------------------*/

    template <class E>
    class optraits<sint_value<E> > : public optraits<int_value<E> >
    {};

  } // end of internal

} // end of ntg

#endif // ndef NTG_OPTRAITS_REAL_HH_
