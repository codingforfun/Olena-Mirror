// Copyright (C) 2001, 2002, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_NITER_HH
# define OLENA_CORE_ABSTRACT_NITER_HH

# include <vector>

# include <mlc/any.hh>
# include <mlc/types.hh>
# include <mlc/contract.hh>

# include <oln/core/gen/image_with_nbh.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/properties.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/neighborhood.hh>
# include <oln/core/abstract/image_neighbness.hh>


# define for_all_n(n) \
  for(n.start(); n.is_valid(); n.next())

# define for_all_remaining_n(n) \
  for(; n.is_valid(); n.next())


// FIXME : bad place
# include <oln/core/abstract/piter.hh>

# define for_all_n_of_p(p, n) \
  for(p.start(); p.is_valid(); p.next()) \
  for(n.center_at(p), n.start(); n.is_valid(); n.next())

namespace oln {

  // fwd decl
  namespace abstract {
    template <typename E> struct niter;
  }

  // category
  template <typename E>
  struct set_category< abstract::niter<E> > { typedef category::niter ret; };


  /// properties of any type in category::niter

  template <typename type>
  struct props_of < category::niter, type >
  {
    typedef mlc::true_type user_defined_;

    mlc_decl_prop(category::niter, point_type);
    mlc_decl_prop(category::niter, dpoint_type);
    mlc_decl_prop(category::niter, neighb_type);
    mlc_decl_prop(category::niter, image_type);

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( category::niter, "
	   << typeid(type).name() << ") = {"
	   << "  neighb_type = " << typeid(neighb_type).name()
	   << "  image_type = " << typeid(image_type).name()
	   << "  point_type = " << typeid(point_type).name() << "  }" << std::endl;
    }

  };

  mlc_register_prop(category::niter, neighb_type);
  mlc_register_prop(category::niter, point_type);
  mlc_register_prop(category::niter, dpoint_type);
  mlc_register_prop(category::niter, image_type);


  namespace abstract {

    template <typename E>
    struct niter : public mlc::any__best_speed<E>
    {

      /// typedefs

      typedef niter<E> self_type;

      typedef oln_type_of(E, point) point_type;
      typedef oln_type_of(E, neighb) neighb_type;
      typedef oln_type_of(E, image) image_type;

      void start()
      {
	this->exact().impl_start();
      }

      void next()
      {
	this->exact().impl_next();
      }

      operator point_type() const
      {
	return this->exact().impl_cast_point();
      }

      void center_at(const point_type& pt)
      {
	this->exact().impl_center_at(pt);
      }

      bool is_valid() const
      {
	return this->exact().impl_is_valid();
      }

      void invalidate()
      {
	return this->exact().impl_invalidate();
      }

    protected:

      template <typename T>
      niter(const abstract::image_with_nbh<T>& ima) :
	p_(), nbh_(ima.nbh_get())
      {
      }

      point_type p_;
      const neighb_type& nbh_;
    };
  }
}


#endif // ! OLENA_CORE_ABSTRACT_NITER_HH
