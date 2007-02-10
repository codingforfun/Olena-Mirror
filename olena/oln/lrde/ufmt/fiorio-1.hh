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
// Boston, Boston, MA 02110-1301, USA.
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


/** \brief An adaptation of Fiorio's and Gustedt's algorithm
    (fiorio.96.tcs).

    Ref.: Christophe Fiorio and Jens Gustedt.  Two linear time
    Union-Find strategies for image processing.  Theoretical Computer
    Science, 1996.

    \note This is the original version of my implementation, that
    started to work on August 25th, 2006.  */


#ifndef OLENA_LRDE_UFMT_FIORIO_1_HH
# define OLENA_LRDE_UFMT_FIORIO_1_HH

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
	typedef typename mute<I, point>::ret parent_type;

      public:
	fiorio(const abstract::image<I>& ima) :
	  ima_(ima.exact().clone()), parent_(ima.size())
	  , n_level_roots_cpt_(ima.npoints())
	{
	  // Init the image of parents.
	  piter p (parent_);
	  for_all (p)
	    parent_[p] = p;

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
	// FIXME: Improve to handle other neighborhoods than 4-c.
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
	      merge(left, this_one);
	    }
	}

	void merge_line(coord row)
	{
	  precondition(row > 0);

	  // Special treatment of the pixel on the first column.
	  {
	    point p     (row,     0);
	    point upper (row - 1, 0);
	    // Merge with upper pixel.
	    merge(p, upper);
	  }
	  for (coord col = 1; col < ima_.ncols(); ++col)
	    {
	      point p     (row,     col    );
	      point upper (row - 1, col    );

	      // Note: merging with the left pixel is useless.

	      // Merge with upper pixel.
	      merge(p, upper);
	    }
	}

	// FIXME: Turn this recursive method into a loop!
	// FIXME: Get rid of the swap, using an introductory method.
	/// \note Theo calls this method ``update'.
	void merge(const point& p, const point& q)
	{
	  point r = find_level_root(p);
	  point s = find_level_root(q);

	  // Stop the recursion when R and S are equal.
	  if (r == s)
	    return;

	  // Update the number of level roots.
	  if (ima_[r] == ima_[s])
	    --n_level_roots_cpt_;

	  // The rest of the routine assumes that R has a value
	  // greater or equal to S; swap them if needed.
	  if (ima_[r] < ima_[s])
	    std::swap(r, s);

	  point t = parent_[r];

	  // Join R and S only if S is higher than the parent of R.
	  if (r == t or ima_[s] > ima_[t])
	    parent_[r] = s;
	  // FIXME: Study whether the order (S, T) is important.  That
	  // might be the case when S and T have the same value.
	  merge(t, s);
	}

	/// \note Theo calls this function ``is_root'.
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
	///
	/// Based on find_compress.
	/// \note (This is my version.)
	point find_level_root(const point& p)
	{
	  // Path compression.
	  if (not is_top(p))
	    parent_[p] = find_level_root(parent_[p]);
	  // Is P a level root at the end the routine?
	  if (is_level_root(p))
	    return p;
	  else
	    return parent_[p];
	}

      protected:
	  /// \note Theo calls this image ``f''.
	I ima_;
	  /// \note Theo calls this image ``par''.
	parent_type parent_;
	// Counter of level roots (method 1).
 	unsigned n_level_roots_cpt_;

      };

    } // end of namespace oln::lrde::ufmt

  } // end of namespace oln::lrde

} // end of namespace oln

#endif // ! OLENA_LRDE_UFMT_FIORIO_1_HH
