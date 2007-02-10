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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_MORPHO_RECONSTRUCTION_HH
# define OLENA_MORPHO_RECONSTRUCTION_HH

# include <oln/basics.hh>
# include <oln/convert/conversion_ng_se.hh>
# include <oln/level/compare.hh>
# include <oln/morpho/geodesic_dilation.hh>
# include <oln/morpho/geodesic_erosion.hh>
# include <oln/morpho/splitse.hh>
# include <oln/morpho/stat.hh>

# include <ntg/basics.hh>

# include <queue>

namespace oln {
  namespace morpho {
    namespace sure {
      /*!
      ** \brief Perform a geodesic reconstruction dilation.
      **
      ** Compute the reconstruction by dilation of marker with respect
      ** to   the   mask   mask   image  using   se   as   structuring
      ** element.  Soille  p.160.  This  is  the  simplest  algorithm:
      ** iteration is performed until stability.
      **
      ** \warning This version is slow, since it is a sure one.
      **
      ** \pre Mask must be greater or equal than marker.
      **
      ** \param I1 Exact type of image marker.
      ** \param I2 Exact type of image mask.
      ** \param N Exact type of neighborhood.
      **
      ** \arg marker Image to work on.
      ** \arg mask Image used for geodesic dilation.
      ** \arg Ng Neighborhood to use.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/opening.hh>
      ** #include <oln/morpho/reconstruction.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
      **   im_type	im2 = oln::morpho::opening(im1, oln::win_c4p());
      **
      **   oln::save(oln::morpho::sure::geodesic_reconstruction_dilation(im2,
      **                                                                 im1,
      **                                                                 oln::neighb_c4()),
      **             IMG_OUT "oln_morpho_sure_geodesic_reconstruction_dilation.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_sure_geodesic_reconstruction_dilation.png
      ** \image latex oln_morpho_sure_geodesic_reconstruction_dilation.png
      **
      */
      template<class I1, class I2, class N>
      oln_concrete_type(I1)
	geodesic_reconstruction_dilation(const abstract::non_vectorial_image<I1> & marker,
					 const abstract::non_vectorial_image<I2> & mask,
					 const abstract::neighborhood<N>& Ng)
      {
	mlc::eq<I1::dim, I2::dim>::ensure();
	mlc::eq<I1::dim, N::dim>::ensure();
	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(mask, marker));
	oln_concrete_type(I1) output = marker.clone();
	bool non_stability = true;
	while (non_stability)
	  {
	    oln_concrete_type(I1) work = geodesic_dilation(output, mask, Ng);
	    non_stability = !(level::is_equal(work, output));
	    output = work;
	  }
	return output;
      }
    }// sure


    namespace sequential {
      /*!
      ** \brief Perform a geodesic reconstruction dilation.
      **
      ** Compute the reconstruction by dilation of marker with respect
      ** to  the mask  image using  se as  structuring  element. Soille
      ** p.160. The algorithm used is the one defined as sequential in
      ** Vincent(1993),  Morphological   grayscale  reconstruction  in
      ** image analysis: applications  and efficient algorithms, itip,
      ** 2(2), 176--201.
      **
      ** \pre Mask must be greater or equal than marker.
      **
      ** \param I1 Exact type of image marker.
      ** \param I2 Exact type of image mask.
      ** \param N Exact type of neighborhood.
      **
      ** \arg marker Image to work on.
      ** \arg mask Image used for geodesic dilation.
      ** \arg Ng Neighborhood to use.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/opening.hh>
      ** #include <oln/morpho/reconstruction.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
      **   im_type	im2 = oln::morpho::opening(im1, oln::win_c4p());
      **
      **   oln::save(oln::morpho::sequential::geodesic_reconstruction_dilation(im2,
      **                                                                 im1,
      **                                                                 oln::neighb_c4()),
      **             IMG_OUT "oln_morpho_sequential_geodesic_reconstruction_dilation.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_sequential_geodesic_reconstruction_dilation.png
      ** \image latex oln_morpho_sequential_geodesic_reconstruction_dilation.png
      **
      */
      template<class I1, class I2, class N>
      oln_concrete_type(I1)
	geodesic_reconstruction_dilation(const abstract::non_vectorial_image<I1> & marker,
					 const abstract::non_vectorial_image<I2> & mask,
					 const abstract::neighborhood<N>& Ng)
      {
	mlc::eq<I1::dim, I2::dim>::ensure();
	mlc::eq<I1::dim, N::dim>::ensure();
	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(mask, marker));

	// Conversion of neighborhood into a SE.
	typedef typename abstract::neighborhood<N>::win_type E;
	E se_plus = get_plus_se_p(convert::ng_to_cse(Ng));
	E se_minus = get_minus_se_p(convert::ng_to_cse(Ng));

	oln_concrete_type(I1) output = marker.clone();
	bool non_stability = true;
	typename I1::fwd_iter_type fwd_p(output);
	typename I1::bkd_iter_type bkd_p(output);
	while (non_stability)
	  {
	    oln_concrete_type(I1) work = output.clone();
	    work.border_adapt_copy(Ng.delta());
	    for_all (fwd_p)
	      work[fwd_p] = ntg::min(morpho::max(work, fwd_p, se_plus), mask[fwd_p]);
	    for_all (bkd_p)
	      work[bkd_p] = ntg::min(morpho::max(work, bkd_p, se_minus), mask[bkd_p]);
	    non_stability = !(level::is_equal(work, output));
	    output = work;
	  }
	return output;
      }
    }// sequential


    namespace hybrid {

      namespace internal {

	/*!
	** \brief Check if it exists initialization for dilation.
	**
	** \arg p Point to consider.
	** \arg marker Image to work on.
	** \arg mask Image used as mask.
	** \arg Ng Neighborhood to use.
	*/
	template<class P, class I1, class I2, class E> inline
	static bool
	exist_init_dilation(const abstract::point<P>& p,
			    const abstract::non_vectorial_image<I1>& marker,
			    const abstract::non_vectorial_image<I2>& mask,
			    const abstract::struct_elt<E>& se)
	{
	  mlc::eq<I1::dim, I2::dim>::ensure();
	  mlc::eq<I1::dim, E::dim>::ensure();
	  mlc::eq<I1::dim, P::dim>::ensure();

	  oln_neighb_type(E) q(se, p);
	  for_all (q)
	    if (marker.hold(q) && (marker[q] < marker[p]) && (marker[q] < mask[q]))
	      return true;
	  return false;
	}

      } //internal

      /*!
      ** \brief Perform a geodesic reconstruction dilation.
      **
      **  Compute  the  reconstruction  by  dilation  of  marker  with
      ** respect   to  the   mask  image   using  se   as  structuring
      ** element. Soille p.160. The  algorithm used is the one defined
      ** as   hybrid   in   Vincent(1993),   Morphological   grayscale
      ** reconstruction in image  analysis: applications and efficient
      ** algorithms, itip, 2(2), 176--201.
      **
      ** \pre Mask must be greater or equal than marker.
      **
      ** \param I1 Exact type of image marker.
      ** \param I2 Exact type of image mask.
      ** \param N Exact type of neighborhood.
      **
      ** \arg marker Image to work on.
      ** \arg mask Image used for geodesic dilation.
      ** \arg Ng Neighborhood to use.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/opening.hh>
      ** #include <oln/morpho/reconstruction.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
      **   im_type	im2 = oln::morpho::opening(im1, oln::win_c4p());
      **
      **   oln::save(oln::morpho::hybrid::geodesic_reconstruction_dilation(im2,
      **                                                                 im1,
      **                                                                 oln::neighb_c4()),
      **             IMG_OUT "oln_morpho_hybrid_geodesic_reconstruction_dilation.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_hybrid_geodesic_reconstruction_dilation.png
      ** \image latex oln_morpho_hybrid_geodesic_reconstruction_dilation.png
      **
      */
      template<class I1, class I2, class N>
      oln_concrete_type(I1)
	geodesic_reconstruction_dilation(const abstract::non_vectorial_image<I1> & marker,
					 const abstract::non_vectorial_image<I2> & mask,
					 const abstract::neighborhood<N>& Ng)
      {
	mlc::eq<I1::dim, I2::dim>::ensure();
	mlc::eq<I1::dim, N::dim>::ensure();

	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(mask, marker));

	oln_concrete_type(I1) output = marker.clone();
	output.border_adapt_copy(Ng.delta());
	{
	  typedef typename abstract::neighborhood<N>::win_type E;
	  E Ng_plus = get_plus_se_p(convert::ng_to_cse(Ng));
	  E Ng_minus = get_minus_se_p(convert::ng_to_cse(Ng));
	  typename I1::fwd_iter_type fwd_p(output);
	  typename I1::bkd_iter_type bkd_p(output);
	  for_all (fwd_p)
	    output[fwd_p] = ntg::min(morpho::max(output, fwd_p, Ng_plus),
				     mask[fwd_p]);

	  std::queue<oln_point_type(I1) > fifo;
	  for_all (bkd_p)
	    {
	      output[bkd_p] = ntg::min(morpho::max(output, bkd_p, Ng_minus),
				       mask[bkd_p]);
	      if (internal::exist_init_dilation(bkd_p.cur(), output, mask, Ng_minus))
		fifo.push(bkd_p);
	    }
	  // Propagation Step
	  while (!fifo.empty())
	    {
	      oln_point_type(I1) p = fifo.front();
	      fifo.pop();
	      oln_neighb_type(N) q(Ng, p);
	      for_all (q) if (output.hold(q))
		{
		  if ((output[q] < output[p]) && (mask[q] != output[q]))
		    {
		      output[q] = ntg::min(output[p], mask[q]);
		      fifo.push(q);
		    }
		}
	    }
	}
	return output;
      }
    }// hybrid


    //GEODESIC RECONSTRUCTION EROSION

    namespace sure {

      /*!
      ** \brief Perform a geodesic reconstruction erosion.
      **
      **  Compute the reconstruction by erosion of marker with respect
      ** to  the mask image  using se  as structuring  element. Soille
      ** p.160. This is the simplest algorithm: iteration is performed
      ** until stability.
      **
      ** \warning This version is slow, since it is a sure one.
      **
      ** \pre Marker must be greater or equal than mask.
      **
      ** \param I1 Exact type of image marker.
      ** \param I2 Exact type of image mask.
      ** \param N Exact type of neighborhood.
      **
      ** \arg marker Image to work on.
      ** \arg mask Image used for geodesic erosion.
      ** \arg Ng Neighborhood to use.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/opening.hh>
      ** #include <oln/morpho/reconstruction.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
      **   im_type	im2 = oln::morpho::opening(im1, oln::win_c4p());
      **
      **   oln::save(oln::morpho::sure::geodesic_reconstruction_erosion(im1,
      **                                                                 im2,
      **                                                                 oln::neighb_c4()),
      **             IMG_OUT "oln_morpho_sure_geodesic_reconstruction_erosion.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_sure_geodesic_reconstruction_erosion.png
      ** \image latex oln_morpho_sure_geodesic_reconstruction_erosion.png
      **
      */
      template<class I1, class I2, class N>
      oln_concrete_type(I1)
	geodesic_reconstruction_erosion(const abstract::non_vectorial_image<I1> & marker,
					const abstract::non_vectorial_image<I2> & mask,
					const abstract::neighborhood<N>& Ng)
      {
	mlc::eq<I1::dim, I2::dim>::ensure();
	mlc::eq<I1::dim, N::dim>::ensure();
	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(marker, mask));
	oln_concrete_type(I1) output = marker.clone();
	bool non_stability = true;
	while (non_stability)
	  {
	    oln_concrete_type(I1) work = geodesic_erosion(output, mask, Ng);
	    non_stability = !(level::is_equal(work, output));
	    output = work;
	  }
	return output;
      }
    } // sure


    namespace sequential {

      /*!
      ** \brief Perform a geodesic reconstruction erosion.
      **
      **  Compute the reconstruction by erosion of marker with respect
      ** to  the mask image  using se  as structuring  element. Soille
      ** p.160.  The  algorithm used is the one  defined as sequential
      ** in  Vincent(1993), Morphological grayscale  reconstruction in
      ** image analysis: applications  and efficient algorithms, itip,
      ** 2(2), 176--201.
      **
      ** \pre Marker must be greater or equal than mask.
      **
      ** \param I1 Exact type of image marker.
      ** \param I2 Exact type of image mask.
      ** \param N Exact type of neighborhood.
      **
      ** \arg marker Image to work on.
      ** \arg mask Image used for geodesic erosion.
      ** \arg Ng Neighborhood to use.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/opening.hh>
      ** #include <oln/morpho/reconstruction.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
      **   im_type	im2 = oln::morpho::opening(im1, oln::win_c4p());
      **
      **   oln::save(oln::morpho::sequential::geodesic_reconstruction_erosion(im1,
      **                                                                 im2,
      **                                                                 oln::neighb_c4()),
      **             IMG_OUT "oln_morpho_sequential_geodesic_reconstruction_erosion.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_sequential_geodesic_reconstruction_erosion.png
      ** \image latex oln_morpho_sequential_geodesic_reconstruction_erosion.png
      **
      */
      template<class I1, class I2, class N>
      oln_concrete_type(I1)
	geodesic_reconstruction_erosion(const abstract::non_vectorial_image<I1>& marker,
					const abstract::non_vectorial_image<I2>& mask,
					const abstract::neighborhood<N>& Ng)
      {
	mlc::eq<I1::dim, I2::dim>::ensure();
	mlc::eq<I1::dim, N::dim>::ensure();
	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(marker, mask));

	typedef typename abstract::neighborhood<N>::win_type E;
	E se_plus = get_plus_se_p(convert::ng_to_cse(Ng));
	E se_minus = get_minus_se_p(convert::ng_to_cse(Ng));
	oln_concrete_type(I1) output = marker.clone();

	bool non_stability = true;
	typename I1::fwd_iter_type fwd_p(output);
	typename I1::bkd_iter_type bkd_p(output);
	while (non_stability)
	  {
	    oln_concrete_type(I1) work = output.clone();
	    work.border_adapt_copy(Ng.delta());
	    for_all (fwd_p)
	      work[fwd_p] = ntg::max(morpho::min(work, fwd_p, se_plus), mask[fwd_p]);
	    for_all (bkd_p)
	      work[bkd_p] = ntg::max(morpho::min(work, bkd_p, se_minus), mask[bkd_p]);
	    non_stability = !(level::is_equal(work, output));
	    output = work;
	  }
	return output;
      }
    } // sequential


    namespace hybrid {
      namespace internal {
	/*!
	** \brief Check if it exists initialization for erosion.
	**
	** \arg p Point to consider.
	** \arg marker Image to work on.
	** \arg mask Image used as mask.
	** \arg Ng Neighborhood to use.
	*/
	template<class P, class I1, class I2, class E> inline
	static bool
	exist_init_erosion(const abstract::point<P>& p,
			   const abstract::non_vectorial_image<I1>& marker,
			   const abstract::non_vectorial_image<I2>& mask,
			   const abstract::struct_elt<E>& se)
	{
	  mlc::eq<I1::dim, I2::dim>::ensure();
	  mlc::eq<I1::dim, E::dim>::ensure();
	  mlc::eq<I1::dim, P::dim>::ensure();

	  oln_neighb_type(E) q(se, p);
	  for_all (q)
	    if (marker.hold(q) && (marker[q] > marker[p]) && (marker[q] > mask[q]))
	      return true;
	  return false;
	}
      } // internal

      /*!
      ** \brief Perform a geodesic reconstruction erosion.
      **
      **  Compute the reconstruction by erosion of marker with respect
      ** to   the   mask   mask   image  using   se   as   structuring
      ** element. Soille p.160.  The algorithm used is the one defined
      ** as   hybrid   in   Vincent(1993),   Morphological   grayscale
      ** reconstruction in image  analysis: applications and efficient
      ** algorithms, itip, 2(2), 176--201.
      **
      ** \pre Marker must be greater or equal than mask.
      **
      ** \param I1 Exact type of image marker.
      ** \param I2 Exact type of image mask.
      ** \param N Exact type of neighborhood.
      **
      ** \arg marker Image to work on.
      ** \arg mask Image used for geodesic erosion.
      ** \arg Ng Neighborhood to use.
      **
      ** \code
      ** #include <oln/basics2d.hh>
      ** #include <oln/morpho/opening.hh>
      ** #include <oln/morpho/reconstruction.hh>
      ** #include <oln/level/compare.hh>
      ** #include <ntg/all.hh>
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8>	im_type;
      **
      **   im_type	im1(oln::load(IMG_IN "lena128.pgm"));
      **   im_type	im2 = oln::morpho::opening(im1, oln::win_c4p());
      **
      **   oln::save(oln::morpho::hybrid::geodesic_reconstruction_erosion(im1,
      **                                                                 im2,
      **                                                                 oln::neighb_c4()),
      **             IMG_OUT "oln_morpho_hybrid_geodesic_reconstruction_erosion.pbm");
      **   return  0;
      ** }
      ** \endcode
      **
      ** \image html lena128_pgm.png
      ** \image latex lena128_pgm.png
      ** =>
      ** \image html oln_morpho_hybrid_geodesic_reconstruction_erosion.png
      ** \image latex oln_morpho_hybrid_geodesic_reconstruction_erosion.png
      **
      */
      template<class I1, class I2, class N>
      oln_concrete_type(I1)
	geodesic_reconstruction_erosion(const abstract::non_vectorial_image<I1> & marker,
					const abstract::non_vectorial_image<I2> & mask,
					const abstract::neighborhood<N>& Ng)
      {
	mlc::eq<I1::dim, I2::dim>::ensure();
	mlc::eq<I1::dim, N::dim>::ensure();

	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(marker, mask));

	oln_concrete_type(I1) output = marker.clone();
	output.border_adapt_copy(Ng.delta());
	{
	  typedef typename abstract::neighborhood<N>::win_type E;
	  E Ng_plus = get_plus_se_p(convert::ng_to_cse(Ng));
	  E Ng_minus = get_minus_se_p(convert::ng_to_cse(Ng));
	  typename I1::fwd_iter_type fwd_p(output);
	  typename I1::bkd_iter_type bkd_p(output);
	  for_all (fwd_p)
	    output[fwd_p] = ntg::max(morpho::min(output, fwd_p, Ng_plus),
				     mask[fwd_p]);

	  std::queue<oln_point_type(I1) > fifo;
	  for_all (bkd_p)
	    {
	      output[bkd_p] = ntg::max(morpho::min(output, bkd_p, Ng_minus),
				       mask[bkd_p]);
	      if (internal::exist_init_erosion(bkd_p.cur(), output, mask, Ng_minus))
		fifo.push(bkd_p);
	    }
	  //  	 Propagation Step
	  while (!fifo.empty())
	    {
	      oln_point_type(I1) p = fifo.front();
	      fifo.pop();
	      oln_neighb_type(N) q(Ng, p);
	      for_all (q) if (output.hold(q))
		{
		  if ((output[q] > output[p]) && (mask[q] != output[q]))
		    {
		      output[q] = ntg::max(output[p], mask[q]);
		      fifo.push(q);
		    }
		}
	    }
	}
	return output;
      }


    } // hybrid
  } // morpho
} // oln

#endif // OLENA_MORPHO_RECONSTRUCTION_HH
