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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_CORE_FPOINT2D_HH
# define OLENA_CORE_FPOINT2D_HH

# include <iostream>

# include <oln/core/image2d.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/point2d.hh>

namespace oln {


  // fwd decl
  template <class F>
  class fpoint2d;

  template <class F>
  class fdpoint2d;

  /*! \class point_traits<fpoint2d>
  **
  ** The specialized version for fpoint2d.
  */

  template<class F>
  struct point_traits<fpoint2d<F> >
  {
    enum { dim = 2 };
    typedef fdpoint2d<F> dpoint_type;
  };

  /*! \class fpoint2d
  **
  ** Olena Floating class point2d.
  */

  template <class F>
  class fpoint2d
  {
  private:

    F coord_[2];

  public:

    typedef fpoint2d<F> exact_type;
    typedef fpoint2d<F> self_type;
    typedef fdpoint2d<F> dpoint_type;

    fpoint2d(F x, F y)
    {
      coord_[0] = x;
      coord_[1] = y;
    }

    fpoint2d()
    {}

    /// Give the value of the nth coordinate of the point.

    F
    row() const
    {
      return coord_[0];
    }

    F&
    row()
    {
      return coord_[0];
    }

    F
    col() const
    {
      return coord_[1];
    }

    F&
    col()
    {
      return coord_[1];
    }

    F
    nth(const unsigned dim) const
    {
      assert(dim < 2);
      return coord_[dim];
    }

    /// Return a reference to the nth coordinate of the point.

    F&
    nth(const unsigned dim)
    {
      assert(dim < 2);
      return coord_[dim];
    }

    exact_type
    operator-() const
    {
      return exact_type(-coord_[0], -coord_[1]);
    }

    exact_type&
    operator+=(const dpoint_type& dp)
    {
      row() += dp.row();
      col() += dp.col();
      return *this;
    }

    exact_type&
    operator-=(const dpoint_type& dp)
    {
      row() += dp.row();
      col() += dp.col();
      return *this;
    }

    dpoint_type
    operator-(const self_type& p) const
    {
      dpoint_type dp(row() - p.row(), col() - p.col());
      return dp;
    }


    exact_type
    operator-(const dpoint_type& dp) const
    {
      exact_type p = *this;
      p += dp;
      return p;
    }

    dpoint_type
    operator+(const self_type& p) const
    {
      dpoint_type dp(row() + p.row(), col() + p.col());
      return dp;
    }

    exact_type
    operator+(const dpoint_type& dp) const
    {
      exact_type p = *this;
      p -= dp;
      return p;
    }


    /*! \brief Test if \a p and the current point have the same coordinates
    **
    ** \return True if \a p and the current point have the same coordinates,
    ** false otherwise.
    */

    bool
    operator==(const self_type& p) const
    {
      return p.nth(0) == coord_[0] && p.nth(1) == coord_[1];
    }


    /*! \brief Test if \a p and the current point do
    ** not have the same coordinates.
    **
    ** \return False if \a p and the current point have
    ** the same coordinates, true otherwise.
    */

    bool
    operator!=(const self_type& p) const
    {
      return !(p.nth(0) == coord_[0] && p.nth(1) == coord_[1]);
    }



  };

  template <class T>
  struct func_traits
  {
  };

  template <class T, class F>
  struct nearest;

  template <class T, class F>
  struct bilinear;

  template <class T, class F>
  struct func_traits<nearest<T, F> >
  {
    typedef F f_type;
  };

  template <class T, class F>
  struct func_traits<bilinear<T, F> >
  {
    typedef F f_type;
  };

  template <class T, class Exact>
  struct fpoint2d_access
  {

    typedef typename func_traits<Exact>::f_type f_type;

    const T operator()(const image2d<T> &ima, const fpoint2d<f_type> p) const
    {
      return (static_cast<const Exact *>(this))->access_impl(ima, p);
    }

  };


  template <class T, class F>
  struct nearest : public fpoint2d_access<T, nearest<T, F> >
  {

    const T access_impl(const image2d<T> &ima, const fpoint2d<F> p) const
    {

      point2d p2d(int(p.nth(0)), int(p.nth(1)));

      if (ima.hold(p2d))
	return ima[p2d];
      else // FIXME : should be ntg_max_val(T)
	return ntg::rgb_8(255, 255, 255);
    }

  };

  template <class T, class F>
  struct bilinear : public fpoint2d_access<T, bilinear<T, F> >
  {

    const T access_impl(const image2d<T> &ima, const fpoint2d<F> p) const
    {
      double x = floorf(p.nth(0));
      double y = floorf(p.nth(1));

      int a = int(x);
      int b = int(y);


      if (ima.hold(point2d(a, b)) && ima.hold(point2d(a + 1, b + 1)))
	{
	  int res[3];

	  res[0] = int((double(ima(a, b)[0]) * (x + 1 - p.nth(0)) +
			double(ima(a + 1, b)[0]) * (p.nth(0) - x)) * (y + 1 - p.nth(1)) +
		       (double(ima(a, b + 1)[0]) * (p.nth(0) - x) +
			double(ima(a + 1, b + 1)[0]) * (x + 1 - p.nth(0))) * (p.nth(1) - y));
	  res[1] = int((double(ima(a, b)[1]) * (x + 1 - p.nth(0)) +
			double(ima(a + 1, b)[1]) * (p.nth(0) - x)) * (y + 1 - p.nth(1)) +
		       (double(ima(a, b + 1)[1]) * (p.nth(0) - x) +
			double(ima(a + 1, b + 1)[1]) * (x + 1 - p.nth(0))) * (p.nth(1) - y));
	  res[2] = int((double(ima(a, b)[2]) * (x + 1 - p.nth(0)) +
			double(ima(a + 1, b)[2]) * (p.nth(0) - x)) * (y + 1 - p.nth(1)) +
		       (double(ima(a, b + 1)[2]) * (p.nth(0) - x) +
			double(ima(a + 1, b + 1)[2]) * (x + 1 - p.nth(0))) * (p.nth(1) - y));
	  return ntg::rgb_8(res[0], res[1], res[2]);
	}
      else if (ima.hold(point2d(a, b)))
	return ima(a, b);
      else // FIXME : should be ntg_max_val(T)
	return ntg::rgb_8(255, 255, 255);
    }

  };


} // end of oln


/// Write on an output stream \a o the coordinate of the fpoint2d \a p.

template <class F>
inline std::ostream&
operator<<(std::ostream& o, const oln::fpoint2d<F>& p)
{
  return o << '(' << p.nth(0) << ',' << p.nth(1) << ')';
}





#endif // ! OLENA_CORE_FPOINT2D_HH
