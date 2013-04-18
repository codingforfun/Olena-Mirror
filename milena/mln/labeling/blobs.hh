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

#ifndef MLN_LABELING_BLOBS_HH
# define MLN_LABELING_BLOBS_HH

/// \file
///
/// Connected component labeling of the binary objects of a binary
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

    /*! \brief Connected component labeling of the objects of an
     *  image.

      \param[in] input    The input image.
      \param[in] nbh      The connexity of the objects.
      \param[out] nlabels  The Number of labels. Its value is set in the
                           algorithms.
      \param[in] background_value The value to be considered as
                                  background and not to be labeled.
      \return              A labeled image.

      A fast queue is used so that the algorithm is not recursive and
        can handle large objects (blobs).

      \ingroup labeling
    */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    blobs(const Image<I>& input, const Neighborhood<N>& nbh,
	  L& nlabels, const mln_value(I)& background_value);

    /*! \overload

      \brief Connected component labeling of the objects of an
      image.

      background_value is set to literal::zero.

      \ingroup labeling
    */
    template <typename I, typename N, typename L>
    inline
    mln_ch_value(I, L)
    blobs(const Image<I>& input_, const Neighborhood<N>& nbh_,
	  L& nlabels);


# ifndef MLN_INCLUDE_ONLY


    namespace internal
    {

      /*! \brief Functor not computing anything. To be passed to the
	labeling blobs canvas.
      */
      template <typename I, typename L>
      struct label_value_functor
      {
	label_value_functor(const mln_value(I)& background_value)
	  : background_value_(background_value) {}

	void init() {}

	mln_value(I) neutral_value() const { return this->background_value_; }

	bool handles(const mln_value(I)& v) const { return v != this->background_value_; }

	void new_label(const mln_value(L)&) {}

	void process_p(const mln_site(L)&) {}

	void process_n(const mln_site(L)&) {}

	void finalize() {}


	// Fastest interface

 	void process_p_(unsigned) {}

	void process_n_(unsigned) {}


	mln_value(I) background_value_;
      };

    } // end of namespace mln::labeling::internal



    // Facade.

    template <typename I, typename N, typename L>
    inline
    mln_ch_value(I, L)
    blobs(const Image<I>& input_, const Neighborhood<N>& nbh_,
	  L& nlabels, const mln_value(I)& background_value)
    {
      mln_trace("labeling::blobs");

      const I& input = exact(input_);
      const N& nbh = exact(nbh_);
      mln_precondition(input.is_valid());

      typedef mln_ch_value(I,L) out_t;
      internal::label_value_functor<I, out_t> functor(background_value);
      out_t
	output = canvas::labeling::blobs(input, nbh, nlabels, functor);

      return output;
    }

    template <typename I, typename N, typename L>
    inline
    mln_ch_value(I, L)
    blobs(const Image<I>& input, const Neighborhood<N>& nbh,
	  L& nlabels)
    {
      return blobs(input, nbh, nlabels,
		   static_cast<mln_value(I)>(literal::zero));
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_BLOBS_HH
