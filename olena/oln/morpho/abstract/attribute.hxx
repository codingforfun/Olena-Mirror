// Copyright (C) 2004 EPITA Research and Development Laboratory
//
// This  file is  part of  the Olena  Library.  This  library  is free
// software; you can redistribute it  and/or modify it under the terms
// of the  GNU General  Public License version  2 as published  by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but
// WITHOUT  ANY  WARRANTY;  without   even  the  implied  warranty  of
// MERCHANTABILITY or  FITNESS FOR A PARTICULAR PURPOSE.   See the GNU
// General Public License for more details.
//
// You should have  received a copy of the  GNU General Public License
// along with  this library; see the  file COPYING.  If  not, write to
// the Free Software Foundation, 59  Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a  special exception, you  may use this  file as part of  a free
// software library without restriction.  Specifically, if other files
// instantiate templates  or use macros or inline  functions from this
// file, or  you compile  this file  and link it  with other  files to
// produce  an executable,  this file  does  not by  itself cause  the
// resulting  executable  to be  covered  by  the  GNU General  Public
// License.   This exception  does  not however  invalidate any  other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_MORPHO_ABSTRACT_ATTRIBUTE_HXX
# define OLN_MORPHO_ABSTRACT_ATTRIBUTE_HXX

namespace oln
{
  namespace morpho
  {
    namespace attr
    {
      namespace abstract
      {
	template <class Exact>
	inline void
	attribute<Exact>::operator+=(const exact_type &rhs)
	{
	  mlc_dispatch(pe)(rhs);
	}

	template <class Exact>
	inline bool
	attribute<Exact>::operator>=(const lambda_type &lambda) const
	{
	  mlc_dispatch(ge)(lambda);
	}

	template <class Exact>
	inline
	bool
	attribute<Exact>::operator<(const lambda_type &lambda) const
	{
	  mlc_dispatch(less)(lambda);
	}

	template <class Exact>
	inline
	bool
	attribute<Exact>::operator<(const exact_type &x) const
	{
	  mlc_dispatch(less2)(x);
	}

	template <class Exact>
	inline
	bool
	attribute<Exact>::operator!=(const lambda_type &lambda) const
	{
	  mlc_dispatch(ne)(lambda);
	}

	template <class Exact>
	inline const typename attribute<Exact>::lambda_type &
	attribute<Exact>::toLambda() const
	{
	  mlc_dispatch(toLambda)();
	}

	template <class Exact>
	inline
	bool
	attribute<Exact>::ge_impl(const lambda_type &lambda) const
	{
	  return !(*this < lambda);
	}

	template <class Exact>
	inline
	bool
	attribute<Exact>::less2_impl(const exact_type &x) const
	{
	  return *this < x.toLambda();
	}

	template <class Exact>
	inline
	attribute<Exact>::attribute()
	{}
      } // !abstract
    } // !attr
  } // !morpho
} // !oln

#endif // !OLN_MORPHO_ABSTRACT_ATTRIBUTE_HXX
