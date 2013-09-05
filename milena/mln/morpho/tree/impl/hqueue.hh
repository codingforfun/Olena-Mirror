// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_MORPHO_TREE_IMPL_HQUEUE_HH
# define MLN_MORPHO_TREE_IMPL_HQUEUE_HH

# include <mln/util/ctree/ctree.hh>
# include <mln/util/hqueues.hh>
# include <mln/value/value_array.hh>
# include <mln/geom/nsites.hh>

# include <mln/histo/array.hh>
# include <mln/histo/compute.hh>
# include <mln/data/fill.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      namespace impl
      {

	template <typename T, typename I, typename N>
	inline
	util::ctree::ctree<I>
	hqueue(const tag::tree::tree_t<T>&, const Image<I>& f, const Neighborhood<N>& nbh);

      } // end of namespace mln::morpho::tree::impl

# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename T, typename I>
	struct op_psite_less;

	template <typename I>
	struct op_psite_less< tag::tree::max_t, I>
	{
	  op_psite_less(const I& f) : f_(f) {}

	  bool operator() (const mln_psite(I)& lhs, const mln_psite(I)& rhs) const
	  {
	    return util::ord_strict(f_(lhs), f_(rhs));
	  }

	private:
	  const I& f_;
	};

	template <typename I>
	struct op_psite_less< tag::tree::min_t, I>
	{
	  op_psite_less(const I& f) : f_(f) {}

	  bool operator() (const mln_psite(I)& lhs, const mln_psite(I)& rhs) const
	  {
	    return util::ord_strict(f_(rhs), f_(lhs));
	  }

	private:
	  const I& f_;
	};

      }


      namespace impl
      {


	template <typename I, typename N, typename E>
	struct flooder_base_ : Object<E>
	{
	  typedef unsigned P;
	  typedef mln_value(I) V;

	  // Constructor.
	  flooder_base_(const I& f_, const N& nbh_);

	  // Returned values.
	  mln_ch_value(I, unsigned)	map;
	  std::vector<P>		location;
	  std::vector<P>		parent;
	  std::vector<unsigned>		area;
	  unsigned			n_node;


	protected:
	  // Recursive function that floods. Canevas function.
	  unsigned flood(const unsigned h_idx);

	  // HLevel comparaison. To be specialized for min/max tree.
	  bool hlevel_less(unsigned h_idx1, unsigned h_idx2);

	  // Parent attachment function. To be specialized for min/max tree.
	  unsigned attach_parent(unsigned h_idx);

	  // Data.
	  // \{
	  const I& f;
	  const N& nbh;

	  // Auxiliary data.
	  util::hqueues<mln_psite(I), V>	hqueues;
	  value::set<V>				vset;

	  mln_ch_value(I, bool)		deja_vu;
	  value::value_array<V, bool>	is_node_at_level;
	  value::value_array<V, P>	node_at_level;
	  unsigned			loc;
	  // \}
	};

	template <typename T, typename I, typename N>
	struct flooder;

	template <typename I, typename N>
	struct flooder<tag::tree::max_t, I, N> :
	  flooder_base_< I, N, flooder<tag::tree::max_t, I, N> >
	{
	  typedef mln_value(I) V;
	  typedef flooder_base_< I, N, flooder<tag::tree::max_t, I, N> > super_;

	  // Constructor.
	  flooder(const I& f_, const N& nbh_);

	  // HLevel comparaison.
	  bool hlevel_less_(unsigned h_idx1, unsigned h_idx2);

	  // Parent attachment function.
	  unsigned attach_parent_(unsigned h_idx);

	public:
	  using super_::parent;
	  using super_::area;
	  using super_::location;

	protected:
	  using super_::is_node_at_level;
	  using super_::node_at_level;
	  using super_::vset;
	  using super_::loc;
	};

	template <typename I, typename N>
	struct flooder<tag::tree::min_t, I, N> :
	  flooder_base_< I, N, flooder<tag::tree::min_t, I, N> >
	{
	  typedef mln_value(I) V;
	  typedef flooder_base_< I, N, flooder<tag::tree::min_t, I, N> > super_;

	  // Constructor.
	  flooder(const I& f_, const N& nbh_);

	  // HLevel comparaison.
	  bool hlevel_less_(unsigned h_idx1, unsigned h_idx2);

	  // Parent attachment function.
	  unsigned attach_parent_(unsigned h_idx);

	public:
	  using super_::parent;
	  using super_::area;
	  using super_::location;

	protected:
	  using super_::is_node_at_level;
	  using super_::node_at_level;
	  using super_::vset;
	  using super_::loc;
	};


	template <typename I, typename N, typename E>
	inline
	bool
	flooder_base_<I, N, E>::hlevel_less(unsigned h_idx1, unsigned h_idx2)
	{
	  E& obj = exact(*this);
	  return obj.hlevel_less_(h_idx1, h_idx2);
	}

	template <typename I, typename N, typename E>
	inline
	unsigned
	flooder_base_<I, N, E>::attach_parent(unsigned h_idx)
	{
	  E& obj = exact(*this);
	  return obj.attach_parent_(h_idx);
	}

	template <typename I, typename N>
	inline
	flooder<tag::tree::max_t, I, N>::flooder(const I& f_, const N& nbh_,
						 const histo::array< mln_value(I) >& histo_)
	  : flooder_base_<I, N, flooder<tag::tree::max_t, I, N> >(f_, nbh_, histo_)
	{
	}

	template <typename I, typename N>
	inline
	flooder<tag::tree::min_t, I, N>::flooder(const I& f_, const N& nbh_,
						 const histo::array<mln_value(I)>& histo_)
	  : flooder_base_<I, N, flooder<tag::tree::min_t, I, N> >(f_, nbh_, histo_)
	{
	}

	template <typename I, typename N>
	inline
	bool
	flooder<tag::tree::max_t, I, N>::hlevel_less_(unsigned h_idx1, unsigned h_idx2)
	{
	  return h_idx1 < h_idx2;
	}

	template <typename I, typename N>
	inline
	bool
	flooder<tag::tree::min_t, I, N>::hlevel_less_(unsigned h_idx1, unsigned h_idx2)
	{
	  return h_idx2 < h_idx1;
	}

	template <typename I, typename N>
	inline
	unsigned
	flooder<tag::tree::max_t, I, N>::attach_parent_(unsigned h_idx)
	{
	  unsigned c = h_idx;
	  while (c > 0 && !is_node_at_level[c])
	    --c;

	  unsigned x = node_at_level[h_idx];
	  if (is_node_at_level[c]) // || c > 0
	    {
	      unsigned p = node_at_level[c];
	      parent[x] = p;
	      area[p] += area[x] + 1;
	    }
	  else
	    parent[x] = x;

	  location[x] = loc++;
	  return c;
	}

	template <typename I, typename N>
	inline
	unsigned
	flooder<tag::tree::min_t, I, N>::attach_parent_(unsigned h_idx)
	{
	  unsigned nv = vset.nvalues() - 1;
	  unsigned c = h_idx;

	  while (c < nv && !is_node_at_level[c])
	    ++c;

	  unsigned x = node_at_level[h_idx];
	  if (is_node_at_level[c]) // || c < nv
	    {
	      unsigned p = node_at_level[c];
	      parent[x] = p;
	      area[p] += area[x] + 1;
	    }
	  else
	    parent[x] = x;

	  location[x] = loc++;
	  return c;
	}


	template <typename I, typename N, typename E>
	flooder_base_<I, N, E>::flooder_base_(const I& f_, const N& nbh_,
					      const histo::array< mln_value(I) >& histo_)
	  : f (f_), nbh (nbh_), is_node_at_level (false)
	  {
	    // Initialization
	    {
	      unsigned nsites = geom::nsites(f);
	      initialize(map, f);
	      initialize(deja_vu, f);

	      parent.resize(nsites);
	      location.resize(nsites);
	      area.resize(nsites, 0);
	      data::fill(deja_vu, false);

	      vset = value::set< mln_value(I) >::the();

	      n_node = 0;
	      loc = 0;
	    }

	    // Get start value
	    mln_psite(I) pstart = f.domain().pmin();
	    mln_value(I) hstart = f(pstart);
	    {
	      mln_piter(I) p(f.domain());
	      for_all(p)
	      {
		mln_value(I) v = f(p);
		++h[v];
		if (hlevel_less(v, hstart))
		  {
		    hstart = v;
		    pstart = p;
		  }
	      }

	      for (unsigned i = 0; i < nvalues; ++i)
		hqueues[i].reserve(h[i]);
	    }

	    // Start flooding
	    unsigned r = vset.index_of(hstart);
	    deja_vu(pstart) = true;
	    hqueues[r].push(pstart);
	    node_at_level[r] = n_node++;
	    is_node_at_level[r] = true;

	    flood(r);
	  }

	template <typename I, typename N, typename E>
	unsigned
	flooder_base_<I, N, E>::flood(const unsigned h_idx)
	{
	  mln_assertion(is_node_at_level[h_idx]);

	  while (!hqueues[h_idx].empty())
	    {
	      mln_psite(I) p = hqueues[h_idx].pop_front();
	      mln_assertion(vset.index_of(f(p)) == h_idx);

	      map(p) = node_at_level[h_idx];

	      // Process the neighbors
	      mln_niter(N) n(nbh, p);
	      for_all(n)
		if (f.domain().has(n) && !deja_vu(n))
		  {
		    unsigned n_idx = vset.index_of(f(n));
		    hqueues[n_idx].push(n);
		    deja_vu(n) = true;

		    if (!is_node_at_level[n_idx])
		      {
			node_at_level[n_idx] = n_node++;
			is_node_at_level[n_idx] = true;
		      }

		    while (hlevel_less(h_idx, n_idx))
		      n_idx = flood(n_idx);
		  }
	    }

	  is_node_at_level[h_idx] = false;
	  return attach_parent(h_idx);
	}

	template <typename T, typename I, typename N>
	inline
	util::ctree::ctree<I>
	hqueue(const tag::tree::tree_t<T>&, const Image<I>& f_, const Neighborhood<N>& nbh_)
	{
	  trace::entering("mln::morpho::tree::impl::hqueue");
	  const I& f = exact(f_);
	  const N& nbh = exact(nbh_);

	  mln_precondition(f.is_valid());
	  mln_precondition(nbh.is_valid());

	  // Process
	  impl::flooder<T, I, N> args(f, nbh);
	  // \todo handles here discontinous domains: so multi-roots trees.

	  args.parent.resize(args.n_node);
	  args.area.resize(args.n_node);
	  args.location.resize(args.n_node);

	  util::ctree::ctree<I> tree(f, args.map, args.location, args.parent,
				     args.area);

	  trace::exiting("mln::morpho::tree::impl::hqueue");
	  return tree;
	}

      } // end of namespace mln::morpho::tree::impl

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_MORPHO_TREE_IMPL_HQUEUE_HH
