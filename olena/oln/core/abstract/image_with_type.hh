// Copyright (C) 2001  EPITA Research and Development Laboratory
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


#ifndef OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_HH

# include <oln/core/abstract/image.hh>

namespace oln {
  namespace abstract {
    
    template<class T, class Exact>
    class image_with_type;
  } // end of namespace abstract

    template <class T, class Exact>
    struct image_traits<abstract::image_with_type<T, Exact> >: public image_traits<abstract::image<Exact> >
    {
      typedef T value_type;
    };


  namespace abstract {

    template<class T, class Exact>
    struct type_switch
    {
      typedef abstract::image<Exact> ret;
    };

    template<class T, class Exact>
    class image_with_type: public virtual type_switch<T, Exact>::ret
    {
    public:

      typedef image_with_type<T, Exact> self_type;
      typedef Exact exact_type;

      exact_type& operator=(self_type rhs)
      {
	return to_exact(this)->assign(to_exact(rhs));
      }

      static std::string name()
      {
	return
	  std::string("abstract::image_with_type<")
	  + T::name() + ", "
	  + Exact::name() + ">";
      }

    protected:
      image_with_type() {}
    };

  } // end of namespace abstract
} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_TYPE_HH
