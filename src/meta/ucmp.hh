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

#ifndef OLENA_META_UCMP_HH
# define OLENA_META_UCMP_HH

# include "meta/basics.hh"


namespace oln {

  namespace meta {


    namespace internal {

      // less

      template<unsigned n, unsigned m>
      struct less { typedef typename less<n-1,m-1>::ret_t ret_t; };

      template<unsigned n> struct less<n,0> { typedef false_t ret_t; };
      template<unsigned n> struct less<n,n> { typedef false_t ret_t; };
      template<unsigned m> struct less<0,m> { typedef true_t  ret_t; };
      template<>           struct less<0,0> { typedef false_t ret_t; };

      // lesseq

      template<unsigned n, unsigned m>
      struct lesseq { typedef typename less<n,m>::ret_t ret_t; };

      template<unsigned n> struct lesseq<n,n> { typedef true_t ret_t; };

      // eq

      template<unsigned n, unsigned m> struct eq { typedef false_t ret_t; };
      template<unsigned n> struct eq<n,n> { typedef true_t ret_t; };

      // value

      template<unsigned n, unsigned m, class B> struct value;

      template<unsigned n, unsigned m>
      struct value<n,m,true_t>  { enum { min = n, max = m }; };

      template<unsigned n, unsigned m>
      struct value<n,m,false_t> { enum { min = m, max = n }; };


    } // end of internal



    template<unsigned n, unsigned m>
    class ucmp {
      typedef internal::value< n, m, typename internal::less<n,m>::ret_t > value;
    public:

      enum {
	maxval = value::max,
	minval = value::min
      };

      typedef typename internal::less<n,m>::ret_t   is_less_t;
      typedef typename internal::lesseq<n,m>::ret_t is_lesseq_t;
      typedef typename internal::eq<n,m>::ret_t     is_eq_t;
      typedef typename is_eq_t::not_t               is_neq_t;

      static void ensure_less()   { is_less_t::is_true(); }
      static void ensure_lesseq() { is_lesseq_t::is_true(); }
      static void ensure_eq()     { is_eq_t::is_true(); }
      static void ensure_neq()    { is_neq_t::is_true(); }
    };



  } // end of meta

} // end of oln


#endif
