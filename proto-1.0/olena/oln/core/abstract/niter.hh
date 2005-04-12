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

# include <mlc/contract.hh>
# include <oln/core/typedefs.hh>
# include <oln/core/abstract/iter.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/neighborhood.hh>


# define for_all_n_of_p(n, p) \
  for(n.ensure_is_niter(), n.center_at(p), n.start(); n.is_valid(); n.next())

# define for_all_remaining_n(n) \
  for(n.ensure_is_niter(); n.is_valid(); n.next())


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
    typedef oln_nit_type_of(N, point)  point_type;
    typedef oln_nit_type_of(N, neighb) neighb_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::niter, " << mlc_to_string(N) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t point_type  = " << mlc_to_string(point_type)  << std::endl
	   << "\t neighb_type = " << mlc_to_string(neighb_type) << std::endl
	   << "}" << std::endl;
    }
  };



  namespace abstract {

    template <typename E>
    struct niter : public iter<E>
    {

      typedef oln_nit_type_of(E, point)  point_type;
      typedef oln_nit_type_of(E, neighb) neighb_type;

      void ensure_is_niter() const
      {
      }

      operator point_type() const
      {
	precondition(this->is_valid());
	return this->exact().impl_cast_point();
      }

      void center_at(const point_type& p)
      {
	this->p_ = p;
      }

    protected:

      niter(const neighb_type& ima) :
	nbh_(nbh),
	p_()
      {
      }

      const neighb_type& nbh_;
      point_type p_;

      ~niter()
      {
	{ // impl_cast_point
	  typedef const point_type (E::*meth)() const;
	  meth adr = &E::impl_cast_point;
	  adr = 0;
	}
      }

    };

  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_NITER_HH
