// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MORPHO_TREE_LEAF_LAST_HH
# define MLN_MORPHO_TREE_LEAF_LAST_HH

# include <mln/accu/stat/max.hh>
# include <mln/util/ctree/ctree.hh>
# include <mln/morpho/tree/propagate_node.hh>
# include <mln/convert/to_p_array.hh>

namespace mln
{

  namespace morpho
  {

    namespace tree
    {

      template <typename T, typename V>
      util::array<typename T::node_t>
      leaf_last(const attribute_image<T, V>& ima);

# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename T, typename V>
	struct less_
	{
	  typedef typename T::node_t node_t;

	  less_(const attribute_image<T, V>& img) :
	    f_ (img)
	  {
	  }

	  bool operator() (const node_t& lhs, const node_t& rhs)
	  {
	    return f_(lhs)  > f_(rhs);
	  }

	private:
	  const attribute_image<T, V>& f_;
	};
      }

      template <typename T, typename V>
      util::array<typename T::node_t>
      leaf_last(const attribute_image<T, V>& ima)
      {
	typedef attribute_image<T, V> A;
        typedef typename T::node_t node_t;

        p_array<node_t> sorted_sites = convert::to_p_array(ima);
        std::vector<node_t>& hook = sorted_sites.hook_std_vector_();
	std::sort(hook.begin(), hook.end(), internal::less_<T, V> (ima));

	mln_ch_value(A, char) deja_vu;
	initialize(deja_vu, ima);
	data::fill(deja_vu, 0);

	util::array<node_t> result;

        mln_piter(p_array<node_t>) n(sorted_sites);
	for_all(n)
	{
	  if (deja_vu(n))
	    continue;
	  result.append(n);
	  deja_vu(n) = true; // useless
          propagate_node_to_descendants<T, char>(deja_vu, n, 1);
          propagate_node_to_ancestors<T, char>(deja_vu, n, 1);
	}

	return result;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::morpho::tree

  } // end of namespace mln::morpho

} // end of namespace mln

#endif // !MLN_MORPHO_TREE_LEAF_LAST_HH
