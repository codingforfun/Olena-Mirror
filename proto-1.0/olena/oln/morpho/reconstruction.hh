// Copyright (C) 2001, 2002, 2004, 2005  EPITA Research and Development Laboratory
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
// As a special exception, you may use this filek as part of a free
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

# include <queue>

# include <mlc/contract.hh>
# include <mlc/cmp.hh>

# include <oln/convert/ng_to_se.hh>
# include <oln/morpho/splitse.hh>
# include <oln/level/compare.hh>
# include <oln/core/abstract/neighborhood.hh>
# include <oln/core/properties.hh>
# include <oln/core/abstract/image_operator.hh>
# include <oln/utils/clone.hh>

// FIXME: ADD TESTS !!!!

namespace oln {

  namespace morpho {

    // fwd decl
    template <typename I, typename E> struct reconstruction_ret;

  }

  // category
  template <typename I, typename E>
  struct set_category< morpho::reconstruction_ret<I,E> > { typedef category::image ret; };

  // super_type
  template <typename I, typename E>
  struct set_super_type< morpho::reconstruction_ret<I,E> >
  {
    typedef abstract::image_binary_operator<I, I, I, morpho::reconstruction_ret<I, E> > ret;
    // FIXME: see below
  };


  namespace morpho {

    template <typename I, typename N>
    struct reconstruction_ret : public abstract::image_binary_operator<I, I, I, reconstruction_ret<I, N> >
      // FIXME: abstract::image_binary_operator<oln_type_of(I, concrete), ...
    {
      typedef abstract::image_binary_operator<I, I, I, reconstruction_ret<I, N> > super_type;

      const N ng;

      reconstruction_ret(const abstract::non_vectorial_image<I>& input1,
			 const abstract::non_vectorial_image<I>& input2,
			 const abstract::neighborhood<N>& ng) :
	super_type(input1.exact(), input2.exact()),
	ng(ng.exact())
      {}

    };

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

      namespace impl {

	template <typename I, typename N>
	struct reconstruction_dilation_ret : public reconstruction_ret<I, N>
	{
	  typedef reconstruction_ret<I, N> super_type;

	  reconstruction_dilation_ret(const abstract::non_vectorial_image<I>& input1, //marker
				      const abstract::non_vectorial_image<I>& input2, //mask
				      const abstract::neighborhood<N>& ng)

	    : super_type(input1, input2, ng)
	  {}

	  void impl_run()
	  {
	    mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();
	    precondition(this->input1.size() == this->input2.size());
	    precondition(level::is_greater_or_equal(this->input2, this->input1));

	    // Conversion of neighborhood into a SE.
	    oln_type_of(N, window) se_plus = get_plus_se_p(convert::ng_to_cse(this->ng));
	    oln_type_of(N, window) se_minus = get_minus_se_p(convert::ng_to_cse(this->ng));

	    I output;
	    output = utils::clone(this->input1);
	    bool non_stability = true;
	    oln_type_of(I, fwd_piter) fwd_p(output.size());
	    oln_type_of(I, fwd_piter) bkd_p(output.size());
	    while (non_stability)
	      {
		I work;
		work = utils::clone(output);
		for_all (fwd_p)
		  work[fwd_p] = ntg::min(morpho::max(work, fwd_p, se_plus), this->input2[fwd_p].value());
		for_all (bkd_p)
		  work[bkd_p] = ntg::min(morpho::max(work, bkd_p, se_minus), this->input2[bkd_p].value());
		non_stability = !(level::is_equal(work, output));
		output = work;
	      }
	    this->output = output;
	  }
	};

      }

      template<class I, class N>
      reconstruction_ret<I, N>
      geodesic_reconstruction_dilation(const abstract::non_vectorial_image<I> & marker,
				       const abstract::non_vectorial_image<I> & mask,
				       const abstract::neighborhood<N>& ng)
      {
	impl::reconstruction_dilation_ret<I, N> tmp(marker, mask, ng);
	tmp.run();
	return tmp;
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
	  oln_type_of(E, fwd_witer) dp(se.exact());
	  for_all (dp)
	    {
	      P q = (oln_type_of(E, dpoint))dp + p.exact();
	      if (marker.hold(q) && (marker[q] < marker[p.exact()]) &&
		  (marker[q] < mask[q]))
		return true;
	    }
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

      namespace impl {

	template <typename I, typename N>
	struct reconstruction_dilation_ret : public reconstruction_ret<I, N>
	{
	  typedef reconstruction_ret<I, N> super_type;

	  reconstruction_dilation_ret(const abstract::non_vectorial_image<I>& input1, //marker
				      const abstract::non_vectorial_image<I>& input2, //mask
				      const abstract::neighborhood<N>& ng)

	    : super_type(input1, input2, ng)
	  {}

	  void impl_run()
	  {
	    mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();
	    precondition(this->input1.size() == this->input2.size());
	    precondition(level::is_greater_or_equal(this->input2, this->input1));

	    oln_type_of(I, concrete) output;
	    output = utils::clone(this->input1);
	    {
	      oln_type_of(N, window) se_plus = get_plus_se_p(convert::ng_to_cse(this->ng));
	      oln_type_of(N, window) se_minus = get_minus_se_p(convert::ng_to_cse(this->ng));
	      oln_type_of(I, fwd_piter) fwd_p(output.size());
	      oln_type_of(I, fwd_piter) bkd_p(output.size());

	      for_all (fwd_p)
		output[fwd_p] = ntg::min(morpho::max(output, fwd_p, se_plus),
					 this->input2[fwd_p].value());

	      std::queue<oln_type_of(I, point) > fifo;
	      for_all (bkd_p)
		{
		  output[bkd_p] = ntg::min(morpho::max(output, bkd_p, se_minus),
					   this->input2[bkd_p].value());
		  if (internal::exist_init_dilation((oln_type_of(I, point))bkd_p, output,
						    this->input2, se_minus))
		    fifo.push(bkd_p);
		}
	      // Propagation Step
	      while (!fifo.empty())
		{
		  oln_type_of(I, point) p = fifo.front();
		  fifo.pop();
		  typedef oln_type_of(N, window) window_type;
		  window_type w = convert::ng_to_se(this->ng);
		  oln_type_of(window_type, fwd_witer) dp(w);

		  for_all (dp)
		    {
		      oln_type_of(I, point) q = (oln_type_of(window_type, dpoint))dp + p;
		      if (output.hold(q))
			{
			  if ((output[q] < output[p]) &&
			      (this->input2[q] != output[q]))
			    {
			      output[q] = ntg::min(output[p].value(),
							this->input2[q].value());
			      fifo.push(q);
			    }
			}
		    }
		}
	    }
	    this->output = output;
	  }
	};

      }

      template<class I, class N>
      reconstruction_ret<I, N>
      geodesic_reconstruction_dilation(const abstract::non_vectorial_image<I> & marker,
				       const abstract::non_vectorial_image<I> & mask,
				       const abstract::neighborhood<N>& ng)
      {
	impl::reconstruction_dilation_ret<I, N> tmp(marker, mask, ng);
	tmp.run();
	return tmp;
      }
    }// hybrid


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


      namespace impl {

	template <typename I, typename N>
	struct reconstruction_erosion_ret : public reconstruction_ret<I, N>
	{
	  typedef reconstruction_ret<I, N> super_type;

	  reconstruction_erosion_ret(const abstract::non_vectorial_image<I>& input1, //marker
				      const abstract::non_vectorial_image<I>& input2, //mask
				      const abstract::neighborhood<N>& ng)

	    : super_type(input1, input2, ng)
	  {}

	  void impl_run()
	  {
	    mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();
	    precondition(this->input1.size() == this->input2.size());
	    precondition(level::is_greater_or_equal(this->input2, this->input1));

	    // Conversion of neighborhood into a SE.
	    oln_type_of(N, window) se_plus = get_plus_se_p(convert::ng_to_cse(this->ng));
	    oln_type_of(N, window) se_minus = get_minus_se_p(convert::ng_to_cse(this->ng));

	    I output;
	    output = utils::clone(this->input1);
	    bool non_stability = true;
	    oln_type_of(I, fwd_piter) fwd_p(output.size());
	    oln_type_of(I, fwd_piter) bkd_p(output.size());
	    while (non_stability)
	      {
		I work;
		work = utils::clone(output);
		for_all (fwd_p)
		  work[fwd_p] = ntg::max(morpho::min(work, fwd_p, se_plus),
					 this->input2[fwd_p].value());
		for_all (bkd_p)
		  work[bkd_p] = ntg::max(morpho::min(work, bkd_p, se_minus),
					 this->input2[bkd_p].value());
		non_stability = !(level::is_equal(work, output));
		output = work;
	      }
	    this->output = output;
	  }
	};

      }

      template<class I, class N>
      reconstruction_ret<I, N>
      geodesic_reconstruction_erosion(const abstract::non_vectorial_image<I> & marker,
				      const abstract::non_vectorial_image<I> & mask,
				      const abstract::neighborhood<N>& ng)
      {
	impl::reconstruction_erosion_ret<I, N> tmp(marker, mask, ng);
	tmp.run();
	return tmp;
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
	  oln_type_of(E, fwd_witer) dp(se.exact());
	  for_all (dp)
	    {
	      P q = (oln_type_of(E, dpoint))dp + p.exact();
	      if (marker.hold(q) && (marker[q] > marker[p.exact()]) &&
		  (marker[q] > mask[q]))
		return true;
	    }
	  return false;
	}

      } //internal

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

      namespace impl {

	template <typename I, typename N>
	struct reconstruction_erosion_ret : public reconstruction_ret<I, N>
	{
	  typedef reconstruction_ret<I, N> super_type;

	  reconstruction_erosion_ret(const abstract::non_vectorial_image<I>& input1, //marker
				      const abstract::non_vectorial_image<I>& input2, //mask
				      const abstract::neighborhood<N>& ng)

	    : super_type(input1, input2, ng)
	  {}

	  void impl_run()
	  {
	    mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();
	    precondition(this->input1.size() == this->input2.size());
	    precondition(level::is_greater_or_equal(this->input2, this->input1));

	    oln_type_of(I, concrete) output;
	    output = utils::clone(this->input1);
	    {
	      oln_type_of(N, window) se_plus = get_plus_se_p(convert::ng_to_cse(this->ng));
	      oln_type_of(N, window) se_minus = get_minus_se_p(convert::ng_to_cse(this->ng));
	      oln_type_of(I, fwd_piter) fwd_p(output.size());
	      oln_type_of(I, fwd_piter) bkd_p(output.size());

	      for_all (fwd_p)
		output[fwd_p] = ntg::max(morpho::min(output, fwd_p, se_plus),
					 this->input2[fwd_p].value());

	      std::queue<oln_type_of(I, point) > fifo;
	      for_all (bkd_p)
		{
		  output[bkd_p] = ntg::max(morpho::min(output, bkd_p, se_minus),
					   this->input2[bkd_p].value());
		  if (internal::exist_init_erosion((oln_type_of(I, point))bkd_p, output,
						    this->input2, se_minus))
		    fifo.push(bkd_p);
		}
	      // Propagation Step
	      while (!fifo.empty())
		{
		  oln_type_of(I, point) p = fifo.front();
		  fifo.pop();
		  typedef oln_type_of(N, window) window_type;
		  window_type w = convert::ng_to_se(this->ng);
		  oln_type_of(window_type, fwd_witer) dp(w);

		  for_all (dp)
		    {
		      oln_type_of(I, point) q = (oln_type_of(window_type, dpoint))dp + p;
		      if (output.hold(q))
			{
			  if ((output[q] > output[p]) &&
			      (this->input2[q] != output[q]))
			    {
			      output[q] = ntg::max(output[p].value(),
						   this->input2[q].value());
			      fifo.push(q);
			    }
			}
		    }
		}
	    }
	    this->output = output;
	  }
	};

      }

      template<class I, class N>
      reconstruction_ret<I, N>
      geodesic_reconstruction_erosion(const abstract::non_vectorial_image<I> & marker,
				      const abstract::non_vectorial_image<I> & mask,
				      const abstract::neighborhood<N>& ng)
      {
	impl::reconstruction_erosion_ret<I, N> tmp(marker, mask, ng);
	tmp.run();
	return tmp;
      }
    }// hybrid

  }

}

#endif // ! OLENA_MORPHO_RECONSTRUCTION_HH
