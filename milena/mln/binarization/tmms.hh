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

#ifndef MLN_BINARIZATION_TMMS_HH
# define MLN_BINARIZATION_TMMS_HH

/// \file
///
/// \brief Binarization using TMMS algorithm.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/morpho/dilation.hh>
# include <mln/morpho/erosion.hh>


namespace mln
{

  namespace binarization
  {

    /*!
       Binarize \p input by the use of TMMS algorithm (see "Text
       Segmentation in Natural Scenes Using Toggle-Mapping" from icip09
       for more details \cite fabrizio2009icip).

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
    tmms(const Image<I>& input, const Window<W>& win,
         const mln_value(I)& c_min, const mln_value(I)& min_level,
         const mln_value(I)& med_level, const mln_value(I)& max_level,
         int pourcent);

    /*! \overload

      \verbatim
      min_level = mln_min(V); //background
      max_level = mln_max(V); //foreground
      med_level = (max_level + min_level) / 2; //homogeneous
      \endverbatim
    */
    template<typename I, typename W>
    mln_concrete(I)
    tmms(const Image<I>& input, const Window<W>& win,
	 const mln_value(I)& c_min);


# ifndef MLN_INCLUDE_ONLY

    template<typename I, typename W>
    mln_concrete(I)
    tmms(const Image<I>& input_, const Window<W>& win_,
         const mln_value(I)& c_min, const mln_value(I)& min_level,
         const mln_value(I)& med_level, const mln_value(I)& max_level,
         int pourcent)
    {
      mln_trace("binarization::tmms");

      const I& input = exact(input_);
      const W& win = exact(win_);

      mln_precondition(input.is_valid());
      mln_precondition(win.is_valid());

      mln_concrete(I) dilation = mln::morpho::dilation(input, win);
      mln_concrete(I) erosion = mln::morpho::erosion(input, win);

      typedef mln_value(I) V;
      //V min_level = mln_min(V); //background
      //V max_level = mln_max(V); //foreground
      //V med_level = (max_level + min_level) / 2; //homogeneous

      mln_concrete(I) output;
      initialize(output, input);

      mln_piter(I) p(input.domain());
      for_all(p)
      {
        V dil = dilation(p); // by def : dil>=ima>=ero
        V ero = erosion(p);
        V ima = input(p);
	if ((V)(dil - ero) >= c_min)
	{
	  if ((dil - ima) < pourcent * (dil - ero) / 100)
	    output(p) = max_level;
	  else
	    output(p) = min_level;
	}
	else
	  output(p) = med_level;
      }

      return output;
    }


    template<typename I, typename W>
    mln_concrete(I)
    tmms(const Image<I>& input, const Window<W>& win,
         const mln_value(I)& c_min)
    {
      typedef mln_value(I) V;
      V min_level = mln_min(V); //background
      V max_level = mln_max(V); //foreground
      V med_level = (max_level + min_level) / 2; //homogeneous

      return tmms(input, win, c_min,
		  min_level, med_level, max_level, 50);
    }



# endif // ! MLN_INCLUDE_ONLY

  } //  end of namespace mln::binarization


} // end of namespace mln

#endif // ! MLN_BINARIZATION_TMMS_HH
