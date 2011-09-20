// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/** \file
    \brief Experimentations with watershed transforms combining 2D and
    1D mathematical morphology.

     Steps:

     \li Watershed cut (by using a watershed flooding on the gradient
         placed on the edges).

     \li Hole-filling on the gradient along the watershed line using a
         connected filter (height closing).

     \li Topological watershed on this reconstructed gradient to
         propagate the hole-filling along the watershed line.  */

#include <cstdlib>

#include <mln/value/int_u8.hh>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/fun/vv2v/diff_abs.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/neighb2d.hh>
#include <mln/world/inter_pixel/dim2/is_pixel.hh>
#include <mln/world/inter_pixel/dim2/is_dot.hh>

#include <mln/value/label.hh>
#include <mln/morpho/watershed/flooding.hh>

#include <mln/morpho/watershed/topological.hh>

#include <mln/morpho/erosion.hh>
#include <mln/morpho/dilation.hh>

#include <mln/morpho/closing/height.hh>

#include <mln/data/paste.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/routine/extend.hh>

#include <mln/data/stretch.hh>

#include <mln/io/pgm/all.hh>
#include <mln/debug/println.hh>

#include <mln/core/var.hh>

#include "apps/data.hh"


int
main(int argc, char* argv[])
{
  using namespace mln;
  namespace inter_pixel = mln::world::inter_pixel;
  using mln::value::int_u8;
  using mln::value::label;

  if (argc != 4)
    {
      std::cerr
	<< "usage: " << argv[0] << " input.pgm lambda output.pgm"
	<< std::endl;
      std::exit(1);
    }

  // Load an image.
  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  // Turn it into a cubical complex-based image.
  mln_VAR(f, inter_pixel::immerse(input));

  // Compute the associated gradient.
  mln_VAR(g, inter_pixel::compute(f, fun::vv2v::diff_abs<int_u8>()));

  // Compute a watershed cut using on this gradient.
  label<24> n_basins;
  mln_VAR(w,
	  morpho::watershed::flooding(g, inter_pixel::e2e(), n_basins));
  std::cout << "nbasins = " << n_basins << std::endl;

  // Reconstruct the gradient by apply a closing area filter on the
  // original gradient restricted to the watershed line.
  g_t rg = duplicate(g);
  int lambda = std::atoi(argv[2]);
  data::paste(morpho::closing::height(g | (pw::value(w) == 0),
				      inter_pixel::e2e(),
				      lambda),
	      rg);

  // Compute a topological watershed transform on this reconstructed
  // gradient.
  typedef morpho::watershed::topo_wst<g_t, inter_pixel::dbl_neighb2d> tree_t;
  tree_t tree(g, inter_pixel::e2e());
  tree.go();
  mln_VAR(tw, morpho::watershed::topological(tree));

  // Propagate to pixels (2-faces) and ``dots'' (0-faces).
  mln_VAR(tw2, tw.unmorph_());
  tw2_t tw3 = duplicate(tw2);
  data::paste(morpho::erosion(extend(tw2 | inter_pixel::dim2::is_pixel(), tw2),
			      inter_pixel::v2e().win()),
	      tw2);
  // FIXME: There is no dot-to-edge neighborhood yet; use `c4' instead.
  data::paste(morpho::dilation(extend(tw3 | inter_pixel::dim2::is_dot(), tw3),
			      c4().win()),
	      tw3);

  io::pgm::save(data::stretch(int_u8(), tw3), argv[3]);
}
