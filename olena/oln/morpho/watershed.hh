// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_MORPHO_WATERSHED_HH
# define OLENA_MORPHO_WATERSHED_HH

# include <oln/basics.hh>

namespace oln {

  namespace morpho {

    /*!
    ** \brief Segmented watershed.
    **
    **   Compute   the  segmented   watershed  for   image   im  using
    **   neighborhood ng.\n
    **
    **   watershed_seg creates  an ouput image whose  values have type
    **   DestValue (which should be  discrete).  In this output image,
    **   DestValue::max()  indicates a watershed,  and all  basins are
    **   labeled    using     values    from    DestValue::min()    to
    **   DestValue::max()   -  4  (the   remaining  values   are  used
    **   internally by the algorithm).\n
    **
    **   When there are more  basins than DestValue can hold, wrapping
    **   occurs (i.e., the same label is used for several basin).
    **
    ** \ref  This is  based  on the  original  algorithm presented  by
    ** Vincent and Soille. (FIXME: ref?)
    **
    ** \param DestValue Type of the data in output image.
    ** \param I Exact type of the image.
    ** \param N Exact type of the neighborhood.
    **
    ** \arg im_i Image of levels.
    ** \arg Ng Neighborhood to consider.
    **
    ** \pre DestValue should be large enough.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/watershed.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::int_u8>	im_type;
    **
    **   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
    **
    **   oln::save(oln::morpho::watershed_seg<ntg::int_u16>(im1,
    **                                                      oln::neighb_c4()),
    **             IMG_OUT "oln_morpho_watershed_seg.pbm");
    **   return  0;
    ** }
    ** \endcode
    **
    ** \image html lena256.png
    ** \image latex lena256.png
    ** =>
    ** \image html oln_morpho_watershed_seg.png
    ** \image latex oln_morpho_watershed_seg.png
    **
    ** \todo Find a more pertinent example.
    */
    template<class DestValue, class I, class N>
    typename mute<I, DestValue>::ret
    watershed_seg(const abstract::non_vectorial_image<I>& im_i,
		  const abstract::neighborhood<N>& Ng);


    /*!
    ** \brief Connected watershed.
    **
    ** Compute the connected watershed for image im using
    ** neighborhood ng.\n
    **
    ** watershed_con creates an ouput image whose values have
    ** type DestValue (which should be discrete).  In this output
    ** all basins are labeled using values from DestValue::min() to
    ** DestValue::max() - 4 (the remaining values are used internally
    ** by the algorithm).\n
    **
    ** When there are more basins than DestValue can hold, wrapping
    ** occurs (i.e., the same label is used for several basin).  This is
    ** potentially harmful, because if two connected basins are labeled
    ** with the same value they will appear as one basin.\n
    **
    ** \ref This is based on the original algorithm
    ** presented by Vincent and Soille, but modified to not output
    ** watersheds.
    **
    ** \param DestValue Type of the data in output image.
    ** \param I Exact type of the image.
    ** \param N Exact type of the neighborhood.
    **
    ** \arg input Image of levels.
    ** \arg Ng Neighborhood to consider.
    **
    ** \pre DestValue should be large enough.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/watershed.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::int_u8>	im_type;
    **
    **   im_type	im1(oln::load(IMG_IN "lena256.pgm"));
    **
    **   oln::save(oln::morpho::watershed_con<ntg::int_u16>(im1,
    **                                                      oln::neighb_c4()),
    **             IMG_OUT "oln_morpho_watershed_con.pbm");
    **   return  0;
    ** }
    ** \endcode
    **
    ** \image html lena256.png
    ** \image latex lena256.png
    ** =>
    ** \image html oln_morpho_watershed_con.png
    ** \image latex oln_morpho_watershed_con.png
    **
    ** \todo Find a more pertinent example.
    */
    template<class DestValue, class I, class N>
    typename mute<I, DestValue>::ret
    watershed_con(const abstract::non_vectorial_image<I>& im_i, const abstract::neighborhood<N>& Ng);

    /*!
    ** \brief Segmented watershed with user-supplied starting points.
    **
    ** Compute   a  segmented   watershed  for   image   levels  using
    ** neighborhood ng, and markers as starting point for the flooding
    ** algorithm.\n
    **
    ** markers is an  image of the same size  as levels and containing
    ** discrete values indicating label  associated to each basin.  On
    ** input, fill markers with oln_value_type(I2)::min() (this is the
    ** unknown label) and mark the starting points or regions (usually
    ** these   are   minima  in   levels)   using   a  value   between
    ** oln_value_type(I2)::min()+1 and oln_value_type(I2)::max()-1.\n
    **
    ** watershed_seg_or  will  flood  levels  from  these  non-unknown
    ** starting points,  labeling basins using the  value you assigned
    ** to    them,     and    markining    watershed     lines    with
    ** oln_value_type(I2)::max().   markers  should  not contains  any
    ** oln_value_type(I2)::min() value on output.
    **
    ** \ref  This is  based  on the  original  algorithm presented  by
    ** D'Ornellas et al. (FIXME: ref?)
    **
    ** \param I1 Exact type of the D image.
    ** \param I2 Exact type of the M image.
    ** \param N Exact type of the neighborhood.
    **
    ** \arg D Input image.
    ** \arg M Image of labels.
    ** \arg Ng Neighborhood to consider.
    */
    template<class I1, class I2, class N>
    oln_concrete_type(I2)&
    watershed_seg_or(const abstract::non_vectorial_image<I1>& D,
		     abstract::non_vectorial_image<I2>& M,
		     const abstract::neighborhood<N>& Ng);

  } // end of morpho

} // end of oln

# include <oln/morpho/watershed.hxx>

#endif // ! OLENA_MORPHO_WATERSHED_HH
