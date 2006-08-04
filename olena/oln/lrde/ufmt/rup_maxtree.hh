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

#ifndef OLENA_LRDE_UFMT_RUP_MAXTREE_HH
# define OLENA_LRDE_UFMT_RUP_MAXTREE_HH

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
      struct rup_maxtree : public ap_maxtree<I>
      {
	typedef ap_maxtree<I> super;
	using super::f;
	using super::nbh;
	using super::par;
	oln_lrde_ufmt_import_ap_maxtree_typedefs;

	// aux data
	std::vector<dpoint> dp;
	unsigned nb;


	// ctor

	rup_maxtree(const abstract::image<I>& f,
		    const oln_neighborhood_type(I)& nbh)
	  :
	  super(f, nbh)
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
	  nb = pre<I>(nbh, dp);
	}

	void compute_parent()
	{
	  oln_iter_type(I) p(f);
	  for_all(p)
	    {
	      make_set(p); // FIXME: is it useful?
	      point n = p + dp[0];
	      if (f.hold(n))
		insert(n, p); // FIXME: is it useful to distinguish insert from update?
	      for (unsigned i = 1; i < nb; ++i)
		{
		  n = p + dp[i];
		  if (f.hold(n))
		    update(n, p);
		}
	    }
	}

	void extract_maxtree()
	{
	  // FIXME: TODO
	}

	void insert(const point& n, const point& p)
	{
	  point r = anc(n, f[p]);
	  
	  if (f[r] <= f[p])
	    par[p] = r;
	  else
	    {
	      if (par[r] != r) // split
		par[p] = par[r];
	      par[r] = p;
	    }
	}

	void update(point x, point y)
	{
	  assert(x != y); // invariant

	  if (f[x] < f[y])
	    swap(x, y);
	  assert(f[x] >= f[y]); // invariant

	  point xx = anc(x, f[y]);
	  assert(f[xx] >= f[y]);

	  point yy = anc(y, f[y]);
	  assert(yy == find_level_root(y));
	  assert(f[yy] == f[y]);

	  assert(f[yy] <= f[xx]);
	  // if (f[yy] < f[xx]) std::cout << '<'; else std::cout << '=';


	  if (xx == yy)
	    return; // already merged

	  point memo = par[yy];

	  if (f[xx] == f[yy])
	    {
	      par[yy] = xx; // simple plug
	      update(xx, memo);
	    }
	  else
	    {
	      assert(f[yy] < f[xx]);
	      if (par[xx] == xx)
		{
		  par[xx] = yy; // new tree root
		  // FIXME: the end?
		}
	      else
		{
		  // insertion
		  point bot = par[xx];
		  par[xx] = yy;
		  par[yy] = bot;
		  update(xx, memo);
		}
	    }

	  // FIXME: this algo looks ok and seems to work (...?)
	  // it is very close to the 'insert' algo running with
	  // "insert(x, find_level_root(y))" after the "if..swap"
	  // FIXME: so it can be bettered...
	}



	void insert__unfold_version(const point& n, const point& p)
	{
	  if (f[n] <= f[p])
	    {
	      // nota bene: "point r = anc(n, f[p]);" leads to "r == n" in that case
	      par[p] = n;
	      // new (temporary?) regional maximum
	    }
	  else
	    // f[n] > f[p]
	    {
	      point r = anc(n, f[p]);
	      assert(f[r] >= f[p]);
	      if (f[r] == f[p])
		{
		  par[p] = r; // p is plugged into this node/component
		}
	      else
		{
		  if (par[r] == r) // tree root
		    par[r] = p;
		  else
		    // insertion between a couple of nodes linked together
		    // 2 new links are created
		    {
		      point bot = par[r];
		      par[r] = p;
		      par[p] = bot;
		    }
		}
	    }
	}


      }; // end of class rup_maxtree



    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_RUP_MAXTREE_HH
