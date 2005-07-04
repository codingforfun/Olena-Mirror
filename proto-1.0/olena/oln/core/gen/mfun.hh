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

#ifndef OLENA_CORE_GEN_MFUN_HH
# define OLENA_CORE_GEN_MFUN_HH

# include <oln/core/abstract/image_by_delegation.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/funobj/abstract/unary.hh>
# include <oln/utils/record.hh>



namespace oln {

  // fwd decl
  template <typename F, typename I> struct mfun;

  // super type
  template <typename F, typename I>
  struct set_super_type < mfun<F, I> >
  {
    typedef abstract::image_by_delegation< const I, mfun<F, I> > ret;
  };

  // set props
  template <typename F, typename I>
  struct set_props < category::image, mfun<F, I> >
  {
    typedef is_a<abstract::not_raw_image>  image_rawness_type;

    typedef oln_type_of(I, value) I_value_type;
    typedef typename f_::munary_result<F, I_value_type>::ret value_type; 
    typedef oln_ch_concrete_type(I, value_type) concrete_type;

    typedef mlc::no_type value_storage_type;
    typedef mlc::no_type storage_type; // FIXME: senseless?
  };


  template <typename F, typename I>
  struct mfun : public abstract::image_by_delegation < const I, mfun<F, I> >
  {
    typedef mfun<F, I> self_type;
    typedef abstract::image_by_delegation < const I, self_type > super_type;

    typedef oln_type_of(self_type, point) point_type;
    typedef oln_type_of(self_type, value) value_type;

  public:

    mfun()
    {
    }

    mfun(box<const I> image) :
      super_type(image)
    {
    }

    mfun(box<const I> image, F f) :
      super_type(image),
      f(f)
    {
    }

    mfun(box<const I> image, const std::string& name) :
      super_type(image)
    {
      registering(*this, name);
    }

    mfun(box<const I> image, F f, const std::string& name) :
      super_type(image),
      f(f)
    {
      registering(*this, name);
    }

    // get

    const value_type impl_get(const point_type& p) const
    {
      return f(this->delegate().get(p));
    }

    // FIXME: what about the methods "call(*)"?

  protected:

    F f;
  };


} // end of namespace oln


#endif // ! OLENA_CORE_GEN_MFUN_HH
