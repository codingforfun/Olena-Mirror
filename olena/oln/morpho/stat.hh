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

#ifndef OLENA_MORPHO_STAT_HH
# define OLENA_MORPHO_STAT_HH

# include <oln/basics.hh>
# include <ntg/bin.hh>

namespace oln {

  namespace morpho {

    namespace internal {

      /* We need to use this inner definition in order to specialize
	 max and min on binary images.  */

      template <class I, class E, class V =oln_value_type(I)>
      struct stat_ 
      {
	static V
	max(const I& input, const oln_point_type(I)& p, const E& se)
	{
	  mlc::eq<I::dim, E::dim>::ensure();

	 oln_iter_type(E) dp(se);
	  dp = begin;
	  V val = input[p + dp];
	  for_all_remaining (dp)
	    if (val < input[p + dp])
	      val = input[p + dp];
	  return val;
	}

	static V
	min(const I& input, const oln_point_type(I)& p, const E& se)
	{
	  mlc::eq<I::dim, E::dim>::ensure();
	 oln_iter_type(E) dp(se);
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
      struct stat_<I, E, ntg::bin> 
      {
	static ntg::bin
	max(const I& input, const oln_point_type(I)& p, const E& se)
	{
	  mlc::eq<I::dim, E::dim>::ensure();
	  oln_iter_type(E) dp(se);
	  for_all (dp)
	    if (input[p + dp] == true)
	      return true;
	  return false;
	}

	static ntg::bin
	min(const I& input, const oln_point_type(I)& p, const E& se)
	{
	  mlc::eq<I::dim, E::dim>::ensure();
	  oln_iter_type(E) dp(se);
	  for_all (dp)
	    if (input[p + dp] == false)
	      return false;
	  return true;
	}

      };

    } // internal

    template<class I, class E>
    oln_value_type(I) 
      max(const abstract::non_vectorial_image<I>& input,
	  const mlc_exact_type(I)::point_type& p,
	  const abstract::struct_elt<E>& se)
    {
      mlc::eq<I::dim, E::dim>::ensure();
      return internal::stat_<I, E>::max(input.exact(), p, se.exact());
    }
    
    template<class I, class E>
    oln_value_type(I) 
      min(const abstract::non_vectorial_image<I>& input,
	  const mlc_exact_type(I)::point_type& p,
	  //		 const mlc_exact_type(I)::iter_type& p,
	  const abstract::struct_elt<E>& se)
    {
      mlc::eq<I::dim, E::dim>::ensure();
      return internal::stat_<I, E>::min(input.exact(), p, se.exact());
    }

  } // morpho

} // oln


#endif // OLENA_MORPHO_STAT_HH
