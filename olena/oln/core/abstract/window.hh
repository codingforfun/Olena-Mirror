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

#ifndef OLENA_CORE_ABSTRACT_WINDOW_HH
# define OLENA_CORE_ABSTRACT_WINDOW_HH

# include <oln/core/abstract/struct_elt.hh>
# include <oln/core/winneighb.hh>
# include <oln/core/abstract/dpoint.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    struct window; // forward declaration
  } // end of abstract

  /*!
  ** Traits for abstract::neighborhood
  */
  template<class Exact>
  struct struct_elt_traits<abstract::window<Exact> > : public
  struct_elt_traits< abstract::struct_elt<Exact> >
  {

  };

  namespace abstract
  {

    /*!
    ** \brief Window.
    **
    ** A window is a set of points and this class defines how to deal
    ** with.
    */
    template<class Exact>
    struct window : public struct_elt<Exact>
    {
      typedef struct_elt<Exact> super_type; ///< Set super type.
      typedef Exact exact_type; ///< Set exact type.

      /*!
      ** \brief The associate image's type of dpoint (move point).
      ** \warning Prefer the macros oln_dpoint_type(Pointable) and
      ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;

      friend class struct_elt<exact_type>;

      /*!
      ** \brief Return its type in a string.
      ** \return The type in a string.
      **
      ** Very useful to debug.
      */
      static std::string
      name()
      {
	return std::string("window<") + Exact::name() + ">";
      }

    protected:

      /*!
      ** \brief Add a point to the window.
      **
      ** Add a new member to the window.
      */
      exact_type&
      add_dp(const abstract::dpoint<dpoint_type>& dp)
      {
	return this->exact().add_(dp.exact());
      }

      /*!
      ** \brief Do nothing, used only by sub-classes
      */
      window() : super_type()
      {}

    };

  } // end of abstract

  /*!
  ** \brief Compute intersection between two windows
  */
  template<class E>
  inline E
  inter(const abstract::window<E>& lhs, const abstract::window<E>& rhs)
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
  ** \brief Compute union between two windows
  */
  template<class E>
  inline E
  uni(const abstract::window<E>& lhs, const abstract::window<E>& rhs)
  {
    E win;
    for (unsigned i = 0; i < lhs.card(); ++i)
      win.add(lhs.dp(i));
    for (unsigned j = 0; j < rhs.card(); ++j)
      if (! win.has(rhs.dp(j)))
	win.add(rhs.dp(j));
    return win;
  }


} // end of oln

#endif // ! OLENA_CORE_ABSTRACT_WINDOW_HH
