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

#ifndef MLN_MORPHO_TREE_FILTER_SUBTRACTIVE_HH
# define MLN_MORPHO_TREE_FILTER_SUBTRACTIVE_HH

# include <mln/core/concept/function.hh>
# include <mln/core/image/attribute_image.hh>
# include <mln/morpho/tree/propagate_node.hh>

/**
** \file   mln/morpho/tree/filter/subtractive.hh
**
** \brief  Filtering with subtractive strategy.
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
	** Subtractive pruning strategy. The node is removed if it
	** does not verify the predicate. The sub-components values
	** are set to the value of the removed component.
	**
	** \param[in] ori The values as an attribute image.
	** \param[in] pred_ Filtering criterion.
	** \return A copy of \p ori filtered with the subtractive strategy.
	*/
	template <typename T, typename V, typename P>
	inline
	attribute_image<T, V>
	subtractive(const attribute_image<T,V>& ori, const Function_v2b<P>& pred_);


# ifndef MLN_INCLUDE_ONLY

	template <typename T, typename V, typename F>
	inline
	attribute_image<T, V>
	subtractive(const attribute_image<T,V>& ori, const Function_v2b<F>& pred_)
	{
	  mln_trace("mln::morpho::tree::filter::subtractive");
	  typedef attribute_image<T,V> A;

	  const F& pred = exact(pred_);
	  const T& tree = ori.tree();

	  mln_precondition(ori.is_valid());

	  A f;
	  initialize(f, ori);

	  // Fixme: introduce mln_root_piter...
	  // and traverse all roots, not just the first one.
	  mln_psite(A) root = util::ctree::node<T>(tree, 0);
	  f(root) = ori(root);

	  mln_piter(A) n(ori.domain());
	  for_all(n)
	    if (!pred(n))
	      f(n) = f(tree.parent(n));
	    else
	      f(n) = f(tree.parent(n)) + ori(n) - ori(tree.parent(n));

	  return f;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::morpho::tree::filter

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_TREE_FILTER_SUBTRACTIVE_HH
