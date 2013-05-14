// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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
/// \brief Voronoi layout analysis algorithm.

#ifndef SCRIBO_LAYOUT_VORONOI_HH
# define SCRIBO_LAYOUT_VORONOI_HH

# include <mln/value/rgb8.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/transform/distance_front.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/int_u16.hh>
# include <mln/data/convert.hh>
# include <mln/make/w_window2d_int.hh>
# include <mln/morpho/watershed/flooding.hh>
# include <mln/util/graph.hh>
# include <mln/make/region_adjacency_graph.hh>
# include <mln/make/p_vertices_with_mass_centers.hh>
# include <mln/util/ord_pair.hh>
# include <mln/debug/draw_graph.hh>
# include <mln/literal/colors.hh>
# include <mln/pw/all.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <scribo/layout/internal/voronoi/labeling.hh>
# include <scribo/layout/internal/voronoi/sort.hh>
# include <scribo/layout/internal/voronoi/distance.hh>
# include <scribo/layout/internal/voronoi/distribution.hh>
# include <scribo/layout/internal/voronoi/filter.hh>

#include <mln/io/ppm/save.hh>

namespace scribo
{

  namespace layout
  {
    using namespace mln;

    /*! \brief Voronoi layout analysis algorithm.

      \param[in] ima A Binary image.

      \return A graph of connected components grouped by logical
      blocks.

      \ingroup grpalgolayout
    */
    template <typename I>
    util::graph
    voronoi(const Image<I>& ima);


# ifndef MLN_INCLUDE_ONLY



    template <typename I>
    util::graph
    voronoi(const Image<I>& ima_)
    {
      mln_trace("scribo::layout::voronoi");

      typedef mln_value(I) V;
      typedef mln_box(I) B;
      mlc_is(V,bool)::check();

      const I& ima = exact(ima_);
      mln_precondition(ima.is_valid());

      using namespace internal::voronoi;

      const box2d& domain = ima.domain();
      mln_concrete(I) input = duplicate(ima);

      //-------------------------
      // Labeling
      //-------------------------

      unsigned nlabels = 0;
      std::vector<outline_bbox> bbox_list;
      image2d<unsigned> labeled_image(domain);
      component_labeling(input, nlabels, bbox_list, labeled_image, 5, 13,
			 500, 500);

      bbox_list.clear();
      data::fill(labeled_image, 0);
      nlabels = 0;

      component_labeling(input, nlabels, bbox_list, labeled_image, 5, 0,
			 500, 500);


      //-------------------------
      // Compute Voronoi diagram
      //-------------------------

      image2d<value::int_u16> ws;
      value::int_u16 nbasins;
      image2d<value::int_u8> dist_map;

      int vals[] = { 0, 11, 0, 11, 0,
		     11, 7, 5, 7, 11,
		     0, 5, 0, 5, 0,
		     11, 7, 5, 7, 11,
		     0, 11, 0, 11, 0 };

      dist_map = transform::distance_front(input, c8(),
					   make::w_window2d_int(vals),
					   mln_max(value::int_u8));


      // Watershed transform
      ws = morpho::watershed::flooding(dist_map, c8(), nbasins);

      // Reorder bounding box list
      reorder_bbox_list(bbox_list, ws);

      //-------------------------
      // Compute Region Adjacency Graph
      //-------------------------

      util::graph rag = make::region_adjacency_graph(ws, c8(), nbasins);
      std::map<util::ord_pair<unsigned>, int> distances;
      int max_distance = 0;

      // Compute minimal distance between sample points of adjacent
      // components outline
      compute_adjacency_distances(bbox_list, rag, distances, max_distance);

      //-------------------------
      // Compute thresholds
      //-------------------------

      int td1 = 0;
      int td2 = 0;
      int ta = 40;
      float t = 0.34;

      compute_distribution(max_distance, distances, t, td1, td2);

      //-------------------------
      // Invalidate edges
      //-------------------------

      util::graph g(rag.v_nmax());
      invalidate_edges(distances, bbox_list, rag, g, td1, td2, ta);

      // //-------------------------
      // // Saving
      // //-------------------------

      // image2d<value::rgb8> output = data::convert(value::rgb8(), input);
      // data::fill((output |(pw::value(ws) == 0)).rw(), literal::orange);

      // p_vertices< util::graph, fun::i2v::array<point2d> > p_v =
      // 	make::p_vertices_with_mass_centers(ws, g);

      // debug::draw_graph(output, p_v, literal::blue, literal::green);
      // io::ppm::save(output, "output.ppm");

      return g;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_VORONOI_HH
