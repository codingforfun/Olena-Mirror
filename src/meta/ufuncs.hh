// Copyright 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_META_UFUNCS_HH
# define OLENA_META_UFUNCS_HH

# include "meta/basics.hh"
# include "meta/ucmp.hh"


namespace oln {

  namespace meta {


    namespace internal {

      // for pow2sup<n>::value

      template<unsigned n> struct is_pow2 { typedef false_t ret_t; };
      template<> struct is_pow2<8>  { typedef true_t ret_t; };
      template<> struct is_pow2<16> { typedef true_t ret_t; };
      template<> struct is_pow2<32> { typedef true_t ret_t; };
      template<> struct is_pow2<64> { typedef true_t ret_t; };

      template<unsigned n, class> struct find_pow2sup;
      template<unsigned n> struct find_pow2sup<n,true_t> {
	enum { value = n };
      };
      template<unsigned n> struct find_pow2sup<n,false_t> {
	enum { value = find_pow2sup< n+1, typename is_pow2<n+1>::ret_t >::value };
      };

    } // end of internal



    // puissance de 2 superieure (FIXME: american!)

    template<unsigned n>
    struct pow2sup {
      enum {
	value = internal::find_pow2sup< n, typename internal::is_pow2<n>::ret_t >::value
      };
    private:
      typedef typename ucmp<n,32>::is_lesseq_t::is_true_t precondition_t;
    };



    // test over n

    template<unsigned n>
    class utest {
    public:
      typedef typename ucmp<n/2,(n+1)/2>::is_neq_t is_odd_t;
      static void ensure_odd()   { is_odd_t::is_true(); }
    };


  } // end of meta

} // end of oln


#endif
