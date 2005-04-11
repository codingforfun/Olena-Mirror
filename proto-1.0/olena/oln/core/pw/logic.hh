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

#ifndef OLENA_CORE_PW_LOGIC_HH
# define OLENA_CORE_PW_LOGIC_HH

# include <oln/core/abstract/image_typeness.hh>
# include <oln/core/pw/abstract/function.hh>
# include <oln/core/pw/cmp.hh>


namespace oln {


  namespace pw {

    // FIXME: move somewhere else
    namespace internal
    {
      struct not_ {
	template <typename T>
	bool operator()(const T& rhs) const {
	  return !rhs;
	}
      };
      struct and_ {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return lhs && rhs;
	}
      };
      struct nand_ {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return !(lhs && rhs);
	}
      };
      struct or_ {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return lhs || rhs;
	}
      };
      struct nor_ {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return !(lhs || rhs);
	}
      };
      struct xor_ {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return (lhs && !rhs) || (!lhs && rhs);
	}
      };
      struct xnor_ {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return (lhs && rhs) || (!lhs && !rhs);
	}
      };
    } // end of oln::pw::internal

  } // end of namespace oln::pw


  // fwd decl
  namespace pw {
    template <typename R> struct not_;
  }

  // super type
  template <typename R>
  struct set_super_type < pw::not_<R> > { typedef pw::abstract::function < pw::not_<R> > ret; };

  // props
  template <typename R>
  struct set_props < category::pw, pw::not_<R> >
  {
    typedef oln_pw_type_of(R, point) point_type;
    typedef oln_pw_type_of(R, size)  size_type;
    typedef bool value_type;
  };


  namespace pw
  {

    template <typename R>
    struct not_ : public abstract::function < not_<R> >
    {
      typedef not_<R> self_type;

      typedef oln_pw_type_of(R, point) point_type;
      typedef oln_pw_type_of(R, size)  size_type;
      typedef oln_pw_type_of(R, value) value_type;

      R right;

      not_(const abstract::function<R>& right) :
	right(right.exact())
      {
      }

      const bool impl_get(const point_type& p) const
      {
	return ! this->right(p);
      }

      const size_type& impl_size() const
      {
	return this->right.size();
      }

      bool impl_hold(const point_type& p) const
      {
	return this->right.hold(p);
      }

      bool impl_hold_large(const point_type& p) const
      {
	return this->right.hold_large(p);
      }

    };
    

    // impl of abstract::function<E>::operator!()

    namespace abstract {

      template <typename E>
      not_<E>
      function<E>::operator!() const
      {
	mlc::eq< oln_typeness_of(oln_pw_type_of(E, value)), typeness::binary_tag >::ensure();
	not_<E> tmp(this->exact());
	return tmp;
      }

    } // end of namespace oln::pw::abstract


  } // end of namespace oln::pw


} // end of namespace oln



/// Ops on pwf

oln_pw_decl_cmp_op(and_, &&)
oln_pw_decl_cmp_op(or_,  ||)

oln_pw_cmp_operator(and_, &&, bool)
oln_pw_cmp_operator(or_,  ||, bool)


#endif // ! OLENA_CORE_PW_LOGIC_HH
