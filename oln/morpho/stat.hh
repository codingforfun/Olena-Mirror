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

#ifndef OLENA_MORPHO_STAT_HH
# define OLENA_MORPHO_STAT_HH

# include <oln/core/image.hh>
# include <oln/core/winiter.hh>
# include <oln/core/structelt.hh>

namespace oln {

  namespace morpho {

    namespace internal {

      /* We need to use this inner definition in order to specialize
	 max and min on binary images.  */

      template <class I, class E, class V = Value(I)>
      struct _stat {

	static V
	max(const I& input, const Point(I)& p, const E& se)
	{
	  meta::eq<I::dim, E::dim>::ensure();

	  Iter(E) dp(se);
	  dp = begin;
	  V val = input[p + dp];
	  for_all_remaining (dp)
	    if (val < input[p + dp])
	      val = input[p + dp];
	  return val;
	}

	static V
	min(const I& input, const Point(I)& p, const E& se)
	{
	  meta::eq<I::dim, E::dim>::ensure();
	  Iter(E) dp(se);
	  dp = begin;
	  V val = input[p + dp];
	  for_all_remaining (dp)
	    if (val > input[p + dp])
	      val = input[p + dp];
	  return val;
	}

      };

      /* Binary specialization.  */

      template <class I, class E>
      struct _stat<I, E, bin> {

	static bin
	max(const I& input, const Point(I)& p, const E& se)
	{
	  meta::eq<I::dim, E::dim>::ensure();
	  Iter(E) dp(se);
	  for_all (dp)
	    if (input[p + dp] == true)
	      return true;
	  return false;
	}

	static bin
	min(const I& input, const Point(I)& p, const E& se)
	{
	  meta::eq<I::dim, E::dim>::ensure();
	  Iter(E) dp(se);
	  for_all (dp)
	    if (input[p + dp] == false)
	      return false;
	  return true;
	}

      };

    } // internal

    template<class _I, class _E>
    Value(_I) max(const image<_I>& _input,
		  const Exact(_I)::point& p,
		  const struct_elt<_E>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(E, se);
      meta::eq<I::dim, E::dim>::ensure();
      return internal::_stat<I, E>::max(input, p, se);
    }

    template<class _I, class _E>
    Value(_I) min(const image<_I>& _input,
		  const Exact(_I)::point& p,
		  const struct_elt<_E>& _se)
    {
      Exact_cref(I, input);
      Exact_cref(E, se);
      meta::eq<I::dim, E::dim>::ensure();
      return internal::_stat<I, E>::min(input, p, se);
    }

  } // morpho

} // oln


#endif // OLENA_MORPHO_STAT_HH
