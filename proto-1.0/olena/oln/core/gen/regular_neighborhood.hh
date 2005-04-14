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

#ifndef OLENA_CORE_GEN_REGULAR_NEIGHBORHOOD_HH
# define OLENA_CORE_GEN_REGULAR_NEIGHBORHOOD_HH

# include <oln/core/abstract/neighborhood.hh>
# include <oln/core/gen/regular_window.hh>


namespace oln {

  // fwd decl
  template <typename G> class regular_neighborhood;

  // super_type
  template <typename G> struct set_super_type< regular_neighborhood<G> > { typedef abstract::neighborhood< regular_neighborhood<G> > ret; };

  // props
  template <typename G>
  struct set_props< category::neighborhood, regular_neighborhood<G> > 
  {
    typedef oln_grd_type_of(G, dpoint) dpoint_type;
    typedef oln_grd_type_of(G, size)   size_type;
    typedef regular_window<G>          window_type;
  };



  template <typename G>
  class regular_neighborhood : public abstract::neighborhood< regular_neighborhood<G> >
  {

  public:
   
    typedef regular_neighborhood<G> self_type;

    typedef oln_grd_type_of(G, coord)    coord_type;
    typedef oln_grd_type_of(G, dpoint)   dpoint_type;
    typedef oln_grd_type_of(G, dimvalue) dimvalue_type;

    static const unsigned dim = dimvalue_type::val;

    regular_neighborhood()
    {
    }

    regular_neighborhood(const regular_window<G>& win)
    {
      for (unsigned i = 0; i < win.card(); ++i)
	this->add(win.dp(i));
    }

    regular_neighborhood(unsigned n, const coord_type crd[])
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

    self_type& add(const dpoint_type& dp)
    {
      precondition(dp != -dp); // means dp is not (0)
      this->win_.add(dp);
      this->win_.add(-dp);
      return *this;
    }

    coord_type delta() const
    {
      return this->win_.delta();
    }

    unsigned card() const
    {
      return this->win_.card();
    }

    const dpoint_type dp(unsigned i) const
    {
      precondition(i < this->card());
      return this->win_.dp(i);
    }

    const std::vector<dpoint_type>& get_dp() const
    {
      return this->win_.get_dp();
    }

    const regular_neighborhood<G>& get_win() const
    {
      return this->win_;
    }

  private:

    /// Only attribute (to delegate to).
    regular_window<G> win_;

  };


} // end of namespace oln



template<class G>
std::ostream& operator<<(std::ostream& ostr, const oln::regular_neighborhood<G>& nbh)
{
  return ostr << nbh.get_win();
}


#endif // OLENA_CORE_GEN_REGULAR_NEIGHBORHOOD_HH
