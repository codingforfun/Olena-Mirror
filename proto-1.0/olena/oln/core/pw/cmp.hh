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

#ifndef OLENA_CORE_PW_CMP_HH
# define OLENA_CORE_PW_CMP_HH

# include <oln/core/pw/abstract/binary_function.hh>
# include <oln/core/pw/macros.hh>


namespace oln {


  namespace pw {


    // FIXME: move somewhere else
    namespace internal
    {
      struct eq {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return lhs == rhs;
	}
      };
      struct neq {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return lhs != rhs;
	}
      };
      struct geq {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return lhs >= rhs;
	}
      };
      struct leq {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return lhs <= rhs;
	}
      };
      struct g {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return lhs > rhs;
	}
      };
      struct l {
	template <typename L, typename R>
	bool operator()(const L& lhs, const R& rhs) const {
	  return lhs < rhs;
	}
      };
    } // end of oln::pw::internal



  } // end of namespace pw


  // fwd decl
  namespace pw {
    template <typename L, typename R, typename C> struct cmp;
  }

  // super type
  template <typename L, typename R, typename C>
  struct set_super_type < pw::cmp<L, R, C> > { typedef pw::abstract::binary_function<L, R, pw::cmp<L, R, C> > ret; };

  // props
  template <typename L, typename R, typename C>
  struct set_props < category::pw, pw::cmp<L, R, C> >
  {
    typedef bool value_type;
  };



  
  namespace pw
  {

    template <typename L, typename R, typename C>
    struct cmp : public abstract::binary_function < L, R, cmp<L, R, C> >
    {
      typedef cmp<L, R, C> self_type;

      typedef oln_pw_type_of(self_type, point) point_type;
      typedef oln_pw_type_of(self_type, value) value_type;
      typedef oln_pw_type_of(self_type, size)  size_type;

      typedef abstract::binary_function<L, R, self_type> super_type;

      cmp(const abstract::function<L>& left,
	  const abstract::function<R>& right) :
	super_type(left, right)
      {
      }

      const bool impl_get(const point_type& p) const
      {
	static const C cmpfun = C();
	return cmpfun(this->left(p), this->right(p));
      }

    };

  } // end of namespace oln::pw


} // end of namespace oln



/// Ops on pwf

# define oln_pw_decl_cmp_op(NAME, SYMBOL)			\
template <typename L, typename R>				\
oln::pw::cmp<L, R, oln::pw::internal:: NAME >			\
operator SYMBOL (const oln::pw::abstract::function<L>& lhs,	\
		 const oln::pw::abstract::function<R>& rhs)	\
{								\
  precondition(lhs.size() == rhs.size());			\
  oln::pw::cmp<L, R, oln::pw::internal:: NAME > tmp(lhs, rhs);	\
  return tmp;							\
}

oln_pw_decl_cmp_op(eq,  ==)
oln_pw_decl_cmp_op(neq, !=)
oln_pw_decl_cmp_op(geq, >=)
oln_pw_decl_cmp_op(leq, <=)
oln_pw_decl_cmp_op(g, >)
oln_pw_decl_cmp_op(l, <)

oln_pw_cmp_operators(int)
oln_pw_cmp_operators(float)
oln_pw_cmp_operators(double)



#endif // ! OLENA_CORE_PW_CMP_HH
