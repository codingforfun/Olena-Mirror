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

#ifndef OLENA_CORE_ABSTRACT_WINDOWND_HH
# define OLENA_CORE_ABSTRACT_WINDOWND_HH

# include <oln/core/abstract/window_base.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    struct windownd; // forward declaration
  } // end of abstract

  /*!
  ** \brief Traits for abstract::windownd.
  */
  template<class Exact>
  struct struct_elt_traits<abstract::windownd<Exact> >: public
  struct_elt_traits<abstract::window_base<abstract::window<Exact>, Exact> >
  {

  };

  namespace abstract {

    /*!
    ** \brief Window N dimensions.
    **
    ** A window is a set of points. This class
    ** defines how to deal with. These points have N dimensions.
    */
    template<class Exact>
    struct windownd: public window_base<window<Exact>, Exact>
    {
      typedef window_base<window<Exact>, Exact> super_type;
      ///< Set the super type.
      typedef windownd<Exact> self_type; ///< Set the self type.
      typedef Exact exact_type; ///< Set the exact type.

      /*!
      ** \brief The associate image's type of dpoint (move point).
      ** \warning Prefer the macros oln_dpoint_type(Pointable) and
      ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;

      friend class window<exact_type>;

      /*!
      ** \brief Return his type in a string.
      ** \return The type in a string.
      **
      ** Very useful to debug.
      */
      static std::string
      name()
      {
	return std::string("windownd<") + Exact::name() + ">" ;
      }

    protected:

      /*!
      ** \brief Add a point to the window.
      ** \arg dp The new point.
      **
      ** Add a new member to the window.
      */
      exact_type&
      add_(const dpoint_type& dp)
      {
	if (dp.is_centered())
	  this->centered_ = true;
	if (!(has_(dp)))
	  this->dp_.push_back(dp);
	this->delta_update(dp);
	return this->exact();
      }

      /*!
      ** \brief Construct a window.
      */
      windownd() : super_type()
      { }

      /*!
      ** \brief Construct a w_window of 'size' elements.
      ** \arg size The number of element to reserve for the window.
      */
      windownd(unsigned size) : super_type(size)
      {}

    };
  } // end of abstract
} // end of oln

#endif // OLENA_CORE_ABSTRACT_WINDOWND_HH
