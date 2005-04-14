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

#ifndef OLENA_CORE_ANY_DPOINT_HH
# define OLENA_CORE_ANY_DPOINT_HH

# include <iostream>
# include <mlc/contract.hh>
# include <oln/core/abstract/dpoint.hh>
# include <oln/core/any/point.hh>


namespace oln {


  // fwd decls
  struct any_point;
  namespace abstract {
    template <typename P> struct point;
  }


  struct any_dpoint : public abstract::dpoint < any_dpoint >
  {
    any_dpoint()
    {}

    template <typename D>
    const any_dpoint operator+(const abstract::dpoint<D>& rhs) const
    {
      return any_dpoint();
    }

    template <typename P>
    const any_point operator+(const abstract::point<P>& rhs) const;

    const any_dpoint operator-() const
    {
      return any_dpoint();
    }

    friend class abstract::dpoint<any_dpoint>;

  protected:

    bool impl_fwd_less(const any_dpoint& rhs) const
    {
      precondition(false);
      return false;
    }

    bool impl_eq(const any_dpoint& rhs) const
    {
      return true;
    }

    const coord_t impl_nth(unsigned i) const
    {
      precondition(false);
      return 0;
    }

    coord_t& impl_nth(unsigned i)
    {
      static coord_t dummy = coord_t();
      precondition(false);
      return dummy;
    }

    unsigned impl_dim() const
    {
      return 0;
    }

  };

} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::any_dpoint&)
{
  return ostr << "any";
}

template <typename D>
bool operator==(const oln::abstract::dpoint<D>& lhs,
		const oln::any_dpoint& rhs)
{
  return true;
}

template <typename D>
bool operator!=(const oln::abstract::dpoint<D>& lhs,
		const oln::any_dpoint& rhs)
{
  return false;
}


# include <oln/core/any/point.hh>


namespace oln {

  template <typename P>
  const any_point any_dpoint::operator+(const abstract::point<P>& rhs) const
  {
    return any_point();
  }

}

#endif // ! OLENA_CORE_ANY_DPOINT_HH
