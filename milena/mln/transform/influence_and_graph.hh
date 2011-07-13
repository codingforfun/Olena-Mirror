// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_TRANSFORM_INFLUENCE_AND_GRAPH_HH
# define MLN_TRANSFORM_INFLUENCE_AND_GRAPH_HH

/// \file
///
/// Geodesic influence zone transform.

# include <mln/extension/adjust.hh>
# include <mln/canvas/distance_geodesic.hh>
# include <mln/transform/internal/influence_zone_functor.hh>
# include <mln/util/graph.hh>
# include <mln/util/adjacency_matrix.hh>

namespace mln
{

  namespace transform
  {

    /// Apply a Geodesic influence zone transform and compute the graph.
    ///
    /// \param[in] input	    An image.
    /// \param[out] ouput	    The influce zone geodesic image.
    /// \param[in] nbh		    A neighborhood.
    /// \param[in] nlabels	    The number of detected labels.
    ///
    /// \return A graph.
    //
    template <typename I, typename N>
    util::graph
    influence_and_graph(const Image<I>& input, Image<I>& ouput, const Neighborhood<N>& nbh, const mln_value(I)& nlabels);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      template <typename I, typename N>
      void
      influence_and_graph_tests(const Image<I>& input, Image<I>& ouput,
				    const Neighborhood<N>& nbh, const mln_value(I)& nlabels)
      {
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(nbh).is_valid());

	(void) input;
	(void) nbh;
      }

    } // end of namespace mln::transform::internal


    namespace impl
    {

      namespace generic
      {

	template <typename I, typename N>
	util::graph
	influence_and_graph(const Image<I>& input, Image<I>& ouput,
				const Neighborhood<N>& nbh, const mln_value(I)& nlabels)
	{
	  // FIXME: To be written...
	  mlc_abort(I)::check();
	}

      } // end of namespace mln::transform::impl::generic


      template <typename I, typename N>
      util::graph
      influence_and_graph_fastest(const Image<I>& input_, Image<I>& ouput_,
				      const Neighborhood<N>& nbh_, const mln_value(I)& nlabels)
      {
	
	trace::entering("transform::impl::influence_and_graph_fastest");
	const mln_value(I) extension_val = nlabels.next();
	util::graph g;
	util::adjacency_matrix<> adj(nlabels.next());
	g.add_vertices(nlabels.next());
	const I& input = exact(input_);
	I& output = exact(ouput_);
	const N& nbh = exact(nbh_);

	internal:: influence_and_graph_tests(input,output, nbh, nlabels);

	std::queue<mln_value(I)*> q;

	util::array<int> dp = offsets_wrt(input, nbh);
	const unsigned n_nbhs = dp.nelements();

	// Initialization.
	{
	  extension::adjust(input, nbh);
	  output = duplicate(input);
	  // For the extension to be ignored:
	  extension::fill(input, 0);  // in initialization
	  extension::fill(output, extension_val); // in propagation

	  const unsigned nelts = input.nelements();
	  const mln_value(I)* p_i = input.buffer();
	 
	  mln_value(I)* p_o = output.buffer();
	  for (unsigned i = 0; i < nelts; ++i, ++p_i, ++p_o)
	  {
	    if (*p_i == 0)
	      continue;
	    for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      const mln_value(I)* n_i = p_i + dp[j];
	      if (*n_i == 0)
	      {
		q.push(p_o);
		break;
	      }
	    }
	  }

	}

 	// Propagation.
	{
	  mln_value(I)* ptr;

	  while (! q.empty())
	  {
	    ptr = q.front();
	    q.pop();
	    mln_invariant(*ptr != 0);
	    for (unsigned j = 0; j < n_nbhs; ++j)
	    {
	      mln_value(I)* ntr = ptr + dp[j];
	      if (*ntr == 0)
	      {
		*ntr = *ptr;
		q.push(ntr);
	      }
	      // this part is the creation of the graph
	      else if(*ntr != *ptr && *ntr != extension_val && !adj.are_adjacent(*ntr, *ptr))
	      {
		 adj.add(*ntr, *ptr);
		 adj.add(*ptr, *ntr);
		 g.add_edge(*ptr, *ntr);
	      }
	    }
	  }
	}
	trace::exiting("make::impl::generic::influence_and_graph_fastest");
	return g;
      }


    } // end of namespace mln::transform::impl


    namespace internal
    {

      template <typename I, typename N>
      util::graph
      influence_and_graph_dispatch(trait::image::value_alignment::any,
				       trait::image::value_storage::any,
				       trait::image::value_access::any,
				       const I& input,
					Image<I>& ouput,
				       const N& nbh,
				       const mln_value(I)& nlabels)
      {
	return impl::generic::influence_and_graph(input,ouput, nbh, nlabels);
      }


      template <typename I, typename N>
      util::graph
      influence_and_graph_dispatch(trait::image::value_alignment::with_grid,
				       trait::image::value_storage::one_block,
				       trait::image::value_access::direct,
				       const I& input,
				       Image<I>& ouput,
				       const N& nbh,
				       const mln_value(I)& nlabels)
      {
	return impl::influence_and_graph_fastest(input,ouput, nbh,nlabels);
      }


      template <typename I, typename N>
      util::graph
      influence_and_graph_dispatch(const Image<I>& input,
				   Image<I>& ouput,
				   const Neighborhood<N>& nbh,
				   const mln_value(I)& nlabels
				   )
      {
	return
	  influence_and_graph_dispatch(mln_trait_image_value_alignment(I)(),
					   mln_trait_image_value_storage(I)(),
					   mln_trait_image_value_access(I)(),
					   exact(input), exact(ouput), exact(nbh), nlabels);
      }

    } // end of namespace mln::transform::internal


    template <typename I, typename N>
    util::graph
    influence_and_graph(const Image<I>& input,  Image<I>& ouput, const Neighborhood<N>& nbh, const mln_value(I)& nlabels)
    {
      trace::entering("transform::influence_zone_geodesic");

      internal::influence_and_graph_tests(input,ouput, nbh, nlabels);

      util::graph
	output = internal::influence_and_graph_dispatch(input, ouput, nbh, nlabels);

      trace::exiting("transform::influence_zone_geodesic");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::transform

} // end of namespace mln


#endif // !MLN_TRANSFORM_INFLUENCE_AND_GRAPH_HH
