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

#ifndef OLENA_CORE_2D_DPOINT2D_HH
# define OLENA_CORE_2D_DPOINT2D_HH

# include <ostream>

# include <oln/core/coord.hh>
# include <oln/core/2d/point2d.hh>

// FIXME: there's an assumption here: we do not need inheritance for
// dpoints.  so abstract::dpoint does not exist...

// FIXME: doc!

namespace oln {

  struct dpoint2d
  {
    dpoint2d()
    {
    }

    dpoint2d(coord_t row_, coord_t col_) :
      row_(row_),
      col_(col_)
    {
    }

    dpoint2d(const dpoint2d& rhs) :
      row_(rhs.row_),
      col_(rhs.col_)
    {
    }

    dpoint2d& operator=(const dpoint2d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->row_ = rhs.row_;
      this->col_ = rhs.col_;
      return *this;
    }

    bool operator==(const dpoint2d& rhs) const
    {
      return this->row_ == rhs.row_ and this->col_ == rhs.col_;
    }

    bool operator!=(const dpoint2d& rhs) const
    {
      return not this->operator==(rhs);
    }

    const dpoint2d operator+(const dpoint2d& rhs) const
    {
      dpoint2d tmp(this->row() + rhs.row(), this->col() + rhs.col());
      return tmp;
    }

    const point2d operator+(const point2d& rhs) const
    {
      point2d tmp(this->row() + rhs.row(), this->col() + rhs.col());
      return tmp;
    }

    const dpoint2d operator-() const
    {
      dpoint2d tmp(-this->row(), -this->col());
      return tmp;
    }

    const coord_t row() const { return row_; }
    const coord_t col() const { return col_; }

    coord_t& row() { return row_; }
    coord_t& col() { return col_; }

  protected:
    coord_t row_, col_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::dpoint2d& dp)
{
  return ostr << '(' << dp.row() << ',' << dp.col() << ')';
}


#endif // ! OLENA_CORE_2D_DPOINT2D_HH
