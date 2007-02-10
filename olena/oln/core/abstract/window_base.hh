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

#ifndef OLENA_CORE_ABSTRACT_WINDOW_BASE_HH
# define OLENA_CORE_ABSTRACT_WINDOW_BASE_HH

# include <mlc/contract.hh>
# include <mlc/is_a.hh>
# include <mlc/bool.hh>
# include <oln/core/abstract/window.hh>
# include <oln/core/abstract/w_window.hh>
# include <oln/core/abstract/neighborhood.hh>
# include <oln/core/accum.hh>
# include <iostream>
# include <vector>
# include <utility>

namespace oln {

  namespace abstract {
    template<class Sup, class Exact>
    struct window_base; // forwarding declaration
  } // end of abstract

  /*!
  ** \brief Traits for abstract::window_base.
  */
  template<class Sup, class Exact>
  struct struct_elt_traits<abstract::window_base<Sup, Exact> >: public struct_elt_traits<Sup>
  {

  };

  /*!
  ** Used for conditionnal friend in window_base class.
  */
  template<class Sup>
  struct window_base_friend_traits;

  /*!
  ** If window_base inherits from neighborhood, then mother is neighborhood.
  */
  template< class Exact>
  struct window_base_friend_traits< abstract::neighborhood<Exact> >
  {
    typedef abstract::neighborhood<Exact> ret;
  };

  /*!
  ** If window_base inherits from window, then mother is struct_elt.
  */
  template< class Exact>
  struct window_base_friend_traits< abstract::window<Exact> >
  {
    typedef abstract::struct_elt<Exact> ret;
  };

  /*!
  ** If window_base inherits from w_window, then mother is struct_elt.
  */
  template< class Exact>
  struct window_base_friend_traits< abstract::w_window<Exact> >
  {
    typedef abstract::struct_elt<Exact> ret;
  };

  namespace abstract
  {

    /*!
    ** \brief Window Base.
    **
    ** A window is a set of points and this class defines how to deal
    ** with. This class regroups common things for window, w_window (weight
    ** window) and neighborhood.
    ** Here, a set of point is a window or a weigh window or a neighborhood.
    */
    template<class Sup, class Exact>
    struct window_base: public Sup
    {
      enum { dim = struct_elt_traits<Exact >::dim };
      ///< Set the dimension of the window (depends of point dimension).
      typedef window_base<Sup, Exact> self_type;
      ///< Set self type.

      /*!
      ** \brief The associate image's type of point.
      ** \warning Prefer the macros oln_point_type(Pointable) and
      ** oln_point_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename struct_elt_traits<Exact>::point_type point_type;

      /*!
      ** \brief The associate image's type of dpoint (move point).
      ** \warning Prefer the macros oln_dpoint_type(Pointable) and
      ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;

      typedef Exact exact_type;
      ///< Set exact type.
      typedef Sup super_type;
      ///< Set type of class inherited.

      /*!
      ** \todo FIXME: this has been commented out to satisfy icc and
      ** comeau. I don't know who is right between them and gcc.
      */
      friend class struct_elt<Exact>;
      friend class neighborhood<Exact>;
      // friend class window_base_friend_traits<Sup>::ret;

      /// Return the name of the type.
      static std::string
      name()
      {
	return std::string("window_base<") + Exact::name() + ">";
      }

    protected:

      /*!
      ** \brief Test if the set of points contains this one.
      ** \arg dp a dpoint (deplacement point).
      ** \return True if the set of points contains this dpoint.
      */
      bool
      has_(const dpoint_type& dp) const
      {
	return std::find(dp_.begin(), dp_.end(), dp) != dp_.end();
      }

      /*!
      ** \brief Get the number of points.
      ** \return The number of points.
      */
      unsigned
      card_() const
      {
	return dp_.size();
      }

      /*!
      ** \brief Test if the set of points is centered.
      ** \return True if it's centered.
      **
      ** Centered means :
      **   - at least one element for neighborhood;
      **   - list of point contains 0 for window.
      */
      bool
      is_centered_() const
      {
	return centered_;
      }

      /*!
      ** \brief Compare two sets of points.
      ** \arg win The set of point to compare.
      ** \return True if they are the same.
      */
      bool
      is_equal(const exact_type& win) const
      {
	for (typename std::vector<dpoint_type>::const_iterator it = dp_.begin(); it != dp_.end(); ++it)
	  if (std::find(win.dp_.begin(), win.dp_.end(), *it) == win.dp_.end())
	    return false;
	return true;
      }

      /*!
      ** \brief Get the delta of the set of points.
      ** \return Delta.
      **
      ** Delta is the bigger element of the set of points.
      */
      coord
      get_delta() const
      {
	return delta_;
      }

      /*!
      ** \brief Update delta.
      ** \arg dp a deplacement point.
      ** \return Delta.
      **
      ** If the point is the biggest element of the set of points,
      ** then this point is assigned to delta.
      */
      coord
      delta_update(const dpoint_type& dp)
      {
	return this->exact().delta_update_(dp);
      }

      /*!
      ** \brief Set a set of point to opposite.
      **
      ** Each point of the set of point is assigned to its opposite.
      */
      void
      sym_()
      {
	for (unsigned i = 0; i < this->card(); ++i)
	  dp_[i] = - dp_[i];
      }

      /*!
      ** \brief Get the nth structuring element.
      ** \arg i The nth.
      ** \return The nth dpoint.
      ** \pre i < card().
      */
      const dpoint_type
      at(unsigned i) const
      {
	precondition(i < this->card());
	return dp_[i];
      }

      /*!
      ** \brief CTor
      **
      ** Used only by sub-classes
      */
      window_base() : super_type(), dp_(), delta_(0)
      {
	centered_ = false;
      }

      /*!
      ** \brief Used only by sub-classes
      ** \arg size The number of point.
      **
      ** Set the number of points this object will get.
      ** Used only by sub-classes
      */
      window_base(unsigned size) : super_type(), dp_(), delta_(0)
      {
	dp_.reserve(size);
	centered_ = false;
      }

      std::vector<dpoint_type> dp_; ///< The list of point.
      max_accumulator<coord> delta_; ///< Delta : the maximale point of the list.
      bool centered_; ///< Is the set of point centered ?

    };


  } // end of abstract
} // end of oln

/*!
** \brief Write the coordinates of point on an ostream.
** \arg o The stream.
** \arg w The window_base to write
** \return The ostream
**
** Useful for debugging
*/
template<class Sup, class Exact>
std::ostream&
operator<<(std::ostream& o, const oln::abstract::window_base<Sup, Exact>& w)
{
  unsigned c = w.card();
  o << "[";
  for (unsigned i = 0; i < c; ++i)
    o << w.dp(i);
  o << "]";
  return o;
}

#endif // OLENA_CORE_ABSTRACT_WINDOW_BASE_HH
