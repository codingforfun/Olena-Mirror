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

#ifndef OLENA_CORE_ABSTRACT_PITER_HH
# define OLENA_CORE_ABSTRACT_PITER_HH

# include <mlc/any.hh>
# include <mlc/types.hh>
# include <mlc/contract.hh>

# include <oln/core/abstract/point.hh>
# include <oln/core/cats.hh>
# include <oln/core/props.hh>
# include <oln/core/macros.hh>



# define for_all(P) for(p.start(); p.is_valid(); p.next())


namespace oln {

  namespace abstract {

    template <typename E> struct piter;

  }


  template <typename E>
  struct category_type<abstract::piter<E> > { typedef cat::piter ret; };


  template <>
  struct default_props < cat::piter >
  {
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type size_type;
  };


  namespace abstract {

    template <typename E>
    struct piter : public mlc::any__best_speed<E>
    {
      typedef piter<E> self_type;
      typedef oln_point_type(E) point_type;
      typedef oln_size_type(E) size_type;

      void start()
      {
	this->exact().impl_start();
      }

      void next()
      {
	precondition(this->is_valid());
	this->exact().impl_next();
      }

      bool is_valid() const
      {
	return this->exact().impl_is_valid();
      }

      operator point_type() const
      {
	precondition(this->is_valid());
	return this->p_;
      }

      void invalidate()
      {
	this->exact().impl_invalidate();
	postcondition(! this->is_valid());
      }

    protected:

      piter(const size_type& s) :
	s_(s),
	p_()
      {
      }

      const size_type s_;
      point_type p_;

    };
  }
}


#endif // ! OLENA_CORE_ABSTRACT_PITER_HH
