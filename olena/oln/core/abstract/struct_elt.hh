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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_CORE_STRUCT_ELT_HH
# define OLENA_CORE_STRUCT_ELT_HH

# include <mlc/type.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/dpoint.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    struct struct_elt; // forwarding declaration

  } // end of abstract

  template<class Exact>
  struct struct_elt_traits;

  /*!
  ** \brief Traits for abstract::struct_elt.
  */
  template<class Exact>
  struct struct_elt_traits<abstract::struct_elt<Exact> >
  {
    typedef abstract::struct_elt<Exact> abstract_type;
    ///< Defines the abstract type of the structuring element.
  };

  namespace abstract {

    /*!
    ** Structuring elements (set of dpoints).
    **
    ** This abstract class defines several virtual methods for its
    ** subclasses. Its goal is to deal with a set of 'move' points.
    */
    template<class Exact>
    struct struct_elt : public mlc_hierarchy::any< Exact >
    {
      enum { dim = struct_elt_traits<Exact>::dim };
      ///< Set the dim of the image from which points com.

      typedef struct_elt<Exact> self_type;
      ///< Set the exact self type of the class.
      typedef typename struct_elt_traits<Exact>::abstract_type abstract_type;
      ///< Set the abstract type of hisself.

      /// Return the name of the type.
      static std::string
      name()
      {
	return std::string("struct_elt<") + Exact::name() + ">";
      }

      typedef typename struct_elt_traits<Exact>::point_type point_type;
      ///< Set the point type of the image from which points come.
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;
      ///< Set the dpoint type.
      typedef Exact exact_type;

      /*!
      ** \brief Test if the set of points contains this one.
      ** \arg dp a dpoint (deplacement point).
      ** \return True if the set of points contains this dpoint.
      */
      bool
      has(const abstract::dpoint<dpoint_type>& dp) const
      {
	return this->exact().has_(dp.exact());
      }

      /*!
      ** \brief Get the number of points.
      ** \return The number of points.
      */
      unsigned
      card() const
      {
	return this->exact().card_();
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
	return this->exact().is_centered_();
      }

      /*!
      ** \brief Add a point to the structuring elements.
      **
      ** Add a new member to the structuring elements.
      **
      ** \warning Here for convenience (see morpho algorithms).
      ** Work with w_windows (associate a default weight set to 1).
      */
      exact_type&
      add(const abstract::dpoint<dpoint_type>& dp)
      {
	return this->exact().add_dp(dp);
      }

      /*!
      ** \brief Get the nth structuring element.
      ** \arg i The nth.
      ** \return The nth dpoint.
      */
      dpoint_type
      dp(unsigned i) const
      {
	return this->exact().at(i);
      }

      /*!
      ** \brief Compare two sets of structuring elements.
      ** \arg win The structuring elements to compare.
      ** \return True if they are the same.
      */
      bool
      operator==(const self_type& win) const
      {
	return this->exact().is_equal(win.exact());
      }

      /*!
      ** \brief Get the delta of the structuring elements.
      ** \return Delta.
      **
      ** Delta is the biggest element of the structuring elements.
      */
      coord
      delta() const
      {
	return this->exact().get_delta();
      }

      /*!
      ** \brief Get the nth structuring element.
      ** \arg i The nth.
      ** \return The nth dpoint.
      */
      const dpoint_type
      operator[](unsigned i) const
      {
	return this->exact().at(i);
      }

      /*!
      ** \brief Set structuring elements to opposite.
      **
      ** Each point of structuring elements is assigned to its opposite.
      */
      exact_type
      operator-() const
      {
	exact_type win(this->exact());
	win.sym();
	return win;
      }

    protected:

      /*!
      ** \brief Set structuring elements to opposite.
      **
      ** Each point of structuring elements is assigned to its opposite.
      */
      void
      sym()
      {
	return this->exact().sym_();
      }

      /*!
      ** \brief Do nothing, used only by sub-classes
      */
      struct_elt()
      {}

    };
  } // end of abstract

} // end of oln

#endif // ! OLENA_CORE_STRUCT_ELT_HH
