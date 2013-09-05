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

#ifndef MLN_MORPHO_TREE_IMPL_UNION_FIND_FAST_HH
# define MLN_MORPHO_TREE_IMPL_UNION_FIND_FAST_HH

///
///


namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      namespace impl
      {

	namespace generic
	{

	/// The fast version of union-find algorithm.
	/// \pre The image \p f must be fastest
	///
	/// \param tree_tag The type of tree to build (min or max-tree).
	/// \param f Source image used to build the component tree.
	/// \param nbh Neighborhood used to build the component tree.
	///
	/// \return A component tree object.
	///
	template <typename T, typename I, typename N>
	util::ctree::ctree<I>
	union_find_fast(const tag::tree::tree_t<T>& tree_tag,
			const Image<I>& f,
			const Neighborhood<N>& nbh);
	}

      }

# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename I>
	inline
	void
	sort_sites_set_radix(const tag::tree::max_t&, const I& f, util::array<unsigned>& s, util::array<unsigned>& levels)
	{
	  typedef mln_value(I) V;
	  static const value::set<V>& vset = value::set<V>::the();
	  static const int n = mln_card(V);

	  util::array<unsigned> h(n, 0);
	  s.resize(f.domain().nsites());
	  levels.resize(n + 1);

	  mln_fwd_pixter(const I) pxl(f);
	  for_all(pxl)
	    ++h[vset.index_of(pxl.val())];

	  for (int i = 1; i <= n; ++i)
	    {
	      levels[i] = levels[i - 1] + h[i - 1];
	      h[i - 1] = levels[i - 1];
	    }

	  // computing output data
	  for_all(pxl)
	  {
	    int i = vset.index_of(pxl.val());
	    s[h[i]++] = pxl.offset();
	  }
	}

	template <typename I>
	static inline
	void
	sort_sites_set_radix(const tag::tree::min_t&, const I& f, util::array<unsigned>& s, util::array<unsigned>& levels)
	{
	  typedef mln_value(I) V;
	  static const value::set<V>& vset = value::set<V>::the();
	  static const int n = mln_card(V);

	  util::array<unsigned> h(n, 0);
	  s.resize(f.domain().nsites());
	  levels.resize(n + 1);

	  mln_fwd_pixter(const I) pxl(f);
	  for_all(pxl)
	    ++h[n - 1 - vset.index_of(pxl.val())];

	  for (int i = 1; i <= n; ++i)
	    {
	      levels[i] = levels[i - 1] + h[i - 1];
	      h[i - 1] = levels[i - 1];
	    }

	  // computing output data
	  for_all(pxl)
	  {
	    int i = vset.index_of(pxl.val());
	    s[h[n - 1 - i]++] = pxl.offset();
	  }
	}

	template <typename T, typename I>
	struct sort_op_less_;

	template <typename I>
	struct sort_op_less_<tag::tree::max_t, I>
	{
	  sort_op_less_ (const I& f_) : f(f_) {}

	  bool operator() (unsigned e1, unsigned e2)
	  {
	    return f.element(e1) < f.element(e2) || (f.element(e1) == f.element(e2) && e1 < e2);
	  }

	private:
	  const I& f;
	};

	template <typename I>
	struct sort_op_less_<tag::tree::min_t, I>
	{
	  sort_op_less_ (const I& f_) : f(f_) {}

	  bool operator() (unsigned e1, unsigned e2)
	  {
	    return f.element(e2) < f.element(e1) || (f.element(e1) == f.element(e2) && e2 < e1);
	  }

	private:
	  const I& f;
	};

	template <typename T, typename I>
	inline
	void
	sort_sites_set_std(const T&, const I& f, util::array<unsigned>& s)
	{
	  s.reserve(f.domain().nsites());

	  mln_pixter(const I) pix(f);
	  for_all(pix)
	    s.append(pix.offset());

	  std::vector<unsigned>& v = s.hook_std_vector_();
	  std::sort(v.begin(), v.end(), sort_op_less_<T, I>(f));
	}

	template <typename T, typename I>
	static inline
	void
	sort_sites_set_dispatch(const mln::trait::value::quant::low&, const T& tree_, const I& f, util::array<unsigned>& s, util::array<unsigned>& levels)
	{
	  trace::entering("morpho::tree::internal::sort_sites_set_radix");
	  sort_sites_set_radix(tree_, f, s, levels);
	  trace::exiting("morpho::tree::internal::sort_sites_set_radix");
	}

	template <typename T, typename I>
	static inline
	void
	sort_sites_set_dispatch(const mln::trait::value::quant::any&, const T& tree_, const I& f, util::array<unsigned>& s, util::array<unsigned>& levels)
	{
	  (void) levels;
	  trace::entering("morpho::tree::internal::sort_sites_set_std");
	  sort_sites_set_std(tree_, f, s);
	  trace::exiting("morpho::tree::internal::sort_sites_set_std");
	}


	template <typename T, typename I>
	static inline
	void
	sort_sites_set(const T& tree_, const I& f, util::array<unsigned>& s, util::array<unsigned>& levels)
	{
	  sort_sites_set_dispatch(mln_trait_value_quant(mln_value(I)) (), tree_, f, s, levels);
	}

	template <typename T>
	static inline
	unsigned
	zfind_root(T& zpar, unsigned x)
	{
	  mlc_equal(mln_value(T), unsigned)::check();
	  if (zpar.element(x) == x)
	    return x;
	  else
	    return zpar.element(x) = zfind_root(zpar, zpar.element(x));
	}


      } // end of namespace mln::morpho::tree::internal

      namespace impl
      {

	namespace generic
	{

	  template <typename I, typename F>
	  static inline
	  void
	  union_find_fast_process2_(const I& f, const util::array<int>& dp,
				    F& parent, F& area, F& zpar,
				    unsigned& nnodes, unsigned p)
	  {
	    // Make set.
	    const unsigned n_nbhs = dp.nelements();

	    parent.element(p) = p;
	    zpar.element(p) = p;

	    for (unsigned j = 0; j < n_nbhs; ++j)
	      {
		unsigned n = p + dp[j];

		if (zpar.element(n) == 0) // not deja-vu
		  continue;

		unsigned r = internal::zfind_root(zpar, n);
		if (r != p)
		  {
		    parent.element(r) = p;
		    zpar.element(r) = p;
		    area.element(p) += area.element(r);
		    if (f.element(r) != f.element(p))
		      ++area.element(p);
		    else
		      --nnodes;
		  }
	      }
	  }


	  template <typename I, typename F>
	  static inline
	  unsigned
	  union_find_fast_process(const mln::trait::value::quant::low&, const I& f, const util::array<int>& dp,
				  F& parent, F& area, F& zpar,
				  const util::array<unsigned>& s, const util::array<unsigned>& levels)
	  {
	    trace::entering("morpho::tree::impl::union_find_fast_process_low");
	    static const unsigned nvalues = mln_card(mln_value(I));

	    unsigned nnodes = f.domain().nsites();

	    for (int v = nvalues - 1; v >= 0; --v) // Reverse
	      {
		for (int i = levels[v + 1] - 1; i >= (int)levels[v]; --i) // Reverse
		  union_find_fast_process2_(f, dp, parent, area, zpar, nnodes, s[i]);

		// Fast root compression
		for (unsigned i = levels[v]; i < levels[v + 1]; ++i) // Direct
		  {
		    unsigned p = s[i];
		    zpar.element(p) = zpar.element(zpar.element(p));
		  }
	      }

	    trace::exiting("morpho::tree::impl::union_find_fast_process_low");
	    return nnodes;
	  }

	  template <typename I, typename F>
	  static inline
	  unsigned
	  union_find_fast_process(const trait::value::quant::any&, const I& f, const util::array<int>& dp,
				  F& parent, F& area, F& zpar,
				  const util::array<unsigned>& s, const util::array<unsigned>&)
	  {
	    trace::entering("morpho::tree::impl::union_find_fast_process_any");

	    unsigned nnodes = f.domain().nsites();
	    for (int i = s.size() - 1; i >= 0; --i) // Reverse
	      union_find_fast_process2_(f, dp, parent, area, zpar, nnodes, s[i]);

	    trace::exiting("morpho::tree::impl::union_find_fast_process_any");
	    return nnodes;
	  }


	  template <typename T, typename I, typename N>
	  util::ctree::ctree<I>
	  union_find_fast(const tag::tree::tree_t<T>&,
			  const Image<I>& f_,
			  const Neighborhood<N>& nbh_)
	  {
	    trace::entering("morpho::tree::impl::generic::union_find_fast");

	    typedef p_array<mln_psite(I)> S;
	    typedef mln_value(I) V;

	    const I& f   = exact(f_);
	    const N& nbh = exact(nbh_);
	    util::array< unsigned > s;
	    util::array< unsigned > levels;
	    internal::sort_sites_set(T (), f, s, levels);

	    util::array<int> dp = offsets_wrt(f, nbh);

	    // Auxiliary data.
	    mln_ch_value(I, unsigned) parent;
	    mln_ch_value(I, unsigned) area;
	    mln_ch_value(I, unsigned) zpar;

	    initialize(parent, f);
	    initialize(area, f);
	    initialize(zpar, f);

	    // Initialization.
	    data::fill(zpar, 0);
	    data::fill(area, 0);

	    unsigned nnodes =
	      union_find_fast_process(mln_trait_value_quant(V) (), f, dp, parent, area, zpar, s, levels);

	    util::ctree::ctree<I> tree;
	    tree.reserve(f, nnodes);

	    unsigned root_offset = 0;
	    for (unsigned i = 0; i < s.size(); ++i) // Forward
	      {
		unsigned p = s[i];
		unsigned q = parent.element(p);

		if (f.element(parent.element(q)) == f.element(q)) // Canonization
		  q = (parent.element(p) = parent.element(q));

		if (f.element(p) == f.element(q) && p != q) // Not a node.
		  {
		    mln_assertion(q == parent.element(p));
		    tree.data_->map_.element(p) =
		      tree.data_->map_.element(q);
		  }
		else
		  {
		    unsigned& offset = (p == q) ? root_offset : area.element(q);

		    // Insert Node.
		    mln_assertion(offset < nnodes);
		    tree.data_->map_.element(p) = offset;
		    tree.data_->parent_[offset] = tree.data_->map_.element(q);
		    tree.data_->values_[offset] = f.element(p);
		    tree.data_->length_[offset] = area.element(p);
		    area.element(p) = offset + 1;
		    offset += tree.data_->length_[offset] + 1;
		  }
	      }

	    trace::exiting("morpho::tree::impl::generic::union_find_fast");
	    return tree;
	  }

	} // end of namespace mln::morpho::tree::impl::generic

      } // end of namespace mln::morpho::tree::impl

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_MORPHO_TREE_IMPL_UNION_FIND_FAST_HH
