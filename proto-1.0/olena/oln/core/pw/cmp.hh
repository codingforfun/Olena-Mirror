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

# include <oln/core/pw/abstract/function.hh>


namespace oln {


  namespace pw { // means "point-wise"


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
    }


    // fwd decl
    template <typename L, typename R, typename C> struct cmp;

    template <typename L, typename R, typename C>
    struct traits < cmp<L, R, C> >
    {
      typedef oln_pw_point_type(L) point_type;
      typedef bool value_type;
      typedef oln_pw_size_type(L)  size_type;
    };

    template <typename L, typename R, typename C>
    struct cmp : public abstract::function < cmp<L, R, C> >
    {
      typedef cmp<L, R, C> self_type;

      typedef oln_pw_point_type(self_type) point_type;
      typedef oln_pw_size_type(self_type)  size_type;

      L left;
      R right;

      cmp(const abstract::function<L>& left,
	  const abstract::function<R>& right) :
	left(left.exact()),
	right(right.exact())
      {
      }

      const size_type& impl_size() const
      {
	return this->left.size();
      }

      const bool impl_get(const point_type& p) const
      {
	static const C cmpfun = C();
	return cmpfun(this->left(p), this->right(p));
      }

      bool impl_hold(const point_type& p) const
      {
	return this->left.hold(p);
      }

      bool impl_hold_large(const point_type& p) const
      {
	return this->left.hold_large(p);
      }

    };

  } // end of namespace oln::pw


} // end of namespace oln



/// Op+ on pwf

template <typename L, typename R>
oln::pw::cmp<L, R, oln::pw::internal::eq>
operator == (const oln::pw::abstract::function<L>& lhs,
	     const oln::pw::abstract::function<R>& rhs)
{
  precondition(lhs.size() == rhs.size());
  oln::pw::cmp<L, R, oln::pw::internal::eq> tmp(lhs, rhs);
  return tmp;
}

template <typename L, typename R>
oln::pw::cmp<L, R, oln::pw::internal::neq>
operator != (const oln::pw::abstract::function<L>& lhs,
	     const oln::pw::abstract::function<R>& rhs)
{
  precondition(lhs.size() == rhs.size());
  oln::pw::cmp<L, R, oln::pw::internal::neq> tmp(lhs, rhs);
  return tmp;
}

template <typename L, typename R>
oln::pw::cmp<L, R, oln::pw::internal::geq>
operator >= (const oln::pw::abstract::function<L>& lhs,
	     const oln::pw::abstract::function<R>& rhs)
{
  precondition(lhs.size() == rhs.size());
  oln::pw::cmp<L, R, oln::pw::internal::geq> tmp(lhs, rhs);
  return tmp;
}

template <typename L, typename R>
oln::pw::cmp<L, R, oln::pw::internal::leq>
operator <= (const oln::pw::abstract::function<L>& lhs,
	     const oln::pw::abstract::function<R>& rhs)
{
  precondition(lhs.size() == rhs.size());
  oln::pw::cmp<L, R, oln::pw::internal::leq> tmp(lhs, rhs);
  return tmp;
}

template <typename L, typename R>
oln::pw::cmp<L, R, oln::pw::internal::g>
operator > (const oln::pw::abstract::function<L>& lhs,
	    const oln::pw::abstract::function<R>& rhs)
{
  precondition(lhs.size() == rhs.size());
  oln::pw::cmp<L, R, oln::pw::internal::g> tmp(lhs, rhs);
  return tmp;
}

template <typename L, typename R>
oln::pw::cmp<L, R, oln::pw::internal::l>
operator < (const oln::pw::abstract::function<L>& lhs,
	    const oln::pw::abstract::function<R>& rhs)
{
  precondition(lhs.size() == rhs.size());
  oln::pw::cmp<L, R, oln::pw::internal::l> tmp(lhs, rhs);
  return tmp;
}



#endif // ! OLENA_CORE_PW_CMP_HH
