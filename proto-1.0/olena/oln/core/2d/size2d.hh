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

#ifndef OLENA_CORE_2D_SIZE2D_HH
# define OLENA_CORE_2D_SIZE2D_HH

# include <iostream>

# include <oln/core/abstract/size.hh>
# include <oln/core/coord.hh>

namespace oln {

  // fwd decl
  struct size2d;

  // super type
  template <>
  struct set_super_type < size2d > { typedef abstract::size< size2d > ret; };



  struct size2d : public abstract::size< size2d >
  {
    size2d()
    {
      // no initialization here so that members are 'undef'
    }

    size2d(coord_t nrows_, coord_t ncols_) :
      nrows_(nrows_),
      ncols_(ncols_),
      border_(2) // FIXME: 2!
    {}

    size2d(coord_t nrows_, coord_t ncols_, coord_t border_) :
      nrows_(nrows_),
      ncols_(ncols_),
      border_(border_)
    {}

    size2d(const size2d& rhs) :
      nrows_(rhs.nrows_),
      ncols_(rhs.ncols_),
      border_(rhs.border_)
    {}

    void operator=(const size2d& rhs)
    {
      this->nrows_ = rhs.nrows_;
      this->ncols_ = rhs.ncols_;
      this->border_ = rhs.border_;
    }

    unsigned long impl_npoints() const
    {
      return (unsigned long)nrows_ * (unsigned long)ncols_;
    }

    bool impl_eq(const any_size& rhs) const
    {
      return true;
    }

    bool impl_eq(const size2d& rhs) const
    {
      return this->nrows_ == rhs.nrows_ &&
             this->ncols_ == rhs.ncols_;
    }

    const coord_t nrows() const { return nrows_; }
    const coord_t ncols() const { return ncols_; }
    const coord_t border() const { return border_; }

  protected:
    coord_t nrows_, ncols_, border_;
  };


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::size2d& s)
{
  return ostr << "(nrows=" << s.nrows()
	      << ", ncols=" << s.ncols()
	      << ", border=" << s.border() << ")";
}


#endif // ! OLENA_CORE_2D_SIZE2D_HH
