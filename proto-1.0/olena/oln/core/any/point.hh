// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ANY_POINT_HH
# define OLENA_CORE_ANY_POINT_HH

# include <iostream> 
# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/dpoint.hh>


namespace oln {

  // fwd decls
  struct any_point;
  struct any_dpoint;

  // props
  template <>
  struct set_props < category::point, any_point >
  {
    typedef any_dpoint dpoint_type;
  };


  struct any_point : public abstract::point < any_point >
  {

    bool impl_eq(const exact_type& rhs) const
    {
      return this->exact().impl_eq(rhs.exact());
    }

    template <typename D>
    const any_point impl_plus(const abstract::dpoint<D>& rhs) const
    {
      return any_point();
    }

    template <typename P>
    const any_dpoint impl_minus(const abstract::point<P>& rhs) const;

  };


  namespace abstract {

    template <typename E>
    point<E>::operator any_point() const
    {
      return any_point();
    }

  } // end of namespace oln::abstract


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::any_point&)
{
  return ostr << "any";
}


# include <oln/core/any/dpoint.hh>


namespace oln {

  template <typename P>
  const any_dpoint any_point::impl_minus(const abstract::point<P>& rhs) const
  {
    return any_dpoint();
  }

} // end of namespace oln


#endif // ! OLENA_CORE_ANY_POINT_HH
