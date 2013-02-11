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
/// \brief Tree of shapes data structure.

#ifndef MLN_UTIL_TREE_OF_SHAPES_HH
# define MLN_UTIL_TREE_OF_SHAPES_HH

# include <vector>
# include <mln/value/rgb8.hh>
# include <mln/util/map.hh>
# include <mln/core/alias/box2d.hh>

namespace mln
{

  namespace util
  {

    template <typename I>
    struct tree_of_shapes
    {
      typedef mln_site(I) P;
      typedef mln_value(I) V;
      typedef mln_equiv(V) EV;
      typedef mln_ch_value(I,P) parent_t;

      I Fb;

      // Default canonicalization.
      std::vector<P> R;
      parent_t parent;

      V level(const P& p) const;
      bool level_changes_at(unsigned i) const;

      bool is_root(const P& p) const;
      bool is_not_root(const P& p) const;

      bool is_representative(const P& p) const;
      bool is_not_representative(const P& p) const;

      bool is_0_representative(const P& p) const;
    };


# ifndef MLN_INCLUDE_ONLY


    template <typename I>
    typename tree_of_shapes<I>::V
    tree_of_shapes<I>::level(const P& p) const
    {
      return Fb(p);
    }

    template <typename I>
    bool
    tree_of_shapes<I>::level_changes_at(unsigned i) const
    {
      if (i == 0) // the end...
	return true;
      P p = R[i], p_next = R[i-1];
      return Fb(p_next) != Fb(p); // ..or next level is different
    }

    template <typename I>
    bool
    tree_of_shapes<I>::is_root(const P& p) const
    {
      return parent(p) == p;
    }

    template <typename I>
    bool
    tree_of_shapes<I>::is_not_root(const P& p) const
    {
      return ! is_root(p);
    }

    template <typename I>
    bool
    tree_of_shapes<I>::is_representative(const P& p) const
    {
      return is_root(p) || Fb(parent(p)) != Fb(p);
    }

    template <typename I>
    bool
    tree_of_shapes<I>::is_not_representative(const P& p) const
    {
      return ! is_not_representative(p);
    }

    template <typename I>
    bool
    tree_of_shapes<I>::is_0_representative(const P& p) const
    {
      return (is_root(p) || Fb(parent(p)) != Fb(p)) && (Fb(p) == V(0));
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_TREE_OF_SHAPES_HH
