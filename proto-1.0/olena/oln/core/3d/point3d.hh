// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_3D_POINT3D_HH
# define OLENA_CORE_3D_POINT3D_HH

# include <ostream>

# include <oln/core/abstract/point.hh>
# include <oln/core/coord.hh>

namespace oln {


  struct point3d;
  struct dpoint3d;

  // category
  template <>
  struct set_category< point3d > { typedef category::point ret; };

  // super_type
  template <>
  struct set_super_type< point3d > { typedef abstract::point< point3d > ret; };

  // props
  template <>
  struct set_props < category::point, point3d > : public props_of<category::point>
  {
    typedef dpoint3d dpoint_type;
  };


  struct point3d : public abstract::point< point3d >
  {
    point3d()
    {
      // no initialization here so that slice_, row_, and col_ are 'undef'
    }

    point3d(coord_t slice_, coord_t row_, coord_t col_) :
      slice_(slice_),
      row_(row_),
      col_(col_)
    {
    }

    point3d(const point3d& rhs) :
      slice_(rhs.slice_),
      row_(rhs.row_),
      col_(rhs.col_)
    {
    }

    point3d& operator=(const point3d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->slice_ = rhs.slice_;
      this->row_ = rhs.row_;
      this->col_ = rhs.col_;
      return *this;
    }

    bool impl_eq(const point3d& rhs) const
    {
      return this->slice_ == rhs.slice_ and
             this->row_ == rhs.row_     and
             this->col_ == rhs.col_;
    }

    const coord_t slice() const { return slice_; }
    const coord_t row() const { return row_; }
    const coord_t col() const { return col_; }

    coord_t& slice() { return slice_; }
    coord_t& row() { return row_; }
    coord_t& col() { return col_; }

  protected:
    coord_t slice_, row_, col_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::point3d& p)
{
  return ostr << '(' << p.slice() << ',' << p.row() << ',' << p.col() << ')';
}


#endif // ! OLENA_CORE_3D_POINT3D_HH
