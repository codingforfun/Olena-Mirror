// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///
/// \brief Compute the tree of shape from a Kn 2D image.

#ifndef MLN_WORLD_KN_COMPUTE_TREE_OF_SHAPES_HH
# define MLN_WORLD_KN_COMPUTE_TREE_OF_SHAPES_HH

# include <cstdlib>
# include <vector>
# include <utility>

# include <mln/core/alias/neighb2d.hh>

# include <mln/data/compute.hh>

# include <mln/accu/math/span.hh>

# include <mln/value/dec.hh>
# include <mln/value/inc.hh>

# include <mln/util/hqueue.hh>
# include <mln/util/tree_of_shapes.hh>
# include <mln/world/kn/is_2_face.hh>


// FIXME: to be removed or disabled.
# include <mln/world/kn/internal/display.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {

      /// \brief Compute the tree of shape from a Kn 2D image.
      template <typename I>
      util::tree_of_shapes<I>
      compute_tree_of_shapes(const Image<I>& F);


#  ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename I>
	struct compute_tree_of_shapes_t
	{
	  typedef mln_value(I) V;
	  typedef mln_equiv(V) EV;
	  typedef mln_site(I) P;
	  typedef mln_box(I) B;

	  typedef mln_ch_value(I,P) T;
	  typedef mln_ch_value(I,unsigned) U;
	  typedef std::vector<P> Array_P;
	  typedef util::hqueue<P,EV> q_type;

	  compute_tree_of_shapes_t();

	  util::tree_of_shapes<I> operator()(const I& F_);

	  void do_union(P p_, P r_, T& zpar, U& rank, U& last);

	  P find_root(T& zpar, P x);

	  T union_find(const Array_P& R, display& dsp);

	  void priority_push(q_type& q, const P& p, const I& F);
	  P priority_pop(q_type& q);

	  EV upper_level_next_to_lcur(q_type& q, bool& found);

	  EV lower_level_next_to_lcur(q_type& q, bool& found);

	  EV level_next_to_lcur(q_type& q);

	  void sort(const Image<I>& F_, util::tree_of_shapes<I>& t, display& dsp);
	  void canonicalize_tree(util::tree_of_shapes<I>& t);


	protected:
	  // Attributes
	  mln_equiv(V) lcur;

	  P undef;
	  P p_0;

	  unsigned N;
	  box2d D;

	  V vspan_;
	};


	template <typename I>
	void
	compute_tree_of_shapes_t<I>::do_union(P p_, P r_,
					      T& zpar, U& rank, U& last)
	// modify zpar, rank, and last
	{
	  if (rank(p_) > rank(r_))
	  {
	    // new root is p_
	    zpar(r_) = p_;
	    if (last(r_) < last(p_))
	      last(p_) = last(r_);
	  }
	  else
	  {
	    // new root is r_
	    zpar(p_) = r_;
	    if (last(p_) < last(r_))
	      last(r_) = last(p_);
	    if (rank(p_) == rank(r_))
	      rank(r_) = rank(r_) + 1;
	  }
	}


	template <typename I>
	typename compute_tree_of_shapes_t<I>::P
	compute_tree_of_shapes_t<I>::find_root(T& zpar, P x)
	// modify zpar
	{
	  if (zpar(x) == x)
	    return x;
	  else {
	    zpar(x) = find_root(zpar, zpar(x));
	    return zpar(x);
	  }
	}

	template <typename I>
	typename compute_tree_of_shapes_t<I>::T
	compute_tree_of_shapes_t<I>::union_find(const Array_P& R,
						display& dsp)
	// with balancing
	{
	  T zpar(D), parent(D);
	  U rank(D), last(D);
	  mln_ch_value(I,bool) done(D);
	  mln_piter(B) p(D);

	  for_all(p)
	  {
	    zpar(p) = undef;
	    rank(p) = 0;
	    done(p) = false;
	  }
	  for (int i = N - 1; i >= 0; --i)
	  {
	    P p = R[i];  // p goes from leaves to root
	    parent(p) = p;
	    zpar(p) = p;
	    last(p) = i;
	    mln_niter(neighb2d) n(c4(), p);
	    for_all(n) if (D.has(n) && zpar(n) != undef)
	    {
	      P p_ = find_root(zpar, p),
		r_ = find_root(zpar, n);
	      if (r_ != p_)
	      {
		P r = R[ last(r_) ];
		parent(r) = p;
		do_union(p_, r_, zpar, rank, last);  // update zpar
	      }
	    }
	    done(p) = true;

	    if (dsp.level_changes_at(i))
	    {
	      std::cout << "union-find: done with level " << dsp.level(p) << std::endl;
	      dsp.show(done);
	    }
	  }
	  return parent;
	}



	// std::ostream&
	// operator<<(std::ostream& ostr, const q_type& q)
	// {
	// 	ostr << "q = ";
	// 	for (unsigned i = 0; i < q.size(); ++i)
	// 	  if (! q[i].empty())
	// 	    ostr << i << ':' << q[i].size() << "  ";
	// 	return ostr;
	// }


	template <typename I>
	void
	compute_tree_of_shapes_t<I>::priority_push(q_type& q, const P& p, const I& F)
	// modify q
	{
	  EV
	    lcur_,
	    lower = F(p).first(),
	    upper = F(p).last();
	  if (lower > lcur)
	    lcur_ = lower;
	  else if (upper < lcur)
	    lcur_ = upper;
	  else
	    lcur_ = lcur;
	  q.push(p, lcur_);
	}


	template <typename I>
	typename compute_tree_of_shapes_t<I>::EV
	compute_tree_of_shapes_t<I>::upper_level_next_to_lcur(q_type& q, bool& found)
	{
	  for (EV l_ = lcur; l_ <= vspan_.last(); value::inc(l_))
	    if (! q[l_].empty())
	    {
	      found = true;
	      return l_;
	    }
	  found = false;
	  return EV();
	}

	template <typename I>
	typename compute_tree_of_shapes_t<I>::EV
	compute_tree_of_shapes_t<I>::lower_level_next_to_lcur(q_type& q, bool& found)
	{
	  for (EV l_ = lcur; l_ >= vspan_.first(); value::dec(l_))
	    if (! q[l_].empty())
	    {
	      found = true;
	      return l_;
	    }
	  found = false;
	  return EV();
	}


	template <typename I>
	typename compute_tree_of_shapes_t<I>::EV
	compute_tree_of_shapes_t<I>::level_next_to_lcur(q_type& q)
	{
	  EV l_;
	  bool found;

	  bool up = int(2. * std::rand() / (RAND_MAX + 1.));
	  if (up)
	  {
	    l_ = upper_level_next_to_lcur(q, found);
	    if (found)
	      return l_;
	    else
	    {
	      l_ = lower_level_next_to_lcur(q, found);
	      if (! found)
		std::abort();
	      return l_;
	    }
	  }
	  else
	  {
	    l_ = lower_level_next_to_lcur(q, found);
	    if (found)
	      return l_;
	    else
	    {
	      l_ = upper_level_next_to_lcur(q, found);
	      if (! found)
		std::abort();
	      return l_;
	    }
	  }
	}


	template <typename I>
	typename compute_tree_of_shapes_t<I>::P
	compute_tree_of_shapes_t<I>::priority_pop(q_type& q)
	// modify q, and sometimes l
	{
	  if (q[lcur].empty())
	  {
	    EV lcur_ = level_next_to_lcur(q);  // such as q[lcur_] is not empty
	    if (q[lcur_].empty())
	      std::abort();
	    lcur = lcur_;
	  }
	  return q.pop(lcur);
	}




	template <typename I>
	void
	compute_tree_of_shapes_t<I>::sort(const Image<I>& F_,
					  util::tree_of_shapes<I>& t, display& dsp)
	{
	  trace::entering("mln::world::kn::sort");
	  mln_precondition(exact(F_).is_valid());

	  const I& F = exact(F_);

	  typedef mln_site(I) P;

	  B D = F.domain();
	  mln_concrete(I) Fb(D);
	  Array_P R(F.nsites());

	  mln_ch_value(I,bool) deja_vu(D), done(D);
	  mln_piter(B) p(D);

	  q_type q(vspan_);

	  for_all(p)
	  {
	    deja_vu(p) = false;
	    done(p) = false;
	    // p is in queue if p is 'deja_vu' but not 'done'
	  }
	  unsigned i = 0;
	  lcur = safe_cast_to<EV>(F(p_0));   // the start level is the one of root
	  priority_push(q, p_0, F);  // realize push(q[lcur], p_0)
	  deja_vu(p_0) = true;
	  do
	  {
	    P p = priority_pop(q);

	    Fb(p) = lcur;
	    R[i] = p;
	    mln_niter(neighb2d) n(c4(), p);
	    for_all(n) if (D.has(n) && deja_vu(n) == false)
	    {
	      priority_push(q, n, F);
	      deja_vu(n) = true;
	    }
	    i = i + 1;
	    done(p) = true;

	    if (q[lcur].empty())
	    {
	      std::cout << "sort: done with level " << lcur << std::endl;
	      dsp.show(done);
	    }
	  }
	  while (i != N);

	  t.Fb = Fb;
	  t.R = R;
	}


	template <typename I>
	void
	compute_tree_of_shapes_t<I>::canonicalize_tree(util::tree_of_shapes<I>& t)
	// modify parent
	{
	  const I& Fb = t.Fb;
	  const Array_P& R = t.R;
	  T& parent = t.parent;

	  for (unsigned i = 0; i <= N - 1; ++i)
	  {
	    P p = R[i];  // p goes from root to leaves
	    P q = parent(p);
	    if (Fb(parent(q)) == Fb(q))
	      parent(p) = parent(q);
	  }

	  mln_ch_value(I,bool) show(D);
	  for (unsigned i = 0; i <= N - 1; ++i)
	  {
	    P p = R[i];
	    show(p) = k2::is_primary_2_face(p) || t.is_representative(p);
	  }

	  for (unsigned i = 0; i <= N - 1; ++i)
	  {
	    P p = R[i];  // p goes from root to leaves
	    if (! show(p))
	      continue;
	    P q = parent(p);

	    if (show(q) == false) // skip node q
	    {
	      if (parent(q) == q) // q cannot be root
		std::abort();

	      P r = parent(q); // new representative
	      if (p != r)  // if p is a repr node, do nothing
		parent(p) = r;
	    }
	    else
	      if (Fb(q) == Fb(p) && k2::is_primary_2_face(p) && ! k2::is_primary_2_face(q))
	      {
		show(q) = false;

		if (parent(q) == q) // q is root
		  parent(p) = p;  // p is the new root
		else
		  parent(p) = parent(q); // new parent of the representative
		parent(q) = p; // the new representative is p, stored as q's parent
	      }
	  }
	  t.show = show;
	}


	template <typename I>
	compute_tree_of_shapes_t<I>::compute_tree_of_shapes_t()
	  : undef(-1, -1), p_0(-1, -1)
	{
	}

	template <typename I>
	util::tree_of_shapes<I>
	compute_tree_of_shapes_t<I>::operator()(const I& F)
	{
	  trace::entering("mln::world::kn::compute_tree_of_shapes");
	  mln_precondition(F.is_valid());

	  /// FIXME: Really ?
	  /// Useful while sorting sites and constructing the hqueue.
	  accu::math::span<V> accu;
	  vspan_ = data::compute(accu, F | kn::is_2_face);
	  /// End of FIXME

	  N = F.nsites();
	  D = F.domain();

	  util::tree_of_shapes<I> t;

	  display_in_K2<util::tree_of_shapes<I> > dsp(t, std::cout);
	  sort(F, t, dsp);

	  t.parent = union_find(t.R, dsp);
	  canonicalize_tree(t);

	  return t;
	}

      } // end of namespace mln::world::kn::internal



      template <typename I>
      util::tree_of_shapes<I>
      compute_tree_of_shapes(const Image<I>& F)
      {
	trace::entering("mln::world::kn::compute_tree_of_shapes");
	mln_precondition(exact(F).is_valid());

	util::tree_of_shapes<I>
	  t = internal::compute_tree_of_shapes_t<I>()(exact(F));

	trace::exiting("mln::world::kn::compute_tree_of_shapes");
	return t;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

 } // end of namespace mln


#endif // ! MLN_WORLD_KN_COMPUTE_TREE_OF_SHAPES_HH
