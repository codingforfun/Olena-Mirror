// Copyright (C) 2007, 2008, 2009, 2012, 2013 EPITA Research and
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

#ifndef MLN_LABELING_ALL_BLOBS_HH
# define MLN_LABELING_ALL_BLOBS_HH

/// \file
///
/// Connected component labeling of the homogeneous regions of an
/// image using a queue-based algorithm.
///
/// \todo Handle abort in a nice way...

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/canvas/labeling/blobs.hh>


namespace mln
{

  namespace labeling
  {

    /*! \brief Connected component labeling of the homogeneous regions
     *  of an image.

      \param[in] input    The input image.
      \param[in] nbh      The connexity of the objects.
      \param[out] nlabels  The Number of labels. Its value is set in the
                           algorithms.
      \return              The label image.

      A fast queue is used so that the algorithm is not recursive and
        can handle large objects (blobs).

      \ingroup labeling
    */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    all_blobs(const Image<I>& input, const Neighborhood<N>& nbh,
	      L& nlabels);



# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      /*! \brief Functor not computing anything. To be passed to the
	labeling blobs canvas.
      */
      template <typename I, typename L>
      struct label_values_functor
      {
	void init() {}

	bool handles(const mln_value(I)&) { return true; }

	void new_label(const mln_value(L)&) {}

	void process_p(const mln_site(L)&) {}

	void process_n(const mln_site(L)&) {}

	void finalize() {}
      };

    } // end of namespace mln::labeling::internal



    // Facade.

    template <typename I, typename N, typename L>
    inline
    mln_ch_value(I, L)
    all_blobs(const Image<I>& input_, const Neighborhood<N>& nbh_,
	      L& nlabels)
    {
      mln_trace("labeling::all_blobs");
      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      mln_precondition(input.is_valid());

      typedef mln_ch_value(I,L) out_t;
      internal::label_values_functor<I, out_t> functor;
      out_t
	output = canvas::labeling::blobs(input, nbh, nlabels, functor);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_ALL_BLOBS_HH
