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

#ifndef OLENA_CORE_1D_POINT1D_HH
# define OLENA_CORE_1D_POINT1D_HH

# include <ostream>

# include <oln/core/abstract/point.hh>
# include <oln/core/coord.hh>
# include <oln/core/properties.hh>

// FIXME: doc

// FIXME: test that coords are defined

namespace oln {


  struct point1d;
  struct dpoint1d;

  // category
  template <>
  struct set_category< point1d > { typedef category::point ret; };

  // super_type
  template <>
  struct set_super_type< point1d > { typedef abstract::point< point1d > ret; };

  // props
  template <>
  struct set_props < category::point, point1d > : public props_of<category::point>
  {
    typedef dpoint1d dpoint_type;
  };



  struct point1d : public abstract::point< point1d >
  {
    point1d()
    {
      // no initialization here so that index_ is 'undef'
    }

    point1d(coord_t index_) :
      index_(index_)
    {
    }

    point1d(const point1d& rhs) :
      index_(rhs.index_)
    {
    }

    point1d& operator=(const point1d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->index_ = rhs.index_;
      return *this;
    }

    const coord_t index() const { return index_; }
    coord_t& index() { return index_; }

# if defined __GNUC__ && __GNUC__ >= 3
    friend class abstract::point< point1d >;
  protected:
# endif

    const point1d impl_plus(const dpoint1d& rhs) const;

    const dpoint1d impl_minus(const point1d& rhs) const;

    bool impl_eq(const point1d& rhs) const
    {
      return this->index_ == rhs.index_;
    }

  protected:

    coord_t index_;
  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::point1d& p)
{
  return ostr << '(' << p.index() << ')';
}


# include <oln/core/1d/dpoint1d.hh>


namespace oln {

  const point1d point1d::impl_plus(const dpoint1d& rhs) const
  {
    point1d tmp(this->index() + rhs.index());
    return tmp;
  }

  const dpoint1d point1d::impl_minus(const point1d& rhs) const
  {
    dpoint1d tmp(this->index() - rhs.index());
    return tmp;
  }

} // end of namespace oln



#endif // ! OLENA_CORE_1D_POINT1D_HH
