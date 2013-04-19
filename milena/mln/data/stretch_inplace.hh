// Copyright (C) 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_DATA_STRETCH_INPLACE_HH
# define MLN_DATA_STRETCH_INPLACE_HH

/// \file
///
/// Transform linearly the contents of an image into another one in a
/// stretching way.
///
/// \todo Make it work with other types than scalars (e.g., vectors).


# include <mln/estim/min_max.hh>
# include <mln/value/int_u.hh>
# include <mln/fun/v2v/linear.hh>
# include <mln/data/transform_inplace.hh>


# include <mln/value/internal/encoding.hh>
# include <iomanip>
namespace mln
{

  namespace data
  {

    /*! \brief Stretch the values of \p ima so that they can be
     *  stored inplace.
     *
     * \param[in,out] ima   The input image.
     *
     *
     * \pre input.is_valid
     *
     * \ingroup mlndata
     */
    template <typename I>
    void
    stretch_inplace(Image<I>& ima);

    /*! \brief Stretch the values of \p ima so that they can be
     *  stored inplace.
     *
     * \param[in,out] ima   The input image.
     * \param[out] stretched The image has changed. 
     *
     * \pre input.is_valid
     *
     * \ingroup mlndata
     */
    template <typename I>
    void
    stretch_inplace(Image<I>& ima, bool& stretched);


# ifndef MLN_INCLUDE_ONLY


      template <typename I>
      inline
      void
      stretch_inplace(Image<I>& ima, bool& stretched)
      {
        mln_trace("data::stretch_inplace");
        typedef mln_value(I) V;
        mln_precondition(exact(ima).is_valid());
	
        V min_, max_;
        estim::min_max(ima, min_, max_);
        if (max_ != min_ && (mln_max(V)>max_ || mln_min(V)>min_))
        {
        //FIXME: we would like to use float instead of double but we
        //can't for precision reasons. See ticket #179.
        double
          min = double(min_),
          max = double(max_),
          epsilon = mln_epsilon(float),
          M = mln_max(V) + 0.5f - epsilon,
          m = 0.0f - 0.5f + epsilon,
          a = (M - m) / (max - min),
          b = (m * max - M * min) / (max - min);
          fun::v2v::linear_sat<V, double, V> f(a, b);
          data::transform_inplace(ima, f);
          stretched = true;
        }
        else
        {
          stretched = false;
          mln_trace_warning("output has no significative data!");
        }
      }

    template <typename I>
    void
    stretch_inplace(Image<I>& ima)
    {
      bool b;
      stretch_inplace(ima, b);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_STRETCH_INPLACE_HH
