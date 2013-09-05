man // Copyright (C) 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE).
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

#ifndef MLN_MORPHO_TREE_IMPL_COMPUTE_PARENT_DUAL_HQUEUE_HH
# define MLN_MORPHO_TREE_IMPL_COMPUTE_PARENT_DUAL_HQUEUE_HH

///
/// \brief The dual input tree algorithm specialized for low quantized image.
///
/// This implementation is based on P. Salembier algorithm using
/// hierachical queues. This implies a low-quantized input image so
/// that the number of queues is limited.

# include <mln/util/ctree/ctree.hh>
# include <mln/data/fill.hh>

# include <mln/extension/fill.hh>
# include <mln/extension/adjust.hh>
# include <mln/geom/nsites.hh>


# include <mln/util/hqueues.hh>

# include <mln/value/value_array.hh>
# include <mln/value/set.hh>
# include <mln/util/timer.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      namespace impl
      {

	/// Compute a tree using hqueues.
	///
	/// \param f The original image.
	/// \param m The connectivity mask.
	/// \param nbh The neighborhood of the mask.
	///
	/// \return The tree structure.
	///
	template <typename T, typename I, typename N>
	util::ctree::ctree<I>
	dual_hqueue(const tag::tree::tree_t<T>&,
		    const Image<I>& f,
		    const Image<I>& m,
		    const Neighborhood<N>& nbh);


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
	  flooder_base_(const I& f_, const I& m_, const N& nbh_);

	  // Returned values.
	  mln_ch_value(I, unsigned)	map;
	  std::vector<P>		location;
	  std::vector<V>		values;
	  std::vector<P>		parent;
	  std::vector<unsigned>		area;
	  unsigned			nnode;


	protected:
	  enum {
	    nvalues = mln_card(mln_value(I))
	  };


	  // Recursive function that floods. Canevas function.
	  int flood(int h_idx);

	  // HLevel comparaison. To be specialized for min/max tree.
	  bool hlevel_less(int h_idx1, int h_idx2);

	  // Parent attachment function. To be specialized for min/max tree.
	  int attach_parent(int h_idx);

	  // Data.
	  // \{
	  const I& f;
	  const I& m;
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
	  flooder(const I& f, const I& m, const N& nbh);

	  // HLevel comparaison.
	  bool hlevel_less_(int h_idx1, int h_idx2);

	  // Parent attachment function.
	  int attach_parent_(int h_idx);

	public:
	  using super_::parent;
	  using super_::area;
	  using super_::location;

	protected:
	  using super_::is_node_at_level;
	  using super_::node_at_level;
	  using super_::vset;
	  using super_::loc;
	  using super_::hqueues;
	};

	template <typename I, typename N>
	struct flooder<tag::tree::min_t, I, N> :
	  flooder_base_< I, N, flooder<tag::tree::min_t, I, N> >
	{
	  typedef mln_value(I) V;
	  typedef flooder_base_< I, N, flooder<tag::tree::min_t, I, N> > super_;

	  // Constructor.
	  flooder(const I& f, const I& m, const N& nbh);

	  // HLevel comparaison.
	  bool hlevel_less_(int h_idx1, int h_idx2);

	  // Parent attachment function.
	  unsigned attach_parent_(int h_idx);

	public:
	  using super_::parent;
	  using super_::area;
	  using super_::location;

	protected:
	  using super_::is_node_at_level;
	  using super_::node_at_level;
	  using super_::vset;
	  using super_::loc;
	  using super_::hqueues;
	};


	template <typename I, typename N, typename E>
	inline
	bool
	flooder_base_<I, N, E>::hlevel_less(int h_idx1, int h_idx2)
	{
	  E& obj = exact(*this);
	  return obj.hlevel_less_(h_idx1, h_idx2);
	}

	template <typename I, typename N, typename E>
	inline
	int
	flooder_base_<I, N, E>::attach_parent(int h_idx)
	{
	  E& obj = exact(*this);
	  return obj.attach_parent_(h_idx);
	}

	template <typename I, typename N>
	inline
	flooder<tag::tree::max_t, I, N>::flooder(const I& f_, const I& m_, const N& nbh_)
	  : flooder_base_<I, N, flooder<tag::tree::max_t, I, N> >(f_, m_, nbh_)
	{
	}

	template <typename I, typename N>
	inline
	flooder<tag::tree::min_t, I, N>::flooder(const I& f_, const I& m_, const N& nbh_)
	  : flooder_base_<I, N, flooder<tag::tree::min_t, I, N> >(f_, m_, nbh_)
	{
	}

	template <typename I, typename N>
	inline
	bool
	flooder<tag::tree::max_t, I, N>::hlevel_less_(int h_idx1, int h_idx2)
	{
	  return h_idx1 < h_idx2;
	}

	template <typename I, typename N>
	inline
	bool
	flooder<tag::tree::min_t, I, N>::hlevel_less_(int h_idx1, int h_idx2)
	{
	  return h_idx2 < h_idx1;
	}

	template <typename I, typename N>
	inline
	int
	flooder<tag::tree::max_t, I, N>::attach_parent_(int h_idx)
	{
	  mln_assertion(h_idx >= 0);
	  mln_assertion(is_node_at_level[h_idx]);


	  unsigned x = node_at_level[h_idx];
	  int c = h_idx - 1;

	  is_node_at_level[h_idx] = false;
	  while (c >= 0 && !is_node_at_level[c])
	    --c;

	  if (c >= 0)
	    {
	      unsigned p = node_at_level[c];
	      parent[x] = p;
	      area[p] += area[x] + 1;
	    }
	  else // root case.
	    parent[x] = x;

	  location[x] = loc++;
	  return c;
	}

	template <typename I, typename N, typename E>
	flooder_base_<I, N, E>::flooder_base_(const I& f_, const I& m_, const N& nbh_)
	  : f (f_), m(m_), nbh (nbh_), is_node_at_level (false)
        {
          typedef mln_value(I) V;

          unsigned pmin_offset;
          V vmin;

          // Allocate hierarchical queues and retrieve the starting point
          {
            util::array< unsigned > h(nvalues, 0);

            pmin_offset = m.index_of_point(m.domain().pmin());
            vmin = m.element(pmin_offset);

            mln_fwd_pixter(const I) pxl(m);
            for_all(pxl)
            {
              ++h[pxl.val()];
              if (hlevel_less(pxl.val(), vmin))
                {
                  vmin = pxl.val();
                  pmin_offset = pxl.offset();
                }
            }

            for (unsigned i = 0; i < nvalues; ++i)
              hqueues[i].reserve(h[i]);
          }

          // Initialize aux data
          {
            dp = offsets_wrt(f, nbh);
            n_nbhs = dp.nelements();
            vset = value::set<V>::the();

            extension::adjust(f, nbh);
            extension::adjust(m, nbh);

            initialize(deja_vu, f);
            initialize(map, f);
            data::fill(deja_vu, false);
            extension::fill(deja_vu, true);

            unsigned nsites = geom::nsites(f);
            parent.resize(2 * nsites);
            location.resize(2 * nsites);
            area.resize(2 * nsites, 0);
            values.resize(2 * nsites);

            nnode = 0;
            loc = 0;
          }

          // Start flooding
          unsigned hmin = vset.index_of(vmin);
          unsigned fmin = vset.index_of(f.element(pmin_offset));
          std::cout << vmin << std::endl;
          hqueues[hmin].push(pmin_offset);
          deja_vu.element(pmin_offset) = true;
          values[nnode] = vmin;
          node_at_level[fmin] = nnode++;
          is_node_at_level[fmin] = true;
          if (hmin != fmin)
            {
              values[nnode] = f.element(pmin_offset);
              node_at_level[hmin] = nnode++;
              is_node_at_level[hmin] = true;
            }
          int c = flood(hmin);
          while (c != -1) // There still are nodes under minimal mask level.
            c = attach_parent(c);
          mln_assertion(c == -1);
        }

	template <typename I, typename N, typename E>
	int
	flooder_base_<I, N, E>::flood(int h_idx)
	{
	  mln_assertion(h_idx >= 0);
	  mln_assertion(is_node_at_level[h_idx]);

	  while (!hqueues[h_idx].empty())
	    {
	      unsigned p = hqueues[h_idx].pop_front();
	      int p_idx = vset.index_of(f.element(p));
	      mln_assertion(vset.index_of(m.element(p)) == (unsigned)h_idx);
	      //mln_assertion(is_node_at_level[p_idx]);


	      if (h_idx < p_idx) // FIXME: max tree only
		{
		  // Singleton
		  int par_i = node_at_level[h_idx];
		  int self_i = nnode++; // Create the node.
		  map.element(p) = self_i;
		  values[self_i] = m.element(p);
		  area[self_i] = 1;
		  parent[self_i] = par_i;
		  location[self_i] = loc++;
		  area[par_i]++;
		}
	      else
		map.element(p) = node_at_level[p_idx];

	      // Process the neighbors
	      for (unsigned j = 0; j < n_nbhs; ++j)
		{
		  unsigned n = p + dp[j];

		  if (!deja_vu.element(n))
                    {
                      unsigned n_hidx = vset.index_of(m.element(n));
                      unsigned n_fidx = vset.index_of(f.element(n));
                      hqueues[n_hidx].push(n);
                      deja_vu.element(n) = true;

                      if (!is_node_at_level[n_hidx])
                        {
                          values[nnode] = (m.element(n));
                          node_at_level[n_hidx] = nnode++;
                          is_node_at_level[n_hidx] = true;
                        }
                      if (!is_node_at_level[n_fidx] && n_fidx < n_hidx)
                        {
                          values[nnode] = (f.element(n));
                          node_at_level[n_fidx] = nnode++;
                          is_node_at_level[n_fidx] = true;
                        }

                      while (hlevel_less(h_idx, n_hidx))
                        n_hidx = flood(n_hidx);
                    }
		}
	    }

	  return attach_parent(h_idx);
	}

	template <typename T, typename I, typename N>
	inline
	util::ctree::ctree<I>
	dual_hqueue(const tag::tree::tree_t<T>&,
		    const Image<I>& f_,
		    const Image<I>& m_,
		    const Neighborhood<N>& nbh_)
	{
	  trace::entering("mln::morpho::tree::impl::hqueue");
	  const I& f = exact(f_);
	  const I& m = exact(m_);
	  const N& nbh = exact(nbh_);

	  mln_precondition(f.is_valid());
	  mln_precondition(m.is_valid());
	  mln_precondition(nbh.is_valid());

	  // Process
	  impl::flooder<T, I, N> args(f, m, nbh);

	  // Reserve
	  util::ctree::ctree<I> tree;
	  tree.reserve(f, args.nnode);

	  // Set values
	  {
	    mln_piter(I) p(f.domain());
	    for_all(p)
	    {
	      unsigned idx = args.map(p);
	      unsigned loc = args.nnode - args.location[idx] - 1;
	      tree.node_at_(p) = loc;
	    }

	    for (int idx = 0; idx < args.nnode; ++idx)
              {
                unsigned loc = args.nnode - args.location[idx] - 1;

                tree.parent_at_(loc) = args.nnode - args.location[args.parent[idx]] - 1;
                tree.f_at_(loc) = args.values[idx];
                tree.length_at_(loc) = args.area[idx];
              }
	  }

	  trace::exiting("mln::morpho::tree::impl::hqueue");
	  return tree;
	}

      } // end of namespace mln::morpho::tree::impl


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_MORPHO_TREE_COMPUTE_PARENT_DUAL_HQUEUE_HH



