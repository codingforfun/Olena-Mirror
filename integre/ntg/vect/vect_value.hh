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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef NTG_VECT_VECT_VALUE_HH
# define NTG_VECT_VECT_VALUE_HH

# include <ntg/core/value.hh>

namespace ntg
{

  /*-----------.
  | vect_value |
  `-----------*/

  template <class E>
  class vect_value : public value<E>
  {
    typedef typename internal::typetraits<E>::comp_type comp_type;
    enum { nb_comp = internal::typetraits<E>::nb_comp };

  public:
    // accessor
    comp_type& operator[](unsigned i)	    { return this->val_[i]; }
    comp_type  operator[](unsigned i) const { return this->val_[i]; }

    static unsigned size() { return nb_comp; }
  };

  namespace internal {

    template <class E>
    class typetraits<vect_value<E> > : public typetraits<value<E> >
    {};

    template <class E>
    class optraits<vect_value<E> > : public optraits<value<E> >
    {};

  }

}

#endif // !NTG_VECT_VECT_VALUE_HH
