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


#ifndef OLENA_CORE_3D_SIZE3D_HH
# define OLENA_CORE_3D_SIZE3D_HH

# include <ostream>

# include <oln/core/abstract/size.hh>
# include <oln/core/coord.hh>

namespace oln {


  struct size3d : public abstract::size< size3d >
  {
    size3d()
    {
      // no initialization here so that members are 'undef'
    }

    size3d(coord_t nslices_, coord_t nrows_, coord_t ncols_) :
      nslices_(nslices_),
      nrows_(nrows_),
      ncols_(ncols_),
      border_(2) // FIXME: 2!
    {}

    size3d(coord_t nslices_, coord_t nrows_, coord_t ncols_, coord_t border_) :
      nslices_(nslices_),
      nrows_(nrows_),
      ncols_(ncols_),
      border_(border_)
    {}

    size3d(const size3d& rhs) :
      nslices_(rhs.nslices_),
      nrows_(rhs.nrows_),
      ncols_(rhs.ncols_),
      border_(rhs.border_)
    {}

    void operator=(const size3d& rhs)
    {
      this->nslices_ = rhs.nslices_;
      this->nrows_ = rhs.nrows_;
      this->ncols_ = rhs.ncols_;
      this->border_ = rhs.border_;
    }

    unsigned long impl_npoints() const
    {
      return (unsigned long)nslices_ * 
             (unsigned long)nslices_ * 
             (unsigned long)ncols_;
    }

    bool impl_eq(const any_size& rhs) const
    {
      return true;
    }

    bool impl_eq(const size3d& rhs) const
    {
      return this->nslices_ == rhs.nslices_ &&
             this->nrows_ == rhs.nrows_ &&
             this->ncols_ == rhs.ncols_;
    }

    const coord_t nslices() const { return nslices_; }
    const coord_t nrows() const { return nrows_; }
    const coord_t ncols() const { return ncols_; }
    const coord_t border() const { return border_; }

  protected:
    coord_t nslices_, nrows_, ncols_, border_;
  };


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::size3d& s)
{
  return ostr << "(nslices=" << s.nslices()
	      << ", nrows=" << s.nrows()
	      << ", ncols=" << s.ncols()
	      << ", border=" << s.border() << ")";
}


#endif // ! OLENA_CORE_3D_SIZE3D_HH
