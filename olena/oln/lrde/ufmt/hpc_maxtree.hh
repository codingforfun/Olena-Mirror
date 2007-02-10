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

#ifndef OLENA_LRDE_UFMT_HPC_MAXTREE_HH
# define OLENA_LRDE_UFMT_HPC_MAXTREE_HH

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>
# include <oln/lrde/ufmt/ap_maxtree.hh>
# include <oln/lrde/ufmt/attributes.hh>



namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {

      // FIXME: doc.

      template <class I>
      struct hpc_maxtree : public ap_maxtree<I>
      {
	typedef ap_maxtree<I> super;
	using super::f;
	using super::nbh;
	using super::par;
	oln_lrde_ufmt_import_ap_maxtree_typedefs;

	// aux data
	std::vector<point> S;
	std::vector<size_t> H;
	std::vector<dpoint> dp_pre, dp_post;
	unsigned nb;
	unsigned nnodes;
	typename mute<I, unsigned>::ret label;


	// ctor

	hpc_maxtree(const abstract::image<I>& f,
		   const oln_neighborhood_type(I)& nbh)
	  :
	  super(f, nbh),
	  label(f.size())
	{
	}

	template <class A> // void if no attributes
	void go()
	{
	  init();
	  compute_parent();  // 1st pass
	  extract_maxtree<A>(); // 2nd pass
	}

	void init()
	{
	  S = histogram_reverse_sort_p(f, H);
	  nb = split<I>(nbh, dp_pre, dp_post);
	  nnodes = 0;
	}

	void compute_parent()
	{
	  int i = 0;
	  for (int h = uint_nvalues(f) - 1; h >= 0; --h)
	    {
	      int i_first = i;
	      int i_last  = i_first + H[h];

	      // union-find

	      for (i = i_first; i < i_last; ++i)
		{
		  point p = S[i];
		  make_set(p);
		  // pre
		  for (unsigned j = 0; j < nb; ++j)
		    {
		      point n = p + dp_pre[j];
		      if (f.hold(n) and f[n] >= h)
			do_union(n, p, h);
		    }
		  // post
		  for (unsigned j = 0; j < nb; ++j)
		    {
		      point n = p + dp_post[j];
		      if (f.hold(n) and f[n] > h)
			do_union(n, p, h);
		    }
		}

	      // enhancing level compression:
	      //   we compress within 'level h' but we can end up with:
	      //   p' @ h+ --> p @ h --> lr @ h
	      //   so we have p -> lr for all p at level h
	      //   but p' --> p cannot be compressed by the code below
	      for (int k = i_last - 1; k >= i_first; --k)
		{
		  point p = S[k];
		  if (is_root(p))
		    ++nnodes;
		  else
		    par[p] = par[par[p]];
		}
	    } // end of "for all h"
	}


	template <class A> // A for attributes
	std::vector< node_<A> >
	extract_maxtree()
	{
	  std::vector< node_<A> > node(nnodes);

	  // bkd
	  unsigned cur_l = nnodes - 1;
	  for (int i = S.size() - 1; i != 0; --i)
	    {
	      point p = S[i];

	      // nota: level compression is not completed here;
	      // however, though it is useless to finalize it, the
	      // code to perform completion is given but commented
	      if (is_level_root(p))
		{
		  /*
		  // completing level compression:

		  if (not is_level_root(par[p]))
		  {
		    // Cf. above comments; here we have
		    // p @ h+ --> par[p] @ h --> lr @ h
		    if (not is_root(p))
		      {
			assert(f[p] > f[par[p]]);
			assert(f[par[p]] == f[par[par[p]]]);
			assert(is_level_root(par[par[p]]));
		      }
		    par[p] = par[par[p]];
		  }
		  // now the level compression is completed!
		  assert(is_level_root(par[p]));
		  */

		  // node initialization
		  label[p] = cur_l;
		  node[cur_l].par = label[par[p]];
		  --cur_l;
		}
	      else
		// label propagation
		label[p] = label[par[p]];
	    }
	  return node;
	}


	template <class A> // A for attributes
	void compute_attributes(std::vector< node_<A> >& node)
	  // point-wise version
	{
	  for (int i = S.size() - 1; i != 0; --i)
	    {
	      point p = S[i];
	      if (is_level_root(p))
		node[label[p]].init(f, p);
	      else
		node[label[p]].insert(f, p);
	    }
	  for (int l = 0; l < nnodes; ++l)
	    if (node[l].par != l) // not root node
	      node[node[l].par].embrace(node[l]);
	}

	void make_set(const point& x)
	{
	  par[x] = x;
	}

	bool is_root(const point& x) const
	{
	  return par[x] == x;
	}

	bool is_level_root(const point& x) const
	{
	  return is_root(x) or f[par[x]] < f[x];
	}

	point find_level_root(const point& x)
	{
	  if (is_level_root(x))
	    return x;
	  else
	    return par[x] = find_level_root(par[x]);
	}

	point find_root(point x, value h)
	{
	  while (not is_root(x))
	    if (f[x] > h)
	      x = par[x];
	    else
	      x = find_level_root(par[x]);
	  return x;
	}

	void do_union(const point& n, const point& p, value h)
	{
	  point r = find_root(n, h);
	  if (r != p)
	    par[r] = p;
	}


      }; // end of class hpc_maxtree



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_HPC_MAXTREE_HH
