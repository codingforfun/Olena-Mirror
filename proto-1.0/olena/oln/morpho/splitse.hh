// Copyright (C) 2001, 2002, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_SPLITSE_HH
# define OLENA_MORPHO_SPLITSE_HH

# include <oln/basics.hh>

# include <oln/core/1d/size1d.hh>
# include <oln/core/2d/size2d.hh>
# include <oln/core/3d/size3d.hh>

namespace oln {
  namespace morpho {

    template <class S>
    struct dim_traits
    {
    };

    template <>
    struct dim_traits<size1d>
    {
      enum {dim = 1};
    };

    template <>
    struct dim_traits<size2d>
    {
      enum {dim = 2};
    };

    template <>
    struct dim_traits<size3d>
    {
      enum {dim = 3};
    };

    /*!
    ** \brief Get a sub part of a window.
    **
    ** \param E Exact type of the window.
    **
    ** \arg win The window.
    **
    ** A point p take part of the new window if it exists
    ** a i  that belongs to [[0..dim-1]] like  p(i) < 0 and  for all j
    ** that belongs to [[0..i-1]] p(j) = 0.
    **
    */


    template<class W>
    W
    get_plus_win_only(const abstract::window<W>& win)
    {
      oln_wn_type_of(W, fwd_qiter) dp(win.exact());
      W out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < dim_traits<oln_wn_type_of(W, size)>::dim; ++n)
	    if (dp.nth(n) < 0) {
	      out.add(dp);
	      break;
	    } else if (dp.nth(n) > 0) {
	      break;
	    }
	}
      return out;
    }

    /*!
    ** \brief Get a sub part of a window.
    **
    ** \param W Exact type of the window.
    **
    ** \arg win The window.
    **
    ** A point p take part of the new window if it exists
    ** a i  that belongs to [[0..dim-1]] like  p(i) < 0 and  for all j
    ** that  belongs to  [[0..i-1]] p(j)  =  0 or  if for  all i  that
    ** belongs to [[0..dim-1]] p(i) = 0.
    **
    */
    template<class W>
    W
    get_plus_win_p(const abstract::window<W>& win)
    {
      oln_wn_type_of(W, fwd_qiter) dp(win.exact());
      W out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < dim_traits<oln_wn_type_of(W, size)>::dim; ++n)
	    if (dp.nth(n) < 0) {
	      out.add(dp);
	      break;
	    } else if (dp.nth(n) > 0) {
	      break;
	    }
	  // All p.nth(n) are 0.
	  if (n == dim_traits<oln_wn_type_of(W, size)>::dim)
	    out.add(dp);
	}
      return out;
    }

    /*!
    ** \brief Get a sub part of a window.
    **
    ** \param W Exact type of the window.
    **
    ** \arg win The window.
    **
    ** A point p take part of the new window if it exists
    ** a i  that belongs to [[0..dim-1]] like  p(i) > 0 and  for all j
    ** that  belongs to  [[0..i-1]] p(j)  =  0.
    **
    */
    template<class W>
    W
    get_minus_win_only(const abstract::window<W>& win)
    {
      oln_wn_type_of(W, fwd_qiter) dp(win.exact());
      W out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < dim_traits<oln_wn_type_of(W, size)>::dim; ++n)
	    if (dp.nth(n) > 0) {
	      out.add(dp);
	      break;
	    } else if (dp.nth(n) < 0) {
	      break;
	    }
	}
      return out;
    }

    /*!
    ** \brief Get a sub part of a window.
    **
    ** \param W Exact type of the window.
    **
    ** \arg win The window.
    **
    ** A point p take part of the new window if it exists
    ** a i  that belongs to [[0..dim-1]] like  p(i) > 0 and  for all j
    ** that  belongs to  [[0..i-1]] p(j)  =  0 or  if for  all i  that
    ** belongs to [[0..dim-1]] p(i) = 0.
    **
    */
    template<class W>
    W
    get_minus_win_p(const abstract::window<W>& win)
    {
      oln_wn_type_of(W, fwd_qiter) dp(win.exact());
      W out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < dim_traits<oln_wn_type_of(W, size)>::dim; ++n)
	    if (dp.nth(n) > 0) {
	      out.add(dp);
	      break;
	    } else if (dp.nth(n) < 0) {
	      break;
	    }
	  // All p.nth(n) are 0.
	  if (n == dim_traits<oln_wn_type_of(W, size)>::dim)
	    out.add(dp);
	}
      return out;
    }

  } // end of namespace oln::morpho

} // end of namespace oln

#endif // ! OLENA_MORPHO_SPLITSE_HH
