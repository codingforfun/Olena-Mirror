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

#ifndef OLENA_CORE_PW_ABSTRACT_UNARY_FUNCTION_HH
# define OLENA_CORE_PW_ABSTRACT_UNARY_FUNCTION_HH

# include <oln/core/pw/abstract/function.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/size.hh>



namespace oln {


  // fwd decl
  namespace pw {
    namespace abstract {
      template <typename T, typename E> struct unary_function;
    }
  }

  // super type
  template <typename T, typename E>
  struct set_super_type < pw::abstract::unary_function<T, E> > { typedef pw::abstract::function<E> ret; };

  // props
  template <typename T, typename E>
  struct set_props < category::pw, pw::abstract::unary_function<T, E> >
  {
    typedef oln_pw_type_of(T, point) point_type;
    typedef oln_pw_type_of(T, size) size_type;
  };


  namespace pw {

    namespace abstract {

      template <typename T, typename E>
      struct unary_function : public function<E>
      {
	typedef T input_type;

	T input;

	unary_function(const abstract::function<T>& input) :
	  input(input.exact())
	{
	}

	typedef oln_pw_type_of(E, point) point_type;
	typedef oln_pw_type_of(E, size)  size_type;

	const size_type& impl_size() const
	{
	  return input.size();
	}

	bool impl_hold(const point_type& p) const
	{
	  return input.hold(p);
	}

	bool impl_hold_large(const point_type& p) const
	{
	  return input.hold_large(p);
	}

      protected:
	unary_function() {}

      };


    } // end of namespace oln::pw::abstract

  } // end of namespace oln::pw

} // end of namespace oln



#endif // ! OLENA_CORE_PW_ABSTRACT_UNARY_FUNCTION_HH
