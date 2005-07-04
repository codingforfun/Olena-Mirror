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

#ifndef OLENA_CORE_GEN_ATTR_HH
# define OLENA_CORE_GEN_ATTR_HH

# include <oln/core/abstract/image_by_delegation.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/utils/record.hh>


namespace oln {

  // fwd decl
  template <typename A, typename I> struct attr;

  // super type
  template <typename A, typename I>
  struct set_super_type < attr<A, I> >
  {
    typedef abstract::image_by_delegation< I, attr<A, I> > ret;
  };

  // set props
  template <typename A, typename I>
  struct set_props < category::image, attr<A, I> >
  {
    typedef oln_type_of(I, value_storage) I_value_storage;

    typedef typename A::template type<I_value_storage>::ret value_type; 
    typedef value_type                          value_storage_type;
    typedef oln_ch_concrete_type(I, value_type) concrete_type;

    typedef mlc::no_type storage_type; // FIXME: senseless?
  };


  template <typename A, typename I>
  struct attr : public abstract::image_by_delegation < I, attr<A, I> >
  {
    typedef attr<A, I> self_type;
    typedef abstract::image_by_delegation < I, self_type > super_type;

    typedef oln_type_of(self_type, point) point_type;
    typedef oln_type_of(self_type, value) value_type;
    typedef oln_type_of(self_type, value_storage) value_storage_type;

  public:

    attr()
    {
    }

    attr(box<I> image) :
      super_type(image)
    {
      mlc_is_a(I, abstract::raw_image)::ensure();
    }

    attr(box<I> image, const std::string& name) :
      super_type(image)
    {
      registering(*this, name);
    }

    // .at(p)

    value_storage_type& impl_at(const point_type& p)
    {
      return a_of(this->delegate().at(p));
    }

    const value_storage_type& impl_at(const point_type& p) const
    {
      return a_of(this->delegate().at(p));
    }

    // get/set

    void impl_set(const point_type& p, const value_type& v)
    {
      a_of(this->delegate().at(p)) = v;
    }

    const value_type impl_get(const point_type& p) const
    {
      return a_of(this->delegate().get(p));
    }

    // FIXME: what about the methods "call(*)"?

  protected:

    A a_of;
  };


} // end of namespace oln


#endif // ! OLENA_CORE_GEN_ATTR_HH
