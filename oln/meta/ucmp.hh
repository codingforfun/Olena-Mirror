// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

# include <oln/meta/basics.hh>

// Comparison functions.

namespace oln {

  namespace meta {

    namespace internal {

      // less

      template<unsigned N, unsigned M>
      struct less { typedef typename less<N-1,M-1>::ret_t ret_t; };

      template<unsigned N> struct less<N,0> { typedef false_t ret_t; };
      template<unsigned N> struct less<N,N> { typedef false_t ret_t; };
      template<unsigned M> struct less<0,M> { typedef true_t  ret_t; };
      template<>           struct less<0,0> { typedef false_t ret_t; };

      // lesseq

      template<unsigned N, unsigned M>
      struct lesseq { typedef typename less<N,M>::ret_t ret_t; };

      template<unsigned N> struct lesseq<N,N> { typedef true_t ret_t; };

      // eq

      template<unsigned N, unsigned M> struct eq { typedef false_t ret_t; };
      template<unsigned N> struct eq<N,N> { typedef true_t ret_t; };

      // value

      template<unsigned N, unsigned M, class B> struct value;

      template<unsigned N, unsigned M>
      struct value<N,M,true_t>  { enum { min = N, max = M }; };

      template<unsigned N, unsigned M>
      struct value<N,M,false_t> { enum { min = M, max = N }; };


    } // internal


    template<unsigned N, unsigned M>
    class ucmp {
      typedef internal::value< N, M,
			       typename internal::less<N,M>::ret_t > value;
    public:

      enum {
	maxval = value::max,
	minval = value::min
      };

      typedef typename internal::less<N,M>::ret_t   is_less_t;
      typedef typename internal::lesseq<N,M>::ret_t is_lesseq_t;
      typedef typename internal::eq<N,M>::ret_t     is_eq_t;
      typedef typename is_eq_t::not_t               is_neq_t;

      static void ensure_less()   { is_less_t::is_true(); }
      static void ensure_lesseq() { is_lesseq_t::is_true(); }
      static void ensure_eq()     { is_eq_t::is_true(); }
      static void ensure_neq()    { is_neq_t::is_true(); }
    };

  } // meta

} // oln


#endif  // OLENA_META_UCMP_HH
