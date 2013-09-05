#ifndef MLN_MORPHO_TREE_IMPL_HQUEUE_FASTHH
# define MLN_MORPHO_TREE_IMPL_HQUEUE_FASTHH

# include <mln/extension/adjust.hh>
# include <mln/extension/fill.hh>
# include <mln/util/ctree/ctree.hh>
# include <mln/util/hqueues.hh>
# include <mln/value/value_array.hh>
# include <mln/geom/nsites.hh>
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
	util::ctree::ctree<I>
	hqueue_fast(const tag::tree::tree_t<T>&, const Image<I>& f, const Neighborhood<N>& nbh);

      } // end of namespace mln::morpho::tree::impl

# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	template <typename I, typename N, typename E>
	struct flooder_base_ : Object<E>
	{
	  typedef unsigned P;
	  typedef mln_value(I) V;

	  // Constructor.
	  flooder_base_(const I& f_, const N& nbh_);

	  mln_ch_value(I, unsigned)	map;
	  std::vector<P>		location;
	  std::vector<P>		parent;
	  std::vector<unsigned>		area;
	  unsigned			n_node;

	protected:
	  enum {
	    nvalues = mln_card(mln_value(I))
	  };

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
	  util::hqueues<unsigned, V>		hqueues;
	  value::set<V>				vset;
	  util::array<int>			dp;
	  unsigned				n_nbhs;

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
	flooder<tag::tree::max_t, I, N>::flooder(const I& f_, const N& nbh_)
	  : flooder_base_<I, N, flooder<tag::tree::max_t, I, N> >(f_, nbh_)
	{
	}

	template <typename I, typename N>
	inline
	flooder<tag::tree::min_t, I, N>::flooder(const I& f_, const N& nbh_)
	  : flooder_base_<I, N, flooder<tag::tree::min_t, I, N> >(f_, nbh_)
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
	flooder_base_<I, N, E>::flooder_base_(const I& f_, const N& nbh_)
	  : f (f_), nbh (nbh_), is_node_at_level(false)
	{
	   // Prepare hierarchical queues and retrieve start point.
	   unsigned	pmin;
	   mln_value(I) vmin;
	   {
	     util::array< unsigned > h(nvalues, 0);

	     pmin = f.index_of_point(f.domain().pmin());
	     vmin = f.element(pmin);

	     mln_fwd_pixter(const I) pxl(f);
	     for_all(pxl)
	     {
	       ++h[pxl.val()];
	       if (hlevel_less(pxl.val(), vmin))
		 {
		   vmin = pxl.val();
		   pmin = pxl.offset();
		 }
	     }

	     for (unsigned i = 0; i < nvalues; ++i)
	       hqueues[i].reserve(h[i]);
	   }

	   // Initialize aux data
	   {
	     dp = offsets_wrt(f, nbh);
	     n_nbhs = dp.nelements();
	     vset = value::set< mln_value(I) >::the();

	     extension::adjust(f, nbh);

	     initialize(deja_vu, f);
	     initialize(map, f);
	     data::fill(deja_vu, false);
	     extension::fill(deja_vu, true);

	     unsigned nsites = geom::nsites(f);
	     parent.resize(nsites);
	     location.resize(nsites);
	     area.resize(nsites, 0);

	     n_node = 0;
	     loc = 0;
	   }

	   // Start flooding
	   unsigned hmin = vset.index_of(vmin);
	   deja_vu.element(pmin) = true;
	   hqueues[hmin].push(pmin);
	   node_at_level[hmin] = n_node++;
	   is_node_at_level[hmin] = true;

	   flood(hmin);
	}

	template <typename I, typename N, typename E>
	unsigned
	flooder_base_<I, N, E>::flood(const unsigned h_idx)
	{
	  mln_assertion(is_node_at_level[h_idx]);

	  while (!hqueues[h_idx].empty())
	    {
	      unsigned p = hqueues[h_idx].pop_front();
	      mln_assertion(vset.index_of(f.element(p)) == h_idx);

	      map.element(p) = node_at_level[h_idx];

	      // Process the neighbors
	      for (unsigned j = 0; j < n_nbhs; ++j)
		{
		  unsigned n = p + dp[j];

		  if (!deja_vu.element(n))
		  {
		    unsigned n_idx = vset.index_of(f.element(n));
		    hqueues[n_idx].push(n);
		    deja_vu.element(n) = true;

		    if (!is_node_at_level[n_idx])
		      {
			node_at_level[n_idx] = n_node++;
			is_node_at_level[n_idx] = true;
		      }

		    while (hlevel_less(h_idx, n_idx))
		      n_idx = flood(n_idx);
		  }
		}
	    }

	  is_node_at_level[h_idx] = false;
	  return attach_parent(h_idx);
	}

	template <typename T, typename I, typename N>
	inline
	util::ctree::ctree<I>
	hqueue_fast(const tag::tree::tree_t<T>&, const Image<I>& f_, const Neighborhood<N>& nbh_)
	{
	  trace::entering("mln::morpho::tree::impl::hqueue_fast");
	  const I& f = exact(f_);
	  const N& nbh = exact(nbh_);

	  mln_precondition(f.is_valid());
	  mln_precondition(nbh.is_valid());

	  // Process
	  impl::flooder<T, I, N> args(f, nbh);
	  // \todo handles here discontinous domains: so multi-roots trees.

	  // Create tree and fill it.
	  util::ctree::ctree<I> tree;
	  {
	    tree.reserve(f, args.n_node);

	    mln_pixter(const I) pix(f);
	    for_all(pix)
	    {
	      int idx = args.map.element(pix.offset());
	      int loc = args.n_node - args.location[idx] - 1;
	      tree.data_->map_.element(pix.offset()) = loc;
	      tree.data_->values_[loc] = pix.val();
	    }

	    for (unsigned idx = 0; idx < args.n_node; ++idx)
	      {
		int loc = args.n_node - args.location[idx] - 1;
		tree.data_->parent_[loc] = args.n_node - args.location[args.parent[idx]] - 1;
		tree.data_->length_[loc] = args.area[idx];
	      }
	  }

	  trace::exiting("mln::morpho::tree::impl::hqueue_fast");
	  return tree;
	}

      } // end of namespace mln::morpho::tree::impl

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_MORPHO_TREE_IMPL_HQUEUE_FAST_HH
