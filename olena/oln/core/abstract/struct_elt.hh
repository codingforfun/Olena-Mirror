// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_ABSTRACT_STRUCT_ELT_HH
# define OLN_CORE_ABSTRACT_STRUCT_ELT_HH

# include <oln/core/abstract/dpoint_set.hh>
# include <oln/core/macros.hh>
namespace oln
{
  template <typename Exact>
  struct props;

  namespace abstract
  {
    template <class Exact>
    struct struct_elt: public dpoint_set<Exact>
    {
      /// Return the name of the type.
      static std::string
      name()
      {
	return std::string("struct_elt<") + Exact::name() + ">";
      }

      /*!
      ** \brief Test if the structuring elements is centered.
      ** \return True if it's centered.
      **
      ** Structuring elements are centered when they contains 0.
      */
      bool
      is_centered() const
      {
	return dispatch(is_centered)();
      }

      oln_concrete_type(Exact) image() const
      {
	return oln_dispatch(image)();
      }

    };

    /*!
    ** \brief Compute intersection between two se.
    */
    template<class E>
    inline E
    inter(const abstract::struct_elt<E>& lhs, const abstract::struct_elt<E>& rhs)
    {
      E win;
      for (unsigned i = 0; i < lhs.card(); ++i)
	if (rhs.has(lhs.dp(i)))
	  win.add(lhs.dp(i));
      for (unsigned j = 0; j < rhs.card(); ++j)
	if (! win.has(rhs.dp(j)) && lhs.has(rhs.dp(j)))
	  win.add(rhs.dp(j));
      return win;
    }

    /*!
    ** \brief Compute union between two se.
    */
    template<class E>
    inline E
    uni(const abstract::struct_elt<E>& lhs, const abstract::struct_elt<E>& rhs)
    {
      E win;
      for (unsigned i = 0; i < lhs.card(); ++i)
	win.add(lhs.dp(i));
      for (unsigned j = 0; j < rhs.card(); ++j)
	if (! win.has(rhs.dp(j)))
	  win.add(rhs.dp(j));
      return win;
    }

  } // !abstract
} // !oln

#endif // ! OLN_CORE_STRUCT_ELT_HH
