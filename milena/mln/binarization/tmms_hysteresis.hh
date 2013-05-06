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

#ifndef MLN_BINARIZATION_TMMS_HYSTERESIS_HH
# define MLN_BINARIZATION_TMMS_HYSTERESIS_HH

/// \file
///
/// \brief Binarization using TMMS algorithm combined with hysteresis.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/morpho/dilation.hh>
# include <mln/morpho/erosion.hh>
# include <mln/labeling/blobs.hh>


namespace mln
{

  namespace binarization
  {

    /*!
      Binarize \p input by the use of TMMS algorithm (see "Text
      Segmentation in Natural Scenes Using Toggle-Mapping" from
      icip09 for more details \cite fabrizio2009icip). This version
      is combined with hysteresis.

      \param[in] input the input image.

      \param[in] win the structuring element used to compute erosion
      and dilation of input image (During TMMS algorithm, input image
      is mapped over two fuctions: an erosion and a dilation of the
      input image).

      \param[in] c_min the min contrast allowed to extract feature.

      \return an image with "foreground" marked with the highest
      possible color (hc), the "background" marked with the lowest
      possible color (lc) and homogeneous regions of the image marked
      with the color (hc+lc)/2. (This will fail with two colors
      images, but it seems to be a bit stupid to try to binarize two
      color images?)

      \author J. Fabrizio

      \ingroup modroutines
    */
    template<typename I, typename W>
    mln_concrete(I)
    tmms_hysteresis(const Image<I>& input_, const Window<W>& win,
		    const mln_value(I)& c_min_low,
		    const mln_value(I)& c_min_high,
		    const mln_value(I)& min_level,
		    const mln_value(I)& med_level,
		    const mln_value(I)& max_level,
		    const int pourcent);


# ifndef MLN_INCLUDE_ONLY

    // Implementations

    namespace impl
    {

      namespace generic
      {

        template<typename I, typename W>
        mln_concrete(I)
        tmms_hysteresis(const Image<I>& input_, const Window<W>& win_,
			const mln_value(I)& c_min_low,
			const mln_value(I)& c_min_high,
			const mln_value(I)& min_level,
			const mln_value(I)& med_level,
			const mln_value(I)& max_level,
			const int pourcent)
        {
          mln_trace("mln::binarization::impl::generic::tmms_hysteresis");

          const I& input = exact(input_);
          const W& win = exact(win_);

	  mln_precondition(input.is_valid());
	  mln_precondition(win.is_valid());

          typedef mln_value(I) V;

          mln_concrete(I) dilation = mln::morpho::dilation(input, win);
          mln_concrete(I) erosion = mln::morpho::erosion(input, win);

          mln_concrete(I) low_image;
          initialize(low_image, input);
          mln_concrete(I) high_image;
          initialize(high_image, input);

          mln_piter(I) p(input.domain());
          for_all(p)
          {
            V dil = dilation(p); // by def : dil>=ima>=ero
            V ero = erosion(p);
            V ima = input(p);

	    if ((V)(dil - ero) < c_min_low)
	    {
	      low_image(p) = med_level;
	      high_image(p) = med_level;
	    }
	    else if ((V)(dil - ero) < c_min_high)
	    {
	      low_image(p) = med_level;
	      if ((V)(dil - ima) < pourcent * ((V) (dil - ero)) / 100)
		high_image(p) = max_level;
	      else
		high_image(p) = min_level;
	    }
	    else
	    {
	      if ((V)(dil - ima) < pourcent * ((V)(dil - ero)) / 100)
	      {
		low_image(p) = max_level;
		high_image(p) = max_level;
	      }
	      else
	      {
		low_image(p) = min_level;
		high_image(p) = min_level;
	      }
	    }

          }

          unsigned nlabels;
          mln_ch_value(I, unsigned)
	    labels_high_images = mln::labeling::blobs(high_image, mln::c4(), nlabels);

	  std::vector<V> values(nlabels + 1);

          for (unsigned i = 0; i <= nlabels ; i++)
            values[i] = med_level;

          for_all(p)
	    if (low_image(p) != med_level)
	      values[labels_high_images(p)] = low_image(p);

          for_all(p)
            if (low_image(p) == med_level)
              low_image(p) = values[labels_high_images(p)];

          return low_image;
        }

      } // end of namespace mln::binarization::impl::generic


      template<typename I, typename W>
      mln_concrete(I)
      tmms_hysteresis_fastest(const Image<I>& input_, const Window<W>& win_,
			      const mln_value(I)& c_min_low, const mln_value(I)& c_min_high,
			      const mln_value(I)& min_level,
			      const mln_value(I)& med_level,
			      const mln_value(I)& max_level,
			      const int pourcent)
      {
	mln_trace("mln::binarization::impl::tmms_hysteresis_fastest");

	const I& input = exact(input_);
	const W& win = exact(win_);

	mln_precondition(input.is_valid());
	mln_precondition(win.is_valid());

	typedef mln_value(I) V;

	mln_concrete(I) dilation = mln::morpho::dilation(input, win);
	mln_concrete(I) erosion = mln::morpho::erosion(input, win);

	// Checking border sizes.
	border::resize(dilation, input.border());
	border::resize(erosion, input.border());
	mln_assertion(input.border() == dilation.border());
	mln_assertion(input.border() == erosion.border());

	mln_concrete(I) low_image;
	initialize(low_image, input);
	mln_concrete(I) high_image;
	initialize(high_image, input);

	unsigned nrows = geom::nrows(input);
	unsigned ncols = geom::ncols(input);
	for (unsigned row = 0; row < nrows; ++row)
	{
	  const V *p_input = &(input.at_(row, 0));
	  const V *p_dilation = &(dilation.at_(row, 0));
	  const V *p_erosion = &(erosion.at_(row, 0));
	  V *p_low_image = &(low_image.at_(row, 0));
	  V *p_high_image = &(high_image.at_(row, 0));

	  for (unsigned col = 0; col < ncols; ++col)
	  {
	    V dil = *p_dilation; // by def : dil>=ima>=ero
	    V ero = *p_erosion;
	    V ima = *p_input;

	    if ((V)(dil - ero) < c_min_low)
	    {
	      *p_low_image = med_level;
	      *p_high_image = med_level;
	    }
	    else if ((V)(dil - ero) < c_min_high)
	    {
	      *p_low_image = med_level;

	      if ((V)(dil - ima) < (V)(pourcent * (dil - ero) / 100))
		*p_high_image = max_level;
	      else
		*p_high_image = min_level;
	    }
	    else
	    {
	      if ((V)(dil - ima) < (V)(pourcent * (dil - ero) / 100))
	      {
		*p_low_image = max_level;
		*p_high_image = max_level;
	      }
	      else
	      {
		*p_low_image = min_level;
		*p_high_image = min_level;
	      }
	    }

	    ++p_input;
	    ++p_dilation;
	    ++p_erosion;
	    ++p_low_image;
	    ++p_high_image;
	  }
	}

	unsigned nlabels;
	mln_ch_value(I, unsigned)
	  labels_high_images = mln::labeling::blobs(high_image, c4(), nlabels);

	std::vector<V> values(nlabels + 1);

	for(unsigned i = 0; i <= nlabels; ++i)
	  values[i] = med_level;

	mln_piter(I) p(input.domain());
	for_all(p)
	  if (low_image(p) != med_level)
	    values[labels_high_images(p)] = low_image(p);

	for_all(p)
	  if (low_image(p) == med_level)
	    low_image(p) = values[labels_high_images(p)];

	return low_image;
      }

    }// end of namespace mln::binarization::impl


    // Dispatch

    namespace internal
    {

      template<typename I, typename W>
      mln_concrete(I)
      tmms_hysteresis_dispatch(const I& ima, const W& win,
			       const mln_value(I)& c_min_low,
			       const mln_value(I)& c_min_high,
			       const mln_value(I)& min_level,
			       const mln_value(I)& med_level,
			       const mln_value(I)& max_level,
			       const int pourcent, mln::trait::image::speed::any)
      {
	return impl::generic::tmms_hysteresis(ima, win,
					      c_min_low, c_min_high,
					      min_level, med_level,
					      max_level, pourcent);
      }

      template<typename I, typename W>
      mln_concrete(I)
      tmms_hysteresis_dispatch(const I& ima, const W& win,
			       const mln_value(I)& c_min_low,
			       const mln_value(I)& c_min_high,
			       const mln_value(I)& min_level,
			       const mln_value(I)& med_level,
			       const mln_value(I)& max_level,
			       const int pourcent, mln::trait::image::speed::fastest)
      {
	return impl::tmms_hysteresis_fastest(ima, win,
					     c_min_low, c_min_high,
					     min_level, med_level,
					     max_level, pourcent);
      }

      template<typename I, typename W>
      mln_concrete(I)
      tmms_hysteresis_dispatch(const I& ima, const W& win,
			       const mln_value(I)& c_min_low,
			       const mln_value(I)& c_min_high,
			       const mln_value(I)& min_level,
			       const mln_value(I)& med_level,
			       const mln_value(I)& max_level,
			       const int pourcent)
      {
	return tmms_hysteresis_dispatch(ima, win,
					c_min_low, c_min_high,
					min_level, med_level,
					max_level, pourcent,
					mln_trait_image_speed(I)());
      }

    } //end of namespace namespace mln::binarization::internal


    // Facade

    template<typename I, typename W>
    mln_concrete(I)
    tmms_hysteresis(const Image<I>& input_, const Window<W>& win_,
		    const mln_value(I)& c_min_low, const mln_value(I)& c_min_high,
		    const mln_value(I)& min_level,
		    const mln_value(I)& med_level,
		    const mln_value(I)& max_level,
		    const int pourcent)
    {
      mln_trace("mln::binarization::tmms_hysteresis");

      const I& input = exact(input_);
      const W& win = exact(win_);

      mln_precondition(input.is_valid());
      mln_precondition(win.is_valid());

      return internal::tmms_hysteresis_dispatch(input, win,
						c_min_low, c_min_high,
						min_level, med_level,
						max_level, pourcent);
    }

# endif // ! MLN_INCLUDE_ONLY

  } //  end of namespace mln::binarization


} // end of namespace mln

#endif // ! MLN_BINARIZATION_TMMS_HYSTERESIS_HH
