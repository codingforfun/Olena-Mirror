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

#ifndef OLENA_CORE_ABSTRACT_W_WINDOWND_HH
# define OLENA_CORE_ABSTRACT_W_WINDOWND_HH

# include <oln/core/abstract/window_base.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    struct w_windownd; // forward declaration
  } // end of abstract

  /*!
  ** \brief Traits for abstract::w_windownd.
  */
  template<class Exact>
  struct struct_elt_traits<abstract::w_windownd<Exact> >: public
  struct_elt_traits<abstract::window_base<abstract::w_window<Exact>, Exact> >
  {

  };

  namespace abstract {

    /*!
    ** \brief Weight Window N dimensions
    **
    ** A w_window is a set of points associated with a weight. This class
    ** defines how to deal with. These points have N dimensions.
    */
    template<class Exact>
    struct w_windownd: public window_base<w_window<Exact>, Exact>
    {
      typedef window_base<abstract::w_window<Exact>, Exact> super_type;
      ///< Set the super type.
      typedef w_windownd<Exact> self_type; ///< Set the self type.
      typedef Exact exact_type; ///< Set the exact type.
      typedef typename struct_elt_traits<Exact>::weight_type weight_type;
      ///< Set the type of weight.

      /*!
      ** \brief The associate image's type of dpoint (move point).
      ** \warning Prefer the macros oln_dpoint_type(Pointable) and
      ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;

      friend class w_window<exact_type>;

      /// Return the name of the type.
      static std::string
      name()
      {
	return std::string("w_windownd<") + Exact::name() + ">" ;
      }

    protected:

      /*!
      ** \brief Get the weight of the nth point of the window.
      ** \arg i The nth point.
      ** \return The weight of this point.
      ** \pre i < card().
      */
      weight_type
      get_weight(unsigned i) const
      {
	precondition(i < this->card());
	return w_[i];
      }

      /*!
      ** \brief Add a point (with weight) to the window.
      ** \arg dp The new point.
      ** \arg w The weight of this new point.
      ** \pre !has(dp).
      **
      ** Add a new member to the window (ignored if ist weight is 0).
      */
      exact_type&
      add_(const dpoint_type& dp, const weight_type& w)
      {
	precondition(! has_(dp));
	if (w == 0)		// Don't add 0 weighted entries
	  return this->exact();
	if (dp.is_centered())
	  this->centered_ = true;
	this->dp_.push_back(dp);
	delta_update(dp);
	w_.push_back(w);
	return this->exact();
      }

      /*!
      ** \brief Set the weight of a point if it exists. Otherwise
      ** create a new entry.
      ** \arg dp The point.
      ** \arg weight The weight of the point.
      ** \return The weight of the point.
      */
      const weight_type&
      set_(const dpoint_type& dp, const weight_type& w)
      {
	// if the dp exists, return a ref to the existing entry
	for (unsigned i = 0; i < this->card_(); ++i)
	  if (this->dp_[i] == dp)
	  {
	    w_[i] = w;
	    return w_[i];
	  }

	// otherwise, create new entry
	add(dp, w);
	return w_.back();
      }

      /*!
      ** \brief Construct a w_window.
      */
      w_windownd() : super_type()
      {}

      /*!
      ** \brief Construct a w_window of 'size' elements.
      ** \arg size The number of elements to reserve for the window.
      */
      w_windownd(unsigned size) : super_type(size)
      {
	w_.reserve(size);
      }

      std::vector< weight_type > w_; ///< List of point's weight.

    };
  } // end of abstract
} // end of oln

/*!
** \brief Write coordinates and weight of each window's point
** on an ostream.
** \arg o The stream.
** \arg w The window_base to write
** \return The ostream
**
** Useful for debugging
*/
template<class Exact>
std::ostream&
operator<<(std::ostream& o, const oln::abstract::w_windownd<Exact>& w)
{
  unsigned c = w.card();
  o << "[";
  for (unsigned i = 0; i < c; ++i)
    o << "(" << w.dp(i) << "," << w.w(i) << ")";
  o << "]";
  return o;
}

#endif // OLENA_CORE_ABSTRACT_W_WINDOWND_HH
