// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_ITER1D_HH
# define OLENA_CORE_ABSTRACT_ITER1D_HH

# include <oln/core/coord.hh>
# include <oln/core/point1d.hh>
# include <oln/core/dpoint1d.hh>
# include <oln/core/image1d.hh>
# include <oln/core/abstract/iter.hh>

# include <mlc/contract.hh>
# include <mlc/type.hh>
# include <mlc/objs.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    class iter1d; // fwd_decl
  } // end of abstract

  template<class Exact>
  struct iter_traits<abstract::iter1d<Exact> >: public virtual
  iter_traits<abstract::iter<Exact> >
  {
    enum { dim = 1 };
    typedef point1d point_type;
    typedef dpoint1d dpoint_type;
  };

  namespace abstract {

    template<class Exact>
    class iter1d : public iter< Exact >
    {
    public:

      typedef iter<Exact> super_type;

      point1d to_point() const
      {
	precondition(*this != end);
	invariant(p_.col() >= 0 &&
		  p_.col() < ncols_);
	return p_;
      }

      coord col() const
      {
	return p_.col();
      }

      static std::string name() { return std::string("_iter1d<") + Exact::name() + ">"; }

    protected:
      const coord ncols_;

      iter1d() : super_type(), ncols_(0) {}

      iter1d(const image1d_size& size) :
	super_type(), ncols_(size.ncols())
      {
	precondition(size.ncols() > 0);
	to_exact(this)->goto_begin_();
      }
    };


  } // end of abstract

} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_ITER1D_HH
