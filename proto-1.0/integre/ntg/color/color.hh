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

#ifndef INTEGRE_COLOR_COLOR_HH
# define INTEGRE_COLOR_COLOR_HH

# include <mlc/any.hh>
# include <mlc/types.hh>

# include <ntg/core/cats.hh>
# include <ntg/core/props.hh>

namespace ntg {

  template <typename E>
  struct color;

  template <typename E>
  struct category_type< color<E> > { typedef cat::color ret; };


  template <>
  struct default_props < cat::color >
  {
    enum { max_val = 0 };
    enum { nb_comp = 0 };
    typedef mlc::undefined_type comp_type;
    typedef mlc::undefined_type io_type;


  protected:
    default_props() {}
  };

  template <typename E>
  struct color : public mlc::any__best_memory<E>
  {
    typedef E exact_type;
    typedef typename props<cat::color, E>::comp_type comp_type;

    E& operator=(const exact_type& rhs)
    {
      return this->exact.impl_assign(rhs);
    }

    bool operator==(const exact_type& rhs) const
    {
      return this->exact().impl_eq(rhs);
    }

    bool operator!=(const exact_type& rhs) const
    {
      return this->exact().impl_not_eq(rhs);
    }

    comp_type& operator [](const unsigned int &i)
    {
      return this->exact().impl_op_sqbr(i);
    }

    const comp_type operator [](const unsigned int &i) const
    {
      return this->exact().impl_op_sqbr(i);
    }

  protected:
    color() {}
  };

} // end of namespace ntg


#endif // ! INTEGRE_COLOR_COLOR_HH
