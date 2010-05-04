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

#ifndef PARALLEL_UNION_FIND_HH
# define PARALLEL_UNION_FIND_HH

#include <tbb/tick_count.h>

# include <mln/core/image/image2d.hh>
# include <mln/data/sort_offsets.hh>
# include <mln/extension/adjust.hh>
# include <mln/extension/fill.hh>

# include <sstream>
# include "parallel_apply.hh"

#define BENCH_START(TASK)				\
  { tbb::tick_count t0 = tbb::tick_count::now();	\
  std::stringstream task__;				\
  task__ << TASK;					\
  std::cout << "Start task: " << task__.str() << std::endl;


#define BENCH_END()							\
  tbb::tick_count t1 = tbb::tick_count::now();				\
  std::cout << "End   task: " << task__.str() << " in " << (t1-t0).seconds() << std::endl; \
  }




namespace mln
{

template <typename I, typename N>
struct ll_union_find : public Parallel_app< ll_union_find<I, N> >
{
  typedef mln_domain(I) domain_t;

  ll_union_find(const Image<I>& f, const Neighborhood<N>& nbh,
		mln_ch_value(I, unsigned)& parent,
		mln_ch_value(I, unsigned)& zpar);

  ll_union_find(ll_union_find<I, N>&, tbb::split);

  void operator() (const tbb::blocked_range<int>& subrange);
  void join(ll_union_find<I, N>& o);

  void build(const box2d& subdomain);
  void merge(const box2d& domain);

private:
  typedef mln_value(I) T;

  const I& f;
  const N& nbh;
  box2d	domain;
public:
  mln_ch_value(I, unsigned)& parent;
  mln_ch_value(I, unsigned)& zpar;
};

  template <typename I, typename N>
  mln_ch_value(I, unsigned)
    compute_max_tree(const Image<I>& f_, const Neighborhood<N>& nbh_);

# ifndef MLN_INCLUDE_ONLY

template <typename I, typename N>
inline
ll_union_find<I, N>::ll_union_find(const Image<I>& f_, const Neighborhood<N>& nbh_,
				   mln_ch_value(I, unsigned)& parent_,
				   mln_ch_value(I, unsigned)& zpar_)
  : f	 (exact(f_)),
    nbh (exact(nbh_)),
    parent (parent_),
    zpar (zpar_)
{
  mln_precondition(f.is_valid());
  mln_precondition(nbh.is_valid());
  mln_precondition(parent.is_valid());
  mln_precondition(zpar.is_valid());
  domain.pmax() = point2d(-1, -1);
}

template <typename I, typename N>
inline
ll_union_find<I, N>::ll_union_find(ll_union_find<I, N>& o, tbb::split) :
  f (o.f),
  nbh (o.nbh),
  parent (o.parent),
  zpar (o.zpar)
{
  mln_precondition(f.is_valid());
  mln_precondition(nbh.is_valid());
  mln_precondition(parent.is_valid());
  mln_precondition(zpar.is_valid());
}


namespace internal
{

  template <typename Z>
  inline
  unsigned zfind_root(Z& zpar, unsigned x)
  {
    mlc_equal(mln_value(Z), unsigned)::check();
    if (zpar.element(x) == x)
      return x;
    else
      return zpar.element(x) = zfind_root(zpar, zpar.element(x));
  }

  template <typename T>
  void
  radix_decreasing_sort(const image2d<T>& f, const box2d& subdomain,
			util::array< util::array<unsigned> >& s)
  {
    const unsigned n = unsigned(mln_max(T)) + 1;
    s.resize(n);

    // histogram computation
    util::array<unsigned> h(n, 0);

    // Hack because f | subdomain is not fastest .i.e no pixter
    const int stride = f.delta_index(dpoint2d(1, 0));

    {
      const T* offset = &f(subdomain.pmin());
      for (int y = subdomain.pmin()[0]; y <= subdomain.pmax()[0]; ++y, offset += stride)
    	{
    	  const T* pxl = offset;
    	  for (int x = subdomain.pmin()[1]; x <= subdomain.pmax()[1]; ++x, ++pxl)
	    ++h[*pxl];
    	}
    }

    for (unsigned v = 0; v < n; ++v)
      s[v].reserve(h[v]);

    // computing output data
    {
      unsigned offset =
	(f.border() + subdomain.pmin()[0]) * stride +
    	(f.border() + subdomain.pmin()[1]);
      assert(offset == f.index_of_point(subdomain.pmin()));
      for (int y = subdomain.pmin()[0]; y <= subdomain.pmax()[0]; ++y, offset += stride)
    	for (int x = subdomain.pmin()[1], pxl = offset; x <= subdomain.pmax()[1]; ++x, ++pxl)
	  s[f.element(pxl)].append(pxl);
    }
  }

  template <typename T>
  inline
  void
  find_node(const image2d<T>& f, image2d<unsigned>& parent, unsigned& x)
  {
    unsigned& z = parent.element(x);
    if (f.element(z) == f.element(x) && z != x)
      {
	find_node(f, parent, z);
	x = z;
      }
  }


  /// Merge two trees.
  ///
  ///
  /// \param f 
  /// \param parent 
  /// \param x 
  /// \param y 
  ///
  template <typename T>
  inline
  void
  connect(const image2d<T>& f, image2d<unsigned>& parent,
	  unsigned x, unsigned y)
  {
    find_node(f, parent, x);
    find_node(f, parent, y);

    if (f.element(y) > f.element(x)) // fixme: max tree only
      std::swap(x, y);
    while (x != y)
      if (parent.element(x) == x) // root
	{
	  parent.element(x) = y;
	  return;
	}
      else
	{
	  unsigned z = parent.element(x);
	  find_node(f, parent, z);
	  if (f.element(z) < f.element(y))
	    {
	      parent.element(x) = y;
	      x = y;
	      y = z;
	    }
	  else
	    x = z;
	}
  }


}

template <typename I, typename N>
void
ll_union_find<I, N>::operator() (const tbb::blocked_range<int>& r)
{
  if (!this->domain.is_valid())
    {
      this->domain = f.domain();
      this->domain.pmin()[0] = r.begin();
      this->domain.pmax()[0] = r.end() - 1;
      this->build(this->domain);
    }
  else
    {
      box2d subdomain = f.domain();
      subdomain.pmin()[0] = r.begin();
      subdomain.pmax()[0] = r.end() - 1;

      this->build(subdomain);
      std::cout << "Warning: Not parallel merge. " << std::endl;
      this->merge(subdomain);
      this->domain.pmax() = subdomain.pmax();
    }
}


template <typename I, typename N>
void
ll_union_find<I, N>::build(const box2d& subdomain)
{
  BENCH_START("Build: " << subdomain)

  mln_precondition(subdomain.is_valid());
  const unsigned n_values = unsigned(mln_max(T)) + 1;

  util::array<int> dp = offsets_wrt(f, nbh);
  const unsigned n_nbhs = dp.nelements();

  util::array< util::array<unsigned> > s;
  radix_decreasing_sort(f, subdomain, s); // so a max-tree

  const int stride = f.delta_index(dpoint2d(1, 0));

  // Tree construction.
  for (int v = n_values - 1; v >= 0; --v)
    {
      const util::array<unsigned>& s_v = s[v];
      unsigned n_p = s_v.nelements();
      for (unsigned i = 0; i < n_p; ++i)
	{
	  unsigned p = s_v[i];
	  parent.element(p) = p;
	  zpar.element(p) = p;
	  for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      unsigned n = p + dp[j];
	      if (zpar.element(n) == 0) // not deja vu  || << 3s a l'exec
		continue;
	      int col = (n / stride - f.border());
	      if (subdomain.pmin()[0] > col || col > subdomain.pmax()[0]) // !subdomain.has(f.point_at_index(n))
		continue;

	      unsigned r = zfind_root(zpar, n);
	      if (r != p)
		{
		  parent.element(r) = p;
		  zpar.element(r) = p;
		}
	    }
	}

      // Finalizing compression per gray-level.
      for (int i = n_p - 1; i >= 0; --i)
	{
	  unsigned p = s_v[i];
	  zpar.element(p) = zpar.element(zpar.element(p));
	}
    }
  BENCH_END()
}


template <typename I, typename N>
void
ll_union_find<I, N>::merge(const box2d& d2)
{
  const int stride = f.delta_index(dpoint2d(1, 0));
  box2d& d1 = this->domain;

  BENCH_START("Merge: " << d1 << "&" << d2)

  // Split along x axis
  assert(d1.pmin()[1] == d2.pmin()[1]);
  assert(d1.pmax()[1] == d2.pmax()[1]);
  assert((d1.pmax()[0] + 1) == d2.pmin()[0]);

  int x2 = f.index_of_point(d2.pmin());
  int x1 = x2;
  x1 -= stride;
  for (int x = d1.pmin()[1]; x <= d1.pmax()[1]; ++x)
    {
      internal::connect(f, parent, x1, x2);
      ++x1;
      ++x2;
    }

  BENCH_END()
}

/// Merge two trees.
///
///
template <typename I, typename N>
void
ll_union_find<I, N>::join(ll_union_find& o)
{
  this->merge(o.domain);
  this->domain.pmax() = o.domain.pmax();
}

  template <typename I, typename N>
  mln_ch_value(I, unsigned)
    compute_max_tree(const Image<I>& f_, const Neighborhood<N>& nbh_)
  {
    const I& f = exact(f_);
    const N& nbh = exact(nbh_);

    mln_precondition(f.is_valid());
    mln_precondition(nbh.is_valid());

    extension::adjust(f, nbh);
    extension::fill(f, mln_max(mln_value(I)));

    typedef mln_ch_value(I, unsigned) P;
    P parent, zpar;
    BENCH_START("Compute Max Tree");
    initialize(zpar, f);
    initialize(parent, f);
    data::fill(zpar, 0);
    //data::fill(parent, 69);

    ll_union_find<I, N> fun(f, nbh, parent, zpar);
    parallel_apply(f, fun);

    BENCH_START("Canonisation");
    mln_pixter(P) px(parent);
    for_all(px)
    {
      unsigned q = parent.element(px.val());
      if (f.element(q) == f.element(px.val()))
	px.val() = q;
    }
    BENCH_END();

    BENCH_END()
    return parent;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

#endif // !PARALLEL_UNION_FIND_HH
