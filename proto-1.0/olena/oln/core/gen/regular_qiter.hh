// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_GEN_REGULAR_QITER_HH
# define OLENA_CORE_GEN_REGULAR_QITER_HH

# include <oln/core/abstract/grid.hh>
# include <oln/core/abstract/qiter.hh>
# include <oln/core/gen/regular_window.hh>


namespace oln {

  // fwd decl
  namespace abstract {
    template <typename G, typename E> struct regular_qiter;
  }

  // category
  template <typename G, typename E>
  struct set_super_type< abstract::regular_qiter<G,E> > { typedef abstract::qiter<E> ret; };

  // props
  template <typename G, typename E>
  struct set_props < category::qiter, abstract::regular_qiter<G,E> >
  {
    typedef G grid_type;
  };


  namespace abstract
  {

    template <typename G, typename E>
    struct regular_qiter : public qiter<E>
    {
      typedef qiter<E> super_type;

      typedef oln_grd_type_of(G, coord)  coord_type;
      typedef oln_grd_type_of(G, point)  point_type;
      typedef oln_grd_type_of(G, dpoint) dpoint_type;

      typedef oln_grd_type_of(G, dimvalue) dimvalue_type;
      static const unsigned dim = dimvalue_type::val;

      regular_qiter(const regular_window<G, oln_qit_type_of(E, window)>& win) :
	super_type(win.exact())
      {
      }

      const coord_type nth(unsigned i) const
      {
	precondition(i < dim);
	precondition(this->is_valid());
	return point_type(*this).nth(i);
      }

      friend class abstract::qiter<E>;

    protected:

      int pos_;

      const point_type impl_cast_point() const
      {
	precondition(this->is_valid());
	return this->p_ + this->win_.dp(this->pos_);
      }

    };

  } // end of namespace abstract::oln

} // end of namespace oln


#endif // OLENA_CORE_GEN_REGULAR_QITER_HH
