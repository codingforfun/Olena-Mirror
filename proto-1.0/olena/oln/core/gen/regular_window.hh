// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_GEN_REGULAR_WINDOW_HH
# define OLENA_CORE_GEN_REGULAR_WINDOW_HH

# include <iostream>
# include <algorithm>
# include <iterator>
# include <vector>
# include <set>

# include <mlc/contract.hh>
# include <oln/core/abstract/grid.hh>
# include <oln/core/abstract/dpoint.hh>
# include <oln/core/abstract/window.hh>


namespace oln {

  // fwd decls
  template <typename G, typename W> class regular_fwd_qiter;
  namespace abstract {
    template <typename G, typename E> class regular_window;
  }

  // super_type
  template <typename G, typename E> struct set_super_type< abstract::regular_window<G, E> > { typedef abstract::window<E> ret; };

  // props
  template <typename G, typename E>
  struct set_props< category::window, abstract::regular_window<G, E> >
  {
    typedef G                          grid_type;
    typedef oln_grd_type_of(G, dpoint) dpoint_type;
    typedef oln_grd_type_of(G, size)   size_type;
    typedef regular_fwd_qiter<G,E>     fwd_iter_type;
    //     typedef regular_fwd_dpiter<G>      fwd_dpiter_type; // FIXME: later...
  };



  namespace abstract
  {

    template <typename G, typename E>
    class regular_window : public window<E>
    {

    public:

      typedef regular_window<G, E> self_type;

      typedef oln_grd_type_of(G, coord)    coord_type;
      typedef oln_grd_type_of(G, dpoint)   dpoint_type;
      typedef oln_grd_type_of(G, dimvalue) dimvalue_type;

      static const unsigned dim = dimvalue_type::val;

      coord_type delta() const
      {
	return this->delta_;
      }

      unsigned card() const
      {
	return this->dp_.size();
      }

      const dpoint_type dp(unsigned i) const
      {
	precondition(i < this->card());
	return this->dp_[i];
      }

      dpoint_type& dp(unsigned i)
      {
	precondition(i < this->card());
	return this->dp_[i];
      }

      const E impl_op_minus() const
      {
	E tmp = this->exact(); // FIXME: use clone(?)
	for (unsigned i = 0; i < this->card(); ++i)
	  tmp.dp(i) = - tmp.dp(i);
	return tmp;
      }

      void sym()
      {
	*this = - *this;
      }

      const std::vector<dpoint_type>& get_dp() const
      {
	return this->dp_;
      }


      self_type get_fwd_win() const
      {
	self_type out;

	for (unsigned i = 0; i < this->card(); ++i)
	  {
	    const dpoint_type& dp = dp_[i];

	    for (unsigned n = 0; n < dim; ++n)
	      if (dp.nth(n) < 0)
		{
		  out.add(dp);
		  break;
		}
	      else if (dp.nth(n) > 0)
		break;
	  }
	return out;
      }


      self_type get_fwd_win_p() // abstract::window<W>& win)
      {
	self_type out;


	for (unsigned i = 0; i < this->card(); ++i)
	  {
	    const dpoint_type& dp = dp_[i];

	    unsigned n;
	    for (n = 0; n < dim; ++n)
	      if (dp.nth(n) < 0)
		{
		  out.add(dp);
		  break;
		}
	      else if (dp.nth(n) > 0)
		break;

	    // All p.nth(n) are 0.
	    if (n == dim)
	      out.add(dp);

	  }
	return out;
      }


      self_type get_bkd_win()
      {
	self_type out;

	for (unsigned i = 0; i < card(); ++i)
	  {
	    const dpoint_type& dp = dp_[i];

	    for (unsigned n = 0; n < dim; ++n)
	      if (dp.nth(n) > 0)
		{
		  out.add(dp);
		  break;
		}
	      else if (dp.nth(n) < 0)
		break;
	  }
	return out;
      }


      self_type get_bkd_win_p()
      {
	self_type out;

	for (unsigned i = 0; i < card(); ++i)
	  {
	    const dpoint_type& dp = this->win.get_dp()[i];

	    unsigned n;
	    for (n = 0; n < dim; ++n)
	      if (dp.nth(n) > 0)
		{
		  out.add(dp);
		  break;
		}
	      else if (dp.nth(n) < 0)
		break;
	    // All p.nth(n) are 0.
	    if (n == dim)
	      out.add(dp);
	  }
	return out;
      }

    protected:

      regular_window() :
	dp_(),
	delta_(0)
      {
      }

      regular_window(unsigned n, const coord_type crd[]) :
	dp_(),
	delta_(0)
      {
	precondition(n != 0);
	// FIXME: size of crd wrt n
	for (unsigned i = 0; i < n; ++i)
	  {
	    dpoint_type dp;
	    for (unsigned c = 0; c < dim; ++c)
	      dp.nth(c) = crd[i * dim + c];
	    this->add_(dp);
	  }
      }

      void add_(const dpoint_type& dp)
      {
	this->dpset_.insert(dp);
	this->dp_.clear();
	std::copy(this->dpset_.begin(), this->dpset_.end(),
		  std::back_inserter(this->dp_));
	this->delta_update_(dp);
      }




    private:

      std::set<dpoint_type, fwd_less_dpoint> dpset_;
      std::vector<dpoint_type> dp_;
      coord_type delta_;

      void delta_update_(const dpoint_type& dp)
      {
	for (unsigned c = 0; c < dim; ++c)
	  if (abs(dp.nth(c)) > this->delta_)
	    this->delta_ = abs(dp.nth(c));
      }

    };

  } // end of namespace oln::abstract

} // end of namespace oln



template<typename G, typename E>
std::ostream& operator<<(std::ostream& ostr, const oln::abstract::regular_window<G, E>& win)
{
  ostr << "[";
  for (unsigned i = 0; i < win.card(); ++i)
    ostr << win.dp(i);
  ostr << "]";
  return ostr;
}

# include <oln/core/gen/regular_fwd_qiter.hh>
# include <oln/core/gen/regular_bkd_qiter.hh>


#endif // ! OLENA_CORE_GEN_REGULAR_WINDOW_HH
