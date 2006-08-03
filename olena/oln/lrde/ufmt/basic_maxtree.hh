// Copyright (C) 2006  EPITA Research and Development Laboratory
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

#ifndef OLENA_LRDE_UFMT_BASIC_MAXTREE_HH
# define OLENA_LRDE_UFMT_BASIC_MAXTREE_HH

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>



namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {


      // FIXME: doc.

      template <class I>
      struct basic_maxtree
      {
	typedef I image;
	typedef oln_point_type(I) point;
	typedef oln_value_type(I) value;
	typedef oln_neighborhood_type(I) Nbh;
	typedef oln_iter_type(Nbh) niter;

	// input
	const I& f;
	const Nbh& nbh;

	// aux data
	typename mute<I, point>::ret par;
	typename mute<I, bool>::ret isproc;
	std::vector<point> S;


	// ctor

	basic_maxtree(const abstract::image<I>& f,
		      const oln_neighborhood_type(I)& nbh)
	  :
	  f(f.exact()),
	  nbh(nbh),
	  // FIXME: par and isproc should NOT be initialized here! (but in "init()")
	  par(f.size()),
	  isproc(f.size())
	{
	}

	void go()
	{
	  init();
	  compute_parent();  // 1st pass
	  extract_maxtree(); // 2nd pass
	}

	void init()
	{
	  S = histogram_reverse_sort_p(f);
	  level::fill(isproc, false);
	}

	void compute_parent()
	{
	  for (int ip = 0; ip < int(S.size()); ++ip)
	    {
	      point p = S[ip];
	      make_set(p);

	      oln_neighb_type(Nbh) n(nbh, p);
	      for_all(n)
		if (f.hold(n) and isproc[n])
		  do_union(n, p);
	      isproc[p] = true;
	    }
	}

	void extract_maxtree()
	{
	  // FIXME: TODO
	}

	void make_set(const point& x)
	{
	  par[x] = x;
	}

	void do_union(const point& n, const point& p)
	{
	  point r = find_tree_root(n);
	  if (r != p)
	    par[r] = p;
	}

	bool is_tree_root(const point& x) const
	{
	  return par[x] == x;
	}

	bool is_level_root(const point& x) const
	{
	  return is_tree_root(x) or f[par[x]] < f[x];
	}

	point find_level_root(const point& x)
	{
	  if (is_level_root(x))
	    return x;
	  else
	    return par[x] = find_level_root(par[x]);
	}

	point find_tree_root(point x)
	{
	  while (not is_tree_root(x))
	    x = find_level_root(par[x]);
	  return x;
	}



	// uniformized interface

	const point& parent_(const point& p) const {
	  return par[p];
	}
	const value& f_(const point& p) const {
	  return f[p];
	}
	const I& f_() const {
	  return f;
	}
	bool is_tree_root_(const point& p) const {
	  return is_tree_root(p);
	}
	bool is_level_root_(const point& p) const {
	  return is_level_root(p);
	}

	// end of uniformized interface



      }; // end of class basic_maxtree



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_BASIC_MAXTREE_HH
