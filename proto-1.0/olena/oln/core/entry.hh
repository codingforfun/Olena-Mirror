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

#ifndef OLENA_CORE_ENTRY_HH
# define OLENA_CORE_ENTRY_HH

# include <oln/core/abstract/image_constness.hh>
# include <oln/core/abstract/image_dimension.hh>


namespace oln {

  template <template <typename> class Base>
  struct prop_of
  {
    template <typename E>
    struct inherits
    {
      typedef Base<E> ret;
    };
  };

  namespace abstract {

    template <typename E>
    struct image_entry : 
      public props < cat::image, E >::image_constness
        ::template inherits<E>::ret,
      public props < cat::image, E >::image_dimension
        ::template inherits<E>::ret
      // ...
    {
    protected:
      image_entry() {}
    };

  }

} // end of namespace oln


#endif // ! OLENA_CORE_ENTRY_HH
