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

#ifndef NTG_OPTRAITS_SCALAR_HH_
# define NTG_OPTRAITS_SCALAR_HH_

# include <typeinfo>

# include <mlc/is_a.hh>

# include <ntg/real/real_value.hh>
# include <ntg/core/type_traits.hh>
# include <ntg/core/abstract_hierarchy.hh>

// --

# include <ntg/core/internal/global_ops_traits.hh>
# include <ntg/core/type_traits.hh>
# include <ntg/core/macros.hh>

// macros definitions
# include <ntg/real/optraits_scalar_defs.hh>

namespace ntg {

  namespace internal {

    //
    //  class optraits_scalar : implement common operators for scalars
    //
    ///////////////////////////////////////////////////////////////////

    template <class Self>
    class optraits_scalar : public optraits_top<Self>
    {
    private:
      typedef typename typetraits<Self>::storage_type storage_type_;

    public:
      static storage_type_ zero () { return 0; }
      static storage_type_ unit () { return 1; }
      static storage_type_ default_val() { return zero(); }

      //
      // dev note : the goal in those default operators is to check the kind
      // of operands (value or not), and then call the good function.
      //
      // ASSIGN_SCALAR_OPERATOR includes default check_xxx_equal functions
      //

      ASSIGN_SCALAR_OPERATOR(plus_equal,  +);
      ASSIGN_SCALAR_OPERATOR(minus_equal, -);
      ASSIGN_SCALAR_OPERATOR(times_equal, *);
      ASSIGN_SCALAR_OPERATOR(div_equal,   /);

      ARITH_SCALAR_OPERATOR(plus,  +=);
      ARITH_SCALAR_OPERATOR(minus, -=);
      ARITH_SCALAR_OPERATOR(times, *=);
      ARITH_SCALAR_OPERATOR(div,   /=);

      CMP_SCALAR_OPERATOR(cmp_eq, ==);
      CMP_SCALAR_OPERATOR(cmp_lt, <);
    };


    //
    //  optraits_float
    //
    ///////////////////

    template <class Self>
    class optraits_float : public optraits_scalar<Self>
    {};


    //
    //  optraits_int
    //
    /////////////////

    template <class Self>
    class optraits_int : public optraits_scalar<Self>
    {
      typedef typename typetraits<Self>::storage_type storage_type_;

    public:
      static storage_type_ inf () { return optraits<Self>::min(); }
      static storage_type_ sup () { return optraits<Self>::max(); }

      ASSIGN_INT_OPERATOR(mod_equal,  %);
      ARITH_INT_OPERATOR(mod,  %=);
    };


    //
    //  optraits_int_u
    //
    ///////////////////

    template <class Self>
    class optraits_int_u : public optraits_int<Self>
    {
    private:
      typedef typename typetraits<Self>::storage_type storage_type_;

    public:
      static storage_type_ min () { return 0; }
    };


    //
    //  optraits_int_s
    //
    ///////////////////

    template <class Self>
    class optraits_int_s : public optraits_int<Self>
    {};

  } // end of internal.

} // end of ntg.

#endif // ndef NTG_OPTRAITS_SCALAR_HH_
