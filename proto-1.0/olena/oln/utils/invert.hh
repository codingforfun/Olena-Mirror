// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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


#ifndef OLENA_CORE_UTILS_INVERT_HH
# define OLENA_CORE_UTILS_INVERT_HH

# include <oln/core/abstract/image_by_delegation.hh>
# include <oln/funobj/invert.hh>

namespace oln {

  namespace utils {
    template <typename I> struct image_inverted;
  }

  // super type

  template <typename I>
  struct set_super_type < utils::image_inverted<I> >
  { typedef abstract::image_by_delegation<const I, utils::image_inverted<I> > ret; };

  // props

  template <typename I>
  struct set_props < category::image, utils::image_inverted<I> >
  {
    typedef oln_type_of(I, concrete) concrete_type;
  };



  namespace utils {

    template <typename I>
    struct image_inverted :
      public abstract::image_by_delegation<const I, image_inverted<I> >
    {
      typedef abstract::image_by_delegation<const I, image_inverted<I> > super_type;
      typedef oln_type_of(I, value) value_type;
      typedef oln_type_of(I, point) point_type;

    public:

      image_inverted()
      {
      }

      image_inverted(const abstract::image<I>& image) :
	super_type(image)
      {
      }

      I& real() const
      {
	return this->image_.unbox();
      }

      const value_type impl_get(const point_type& p) const
      {
	const static oln::funobj::invert& obj = oln::funobj::invert();
	return obj(this->image_[p].value());
      }

    };

    template <typename I>
    image_inverted<I> invert(const abstract::image<I>& input)
    {
      image_inverted<I> tmp(input);
      return tmp;
    }

  } // end of namespace utils

} // end of namespace oln

#endif // ! OLENA_CORE_UTILS_INVERT_HH

