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

#ifndef OLENA_CORE_GEN_IMAGE_WITH_NBH_HH
# define OLENA_CORE_GEN_IMAGE_WITH_NBH_HH

# include <oln/core/abstract/image_with_extension.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/neighborhood.hh>
# include <oln/core/properties.hh>

namespace oln {

  // fwd decls

  namespace internal {
    template <typename I, typename N> struct image_with_nbh;
  }

  // category

  template <typename I, typename N>
  struct set_category< internal::image_with_nbh<I, N> > {
    typedef category::image ret;
  };

  // super type

  template <typename I, typename N>
  struct set_super_type < internal::image_with_nbh<I, N> >
  {
    typedef abstract::image_with_extension_ < I, internal::image_with_nbh<I, N> > ret;
  };

  template <typename I, typename N>
  struct set_props < category::image, internal::image_with_nbh<I, N> > : public props_of<category::image>
  {
    typedef is_a<abstract::image_with_nbh> image_neighbness_type;
    typedef N neighb_type;
  };

  namespace internal
  {

    template <typename I, typename N>
    struct image_with_nbh : public oln::abstract::image_with_extension_ < I, image_with_nbh<I, N> >
    {
      typedef image_with_nbh<I, N> self_type;
      typedef oln::abstract::image_with_extension_ < I, self_type > super_type;

    public:

      image_with_nbh(abstract::image<I>& rhs,
		     abstract::neighborhood<N>& nbh) :
	super_type(rhs), nbh_(nbh.exact())
      {
      }

      const N& impl_nbh_get() const
      {
	return nbh_;
      }

    protected:
      N& nbh_;
    };

  } // end of namespace oln::internal



  /// the 'image_with_nbh' method for images

  template <typename I, typename N>
  internal::image_with_nbh<I, N>
  join(abstract::image<I>& ima,
       abstract::neighborhood<N>& nbh)
  {
    internal::image_with_nbh<I, N> tmp(ima, nbh);
    return tmp;
  }

} // end of namespace oln


#endif // ! OLENA_CORE_GEN_IMAGE_WITH_NBH_HH
