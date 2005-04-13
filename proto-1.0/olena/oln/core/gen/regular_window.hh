// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

# include <mlc/contract.hh>
# include <oln/core/abstract/window.hh>
# include <oln/core/abstract/grid.hh>
# include <oln/core/accum.hh>


namespace oln {

  // fwd decls
  template <typename G> class regular_window;
  template <typename G> class regular_fwd_qiter;

  // super_type
  template <typename G> struct set_super_type< regular_window<G> > { typedef abstract::window< regular_window<G> > ret; };

  // props
  template <typename G>
  struct set_props< category::window, regular_window<G> > 
  {
    typedef oln_grd_type_of(G, point)  point_type;
    typedef oln_grd_type_of(G, dpoint) dpoint_type;
    typedef oln_grd_type_of(G, size)   size_type;
    typedef regular_fwd_qiter<G>       fwd_qiter_type;
//     typedef regular_fwd_dpiter<G>      fwd_dpiter_type; // FIXME: later...
  };



  template <typename G>
  class regular_window : public abstract::window< regular_window<G> >
  {

  public:
   
    typedef regular_window<G> self_type;
    typedef oln_wn_type_of(self_type, dpoint) dpoint_type;

    typedef oln_grd_type_of(G, dimvalue) dimvalue_type;
    static const unsigned dim = dimvalue_type::val;

    regular_window() :
      dp_(),
      delta_(0)
    {
    }

    regular_window(unsigned n,
		   const oln_grd_type_of(G, coord) crd[]) :
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
	  this->add(dp);
	}
    }

    bool has(const dpoint_type& dp) const
    {
      return std::find(this->dp_.begin(), this->dp_.end(), dp) != dp_.end();
    }

    self_type& add(const dpoint_type& dp)
    {
      if (! this->has(dp))
	this->dp_.push_back(dp);
      this->delta_update(dp);
      return *this;
    }

    coord_t delta_update(const dpoint_type& dp)
    {
      for (unsigned c = 0; c < dim; ++c)
	this->delta_(abs(dp.nth(c)));
      return this->delta_;
    }

    coord_t get_delta() const
    {
      return this->delta_;
    }

    unsigned card() const
    {
      return this->dp_.size();
    }

    const dpoint_type at(unsigned i) const
    {
      precondition(i < this->card());
      return dp_[i];
    }

    // FIXME: redundant...
    dpoint_type dp(unsigned i) const
    {
      return this->at(i);
    }

    // FIXME: redundant...
    const dpoint_type operator[](unsigned i) const
    {
      return this->at(i);
    }

//     const self_type operator-() const
//     {
//       self_type tmp;
//       for (unsigned i = 0; i < this->card(), ++i)
// 	tmp.add(- this->at(i));
//       return tmp;
//     }

    void sym()
    {
      *this = - *this;
    }

  private:

    std::vector<dpoint_type> dp_;
    max_accumulator<coord_t> delta_;
  };


} // end of namespace oln



template<class G>
std::ostream& operator<<(std::ostream& ostr, const oln::regular_window<G>& win)
{
  unsigned c = win.card();
  ostr << "[";
  for (unsigned i = 0; i < c; ++i)
    ostr << win.dp(i);
  ostr << "]";
  return ostr;
}


# include <oln/core/gen/regular_fwd_qiter.hh>


#endif // OLENA_CORE_GEN_REGULAR_WINDOW_HH
