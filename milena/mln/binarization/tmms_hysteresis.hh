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

# include <mln/morpho/all.hh>
# include <mln/labeling/values.hh>


/// \file
///
/// \brief Binarization using TMMS algorithm (see "Text Segmentation
/// in Natural Scenes Using Toggle-Mapping" from icip09 for more
/// details) combined with hysteresis.


namespace mln
{

  namespace binarization
  {

    /**
     Binarize \p in_image by the use of TMMS algorithm (see "Text
     Segmentation in Natural Scenes Using Toggle-Mapping" from icip09
     for more details).
     \tparam image_in_ the input image (not modified by the algorithm.

     \tparam struct_element the structuring element used to compute
     erosion and dilation of input image (During TMMS algorithm, input
     image is mapped over two fuctions: an erosion and a dilation of
     the input image).

     \param c_min the min contrast allowed to extract feature.

     \return an image with "foreground" marked with the highest
     possible color (hc), the "background" marked with the lower
     possible color (lc) and homogeneous regions of the image marked
     with the color (hc+lc)/2. (This will fail with two colors images,
     but it seems to be a bit stupid to try to binarize two color
     images?)

     \author J. Fabrizio
     */
    template<typename I, typename W>
    mln_concrete(I)
    tmms_hysteresis(const Image<I>& in_image_, const Window<W>& struct_element,
	       const mln_value(I)& c_min_low, const mln_value(I)& c_min_high, 
         const mln_value(I)& min_level, 
         const mln_value(I)& med_level,
         const mln_value(I)& max_level,
         const int pourcent);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      
      namespace generic
      {
    
        template<typename I, typename W>
        mln_concrete(I)
        tmms_hysteresis(const Image<I>& in_image_, const Window<W>& struct_element,
             const mln_value(I)& c_min_low, const mln_value(I)& c_min_high, 
             const mln_value(I)& min_level, 
             const mln_value(I)& med_level,
             const mln_value(I)& max_level,
             const int pourcent)
        {
          mln_trace("binarization::tmms_hysteresis");

          const I& in_image = exact(in_image_);

          mln_concrete(I) low_image;
          mln_concrete(I) high_image;

          mln_concrete(I) dilation = mln::morpho::dilation(in_image, struct_element);
          mln_concrete(I) erosion = mln::morpho::erosion(in_image, struct_element);

          unsigned int nlabels;

          mln_piter(I) p(in_image.domain());

          initialize(low_image, in_image);
          initialize(high_image, in_image);

          typedef mln_value(I) V;
          for_all(p)
          {
            V dil = dilation(p); // by def : dil>=ima>=ero
            V ero = erosion(p);
            V ima = in_image(p);

           if ((V)(dil - ero)<c_min_low) low_image(p) = high_image(p) = med_level;
           else if ((V)(dil - ero)<c_min_high) {
                  low_image(p) = med_level;
                  if ( (V)(dil - ima) < pourcent*((V)(dil - ero))/100 )
                  high_image(p) = max_level;
                  else
                   high_image(p) = min_level;
                } else {
                  if ( (V)(dil - ima) < pourcent*((V)(dil - ero))/100 )
                    low_image(p) = high_image(p) = max_level;
                  else 
                    low_image(p) = high_image(p) = min_level;
                }

          }

          mln_ch_value(I, unsigned int) labels_high_images;
          labels_high_images = mln::labeling::values(high_image,
            mln::c4(), nlabels);
          V *values = new V[nlabels];

          for(unsigned int i=0;i<nlabels;i++) {
            values[i]=med_level;
          }

          for_all(p) {
                if (low_image(p)!=med_level) {
                  values[labels_high_images(p)]=low_image(p);
                }       
          }

          for_all(p) {
            if (low_image(p)==med_level) {
              low_image(p)=values[labels_high_images(p)];
            }
          }
          delete(values);

          return low_image;
        }

      } // end of namespace mln::binarization::impl::generic
    
        template<typename I, typename W>
        mln_concrete(I)
        tmms_hysteresis_fastest(const Image<I>& in_image_, const Window<W>& struct_element,
             const mln_value(I)& c_min_low, const mln_value(I)& c_min_high, 
             const mln_value(I)& min_level, 
             const mln_value(I)& med_level,
             const mln_value(I)& max_level,
             const int pourcent)
        {
          mln_trace("binarization::tmms_hysteresis");

          const I& in_image = exact(in_image_);

          mln_concrete(I) low_image;
          mln_concrete(I) high_image;

          mln_concrete(I) dilation = mln::morpho::dilation(in_image, struct_element);
          mln_concrete(I) erosion = mln::morpho::erosion(in_image, struct_element);

          unsigned int nlabels;

          initialize(low_image, in_image);
          initialize(high_image, in_image);
          border::resize(dilation, in_image.border());
          border::resize(erosion, in_image.border());

          if (in_image.border()!=dilation.border() || 
                  in_image.border()!=erosion.border())
          {
            std::abort();
          }
          
          typedef mln_value(I) V;
          
          unsigned nrows = geom::nrows(in_image);
          unsigned ncols = geom::ncols(in_image);
        	for (row = 0; row < nrows; row += 1)
          {
            V *p_in_image=&(in_image.at_(row, 0));
            V *p_dilation=&(dilation.at_(row, 0)));
            V *p_erosion=&(erosion.at_(row, 0));
            V *p_low_image=&(low_image.at_(row, 0));
            V *p_high_image=&(high_image.at_(row, 0));
            
          	for (unsigned col = 0; col < ncols; col += 1)
          
          {
            V dil = *p_dilation; // by def : dil>=ima>=ero
            V ero = *p_erosion;
            V ima = *p_in_image;

           if ((V)(dil - ero)<c_min_low) (*p_low_image) = (*p_high_image) = med_level;
           else if ((V)(dil - ero)<c_min_high) {
                  (*p_low_image) = med_level;
                  if ( (V)(dil - ima) < pourcent*((V)(dil - ero))/100 )
                   (*p_high_image) = max_level;
                  else
                   (*p_high_image) = min_level;
                } else {
                  if ( (V)(dil - ima) < pourcent*((V)(dil - ero))/100 )
                    (*p_low_image) = (*p_high_image) = max_level;
                  else 
                    (*p_low_image) = (*p_high_image) = min_level;
                }
          }

          mln_ch_value(I, unsigned int) labels_high_images;
          labels_high_images = mln::labeling::values(high_image,
            mln::c4(), nlabels);
          V *values = new V[nlabels];

          for(unsigned int i=0;i<nlabels;i++) {
            values[i]=med_level;
          }

          mln_piter(I) p(in_image.domain());
          for_all(p) {
                if (low_image(p)!=med_level) {
                  values[labels_high_images(p)]=low_image(p);
                }       
          }

          for_all(p) {
            if (low_image(p)==med_level) {
              low_image(p)=values[labels_high_images(p)];
            }
          }
          delete(values);

          return low_image;
        }
     
    }// end of namespace mln::binarization::impl
      
    namespace internal
    {
      
        template<typename I, typename W>
        mln_concrete(I)
        tmms_hysteresis_dispatch(const I& ima, const W& struct_element,
             const I& c_min_low, const I& c_min_high, 
             const I& min_level, const I& med_level, const I& max_level,
             const int pourcent, mln::trait::image::speed::any)
        {
          return mln::binarization::impl::generic::tmms_hysteresis(ima, 
                  struct_element, c_min_low, c_min_high, 
                  min_level, med_level, max_level, pourcent);
        }

        template<typename I, typename W>
        mln_concrete(I)
        tmms_hysteresis_dispatch(const I& ima, const W& struct_element,
             const I& c_min_low, const I& c_min_high, 
             const I& min_level, const I& med_level, const I& max_level,
             const int pourcent, mln::trait::image::speed::fastest)
        {
          return mln::binarization::impl::tmms_hysteresis_fastest(ima, 
                  struct_element, c_min_low, c_min_high, 
                  min_level, med_level, max_level, pourcent);
        }
        
        template<typename I, typename W>
        mln_concrete(I)
        tmms_hysteresis_dispatch(const I& ima, const W& struct_element,
                const mln_value(I)& c_min_low, const mln_value(I)& c_min_high, 
                const mln_value(I)& min_level, const mln_value(I)& med_level,
                const mln_value(I)& max_level,
                const int pourcent)
        {
          return tmms_hysteresis_dispatch(ima, 
                  struct_element, c_min_low, c_min_high, 
                  min_level, med_level, max_level, pourcent,
                  mln_trait_image_speed(I));
        }
        
    } //end of namespace namespace mln::binarization::internal
    
    template<typename I, typename W>
    mln_concrete(I)
    tmms_hysteresis(const Image<I>& in_image_, const Window<W>& struct_element,
	       const mln_value(I)& c_min_low, const mln_value(I)& c_min_high, 
         const mln_value(I)& min_level, 
         const mln_value(I)& med_level,
         const mln_value(I)& max_level,
         const int pourcent)
    {
      const I& ima = exact(in_image_);
      const W& win = exact(struct_element);
      return mln::binarization::internal::tmms_hysteresis_dispatch(ima, 
              win, c_min_low, c_min_high, 
              min_level, med_level, max_level, pourcent);
    }

# endif // ! MLN_INCLUDE_ONLY

  } //  end of namespace mln::binarization


} // end of namespace mln

#endif // ! MLN_BINARIZATION_TMMS_HH
