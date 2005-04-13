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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_NEIGHBNESS_HH
# define OLENA_CORE_ABSTRACT_IMAGE_NEIGHBNESS_HH

# include <oln/core/abstract/image.hh>

/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {


    template <typename E>
    struct image_with_nbh : public virtual image<E>
    {
    public:

      typedef oln_type_of(E, neighb) neighb_type;

      const neighb_type& nbh_get() const // FIXME: rename (?)
      {
	return this->exact().impl_nbh_get();
      }

    protected:

      image_with_nbh() {}

      ~image_with_nbh()
      {
// 	mlc_check_method_impl(E, const neighb_type&, nbh_get, , const);
      }
    };


    template <typename E>
    struct image_without_nbh : public virtual image<E>
    {
    protected:
      image_without_nbh() {}
    };


  } // end of namespace oln::abstract

} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_NEIGHBNESS_HH
