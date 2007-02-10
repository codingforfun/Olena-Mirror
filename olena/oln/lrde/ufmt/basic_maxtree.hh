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

#ifndef OLENA_LRDE_UFMT_BASIC_MAXTREE_HH
# define OLENA_LRDE_UFMT_BASIC_MAXTREE_HH

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>
# include <oln/lrde/ufmt/ap_maxtree.hh>



namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {


      // FIXME: doc.

      template <class I>
      struct basic_maxtree : public ap_maxtree<I>
      {
	typedef ap_maxtree<I> super;
	using super::f;
	using super::nbh;
	using super::par;
	oln_lrde_ufmt_import_ap_maxtree_typedefs;


	// aux data
	typename mute<I, bool>::ret isproc;
	std::vector<point> S;


	// ctor

	basic_maxtree(const abstract::image<I>& f,
		      const oln_neighborhood_type(I)& nbh)
	  :
	  super(f, nbh),
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

	void do_union(const point& n, const point& p)
	{
	  point r = find_root(n);
	  if (r != p)
	    par[r] = p;
	}

	point find_root(point x)
	{
	  while (not is_root(x))
	    x = find_level_root(par[x]);
	  return x;
	}


      }; // end of class basic_maxtree



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_BASIC_MAXTREE_HH
