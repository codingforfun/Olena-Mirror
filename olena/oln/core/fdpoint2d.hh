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

#ifndef OLENA_CORE_FDPOINT2D_HH
# define OLENA_CORE_FDPOINT2D_HH

# include <iostream>

# include <oln/core/abstract/image.hh>
# include <oln/core/fpoint2d.hh>

namespace oln {

  // fwd decl
  template <class F>
  class dfpoint2d;



  /*! \class dpoint_traits<fdpoint2d>
  **
  ** The specialized version for fdpoint2d.
  */

  template<class F>
  struct dpoint_traits<fdpoint2d<F> >
  {
    enum { dim = 2 };
    typedef fpoint2d<F> point_type;
  };

  /*! \class fdpoint2d
  **
  */

  template <class F>
  class fdpoint2d
  {
  private:

    F coord_[2];


  public:


    typedef fdpoint2d<F> exact_type;

    fdpoint2d()
    {}

    /// The coordinates of the dpoint2d are set to \a row and \a col.

    fdpoint2d(F row, F col)
    {
      coord_[0] = row;
      coord_[1] = col;
    }

    /// The coordinates of the dpoint2d are set to the \a p coordinates.
    fdpoint2d(const exact_type& p)
    {
      coord_[0] = p.row();
      coord_[1] = p.col();
    }

    /// Return the value of the dpoint2d row coordinate.

    F
    row() const
    {
      return coord_[0];
    }

    /// Return a reference to the dpoint2d row coordinate.

    F&
    row()
    {
      return coord_[0];
    }

    /// Return the value of the dpoint2d column coordinate.

    F
    col() const
    {
      return coord_[1];
    }

    /// Return a reference to the dpoint2d column coordinate.

    F&
    col()
    {
      return coord_[1];
    }

    static std::string
    name()
    {
      return "fdpoint2d";
    }

    exact_type
    operator+(const exact_type& dp) const
    {
      exact_type tmp(*this);
      tmp += dp;
      return tmp;
    }

    /*! \brief Return a dpoint2d whose coordinates are equal to
    ** the opposite of the current dpoint2d coordinates.
    */

    exact_type
    operator-() const
    {
      exact_type dp(-row(), -col());
      return dp;
    }


    /*! \brief Return a dpoint2d whose coordinates are equal to
    ** the current dpoint2d coordinates minus \a dp coordinates.
    */

    exact_type
    operator-(const exact_type& dp) const
    {
      exact_type tmp = *this;
      tmp -= dp;
      return tmp;
    }


    /*! \brief Return a reference to the current dpoint2d
    ** plus \a dp.
    */

    exact_type&
    operator+=(const exact_type& dp)
    {
      row() += dp.row();
      col() += dp.col();
      return *this;
    }

    /*! \brief Return a reference to the current dpoint2d
    ** minus \a dp.
    */

    exact_type&
    operator-=(const exact_type& dp)
    {
      row() -= dp.row();
      col() -= dp.col();
      return *this;
    }
  };


} // end of oln

/// Write on an output stream \a o the coordinates of the dpoint2d \a dp.

template <class F>
inline std::ostream&
operator<<(std::ostream& o, const oln::fdpoint2d<F> dp)
{
  o << "(" << dp.row() << "," << dp.col() << ")" << std::endl;
  return o;
}


#endif // !OLENA_CORE_FDPOINT2D_HH
