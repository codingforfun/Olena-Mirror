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


namespace mln
{

  namespace util
  {

    enum Tags { None, Spurious, Noise };

    template <typename I>
    struct tree_of_shapes
    {
      typedef mln_site(I) P;
      typedef mln_value(I) V;
      typedef mln_equiv(V) EV;

      I Fb;

      // Default canonicalization.
      std::vector<P> R;
      mln_ch_value(I,P) parent;

      // 0-canonicalization.
      std::vector<P> R0;
      mln_ch_value(I,P) parent0;
      // Attributes/tags for 0-representants.
      std::vector<Tags> tag;

      // 01-canonicalization.
      std::vector<P> R01;
      mln_ch_value(I,P) parent01;


      V level(const P& p) const;
      bool level_changes_at(unsigned i) const;
      bool is_root(const P& p) const;
      bool is_representative(const P& p) const;

      bool is_0_representative(const P& p) const;

      bool is_01_representative(const P& p) const;

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
    tree_of_shapes<I>::is_representative(const P& p) const
    {
      return is_root(p) || Fb(parent(p)) != Fb(p);
    }

    template <typename I>
    bool
    tree_of_shapes<I>::is_0_representative(const P& p) const
    {
      return is_representative(p) && (Fb(p) == V(0));
    }

    template <typename I>
    bool
    tree_of_shapes<I>::is_01_representative(const P& p) const
    {
      return is_root(p)
	|| (Fb(p) == V(0) && Fb(parent(p)) != V(0))
	|| (Fb(p) != V(0) && Fb(parent(p)) == V(0));
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln

#endif // ! MLN_UTIL_TREE_OF_SHAPES_HH
