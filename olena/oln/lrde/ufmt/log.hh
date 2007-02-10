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

#ifndef OLENA_LRDE_UFMT_LOG_HH
# define OLENA_LRDE_UFMT_LOG_HH

# include <map>
# include <oln/level/fill.hh>


namespace oln
{

  namespace lrde
  {

    namespace ufmt
    {

      // couting

      template <class Algo>
      unsigned n_level_roots(const Algo& algo)
      {
	typedef typename Algo::image I;
	unsigned count = 0;
	oln_iter_type(I) p(algo.f_());
	for_all(p)
	  if (algo.is_level_root_(p))
	    ++count;
	return count;
      }

      template <class Algo>
      unsigned n_roots(const Algo& algo)
      {
	typedef typename Algo::image I;
	unsigned count = 0;
	oln_iter_type(I) p(algo.f_());
	for_all(p)
	  if (algo.is_root_(p))
	    ++count;
	return count;
      }



      // building images

      template <class Algo>
      typename Algo::bin_image_t // e.g., image2d<bool>
      level_root_image(const Algo& algo)
      {
	typedef typename Algo::bin_image_t B;
	B lroot(algo.f_().size());
	level::fill(lroot, false);
	oln_iter_type(B) p(algo.f_());
	for_all(p)
	  if (algo.is_level_root_(p))
	    lroot[p] = true;
	return lroot;
      }


      template <class Algo>
      typename Algo::int_u8_image_t
      tree_image(const Algo& algo, bool print_counts = false)
      {
	typedef typename Algo::point point;
	enum {
	  unknown = 0, // temporary label

	  // tree root (black)
	  root = 1,

	  // the following labels are for "not tree root" points

	  // level roots are:
	  // - intermediate (red) or
	  intermediate_level_root = 2,
	  // - leaf (green)
	  leaf_level_root = 3,         // not root

	  // some points belong to a path between a couple of level roots
	  // (white)
	  in_between_level_point = 6,

	  // otherwise points are:
	  // - leaf (blue)
	  leaf_point = 4,
	  // - intermediate (yellow)
	  intermediate_point = 5
	};

	typedef typename Algo::int_u8_image_t T;
	T tree(algo.f_().size());
	level::fill(tree, unknown);

	oln_iter_type(T) p(algo.f_());

	for_all(p)
	  if (algo.is_root_(p))
	    tree[p] = root;
	  else if (algo.is_level_root_(p))
	    tree[p] = leaf_level_root; // but we do not know about 'leaf'

	for_all(p)
	  if (algo.is_level_root_(p))
	    {
	      point q = p; // from p level root
	      do
		{
		  q = algo.parent_(q);
		  if (not algo.is_level_root_(q))
		    // we reach a non level root point
		    tree[q] = in_between_level_point;
		}
	      while (not algo.is_level_root_(q));
	      // level root p --> level root q
	      if (q != p and not algo.is_root_(q))
		tree[q] = intermediate_level_root; // not leaf level root
	    }

	for_all(p)
	  {
	    point par_p = algo.parent_(p);
	    if (tree[par_p] == unknown)
	      tree[par_p] = intermediate_point;
	  }
	for_all(p)
	  if (tree[p] == unknown)
	    tree[p] = leaf_point;

	// printing
	if (print_counts)
	  {
	    unsigned
	      n_roots = 0,
	      n_intermediate_level_roots = 0,
	      n_leaf_level_roots = 0,
	      n_in_between_level_points = 0,
	      n_leaf_points = 0,
	      n_intermediate_points = 0;

	    for_all(p)
	      switch (tree[p]) {
	      case root:
		++n_roots;
		break;
	      case intermediate_level_root:
		++n_intermediate_level_roots;
		break;
	      case leaf_level_root:
		++n_leaf_level_roots;
		break;
	      case in_between_level_point:
		++n_in_between_level_points;
		break;
	      case leaf_point:
		++n_leaf_points;
		break;
	      case intermediate_point:
		++n_intermediate_points;
		break;
	      }

	    std::cout << "n roots               = " << n_roots << std::endl
		      << "n intermediate lroots = " << n_intermediate_level_roots << std::endl
		      << "n leaf lroots         = " << n_leaf_level_roots << std::endl
		      << "n in-between lpoint   = " << n_in_between_level_points << std::endl
		      << "n leaf points         = " << n_leaf_points << std::endl
		      << "n intermediate points = " << n_intermediate_points << std::endl;

	  }

	return tree;
      }



      // comparing images

      template <class I1, class I2>
      bool
      check_equiv_label_images(const abstract::image<I1>& label1,
			       const abstract::image<I2>& label2)
      {
	assert(label1.size() == label2.size());
	std::map<unsigned, unsigned> m;
	oln_iter_type(I1) p(label1);
	for_all(p)
	  {
	    if (m.find(label1[p]) == m.end())
	      m[label1[p]] = label2[p];
	    else
	      if (label2[p] != m[label1[p]])
		return false;
	  }
	return true;
      }



      // invariant checking

      template <class Algo>
      bool check_f_par_p_leq_f_p(const Algo& algo)
	// f(par(p)) <= f(p)
      {
	typedef typename Algo::image I;
	oln_iter_type(I) p(algo.f_());
	for_all(p)
	  if (not (algo.f_(algo.parent_(p)) <= algo.f_(p)))
	    {
	      std::cerr << "invariant FAILED: f(par(p)) <= f(p)"
			<< std::endl;
	      return false;
	    }
	return true;
      }



      // some tests

      template <class Algo>
      bool is_level_compression_completed(const Algo& algo)
      {
	typedef typename Algo::image I;
	oln_iter_type(I) p(algo.f_());
	for_all(p)
	  if (not is_level_root(algo.parent_(p)))
	    {
	      std::cerr << "test FAILED: level compression is NOT completed"
			<< std::endl;
	      return false;
	    }
	return true;
      }


    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln


#endif // ! OLENA_LRDE_UFMT_LOG_HH
