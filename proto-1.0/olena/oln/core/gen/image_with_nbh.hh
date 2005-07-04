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
# include <oln/core/abstract/neighborhood.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/utils/record.hh>


namespace oln {

  // fwd decl
  template <typename I, typename N> struct image_with_nbh;

  // super type
  template <typename I, typename N>
  struct set_super_type < image_with_nbh<I, N> >
  {
    typedef abstract::image_with_extension< I, image_with_nbh<I, N> > ret;
  };

  template <typename I, typename N>
  struct set_props < category::image, image_with_nbh<I, N> >
  {
    typedef is_a<abstract::image_with_nbh> image_neighbness_type;
    typedef N neighb_type;
    typedef image_with_nbh< oln_type_of(I, concrete), N> concrete_type;

    template <typename U>
    struct ch_value_type
    {
      typedef typename oln::ch_value_type<I, U>::ret IU;
      typedef oln::image_with_nbh< IU, N > ret;
    };
  };


  template <typename I, typename N>
  struct image_with_nbh : public abstract::image_with_extension < I, image_with_nbh<I, N> >
  {
    typedef image_with_nbh<I, N> self_type;
    typedef abstract::image_with_extension < I, self_type > super_type;

  public:

    image_with_nbh()
    {
    }

    image_with_nbh(const oln_type_of(I, size)& size)
    {
      I tmp(size); // FIXME: hack
      this->image_ = tmp;
    }

    image_with_nbh(const oln_type_of(I, size)& size,
		   const abstract::neighborhood<N>& nbh) :
      nbh_(nbh.exact())
    {
      I tmp(size); // FIXME: hack
      this->image_ = tmp;
    }

    image_with_nbh(abstract::image<I>& image,
		   const abstract::neighborhood<N>& nbh) :
      super_type(image),
      nbh_(nbh.exact())
    {
    }

    image_with_nbh(const std::string& name)
    {
      registering(*this, name);
    }

    image_with_nbh(const oln_type_of(I, size)& size,
		   const std::string& name)
    {
      // FIXME: illegal hardware instruction when uncommented below!!!
//       I tmp(size, name);
      // FIXME: other attemps below
//       registering(tmp, name, true);
//       tmp.set_name(name);

      I tmp(size); // FIXME: hack
      this->image_ = tmp;
      registering(*this, name); //, true);
    }

    image_with_nbh(abstract::image<I>& image,
		   const abstract::neighborhood<N>& nbh,
		   const std::string& name) :
      super_type(image),
      nbh_(nbh.exact())
    {
      registering(*this, name);
    }
    
    const N& impl_nbh_get() const
    {
      return nbh_;
    }

  protected:

    N nbh_;
  };


  /// the 'image_with_nbh' method for images

  template <typename I, typename N>
  image_with_nbh<I, N>
  join(abstract::image<I>& image, const abstract::neighborhood<N>& nbh)
  {
    entering("join(image,nbh)");
    registering(image, "image");

    image_with_nbh<I, N> tmp(image, nbh, "tmp");

    exiting("join(image,nbh)");
    return tmp;
  }

  // FIXME: add the 'const' version...


} // end of namespace oln


#endif // ! OLENA_CORE_GEN_IMAGE_WITH_NBH_HH
