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

#ifndef OLENA_CORE_PW_ABSTRACT_BINARY_FUNCTION_HH
# define OLENA_CORE_PW_ABSTRACT_BINARY_FUNCTION_HH

# include <oln/core/pw/abstract/function.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/size.hh>



namespace oln {


  // fwd decl
  namespace pw {
    namespace abstract {
      template <typename L, typename R, typename E> struct binary_function;
    }
  }

  // super type
  template <typename L, typename R, typename E>
  struct set_super_type < pw::abstract::binary_function<L, R, E> > { typedef pw::abstract::function<E> ret; };

  // props
  template <typename L, typename R, typename E>
  struct set_props < category::pw, pw::abstract::binary_function<L, R, E> >
  {
    typedef oln_pw_type_of(L, point) left_point_type;
    typedef oln_pw_type_of(R, point) right_point_type;
    
    typedef oln_point_type_from_2(left_point_type, right_point_type) point_type;
    
    typedef oln_pw_type_of(L, size) left_size_type;
    typedef oln_pw_type_of(R, size) right_size_type;
    
    typedef oln_size_type_from_2(left_size_type, right_size_type) size_type;
  };



  namespace pw {

    namespace abstract {


      // internals
      namespace internal {

	template <typename T1, typename T2>
	struct binary_function_helper;

	template <typename T>
	struct binary_function_helper < T, T > {
	  template <typename L, typename R>
	  static const L& select(const L& l, R) {
	    return l;
	  }
	};

	template <typename S>
	struct binary_function_helper < S, any_size > {
	  template <typename L, typename R>
	  static const L& select(const L& l, R) {
	    return l;
	  }
	};

	template <typename S>
	struct binary_function_helper < any_size, S > {
	  template <typename L, typename R>
	  static const R& select(L, const R& r) {
	    return r;
	  }
	};

      } // end of namespace oln::pw::abstract::internal




      template <typename L, typename R, typename E>
      struct binary_function : public function<E>
      {
	typedef L left_type;
	typedef R right_type;

	L left;
	R right;

	binary_function(const abstract::function<L>& left,
			const abstract::function<R>& right) :
	  left(left.exact()),
	  right(right.exact())
	{
	}

	typedef abstract::binary_function<L, R, E> self_type;
	typedef oln_pw_type_of(self_type, point) point_type;
	typedef oln_pw_type_of(self_type, size)  size_type;

	typedef internal::binary_function_helper<oln_pw_type_of(L, size),
						 oln_pw_type_of(R, size) > _helper_type;

	const size_type& impl_size() const
	{
	  return _helper_type::select(this->left, this->right).size();
	}

	bool impl_hold(const point_type& p) const
	{
	  return _helper_type::select(this->left, this->right).hold(p);
	}

	bool impl_hold_large(const point_type& p) const
	{
	  return _helper_type::select(this->left, this->right).hold_large(p);
	}

      protected:
	binary_function() {}

      };


    } // end of namespace oln::pw::abstract

  } // end of namespace oln::pw

} // end of namespace oln



#endif // ! OLENA_CORE_PW_ABSTRACT_BINARY_FUNCTION_HH
