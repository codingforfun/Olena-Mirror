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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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


/** \brief An adadpatation of Fiorio's and Gustedt's algorithm
    (fiorio.96.tcs).

    Ref.: Christophe Fiorio and Jens Gustedt.  Two linear time
    Union-Find strategies for image processing.  Theoretical Computer
    Science, 1996.  

    \note This is the second version of my implementation, augmented
    with ThÅÈo's (better) routines and ideas.  */


#ifndef OLENA_LRDE_UFMT_FIORIO_HH
# define OLENA_LRDE_UFMT_FIORIO_HH

# include <algorithm>

# include <oln/core/image2d.hh>
# include <oln/level/fill.hh>

namespace oln
{
  namespace lrde
  {
    namespace ufmt
    {

      // FIXME: Not generic (works only on 2-D images).
      template <typename I>
      class fiorio
      {
	typedef oln_point_type(I) point;
	typedef oln_value_type(I) value;
	typedef oln_iter_type(I) piter;
	// typedef oln_neighborhood_type(I) Nbh;
	// typedef oln_iter_type(Nbh) niter;
	typedef typename mute<I, point>::ret parent_type;

	// FIXME: Should be elsewhere!
	typedef oln::image2d<unsigned> area_type;

      public:
	fiorio(const abstract::image<I>& ima) :
	  ima_(ima.exact().clone()), parent_(ima.size())
	  , n_level_roots_cpt_(ima.npoints())
//      , area_(ima_.size())
	{
	  // Init the image of parents.
	  piter p (parent_);
	  for_all (p)
	    parent_[p] = p;

	  // FIXME: Should be elsewhere!
	  // Init the image of data.
// 	  oln::level::fill (area_, 1);
	}

	/// Entry point of the algorithm.
	void go()
	{
	  scan_line();
	}


	/// Images accessors.
	/// \{
      public:
	const I& ima() const
	{
	  return ima_;
	}

	const parent_type& parent() const
	{
	  return parent_;
	}
	/// \}


	/// Level roots.
	/// \{
	/// Get the number of level roots (method 1, using a counter
	/// updated by the algorithm).
	unsigned n_level_roots1() const
	{
	  return n_level_roots_cpt_;
	}

	/// Get the number of level roots (method 2, computing from the
	/// PARENT image.
	unsigned n_level_roots2() const
	{
	  unsigned n_level_roots = 0;
	  oln_iter_type(parent_type) p(parent_);
	  for_all (p)
	    if (is_level_root(p))
	      ++n_level_roots;
	  return n_level_roots;
	}

	/// The type of an image of level roots
	typedef typename mute<parent_type, bool>::ret level_roots_image_type;

	/// \brief Compute the image of level roots.
	///
	/// A value of \a true means that the point is a level root in ima.
	level_roots_image_type level_roots() const
	{
 	  // Image of level roots.
	  level_roots_image_type level_roots (ima_.size());
	  oln_iter_type(parent_type) p(ima_);
	  for_all (p)
	    level_roots[p] = is_level_root(p);
	  return level_roots;
	}
	/// \}


      protected:
	// Improve to handle other neighborhoods than 4-c.
	void scan_line()
	{
	  // Special treatment of the first line (no merge).
	  build_tree_of_line(0);
	  for (coord i = 1; i < ima_.nrows(); ++i)
	    {
	      build_tree_of_line(i);
	      // Merge the tree of the current line with the rest of the image.
	      merge_line(i);
	    }
	}

	void build_tree_of_line(coord row)
	{
	  // Start at the second pixel (the first pixel has no left
	  // neighbor.
	  for (coord j = 1; j < ima_.ncols(); ++j)
	    {
	      point left = find_level_root(point(row, j - 1));
	      point this_one = point(row, j);
	      insert(this_one, left);
	    }
	}

	void merge_line(coord row)
	{
	  precondition(row > 0);

	  for (coord col = 0; col < ima_.ncols(); ++col)
	    {
	      point p     (row,     col    );
	      point upper (row - 1, col    );

	      // Note: merging with the left pixel is useless.

	      // Merge with upper pixel.
	      merge(p, upper);
	    }
	}

	/// Insert \a p in a tree of which \a n is a neighbor of \a p.
	void insert(const point& p, const point& n)
	{
	  point r = find_ancestor(n, ima_[p]);
	  if (ima_[r] <= ima_[p])
	    {
	      parent_[p] = r;
	      if (ima_[r] == ima_[p])
		// Update the number of level roots.
		--n_level_roots_cpt_;
	    }
	  else
	    {
	      if (not is_top(r))
		parent_[p] = parent_[r];
	      parent_[r] = p;
	    }
	}

	/// Front-end to \a merge_.
	void merge(const point& p, const point& q)
	{
	  // The \a merge_ routine assumes that P has a value greater
	  // or equal to Q; swap them if needed.
	  if (ima_[p] >= ima_[q])
	    merge_(p, q);
	  else
	    merge_(q, p);
	}

	// FIXME: Turn this recursive method into a loop!
	// FIXME: Get rid of the swap, using an introductory method.
	/// \note ThÅÈo calls this method ``update'.
	void merge_(const point& p, const point& q)
	{
	  precondition (p != q);
	  precondition (ima_[p] >= ima_[q]);

	  point r = find_ancestor(p, ima_[q]);
	  point s = find_level_root(q);

	  // Stop the recursion when R and S are equal (i.e., they are
	  // already merged).
	  if (r == s)
	    return;

	  // FIXME: Have a look at ThÅÈo's ``update'' routine to see
	  // if we can improve this method.
	  // \{
	  point t = parent_[s];

	  if (ima_[s] == ima_[r])
	    {
	      parent_[s] = r;
	      // Update the number of level roots.
	      --n_level_roots_cpt_;
	    }
	  else
	    {
	      if (not is_top(r))
		parent_[s] = parent_[r];
	      parent_[r] = s;
	    }
	  merge_(r, t);
	}

	/// \note ThÅÈo calls this function ``is_root'.
 	bool is_top(const point& p) const
 	{
 	  return parent_[p] == p;
 	}

	bool is_level_root(const point& p) const
	{
	  return
	    is_top(p) ||
	    (ima_[parent_[p]] != ima_[p]);
	}

	/// \brief Find the level root of the component that P belongs to.
	/// \note From ThÅÈo.
	point find_level_root(const point& p)
	{
	  // Is P a level root at the end the routine?
	  if (is_level_root(p))
	    return p;
	  else
	    // Path compression.
	    return parent_[p] = find_level_root(parent_[p]);
	}

	/// \note From ThÅÈo.
	/// \note ThÅÈo calls this method ``anc''.
	point find_ancestor(point p, const value& level)
	{
	  while (not is_top(p) and ima_[parent_[p]] >= level)
	    p = find_level_root(parent_[p]);
	  return p;
	}

      protected:
	  /// \note ThÅÈo calls this image ``f''.
	I ima_;
	  /// \note ThÅÈo calls this image ``par''.
	parent_type parent_;
	// Counter of level roots (method 1).
 	unsigned n_level_roots_cpt_;



// ----------------------------------------------------------------

// FIXME: Should be elsewhere!

//       public:
// 	void area_opening (unsigned size)
// 	{
// 	  oln_iter_type_(area_type) p(area_);

// 	  // First pass: sum areas.
// 	  for_all(p)
// 	    {
// 	      point p_parent = parent_[p];
// 	      while (p != p_parent)
// 		{
// 		  area_[p_parent] += 1;
// 		}
// 	    }

// 	  // Second pass: cut branches in the max-tree.
// 	  for_all(p)
// 	    merge(p, size);
// 	}

//       protected:
// 	// Attach P to Q.
// 	// FIXME: The criterion should be passed as a function object.
// 	point merge (const point& p, unsigned size)
// 	{
// 	  if (not is_top(p) && area_[p] < size)
// 	    {
// 	      // Find the component satisfying the criterion.
// 	      point new_level_root = merge(parent_[p], size);
// 	      // Attach P to its new component representant (level root).
// 	      parent_[p] = new_level_root;
// 	      // Take the color of the new component.
// 	      ima_[p] = ima_[new_level_root];
// 	      return new_level_root;
// 	    }
// 	  else
// 	    return p;
// 	}

//       protected:
// 	area_type area_;
      };

    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln

#endif // ! OLENA_LRDE_UFMT_FIORIO_HH
