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

# include <mlc/any.hh>
# include <mlc/cmp.hh>
# include <oln/core/box.hh>
# include <oln/core/abstract/entry.hh>
# include <oln/core/pw/abstract/function.hh>

// FIXME: remove
# include <oln/core/2d/fwd_piter2d.hh>
# include <oln/core/2d/bkd_piter2d.hh>


namespace oln {


  namespace pw { // means "point-wise"

    // fwd decl
    template <typename I> struct image;
    
    template <typename I>
    struct traits < image<I> >
    {
      typedef oln_type_of(I, point) point_type;
      typedef oln_type_of(I, value) value_type;
      typedef oln_type_of(I, size)  size_type;
    };

    template <typename I>
    struct image : public abstract::function < image<I> >
    {
      oln::box<const I> ima;

      image(const oln::abstract::image<I>& ima) :
	ima(ima)
      {
      }

      typedef oln_type_of(I, point) point_type;
      typedef oln_type_of(I, value) value_type;
      typedef oln_type_of(I, size)  size_type;

      const size_type& impl_size() const
      {
	return this->ima.size();
      }

      const value_type impl_get(const point_type& p) const
      {
	return this->ima.get(p);
      }

      bool impl_hold(const point_type& p) const
      {
	return this->ima.hold(p);
      }

      bool impl_hold_large(const point_type& p) const
      {
	return this->ima.hold_large(p);
      }

    };

  } // end of namespace oln::pw


  /// Routine that takes an image and outputs a "point value" object

  template <typename I>
  pw::image<I> p_value(const abstract::image<I>& ima)
  {
    pw::image<I> tmp(ima);
    return tmp;
  }


  // fwd decl
  template <typename F> class image_from_pw;
  
  // category
  template <typename F>
  struct set_category< image_from_pw<F> > { typedef category::image ret; };

  // super
  template <typename F>
  struct set_super_type < image_from_pw<F> >
  {
    typedef abstract::image_entry< image_from_pw<F> > ret;
  };

  // props
  template <typename F>
  struct set_props < category::image, image_from_pw<F> > : public props_of <category::image>
  {
    typedef oln_pw_point_type(F) point_type;
    typedef oln_pw_value_type(F) value_type;
    typedef oln_pw_size_type(F)  size_type;

    typedef mlc::no_type delegated_type;

    // FIXME: we do not know if it is 2d...
    typedef is_a<abstract::image2d> image_dimension_type;
    typedef vectorialness_from_valuetype(value_type) image_vectorialness_type;

    typedef fwd_piter2d piter_type;
    typedef fwd_piter2d fwd_piter_type;
    typedef bkd_piter2d bkd_piter_type;
  };


  /// Class image_from_pw<F>.

  template <typename F>
  struct image_from_pw : public abstract::image_entry< image_from_pw<F> >
  {
    typedef image_from_pw<F> self_type;

    F fun;

    image_from_pw(const pw::abstract::function<F>& fun) :
      fun(fun.exact())
    {
      this->exact_ptr = this;
    }

    image_from_pw(const self_type& rhs) :
      fun(rhs.fun)
    {
      this->exact_ptr = this;
    }

    /// typedefs

    typedef image_from_pw<F> self_type;

    typedef oln_type_of(self_type, size)  size_type;
    typedef oln_type_of(self_type, value) value_type;
    typedef oln_type_of(self_type, point) point_type;

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


  /// Routine

  template <typename F>
  image_from_pw<F> for_all_p(const pw::abstract::function<F>& fun)
  {
    image_from_pw<F> tmp(fun);
    return tmp;
  }


  /// Specialization of p_value (so that "p_value(for_all_p(fun)) == fun").

  template <typename F>
  F p_value(const image_from_pw<F>& ima)
  {
    return ima.fun;
  }




  /// Routine check.
  // FIXME: this should be an algorithm (pred = binary_image)

  template <typename I>
  bool check(const abstract::image<I>& pred)
  {
    // FIXME: input should be binary
    oln_type_of(I, fwd_piter) p(pred.size());
    for_all (p)
      if (! pred[p])
	return false;
    return true;
  }



  namespace pw {

    /// Routine oln::pw::check.

    template <typename F>
    bool check(const pw::abstract::function<F>& pred)
    {
      // FIXME: ugly syntax
      // FIXME: bool or bin or...
      mlc::is_true<
	mlc::type::eq< oln_pw_value_type(F), bool >::ret
	>::ensure();

      return oln::check(for_all_p(pred));
    }

  } // end of namespace oln::pw


} // end of namespace oln



#endif // ! OLENA_CORE_PW_IMAGE_HH
