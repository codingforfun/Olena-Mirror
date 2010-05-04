// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_FILTER_MIN_HH
# define MLN_MORPHO_TREE_FILTER_MIN_HH

# include <mln/core/concept/function.hh>
# include <mln/core/image/attribute_image.hh>
# include <mln/morpho/tree/propagate_node.hh>


/**
** \file   mln/morpho/tree/filter/min.hh
**
** \brief  Filtering with min strategy.
**
**
*/

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      namespace filter
      {


	/**
	** Min pruning strategy. A node is removed iff its parent is
	** removed or if it does not verify the predicate \p pred_.
	**
	** \param[in,out] attr The attribute image.
	** \param[in] pred_ Filtering criterion.
	*/
	template <typename T, typename V, typename F>
	inline
	void
	min(attribute_image<T,V>& attr, const Function_v2b<F>& pred);




# ifndef MLN_INCLUDE_ONLY

	template <typename T, typename V, typename F>
	inline
	void
	min(attribute_image<T,V>& attr, const Function_v2b<F>& pred_)
	{
	  trace::entering("mln::morpho::tree::filter::min");

	  typedef attribute_image<T,V> A;

	  //mlc_is(mln_psite(A), mln_argument(F))::check();

	  const F& pred = exact(pred_);
	  const T& tree = attr.tree();

	  // Ce qui devrait être !
	  // mln_fwd_piter(A) n(attr);
	  // for_all(n)
	  //   if (!pred(n))
	  //     {
	  // 	data::fill_with_value(attr | tree.desc(n), attr(n));
	  //    n.go_to_brother();
	  //     }

	  // En attendant que le morpher subimage soit spécialisé,
	  // on joue avec les indexes.
	  for (int i = 0; i < tree.n_nodes(); ++i)
	    {
	      mln_invariant(tree.has_index(i));
	      mln_psite(A) n(tree, i);
	      if (!pred(n))
		{
		  attr(n) = attr(tree.parent(n));
		  i += propagate_node_to_descendants(attr, n, attr(n));
		}
	    }
	  trace::exiting("mln::morpho::tree::filter::min");
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::morpho::tree::filter

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_TREE_FILTER_MIN_HH
