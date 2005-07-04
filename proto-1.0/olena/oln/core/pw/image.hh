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

#ifndef OLENA_CORE_PW_IMAGE_HH
# define OLENA_CORE_PW_IMAGE_HH

# include <oln/core/abstract/image_entry.hh>
# include <oln/core/pw/abstract/function.hh>
# include <oln/core/pw/value.hh>

// FIXME: remove
# include <oln/core/2d/grid2d.hh>
# include <oln/core/2d/image2d.hh>
# include <oln/core/2d/fwd_piter2d.hh>
# include <oln/core/2d/bkd_piter2d.hh>


namespace oln {


  // fwd decl
  template <typename F> class image_from_pwf;

  // super
  template <typename F>
  struct set_super_type < image_from_pwf<F> > { typedef abstract::image_entry< image_from_pwf<F> > ret; };

  // props
  template <typename F>
  struct set_props < category::image, image_from_pwf<F> >
  {
    typedef oln_pw_type_of(F, point) point_type;
    typedef oln_pw_type_of(F, value) value_type;
    typedef oln_pw_type_of(F, size)  size_type;

    typedef mlc::no_type delegated_type;

    // FIXME: EXTREMELY AWFUL!!! we do not know if it is 2d...
    typedef is_a<abstract::image2d> image_dimension_type;
    typedef is_a<abstract::readonly_image> image_constness_type;
    typedef is_a<abstract::not_raw_image>  image_rawness_type;

    typedef grid2d      grid_type;
    typedef fwd_piter2d piter_type;
    typedef fwd_piter2d fwd_piter_type;
    typedef bkd_piter2d bkd_piter_type;
    typedef image2d<value_type> concrete_type;

    // FIXME: Is this ``cast'' right?
    template <typename U>
    struct ch_value_type
    {
      typedef image2d<U> ret;
    };
  };


  /// Class image_from_pwf<F>.

  template <typename F>
  struct image_from_pwf : public abstract::image_entry< image_from_pwf<F> >
  {
    typedef image_from_pwf<F> self_type;

    F fun;

    image_from_pwf(const pw::abstract::function<F>& fun) :
      fun(fun.exact())
    {
      this->exact_ptr = this;
    }

    image_from_pwf(const self_type& rhs) :
      fun(rhs.fun)
    {
      this->exact_ptr = this;
    }

    typedef oln_pw_type_of(F, point) point_type;
    typedef oln_pw_type_of(F, value) value_type;
    typedef oln_pw_type_of(F, size)  size_type;

    const size_type& impl_size() const
    {
      return this->fun.size();
    }

    bool impl_hold(const point_type& p) const
    {
      return this->fun.hold(p);
    }

    bool impl_hold_large(const point_type& p) const
    {
      return this->fun.hold_large(p);
    }

    const value_type impl_get(const point_type& p) const
    {
      return this->fun(p);
    }

    // FIXME: remove below
    unsigned long impl_npoints() const
    {
      return 1;
    }
    void impl_resize_border(size_t new_border, bool copy_border) const
    {
    }

  };



  /// Routine image_for_all_p.

  template <typename F>
  image_from_pwf<F> image_for_all_p(const pw::abstract::function<F>& fun)
  {
    image_from_pwf<F> tmp(fun);
    return tmp;
  }


  /// Specialization of image_for_all_p (so that "image_for_all_p(pw_value(ima)) == ima").

  template <typename I>
  const I& image_for_all_p(const pw::value<I>& pwv)
  {
    return pwv.ima.unbox();
  }


  /// Specializations of image_for_all_p to produce compile-time errors.
  // FIXME: struct OLENA_ERROR__arg_of__image_for_all_p__should_be_a_point_wise_function();

  template <typename I>
  void image_for_all_p(const abstract::image<I>&)
  {}

  template <typename P>
  void image_for_all_p(const abstract::point<P>&)
  {}


  /// Specialization of pw_value (so that "pw_value(image_for_all_p(fun)) == fun").

  template <typename F>
  F pw_value(const image_from_pwf<F>& ima)
  {
    F tmp = ima.fun;
    return tmp;
  }


} // end of namespace oln



#endif // ! OLENA_CORE_PW_IMAGE_HH
