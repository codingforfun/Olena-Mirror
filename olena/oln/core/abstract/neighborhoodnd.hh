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

#ifndef OLENA_CORE_ABSTRACT_NEIGHBORHOODND_HH
# define OLENA_CORE_ABSTRACT_NEIGHBORHOODND_HH

# include <oln/core/abstract/window_base.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    struct neighborhoodnd; // forward declaration
  } // end of abstract

  /*!
  ** \brief Traits for abstract::neighborhoodnd.
  */
  template<class Exact>
  struct struct_elt_traits<abstract::neighborhoodnd<Exact> >: public
  struct_elt_traits<abstract::window_base<abstract::neighborhood<Exact>, Exact> >
  {

  };

  namespace abstract {

    /*!
    ** \brief Neighborhoodnd.
    **
    ** It looks like structuring elements but here, when
    ** you add an element, you add his opposite.
    ** Points have N dimensions.
    **
    */
    template<class Exact>
    struct neighborhoodnd: public window_base<neighborhood<Exact>, Exact>
    {
      typedef window_base<neighborhood<Exact>, Exact> super_type;
      ///< Super type.
      typedef neighborhoodnd<Exact> self_type; ///< Self type.
      typedef Exact exact_type;  ///< Exact type.

      /*!
      ** \brief The associate image's type of dpoint (move point).
      ** \warning Prefer the macros oln_dpoint_type(Pointable) and
      ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;

      friend class neighborhood<exact_type>;

      /*!
      ** \brief Return his type in a string.
      ** \return The type in a string.
      **
      ** Very useful to debug.
      */
      static std::string
      name()
      {
	return std::string("neighborhoodnd<") + Exact::name() + ">" ;
      }

    protected:

      /*!
      ** \brief Add a point to the neighborhood.
      ** \arg dp The new point.
      ** \pre !dp.is_centered().
      **
      ** Add a new member to the neighborhood.
      */
      exact_type&
      add_(const dpoint_type& dp)
      {
	precondition( !dp.is_centered() );
	this->centered_ = true;
	if (!(has_(dp)))
	  this->dp_.push_back(dp);
	delta_update(dp);
	return this->exact();
      }

      /*!
      ** \brief Construct a neighborhoodnd.
      */
      neighborhoodnd() : super_type()
      {}

      /*!
      ** \brief Construct a neighborhood of 'size' elements.
      ** \arg size The number of element to reserve for the neighborhood.
      */
      neighborhoodnd(unsigned size) : super_type(size)
      {}

    };
  } // end of abstract
} // end of oln

#endif // OLENA_CORE_ABSTRACT_NEIGHBORHOODND_HH
