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

# include <mlc/any.hh>
# include <mlc/types.hh>
# include <mlc/contract.hh>

# include <oln/core/abstract/point.hh>
# include <oln/core/properties.hh>



# define for_all(p) \
  for(p.start(); p.is_valid(); p.next())

# define for_all_remaining(p) \
  for(; p.is_valid(); p.next())


namespace oln {

  // fwd decl
  namespace abstract {
    template <typename E> struct piter;
  }

  // category
  template <typename E>
  struct set_category< abstract::piter<E> > { typedef category::piter ret; };


  /// properties of any type in category::piter

  template <typename type>
  struct props_of < category::piter, type >
  {
    typedef mlc::true_type user_defined_;
    
    mlc_decl_prop(category::piter, size_type);
    mlc_decl_prop(category::piter, point_type);

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( category::piter, "
	   << typeid(type).name() << ") = {"
	   << "  size_type = " << typeid(size_type).name()
	   << "  point_type = " << typeid(point_type).name() << "  }" << std::endl;
    }

  };

  mlc_register_prop(category::piter, size_type); 
  mlc_register_prop(category::piter, point_type); 



  namespace abstract {

    template <typename E>
    struct piter : public mlc::any__best_speed<E>
    {

      /// typedefs

      typedef piter<E> self_type;

      typedef oln_type_of(E, size)  size_type;
      typedef oln_type_of(E, point) point_type;


      void start()
      {
	this->exact().impl_start();
      }

      void next()
      {
	precondition(this->is_valid());
	this->exact().impl_next();
      }

      bool is_valid() const
      {
	return this->exact().impl_is_valid();
      }

      operator point_type() const
      {
	precondition(this->is_valid());
	return this->p_;
      }

      void invalidate()
      {
	this->exact().impl_invalidate();
	postcondition(! this->is_valid());
      }

    protected:

      piter(const size_type& s) :
	s_(s),
	p_()
      {
      }

      const size_type s_;
      point_type p_;

    };
  }
}


#endif // ! OLENA_CORE_ABSTRACT_PITER_HH
