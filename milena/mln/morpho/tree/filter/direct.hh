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

#ifndef MLN_MORPHO_TREE_FILTER_DIRECT_HH
# define MLN_MORPHO_TREE_FILTER_DIRECT_HH

# include <mln/core/concept/function.hh>
# include <mln/core/image/attribute_image.hh>
# include <mln/morpho/tree/propagate_node.hh>


/**
** \file   mln/morpho/tree/filter/direct.hh
**
** \brief  Filtering with direct strategy.
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
	** Direct non-pruning strategy. A node is removed if it does
	** not verify the predicate. Its pixels are lowered in gray
	** level to the highest ancestor which meets the criterion,
	** and while its descendants are unaffected.
	**
	** \param[in,out] attr The attribute image.
	** \param[in] pred_ Filtering criterion.
	*/
	template <typename T, typename V, typename P>
	inline
	void
	direct(attribute_image<T,V>& attr, const Function_v2b<P>& pred_);




# ifndef MLN_INCLUDE_ONLY

	template <typename T, typename V, typename F>
	inline
	void
	direct(attribute_image<T,V>& attr, const Function_v2b<F>& pred_)
	{
	  trace::entering("mln::morpho::tree::filter::direct");

	  typedef attribute_image<T,V> A;

	  const F& pred = exact(pred_);
	  const T& tree = attr.tree();

	  mln_fwd_piter(A) node(attr.domain());
	  for_all(node)
	    if (!pred(node))
	      attr(node) = attr(tree.parent(node));

	  trace::exiting("mln::morpho::tree::filter::direct");
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::morpho::tree::filter

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // ! MLN_MORPHO_TREE_FILTER_DIRECT_HH
