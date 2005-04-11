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

# include <oln/core/typedefs.hh>

# include <oln/core/gen/image_with_nbh.hh>
# include <oln/core/abstract/point.hh>
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



# define oln_nit_type_of(NiterType, Alias) \
mlc_type_of(oln, oln::category::niter, NiterType, Alias)



namespace oln {


  namespace category
  {
    struct niter;
  }


  template <>
  struct set_default_props < category::niter >
  {
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type neighb_type;
  };


  template <typename N>
  struct get_props < category::niter, N >
  {
    typedef oln_nit_type_of(N, point) point_type;
    typedef oln_nit_type_of(N, neighb) neighb_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::niter, " << mlc_to_string(N) << " ) =" << std::endl
	   << "\t point_type  = " << mlc_to_string(point_type) << std::endl
	   << "\t neighb_type = " << mlc_to_string(neighb_type) << std::endl
	   << std::endl;
    }
  };



  namespace abstract {

    template <typename E>
    struct niter : public mlc::any<E>
    {

      /// typedefs

      typedef niter<E> self_type;

      typedef oln_nit_type_of(E, point) point_type;
      typedef oln_nit_type_of(E, neighb) neighb_type;
      typedef oln_nit_type_of(E, image) image_type;

      void start()
      {
	this->exact().impl_start();
      }

      void next()
      {
	precondition(this->is_valid());
	this->exact().impl_next();
      }

      operator point_type() const
      {
	precondition(this->is_valid());
	return this->exact().impl_cast_point();
      }

      void center_at(const point_type& p)
      {
	this->exact().impl_center_at(p);
      }

      bool is_valid() const
      {
	return this->exact().impl_is_valid();
      }

      void invalidate()
      {
	this->exact().impl_invalidate();
      }

    protected:

      template <typename I>
      niter(const abstract::image_with_nbh<I>& ima) :
	nbh_(ima.nbh_get())
      {
      }

      const neighb_type& nbh_;
      point_type p_;
    };

  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_NITER_HH
