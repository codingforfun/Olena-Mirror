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

#ifndef OLENA_CORE_ABSTRACT_W_WINDOW_HH
# define OLENA_CORE_ABSTRACT_W_WINDOW_HH

# include <oln/core/abstract/struct_elt.hh>
# include <oln/core/abstract/dpoint.hh>

namespace oln {

  namespace abstract  {
    template<class Exact>
    struct w_window; // forwarding declaration

  } // end of abstract

  /*!
  ** \brief Traits for abstract::w_window.
  */
  template<class Exact>
  struct struct_elt_traits<abstract::w_window<Exact> > : public
  struct_elt_traits<abstract::struct_elt<Exact> >
  {

  };

  namespace abstract
  {

    /*!
    ** \brief Weight Window.
    **
    ** A w_window is a set of points associated with a weight. This class
    ** defines how to deal with.
    */
    template<class Exact>
    struct w_window : public struct_elt< Exact >
    {
      typedef Exact exact_type; ///< Set the exact type.
      typedef struct_elt<Exact> super_type; ///< Set the super type.

      /*!
      ** \brief The associate image's type of dpoint (move point).
      ** \warning Prefer the macros oln_dpoint_type(Pointable) and
      ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;

      typedef typename struct_elt_traits<Exact>::weight_type weight_type;
      ///< Set the type of weight.
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
	return std::string("w_window<") + Exact::name() + ">";
      }

      /*!
      ** \brief Add a point (with weight) to the w_window.
      **
      ** Add a new member to the w_window.
      **
      ** \todo FIXME: Add dpoint with default weight
      ** (multiplication neutral element).
      */
      exact_type&
      add(const abstract::dpoint<dpoint_type>& dp, const weight_type& w = 1)
      {
	return this->exact().add_(dp.exact(), w);
      }

      /*!
      ** \brief Get the weight of a point.
      ** \arg i The nth point.
      ** \return The weight of the point.
      */
      weight_type
      w(unsigned i) const
      {
	return this->exact().get_weight(i);
      }

      /*!
      ** \brief Set the weight of a point if it exists. Otherwise
      ** create a new entry.
      ** \arg dp The point.
      ** \arg weight The weight of the point.
      ** \return The weight of the point.
      */
      const weight_type&
      set(const abstract::dpoint<dpoint_type>& dp,
	  const weight_type& weight)
      {
	return this->exact().set_(dp.exact(), weight);
      }

    protected:

      /*!
      ** \brief Add a new point with a weight of 1.
      ** \arg dp The point.
      **
      ** \todo FIXME: Add dpoint with default weight
      ** (multiplication neutral element).
      */
      exact_type&
      add_dp(const abstract::dpoint<dpoint_type>& dp)
      {
	return this->add(dp.exact(), 1);
      }

    protected:

      /*!
      ** \brief Do nothing, used only by sub-classes
      */
      w_window() : super_type()
      {}

    };

  } // end of abstract


  /*!
  ** \brief Compute intersection between two w_windows
  */
  template<class E>
  inline E
  inter(const abstract::w_window<E>& lhs, const abstract::w_window<E>& rhs)
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
  ** \brief Compute union between two  w_windows
  */
  template<class E>
  inline E
  uni(const abstract::w_window<E>& lhs, const abstract::w_window<E>& rhs)
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

#endif // ! OLENA_CORE_ABSTRACT_W_WINDOW_HH
