// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_W_WINDOW2D_HH
# define OLENA_CORE_W_WINDOW2D_HH

# include <mlc/array/all.hh>
# include <ntg/basics.hh>
# include <oln/core/abstract/w_windownd.hh>
# include <oln/core/accum.hh>
# include <oln/core/winiter.hh>
# include <oln/core/winneighb.hh>
# include <oln/core/dpoint2d.hh>
# include <oln/core/window2d.hh>
# include <algorithm>

namespace oln {

  template<class T>
  class w_window2d; // forward declaration

  /*!
  ** \brief Traits for w_windownd2d.
  */
  template<class T>
  struct struct_elt_traits<w_window2d<T> >: public
  struct_elt_traits<abstract::w_windownd<w_window2d<T> > >
  {
    enum { dim = 2 }; ///< Dimension.
    typedef T weight_type; ///< Type of weight.
    typedef point2d point_type; ///< Type of point.
    typedef dpoint2d dpoint_type; ///< Type of dpoint.
    typedef winiter< w_window2d<T> > iter_type; ///< Type of iterator.
    typedef winneighb< w_window2d<T> > neighb_type; ///< Type of neighbor.
  };

  /*!
  ** \brief Window 2 dimensions with weight.
  **
  ** A window is a set of points. This class
  ** defines how to deal with. These points have 2 dimensions.
  */
  template<class T>
  class w_window2d : public abstract::w_windownd<w_window2d<T> >
  {

    typedef abstract::w_windownd< w_window2d<T> > super_type;
    ///< The super type.

  public:

    typedef w_window2d<T> self_type; ///< The self type.

    /*!
    ** \brief The associate image's type of iterator.
    ** \warning Prefer the macros oln_iter_type(Iterable) and
    ** oln_iter_type_(Iterable) (the same without the 'typename' keyword)
    */
    typedef typename struct_elt_traits< self_type >::iter_type   iter_type;

    typedef typename struct_elt_traits< self_type >::neighb_type neighb_type;
    ///< Type of neighbor.

    /*!
    ** \brief The associate image's type of dpoint (move point).
    ** \warning Prefer the macros oln_dpoint_type(Pointable) and
    ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
    */
    typedef typename struct_elt_traits< self_type >::dpoint_type dpoint_type;

    typedef typename struct_elt_traits< self_type >::weight_type weight_type;
    ///< Type of weight.

    friend class abstract::window_base<abstract::w_window<w_window2d>, w_window2d>;

    /*!
    ** \brief Construct a w_window of 2 dimensions.
    */
    w_window2d(): super_type()
    {}

    /*!
    ** \brief Construct a w_window of 2 dimensions.
    ** \arg size The number of element.
    */
    w_window2d(unsigned size) : super_type(size)
    {}

    /*!
    ** \brief Construct a w_window of 2 dimensions from an array of elements.
    ** \arg arr The array of elements.
    ** \todo FIXME: this constructor is not in w_window1d.hh nor w_window3d.hh.
    ** Is it really useful ?
    */
    template<class I, class T2>
    w_window2d(const mlc::array2d<I, T2 >& arr) :
      super_type(I::card)
    {
      unsigned i = 0;
      for (coord row = -I::center_row; row <= I::nrows - I::center_row - 1; ++row)
	for (coord col = -I::center_col; col <= I::ncols - I::center_col - 1; ++col)
          add(row, col, arr[i++]);
    }

    /*!
    ** \brief Add a point by coordinates to the w_window.
    ** \arg row The coordinate (row) of the new point (2 dimensions).
    ** \arg col The coordinate (col) of the new point (2 dimensions).
    ** \arg weight The weight of the new point.
    **
    ** Add a new member by its coordinates to the w_window.
    ** The coordinates are only the column number because the w_window is
    ** of 2 dimensions.
    */
    w_window2d<T>&
    add(coord row, coord col, const weight_type& weight)
    {
      return add(dpoint_type(row, col), weight);
    }

    /*!
    ** \brief Add a dpoint (move point) to the w_window.
    ** \arg dp The new point.
    ** \arg w The weight of the new point.
    **
    ** Add a new member to the w_window. This point must be of 2
    ** dimensions.
    */
    w_window2d<T>&
    add(const dpoint_type& dp, const weight_type& w)
    {
      return this->exact().add_(dp, w);
    }

    /*!
    ** \brief Set the weight of a point.
    ** \arg dp The point to set the weight.
    ** \arg weight The weight of the point.
    */
    const weight_type&
    set(const dpoint_type& dp, const weight_type& weight)
    {
      return this->exact().set_(dp, weight);
    }

    /*!
    ** \brief Set the weight of a point by coordinates.
    ** \arg row The coordinates (row) of the point.
    ** \arg col The coordinates (col) of the point.
    ** \arg weight The weight of the point.
    **
    ** The coordinates are only the column number because the w_window is
    ** of 2 dimensions.
    */
    const weight_type&
    set(coord row, coord col, const weight_type& weight)
    {
      return set(dpoint_type(row, col), weight);
    }

    /*!
    ** \brief Return its type in a string.
    ** \return The type in a string.
    **
    ** Very useful to debug.
    */
    static std::string
    name()
    {
      return std::string("w_window2d<") + ntg_name(T) +  ">";
    }

  protected:

    /*!
    ** \brief Update delta.
    ** \arg dp a move point.
    ** \return Delta.
    **
    ** If the point is the biggest element of the w_window,
    ** then this point is assigned to delta.
    */
    coord
    delta_update_(const dpoint_type& dp)
    {
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return this->delta_;
    }

  };

  /*!
  ** \brief Convert a window (2 dimension) to a w_window (2 dimension).
  ** \arg weight The weight to set for each element of the window.
  ** \arg win The window to convert.
  ** \return The new w_window.
  */
  template<class T>
  w_window2d<T>
  mk_w_win_from_win(T weight, const window2d& win)
  {
    w_window2d<T> w_win(win.card());
    for (unsigned i = 0; i < win.card(); ++i)
      w_win.add(win.dp(i), weight);
    return w_win;
  }

} // end of oln

#endif // ! OLENA_CORE_W_WINDOW2D_HH
