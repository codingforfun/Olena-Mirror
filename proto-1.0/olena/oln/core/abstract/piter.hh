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

#ifndef OLENA_CORE_ABSTRACT_PITER_HH
# define OLENA_CORE_ABSTRACT_PITER_HH

# include <mlc/types.hh>
# include <mlc/contract.hh>

# include <oln/core/abstract/iter.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/typedefs.hh>



# define for_all_p( p ) \
  for(p.ensure_is_piter(), p.start(); p.is_valid(); p.next())

# define for_all_remaining_p( p ) \
  for(p.ensure_is_piter(); p.is_valid(); p.next())



# define oln_pit_type_of(PiterType, Alias) \
mlc_type_of(oln, oln::category::piter, PiterType, Alias)



namespace oln {


  namespace category
  {
    struct piter;
  }


  template <>
  struct set_default_props < category::piter >
  {
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type size_type;
  };


  template <typename P>
  struct get_props < category::piter, P >
  {
    typedef oln_pit_type_of(P, point) point_type;
    typedef oln_pit_type_of(P, size)  size_type;

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::piter, " << mlc_to_string(P) << " ) =" << std::endl
	   << "{" << std::endl
	   << "\t point_type = " << mlc_to_string(point_type) << std::endl
	   << "\t size_type  = " << mlc_to_string(size_type)  << std::endl
	   << "}" << std::endl;
    }

    static void ensure()
    {
      mlc::is_ok< point_type >::ensure();
      mlc::is_ok< size_type >::ensure();
    }
  };


  namespace abstract {

    template <typename E>
    struct piter : public iter<E>
    {

      /// typedefs

      typedef piter<E> self_type;

      typedef oln_pit_type_of(E, size)  size_type;
      typedef oln_pit_type_of(E, point) point_type;
      
      void ensure_is_piter() {}

      operator point_type() const
      {
	precondition(this->is_valid());
	return this->p_;
      }

      void start()
      {
	this->exact().impl_start();
      }

    protected:

      piter(const size_type& s) :
	s_(s),
	p_()
      {
      }

      const size_type s_;
      point_type p_;

      ~piter()
      {
	get_props<category::piter, E>::ensure();
	mlc_check_method_impl(E, void, start, , );
      }
    };

  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_PITER_HH
