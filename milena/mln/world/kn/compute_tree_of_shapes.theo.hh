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

# include <mln/value/dec.hh>
# include <mln/value/inc.hh>
# include <mln/value/interval.hh>
# include <mln/value/is_degenerated.hh>

# include <mln/pw/all.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <mln/world/kn/hqueue.hh>
# include <mln/util/tree_of_shapes.hh>
# include <mln/world/kn/is_2_face.hh>
# include <mln/world/kn/debug/println.hh>


// FIXME: to be removed or disabled.
# include <mln/world/kn/internal/display.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {


      template <typename I>
      struct tree_
      {
	typedef mln_site(I) P;

	util::array<P> R;
	mln_ch_value(I, P) parent;
	mln_concrete(I) Fb;
      };


      /// \brief Compute the tree of shape from a Kn 2D image.
      /*!

	\param[in] F A 2D image in Kn. It's values are considered of
	type value::interval<>.
	\param[in] inter Interval of the possible values in \p F.

      */
      template <typename I, typename V>
      tree_<mln_ch_value(I, V)>
      compute_tree_of_shapes(const Image<I>& F, const value::interval<V>& inter,
			     const mln_site(I)& p_infty);

      // /// \overload
      // template <typename I, typename IV, typename U>
      // util::tree_of_shapes<I>
      // compute_tree_of_shapes(const Image<I>& F, const value::interval<IV>& inter,
      // 			     const mln_site(I)& p_infty, const U& value);


#  ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

	template <typename Q, typename V>
	V
	upper_level_next_to_lcur(const Q& q, V lcur)
	{
	  const value::interval<V>& inter = q.inter();
	  unsigned i = inter.index_of(lcur), n = inter.nelements();
	  while (i < n && q.is_empty_at_index(i))
	    i += 1;
	  if (i == n)
	    // not found
	    return lcur;
	  else
	    return inter.ith_element(i);
	}

	template <typename Q, typename V>
	V
	lower_level_next_to_lcur(const Q& q, V lcur)
	{
	  const value::interval<V>& inter = q.inter();
	  int i = inter.index_of(lcur);
	  while (i >= 0 && q.is_empty_at_index(i))
	    i -= 1;
	  if (i == -1)
	    // not found
	    return lcur;
	  else
	    return inter.ith_element(i);
	}


	template <typename Q, typename V>
	V
	level_next_to_lcur(const Q& q, V lcur)
	{
	  static bool up = true;

	  mln_precondition(! q.is_empty());
	  mln_precondition(q.is_empty_at(lcur));

	  V l_;
	  if (up)
	    {
	      l_ = upper_level_next_to_lcur(q, lcur);
	      if (l_ != lcur) // found
		return l_;
	      up = false;
	      return lower_level_next_to_lcur(q, lcur);
	    }
	  else
	    {
	      l_ = lower_level_next_to_lcur(q, lcur);
	      if (l_ != lcur) // found
		return l_;
	      up = true;
	      return upper_level_next_to_lcur(q, lcur);
	    }
	}

      } // internal


      template <typename Q, typename V>
      mln_element(Q)
      pop(Q& q, V& lcur)
      // modify q, and sometimes l
      {
	mlc_equal(V, typename Q::priority)::check();
	if (q.is_empty())
	  std::abort();
	if (q.is_empty_at(lcur))
	  {
	    lcur = internal::level_next_to_lcur(q, lcur);
	    mln_postcondition(! q.is_empty_at(lcur));
	  }
	return q.pop_at(lcur);
      }


      template <typename Q, typename V>
      void
      push(Q& q, const mln_element(Q)& p, const value::interval<V>& i, V lcur)
      // modify q
      {
	mlc_equal(V, typename Q::priority)::check();
	V l_,
	  lower = i.first(),
	  upper = i.last();
	if (lower > lcur)
	  l_ = lower;
	else if (upper < lcur)
	  l_ = upper;
	else
	  l_ = lcur;
	q.push_at(p, l_);
      }


      template <typename I, typename T, typename V>
      void
      sort(const Image<I>& F_, tree_<T>& t, const value::interval<V>& inter,
	   const mln_site(I)& p_infty, V l_infty)
      {
	trace::entering("mln::world::kn::sort");
	const I& F = exact(F_);

	mln_precondition(F.is_valid());
	mln_precondition(F.domain().has(p_infty));
	mln_precondition(F(p_infty).has(l_infty));

	typedef mln_site(I) P;

	mln_domain(I) D = F.domain();

	typedef mln_concrete(T) Fb_t;
	t.Fb = Fb_t(D);
	util::array<P> R;

	mln_ch_value(I, bool) deja_vu(D), done(D);

	hqueue<P,V> q(inter);

	data::fill(deja_vu, false);
	data::fill(done, false);  // p is in queue if p is 'deja_vu' but not 'done'

	q.push_at(p_infty, l_infty);
	deja_vu(p_infty) = true;

	V lcur = l_infty;

	while (! q.is_empty())
	  {
	    P p = pop(q, lcur);

	    kn::debug::println("deja_vu", deja_vu);
	    kn::debug::println( "done", done);
	    kn::debug::println( deja_vu | k2::is_primary_2_face );
	    // std::cout << p << std::endl;

	    t.Fb(p) = lcur;
	    t.R.append(p);

	    mln_niter(neighb2d) n(c4(), p);
	    for_all(n) if (D.has(n) && deja_vu(n) == false)
	      {
		push(q, n, F(n), lcur);
		deja_vu(n) = true;
	      }
	    done(p) = true;

	    if (q.is_empty_at(lcur))
	      {
		std::cout << "sort: done with level " << lcur << std::endl;
		// FIXME
	      }
	  }

      }



      template <typename T>
      void
      do_union(tree_<T>& t) // P p_, P r_, T& zpar, U& rank, U& last)
      // modify zpar, rank, and last
      {
      }


      template <typename Z, typename P>
      void
      find_root(Z& zpar, P x)
      // modify zpar
      {
	if (zpar(x) == x)
	  return x;
	else {
	  zpar(x) = find_root(zpar, zpar(x));
	  return zpar(x);
	}
      }


      template <typename T>
      void
      union_find(tree_<T>& t)
      {
      }


      template <typename T>
      void
      canonicalize_tree(tree_<T>& t)
      // modify parent
      {
      }


      template <typename I, typename V>
      tree_<mln_ch_value(I, V)>
      compute_tree_of_shapes(const Image<I>& F, const value::interval<V>& inter,
			     const mln_site(I)& p_infty, V l_infty)
      {
	mlc_equal(mln_value(I), value::interval<V>)::check();

	tree_<mln_ch_value(I, V)> t;

	sort(F, t, inter, p_infty, l_infty);
	union_find(t);
	canonicalize_tree(t);

	return t;
      }


      template <typename I, typename V>
      tree_<mln_ch_value(I, V)>
      compute_tree_of_shapes(const Image<I>& F, const value::interval<V>& inter,
			     const mln_site(I)& p_infty)
      {
	mlc_equal(mln_value(I), value::interval<V>)::check();
	
	tree_<mln_ch_value(I, V)> t;

	V l_infty = exact(F)(p_infty).first();
	sort(F, t, inter, p_infty, l_infty); // FIXME
	union_find(t);
	canonicalize_tree(t);

	return t;
      }



# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln


#endif // ! MLN_WORLD_KN_COMPUTE_TREE_OF_SHAPES_HH
