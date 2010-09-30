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

#ifndef CPLX2D_HH
# define CPLX2D_HH

# include <mln/core/var.hh>
# include <mln/data/paste.hh>
# include <mln/morpho/gradient.hh>

namespace mln
{

  template <typename I, typename J>
  void data__paste_values(const Image<I>& input_, Image<J>& output_)
  {
    const I& input = exact(input_);
    J& output = exact(output_);

    mln_fwd_piter(I) pi(input.domain());
    mln_fwd_piter(J) po(output.domain());
    for_all_2(pi, po)
      output(po) = input(pi);
  }

  namespace cplx2d
  {

    typedef neighb< win::multiple<window2d, bool(*)(const point2d&)> > dbl_neighb2d;

    inline
    bool is_row_odd(const point2d& p)
    {
      return p.row() % 2;
    }

    // Pixel to neighboring edges.
    const neighb2d& p2e()
    {
      return c4();
    }

    // Edge to (the couple of) pixels.
    const dbl_neighb2d& e2p()
    {
      static bool e2p_h[] = { 0, 1, 0,
			      0, 0, 0,
			      0, 1, 0 };
      static bool e2p_v[] = { 0, 0, 0,
			      1, 0, 1,
			      0, 0, 0 };
      static dbl_neighb2d nbh = make::double_neighb2d(is_row_odd, e2p_h, e2p_v);
      return nbh;
    }


    inline
    bool is_pixel(const point2d& p)
    {
      // Original pixels.
      return p.row() % 2 == 0 && p.col() % 2 == 0;
    }

    inline
    bool is_edge(const point2d& p)
    {
      // Edges between pixels.
      return p.row() % 2 + p.col() % 2 == 1;
    }

    typedef fun::C<bool (*)(const mln::point2d&)> predicate_t;

    image_if< image2d<value::int_u8>, predicate_t >
    f_to_g(const image2d<value::int_u8>& f, value::int_u8 value_on_non_edges = 0)
    {
      image2d<value::int_u8> f_(2 * f.nrows() - 1, 2 * f.ncols() - 1);
      data::fill(f_, value_on_non_edges); // Useless but for display!

      data__paste_values(f, (f_ | is_pixel).rw());

      mln_VAR(g, f_ | is_edge);
      data::paste(morpho::gradient(extend(g, f_), e2p().win()), g);

      return g;
    }

  }

}

#endif // !CPLX2D_HH
