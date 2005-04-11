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

# include <mlc/cmp.hh>
# include <ntg/bin.hh>
# include <oln/basics.hh>


namespace oln {

  namespace morpho {

    namespace internal {

      /*!
      ** \brief Min and Max on a window.
      **
      ** We need to use this inner definition in order to specialize
      ** max and min on binary images.
      **
      ** \param I Image exact type.
      ** \param W Window type.
      ** \param V Associated value type.
      */
      template <class I, class W, class V = oln_type_of(I, value)>
      struct stat_
      {
	/*!
	** \brief Maximum of a window.
	**
	** Look for  the maximum in the window win disposed
	** on the image input, at the point p.
	**
	** \arg input Input image.
	** \arg p Point of the image to move the window on.
	** \arg win The window to use.
	*/
	static V
	max(const I& input, const oln_type_of(I, point)& p, const W& win)
	{
          // FIXME: test dim I == dim
	  mlc::eq<oln_type_of(I, size), oln_wn_type_of(W, size)>::ensure();

          oln_wn_type_of(W, fwd_qiter) dp(win);
	  dp.start();
	  V val = input[p + dp];
          for_all_remaining (dp)
	    if (input.hold(p + dp))
	      if (val < input[p + dp].value())
		val = input[p + dp].value();
	  return val;
	}

	/*!
	** \brief Minimum of a window.
	**
	** Look for  the minimum in the window win disposed
	** on the image input, at the point p.
	**
	** \arg input Input image.
	** \arg p Point of the image to move the window on.
	** \arg win The window to use.
	*/
	static V
	min(const I& input, const oln_type_of(I, point)& p, const W& win)
	{
          // FIXME: test dim I == dim W
	  mlc::eq<oln_type_of(I, size), oln_wn_type_of(W, size)>::ensure();

	  oln_wn_type_of(W, fwd_qiter) dp(win);
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

      template <class I, class W>
      struct stat_<I, W, ntg::bin>
      {
	static ntg::bin
	max(const I& input, const oln_type_of(I, point)& p, const W& win)
	{
	  mlc::eq<oln_type_of(I, size), oln_wn_type_of(W, size)>::ensure();
	  oln_wn_type_of(W, fwd_qiter) dp(win);
	  for_all (dp)
	    if (input.hold(p + dp))
	      if (input[p + dp] == true)
		return true;
	  return false;
	}

	static ntg::bin
	min(const I& input, const oln_type_of(I, point)& p, const W& win)
	{
	  mlc::eq<oln_type_of(I, size), oln_wn_type_of(W, size)>::ensure();
	  oln_wn_type_of(W, fwd_qiter) dp(win);
	  for_all (dp)
	    if (input.hold(p + dp))
	      if (input[p + dp] == false)
		return false;
	  return true;
	}

      };

    } // internal

    /*!
    ** \brief Maximum of a window.
    **
    ** Look for  the maximum in the window win disposed
    ** on the image input, at the point p.
    **
    ** \param I Image exact type.
    ** \param W Window type.
    **
    ** \arg input Input image.
    ** \arg p Point of the image to move the window on.
    ** \arg win The window to use.
    */
    template<class I, class W>
    oln_type_of(I, value)
    max(const abstract::image<I>& input,
	const oln_type_of(I, point)& p,
	const abstract::window<W>& win)
    {
      // FIXME: test dim I == dim W
      return internal::stat_<I, W>::max(input.exact(), p, win.exact());
    }

    /*! ** \brief Minimum of a window.
    **
    ** Look for  the minimum in the window win disposed
    ** on the image input, at the point p.
    **
    ** \param I Image exact type.
    ** \param W Window type.
    **
    ** \arg input Input image.
    ** \arg p Point of the image to move the window on.
    ** \arg win The window to use.
    */
    template<class I, class W>
    oln_type_of(I, value)
    min(const abstract::image<I>& input,
	const oln_type_of(I, point)& p,
	//		 const mlc_exact_type(I)::iter_type& p,
	const abstract::window<W>& win)
    {
      // FIXME: test dim I == dim W
      return internal::stat_<I, W>::min(input.exact(), p, win.exact());
    }

  } // end of namespace morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_STAT_HH
