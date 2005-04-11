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

#ifndef OLENA_CORE_1D_DPOINT1D_HH
# define OLENA_CORE_1D_DPOINT1D_HH

# include <iostream>

# include <oln/core/coord.hh>
# include <oln/core/abstract/dpoint.hh>


namespace oln {

  // fwd decls
  struct dpoint1d;
  struct point1d;

  // super type
  template <>
  struct set_super_type < dpoint1d > { typedef abstract::dpoint< dpoint1d > ret; };


  struct dpoint1d : public abstract::dpoint < dpoint1d >
  {
    dpoint1d()
    {
    }

    dpoint1d(coord_t index_) :
      index_(index_)
    {
    }

    dpoint1d(const dpoint1d& rhs) :
      index_(rhs.index_)
    {
    }

    dpoint1d& operator=(const dpoint1d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->index_ = rhs.index_;
      return *this;
    }

    bool operator==(const dpoint1d& rhs) const
    {
      return this->index_ == rhs.index_;
    }

    bool operator!=(const dpoint1d& rhs) const
    {
      return this->index_ != rhs.index_;
    }

    const dpoint1d operator+(const dpoint1d& rhs) const
    {
      dpoint1d tmp(this->index() + rhs.index());
      return tmp;
    }

    const point1d operator+(const point1d& rhs) const;

    const dpoint1d operator-() const
    {
      dpoint1d tmp(-this->index());
      return tmp;
    }

    const coord_t index() const { return index_; }
    coord_t& index() { return index_; }

    coord_t impl_nth(unsigned i) const
    {
      // FIXME: remove when add in abstract::point
      precondition(i == 0);
      return index_;
    }

  protected:
    coord_t index_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::dpoint1d& dp)
{
  return ostr << '(' << dp.index() << ')';
}

# include <oln/core/1d/point1d.hh>

namespace oln {

  const point1d dpoint1d::operator+(const point1d& rhs) const
  {
    point1d tmp(this->index() + rhs.index());
    return tmp;
  }

}


#endif // ! OLENA_CORE_1D_DPOINT1D_HH
