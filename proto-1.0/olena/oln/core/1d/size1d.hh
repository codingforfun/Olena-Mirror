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

#ifndef OLENA_CORE_1D_SIZE1D_HH
# define OLENA_CORE_1D_SIZE1D_HH

# include <ostream>

# include <oln/core/abstract/size.hh>
# include <oln/core/coord.hh>

namespace oln {


  struct size1d : public abstract::size< size1d >
  {
    size1d()
    {
      // no initialization here so that members are 'undef'
    }

    size1d(coord_t nindices_) :
      nindices_(nindices_),
      border_(2) // FIXME: 2!
    {}

    size1d(coord_t nindices_, coord_t border_) :
      nindices_(nindices_),
      border_(border_)
    {}

    size1d(const size1d& rhs) :
      nindices_(rhs.nindices_),
      border_(rhs.border_)
    {}

    void operator=(const size1d& rhs)
    {
      this->nindices_ = rhs.nindices_;
      this->border_ = rhs.border_;
    }

    unsigned long impl_npoints() const
    {
      return (unsigned long)nindices_;
    }

    bool impl_eq(const size1d& rhs) const
    {
      return this->nindices_ == rhs.nindices_;
    }

    const coord_t nindices() const { return nindices_; }
    const coord_t border() const { return border_; }

  protected:
    coord_t nindices_, border_;
  };


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::size1d& s)
{
  return ostr << "(nindices=" << s.nindices()
	      << ", border=" << s.border() << ")";
}


#endif // ! OLENA_CORE_1D_SIZE1D_HH
