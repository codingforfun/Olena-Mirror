// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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
# include <mlc/cmp.hh>


namespace oln {

  namespace morpho {

    namespace internal {

      /*!
      ** \brief Min and Max on a structuring element.
      **
      ** We need to use this inner definition in order to specialize
      ** max and min on binary images.
      **
      ** \param I Image exact type.
      ** \param E Structuring element type.
      ** \param V Associated value type.
      */
      template <class I, class E, class V = oln_type_of(I, value)>
      struct stat_
      {
	/*!
	** \brief Maximum of a structuring element.
	**
	** Look for  the maximum in the structuring  element se disposed
	** on the image input, at the point p.
	**
	** \arg input Input image.
	** \arg p Point of the image to move the structuring element on.
	** \arg se The structuring element to use.
	*/
	static V
	max(const I& input, const oln_type_of(I, point)& p, const E& se)
	{
          // FIXME: test dim I == dim
	  mlc::eq<oln_type_of(I, size), oln_type_of(E, size)>::ensure();

          oln_type_of(E, fwd_witer) dp(se);
	  dp.start();
	  V val = input[p + dp];
          for_all_remaining (dp)
	    if (input.hold(p + dp))
	      if (val < input[p + dp].value())
		val = input[p + dp].value();
	  return val;
	}

	/*!
	** \brief Minimum of a structuring element.
	**
	** Look for  the minimum in the structuring  element se disposed
	** on the image input, at the point p.
	**
	** \arg input Input image.
	** \arg p Point of the image to move the structuring element on.
	** \arg se The structuring element to use.
	*/
	static V
	min(const I& input, const oln_type_of(I, point)& p, const E& se)
	{
          // FIXME: test dim I == dim E
	  mlc::eq<oln_type_of(I, size), oln_type_of(E, size)>::ensure();

	  oln_type_of(E, fwd_witer) dp(se);
	  dp.start();
	  V val = input[p + dp];
	  for_all_remaining (dp)
	    if (input.hold(p + dp))
	      if (val > input[p + dp].value())
		val = input[p + dp].value();
	  return val;
	}

      };

      /* Binary specialization.  */

      template <class I, class E>
      struct stat_<I, E, ntg::bin>
      {
	static ntg::bin
	max(const I& input, const oln_type_of(I, point)& p, const E& se)
	{
	  mlc::eq<oln_type_of(I, size), oln_type_of(E, size)>::ensure();
	  oln_type_of(E, fwd_witer) dp(se);
	  for_all (dp)
	    if (input.hold(p + dp))
	      if (input[p + dp] == true)
		return true;
	  return false;
	}

	static ntg::bin
	min(const I& input, const oln_type_of(I, point)& p, const E& se)
	{
	  mlc::eq<oln_type_of(I, size), oln_type_of(E, size)>::ensure();
	  oln_type_of(E, fwd_witer) dp(se);
	  for_all (dp)
	    if (input.hold(p + dp))
	      if (input[p + dp] == false)
		return false;
	  return true;
	}

      };

    } // internal

    /*!
    ** \brief Maximum of a structuring element.
    **
    ** Look for  the maximum in the structuring  element se disposed
    ** on the image input, at the point p.
    **
    ** \param I Image exact type.
    ** \param E Structuring element type.
    **
    ** \arg input Input image.
    ** \arg p Point of the image to move the structuring element on.
    ** \arg se The structuring element to use.
    */
    template<class I, class E>
    oln_type_of(I, value)
    max(const abstract::image<I>& input,
	const oln_type_of(I, point)& p,
	const abstract::struct_elt<E>& se)
    {
      // FIXME: test dim I == dim E
      return internal::stat_<I, E>::max(input.exact(), p, se.exact());
    }

    /*! ** \brief Minimum of a structuring element.
    **
    ** Look for  the minimum in the structuring  element se disposed
    ** on the image input, at the point p.
    **
    ** \param I Image exact type.
    ** \param E Structuring element type.
    **
    ** \arg input Input image.
    ** \arg p Point of the image to move the structuring element on.
    ** \arg se The structuring element to use.
    */
    template<class I, class E>
    oln_type_of(I, value)
    min(const abstract::image<I>& input,
	const oln_type_of(I, point)& p,
	//		 const mlc_exact_type(I)::iter_type& p,
	const abstract::struct_elt<E>& se)
    {
      // FIXME: test dim I == dim E
      return internal::stat_<I, E>::min(input.exact(), p, se.exact());
    }

  } // end of namespace morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_STAT_HH
