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

#ifndef MLN_DATA_COMPUTE_IN_INNER_BORDER_HH
# define MLN_DATA_COMPUTE_IN_INNER_BORDER_HH

/// \file
///
/// Compute an accumulator onto the inner border pixel values.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/accumulator.hh>
# include <mln/geom/nrows.hh>
# include <mln/geom/ncols.hh>

namespace mln
{

  namespace data
  {

    /// Compute an accumulator onto the inner border pixel values of
    /// the image \p input.  Be ware that the given accumulator won't
    /// be modified and won't store any result.
    ///
    /// \param[in] a An accumulator.
    /// \param[in] input The input image.
    /// \param[in] inner_border_thickness The inner border thickness
    /// considered.
    /// \return The accumulator result.
    ///
    /// It fully relies on data::update.
    //
    template <typename A, typename I>
    mln_result(A)
    compute_in_inner_border(const Accumulator<A>& a, const Image<I>& input,
			    unsigned inner_border_thickness);


    /// Compute an accumulator onto the inner border pixel values of
    /// the image \p input.
    ///
    /// \param[in, out] a An accumulator.
    /// \param[in] input The input image.
    /// \param[in] inner_border_thickness The inner border thickness
    /// considered.
    /// \return The accumulator result.
    ///
    /// It fully relies on data::update.
    //
    template <typename A, typename I>
    mln_result(A)
    compute_in_inner_border(Accumulator<A>& a, const Image<I>& input,
			    unsigned inner_border_thickness);


# ifndef MLN_INCLUDE_ONLY


    template <typename A, typename I>
    mln_result(A)
    compute_in_inner_border(const Accumulator<A>& a_, const Image<I>& input,
			    unsigned inner_border_thickness)
    {
      (void) a_;
      A a;
      return compute_in_inner_border(a, input, inner_border_thickness);
    }


    template <typename A, typename I>
    mln_result(A)
    compute_in_inner_border(Accumulator<A>& a_, const Image<I>& input_,
			    unsigned inner_border_thickness)
    {
      trace::entering("mln::data::compute_in_inner_border");
      mln_precondition(exact(input_).is_valid());
      mln_precondition(inner_border_thickness > 0);
      mln_precondition(geom::nrows(input_) >= inner_border_thickness);
      mln_precondition(geom::ncols(input_) >= inner_border_thickness);

      A a = exact(a_);
      const I& input = exact(input_);

      a.init();

      unsigned inner_border_thickness_1 = inner_border_thickness - 1;
      unsigned nrows_1 = geom::nrows(input) - 1;
      unsigned ncols_1 = geom::ncols(input) - 1;

      typedef mln_box(I) B;

      /*
	.--------------------.
	|      b_top         |<------ Image domain
	|--------------------|
	| |                | |
	|b|                |b|
	| |                | |
	|l|                |r|
	|e|                |i|
	|f|                |g|
	|t|                |h|
	| |                |t|
	|--------------------|
	|      b_bot         |
	.--------------------.

       */
      B b_top = B(input.domain().pmin(),
		  input.domain().pmin()
		  + inner_border_thickness_1 * down
		  + ncols_1 * right);
      mln_piter(I) p_top(b_top);

      B b_bot = B(input.domain().pmax()
		  + inner_border_thickness_1 * up
		  + ncols_1 * left,
		  input.domain().pmax());
      mln_piter(I) p_bot(b_bot);

      B b_left = B(input.domain().pmin()
      		   + inner_border_thickness * down,
      		   input.domain().pmin()
      		   + inner_border_thickness_1 * right
      		   + (nrows_1 - inner_border_thickness) * down);
      mln_piter(I) p_left(b_left);

      B b_right = B(input.domain().pmax()
      		    + inner_border_thickness_1 * left
      		    + (nrows_1 - inner_border_thickness) * up,
      		    input.domain().pmax()
      		    + inner_border_thickness * up);
      mln_piter(I) p_right(b_right);

      mln_assertion(b_right.nsites() == b_left.nsites());
      mln_assertion(b_top.nsites() == b_bot.nsites());

      for_all_2(p_top, p_bot)
      {
      	a.take(input(p_top));
      	a.take(input(p_bot));
      }

      for_all_2(p_left, p_right)
      {
      	a.take(input(p_left));
      	a.take(input(p_right));
      }

      trace::exiting("mln::data::compute_in_inner_border");
      return a;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln

#endif // ! MLN_DATA_COMPUTE_IN_INNER_BORDER_HH
