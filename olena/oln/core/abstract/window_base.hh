// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

namespace oln
{
  namespace abstract
  {
    template<class Sup, class Exact>
    struct window_base; // fwd_decl

  } // end of abstract

  
  template<class Sup, class Exact>
  struct struct_elt_traits<abstract::window_base<Sup, Exact> >: public struct_elt_traits<Sup>
  {
    
  };


  template<class Sup>
  struct window_base_friend_traits;

  template< class Exact>
  struct window_base_friend_traits<abstract::neighborhood<Exact> >
  {
    typedef abstract::neighborhood<Exact> ret;
  };

  template< class Exact>
  struct window_base_friend_traits<abstract::window<Exact> >
  {
    typedef abstract::struct_elt<Exact> ret;
  };

  template< class Exact>
  struct window_base_friend_traits<abstract::w_window<Exact> >
  {
    typedef abstract::struct_elt<Exact> ret;
  };

  namespace abstract
  {
    template<class Sup, class Exact>
    struct window_base: public Sup
    {
      enum { dim = struct_elt_traits<Exact >::dim };
      typedef window_base<Sup, Exact> self_type;
      typedef typename struct_elt_traits<Exact>::point_type point_type;
      typedef typename struct_elt_traits<Exact>::dpoint_type dpoint_type;
      typedef Exact exact_type;
      typedef Sup super_type;
 
      // FIXME: this has been commented out to satisfy icc and
      // comeau. I don't know who is right between them and gcc.
      friend class struct_elt<Exact>;
      friend class neighborhood<Exact>;
      // friend class window_base_friend_traits<Sup>::ret;
    
      static std::string 
      name()
      {
	return std::string("window_base<") + Exact::name() + ">";
      }

    protected:

      bool 
      has_(const dpoint_type& dp) const
      {
	return std::find(dp_.begin(), dp_.end(), dp) != dp_.end();
      }

      unsigned 
      card_() const
      {
	return dp_.size();
      }

      bool 
      is_centered_() const
      {
	return centered_;
      }

      bool 
      is_equal(const exact_type& win) const
      {
	for (typename std::vector<dpoint_type>::const_iterator it = dp_.begin(); it != dp_.end(); ++it)
	  if (std::find(win.dp_.begin(), win.dp_.end(), *it) == win.dp_.end())
	    return false;
	return true;
      }

      coord 
      get_delta() const
      {
	return delta_;
      }

      coord 
      delta_update(const dpoint_type& dp)
      {
	return this->exact().delta_update_(dp);
      }

      void 
      sym_()
      {
	for (unsigned i = 0; i < this->card(); ++i)
	  dp_[i] = - dp_[i];
      }

      const dpoint_type 
      at(unsigned i) const
      {
	precondition(i < this->card());
	return dp_[i];
      }

      window_base() : super_type(), dp_(), delta_(0)
      {
	centered_ = false;
      }

      window_base(unsigned size) : super_type(), dp_(), delta_(0)
      {
	dp_.reserve(size);
	centered_ = false;
      }

      std::vector<dpoint_type> dp_;
      max_accumulator<coord> delta_;
      bool centered_;

    }; 
    
    


  } // end of abstract
} // end of oln


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
