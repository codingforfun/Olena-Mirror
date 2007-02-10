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

#ifndef OLENA_LRDE_UFMT_HDC_MAXTREE_HH
# define OLENA_LRDE_UFMT_HDC_MAXTREE_HH

# include <oln/level/fill.hh>
# include <oln/lrde/ufmt/utils.hh>
# include <oln/lrde/ufmt/ad_maxtree.hh>
# include <oln/lrde/ufmt/attributes.hh>



namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {


      // FIXME: make it more generic! and move it...

      template <class T>
      std::vector<int> to_index(const image2d<T>& input,
				const std::vector<dpoint2d>& dp_)
      {
	std::vector<int> dp(dp_.size());
	for (unsigned i = 0; i < dp_.size(); ++i)
	  dp[i] = dp_[i].row() * (input.ncols() + 2 * input.border()) + dp_[i].col();
	return dp;
      }




      // FIXME: doc.

      template <class I>
      struct hdc_maxtree : public ad_maxtree<I>
      {
	typedef ad_maxtree<I> super;
	using super::f;
	using super::dpar;
 	oln_lrde_ufmt_import_ad_maxtree_typedefs;

	const oln_neighborhood_type(I)& nbh;

	// aux data
	std::vector<int> S;
	std::vector<size_t> H;
	std::vector<oln_dpoint_type(I)> dp_pre_, dp_post_; // temp
	std::vector<int> dp_pre, dp_post;
	unsigned nb;
	unsigned nnodes;


	// ctor

	hdc_maxtree(const abstract::image<I>& f,
		   const oln_neighborhood_type(I)& nbh)
	  :
	  super(f),
	  nbh(nbh)
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
	  S = histogram_reverse_sort_i(f.real(), H);
	  nb = split<I>(nbh, dp_pre_, dp_post_);
	  dp_pre  = to_index(f.real(), dp_pre_);
	  dp_post = to_index(f.real(), dp_post_);
	  nnodes = 0;
	  dpar.memset_0();
	}

	void compute_parent()
	{
	  int ip = 0;
	  for (int h = uint_nvalues(f.real()) - 1; h >= 0; --h)
	    {
	      int ip_first = ip;
	      int ip_last = ip + H[h];

	      // union-find

	      for (ip = ip_first; ip < ip_last; ++ip)
		{
		  point p = S[ip];
		  // pre
		  for (unsigned i = 0; i < nb; ++i)
		    {
		      point n = p + dp_pre[i];
		      if (f[n] >= h)
			do_union(n, p, h);
		    }
		  // post
		  for (unsigned i = 0; i < nb; ++i)
		    {
		      point n = p + dp_post[i];
		      if (f[n] > h)
			do_union(n, p, h);
		    }
		}

	      // finalizing level compression

	      for (int i = ip_last - 1; i >= ip_first; --i)
		{
		  point p = S[i];
		  if (is_root(p))
		    ++nnodes;
		  else
		    dpar[p] += dpar[p + dpar[p]];
		}
	      
	    } // end of "for all h"
	}


	template <class A> // A for attributes
	std::vector< node_<A> >
	extract_maxtree()
	{
	  std::vector< node_<A> > node(nnodes);
	  dpar_t& label = dpar;

	  // bkd
	  unsigned cur_l = nnodes - 1;
	  for (int i = S.size() - 1; i != 0; --i)
	    {
	      point p = S[i];
	      assert(is_level_root(p + dpar[p])); // level compression is total
	      if (is_level_root(p))
		{
		  // node initialization
		  point par_p = p + dpar[p]; // this value is saved
		  label[p] = cur_l; // because dpar[p] is modified here
		  node[cur_l].par = label[par_p]; // so this line is ok
		  if (is_root(p))
		    assert(node[cur_l].par == cur_l);
		  --cur_l;
		}
	      else
		// label propagation
		label[p] = label[p + dpar[p]];
	    }
	  return node;
	}


	template <class A> // A for attributes
	void compute_attributes(std::vector< node_<A> >& node)
	  // point-wise version
	{
	  dpar_t& label = dpar;
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


	point find_root(point x, value h)
	{
	  while (not is_root(x))
	    if (f[x] > h)
	      x += dpar[x];
	    else
	      x = find_level_root(x + dpar[x]);
	  return x;
	}

	void do_union(const point& n, const point& p, value h)
	{
	  point r = find_root(n, h);
	  if (r != p)
	    dpar[r] = p - r;
	}


      }; // end of class hdc_maxtree



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_HDC_MAXTREE_HH
