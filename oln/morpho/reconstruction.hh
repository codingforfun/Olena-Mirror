// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_RECONSTRUCTION_HH
# define OLENA_MORPHO_RECONSTRUCTION_HH

# include <oln/basics.hh>
# include <oln/morpho/stat.hh>
# include <oln/morpho/geodesic_dilation.hh>
# include <oln/morpho/geodesic_erosion.hh>
# include <oln/morpho/splitse.hh>
# include <oln/level/compare.hh>
# include <oln/types/global_ops_traits.hh>
# include <oln/convert/conversion_ng_se.hh>
# include <queue>

namespace oln {
  namespace morpho {

    /*=processing sure_geodesic_reconstruction_dilation
     * ns: morpho
     * what: Geodesic reconstruction by dilation.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const image<I2>&, mask, IN, mask image
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc:
     *  Compute the reconstruction by dilation of \var{marker} with respect
     * to the mask \var{mask} image using \var{se}
     *   as structural element. Soille p.160. This is the simplest algorithm:
     * iteration is performed until stability.
     * see: morpho::simple_geodesic_dilation
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ image2d<int_u8> dark = load("dark.pgm");
     * $ save(morpho::sure_geodesic_reconstruction_dilation(light, dark, win_c8p()), "out.pgm");
     * exi: light.pgm dark.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    template<class _I1, class _I2, class _N>
    Concrete(_I1)
    sure_geodesic_reconstruction_dilation(const image<_I1> & _marker,
					  const image<_I2> & _mask,
					  const neighborhood<_N>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(N, Ng);
      meta::eq<_I1::dim, _I2::dim>::ensure();
      meta::eq<I1::dim, N::dim>::ensure();
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));
      Concrete(_I1) output = marker.clone();
      bool non_stability = true;
      while (non_stability)
	{
	  Concrete(_I1) work = simple_geodesic_dilation(output, mask, Ng);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }


    /*=processing sequential_geodesic_reconstruction_dilation
     * ns: morpho
     * what: Geodesic reconstruction by dilation.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const image<I2>&, mask, IN, mask image
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc:
     *  Compute the reconstruction by dilation of \var{marker} with respect
     * to the mask \var{mask} image using \var{se}
     *   as structural element. Soille p.160. The algorithm used is the
     * one defined as sequential
     * in Vincent(1993), Morphological grayscale reconstruction in
     *  image analysis: applications and efficient algorithms, itip, 2(2),
     * 176--201.
     * see: morpho::simple_geodesic_dilation
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ image2d<int_u8> dark = load("dark.pgm");
     * $ save(morpho::sequential_geodesic_reconstruction_dilation(light, dark, win_c8p()), "out.pgm");
     * exi: light.pgm dark.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    template<class _I1, class _I2, class _N>
    Concrete(_I1)
    sequential_geodesic_reconstruction_dilation(const image<_I1> & _marker,
						const image<_I2> & _mask,
						const neighborhood<_N>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(N, Ng);
      meta::eq<_I1::dim, _I2::dim>::ensure();
      meta::eq<I1::dim, N::dim>::ensure();
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));

      // Conversion of neighborhood into a SE.
      typedef typename get_se<N::dim>::ret E;
      E se_plus = get_plus_se_p(convert::ng_to_cse(Ng));
      E se_minus = get_minus_se_p(convert::ng_to_cse(Ng));

      Concrete(I1) output = marker.clone();
      bool non_stability = true;
      typename I1::fwd_iter fwd_p(output);
      typename I1::bkd_iter bkd_p(output);
      while (non_stability)
	{
	  Concrete(I1) work = output.clone();
	  border::adapt_copy(work, Ng.delta());
	  for_all (fwd_p)
	    work[fwd_p] = min(morpho::max(work, fwd_p, se_plus), mask[fwd_p]);
  	  for_all (bkd_p)
	    work[bkd_p] = min(morpho::max(work, bkd_p, se_minus), mask[bkd_p]);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }


    template<class _I1, class _I2, class _N>
    Concrete(_I1)
    vincent_sequential_geodesic_reconstruction_dilation(const image<_I1> & _marker,
							const image<_I2> & _mask,
							const neighborhood<_N>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(N, Ng);
      meta::eq<_I1::dim, _I2::dim>::ensure();
      meta::eq<I1::dim, N::dim>::ensure();
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));

      // Conversion of neighborhood into a SE.
      typedef typename get_se<N::dim>::ret E;
      E se_plus = get_plus_se_p(convert::ng_to_cse(Ng));
      E se_minus = get_minus_se_p(convert::ng_to_cse(Ng));

      Concrete(I1) output = marker.clone();
      bool non_stability = true;
      typename I1::fwd_iter fwd_p(output);
      typename I1::bkd_iter bkd_p(output);
      Concrete(I1) work(marker.size());
      while (non_stability)
	{
	  border::adapt_copy(marker, Ng.delta());
	  for_all (fwd_p)
	    work[fwd_p] = min(morpho::max(work, fwd_p, se_plus), mask[fwd_p]);
  	  for_all (bkd_p)
	    work[bkd_p] = min(morpho::max(work, bkd_p, se_minus), mask[bkd_p]);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }


    template<class _P, class _I1, class _I2, class _E> inline
    static
    bool exist_init_dilation(const point<_P>& _p,
			     const image<_I1>& _marker,
			     const image<_I2>& _mask,
			     const struct_elt<_E>& _se)
    {
      Exact_cref(P, p);
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(E, se);
      meta::eq<_I1::dim, _I2::dim>::ensure();
      meta::eq<I1::dim, E::dim>::ensure();
      meta::eq<I1::dim, P::dim>::ensure();

      Neighb(E) q(se, p);
      for_all (q)
	if (marker.hold(q) && (marker[q] < marker[p]) && (marker[q] < mask[q]))
	  return true;
      return false;
    }


    /*=processing hybrid_geodesic_reconstruction_dilation
     * ns: morpho
     * what: Geodesic reconstruction by dilation.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const image<I2>&, mask, IN, mask image
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc:
     *  Compute the reconstruction by dilation of \var{marker} with respect
     * to the mask \var{mask} image using \var{se}
     *   as structural element. Soille p.160. The algorithm used is the
     * one defined as hybrid
     * in Vincent(1993), Morphological grayscale reconstruction in
     *  image analysis: applications and efficient algorithms, itip, 2(2),
     * 176--201.
     * see: morpho::simple_geodesic_dilation
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ image2d<int_u8> dark = load("dark.pgm");
     * $ save(morpho::hybrid_geodesic_reconstruction_dilation(light, dark, win_c8p()), "out.pgm");
     * exi: light.pgm dark.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    template<class _I1, class _I2, class _N>
    Concrete(_I1)
    hybrid_geodesic_reconstruction_dilation(const image<_I1> & _marker,
					    const image<_I2> & _mask,
					    const neighborhood<_N>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(N, Ng);
      meta::eq<I1::dim, I2::dim>::ensure();
      meta::eq<I1::dim, N::dim>::ensure();

      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(mask, marker));

      Concrete(I1) output = marker.clone();
      border::adapt_copy(output, Ng.delta());
      {
	typedef typename get_se<N::dim>::ret E;
	E Ng_plus = get_plus_se_p(convert::ng_to_cse(Ng));
	E Ng_minus = get_minus_se_p(convert::ng_to_cse(Ng));
	typename I1::fwd_iter fwd_p(output);
	typename I1::bkd_iter bkd_p(output);
	for_all (fwd_p)
	  output[fwd_p] = min(morpho::max(output, fwd_p, Ng_plus),
			      mask[fwd_p]);

	std::queue< Point(I1) > fifo;
	for_all (bkd_p)
	  {
	    output[bkd_p] = min(morpho::max(output, bkd_p, Ng_minus),
				mask[bkd_p]);
	    if (exist_init_dilation(bkd_p.cur(), output, mask, Ng_minus))
	      fifo.push(bkd_p);
	  }
	// Propagation Step
	while (!fifo.empty())
	  {
	    Point(I1) p = fifo.front();
	    fifo.pop();
	    Neighb(N) q(Ng, p);
	    for_all (q) if (output.hold(q))
	      {
		if ((output[q] < output[p]) && (mask[q] != output[q]))
		  {
		    output[q] = min(output[p], mask[q]);
		    fifo.push(q);
		  }
	      }
	  }
      }
      return output;
    }

    //GEODESIC RECONSTRUCTION EROSION


    /*=processing sure_geodesic_reconstruction_erosion
     * ns: morpho
     * what: Geodesic reconstruction by erosion.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const image<I2>&, mask, IN, mask image
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc:
     *  Compute the reconstruction by erosion of \var{marker} with respect
     * to the mask \var{mask} image using \var{se}
     *   as structural element. Soille p.160. This is the simplest algorithm :
     * iteration is performed until stability.
     * see: morpho::simple_geodesic_erosion
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ image2d<int_u8> dark = load("dark.pgm");
     * $ save(morpho::sure_geodesic_reconstruction_erosion(light, dark, win_c8p()), "out.pgm");
     * exi: light.pgm dark.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    template<class _I1, class _I2, class _N>
    Concrete(_I1)
    sure_geodesic_reconstruction_erosion(const image<_I1> & _marker,
					 const image<_I2> & _mask,
					 const neighborhood<_N>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(N, Ng);
      meta::eq<I1::dim, I2::dim>::ensure();
      meta::eq<I1::dim, N::dim>::ensure();
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(marker, mask));
      Concrete(_I1) output = marker.clone();
      bool non_stability = true;
      while (non_stability)
	{
	  Concrete(_I1) work = simple_geodesic_erosion(output, mask, Ng);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }

    /*=processing sequential_geodesic_reconstruction_erosion
     * ns: morpho
     * what: Geodesic reconstruction by erosion.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const image<I2>&, mask, IN, mask image
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc:
     *  Compute the reconstruction by erosion of \var{marker} with respect
     * to the mask \var{mask} image using \var{se}
     *   as structural element. Soille p.160.  The algorithm used is the
     * one defined as sequential
     * in Vincent(1993), Morphological grayscale reconstruction in
     *  image analysis: applications and efficient algorithms, itip, 2(2),
     * 176--201.
     * see: morpho::simple_geodesic_erosion
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ image2d<int_u8> dark = load("dark.pgm");
     * $ save(morpho::sequential_geodesic_reconstruction_erosion(light, dark, win_c8p()), "out.pgm");
     * exi: light.pgm dark.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/
    template<class _I1, class _I2, class _N>
    Concrete(_I1)
    sequential_geodesic_reconstruction_erosion(const image<_I1>& _marker,
					       const image<_I2>& _mask,
					       const neighborhood<_N>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(N, Ng);
      meta::eq<I1::dim, I2::dim>::ensure();
      meta::eq<I1::dim, N::dim>::ensure();
      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(marker, mask));

      typedef typename get_se<N::dim>::ret E;
      E se_plus = get_plus_se_p(convert::ng_to_cse(Ng));
      E se_minus = get_minus_se_p(convert::ng_to_cse(Ng));
      Concrete(I1) output = marker.clone();

      bool non_stability = true;
      typename I1::fwd_iter fwd_p(output);
      typename I1::bkd_iter bkd_p(output);
      while (non_stability)
	{
	  Concrete(I1) work = output.clone();
	  border::adapt_copy(work, Ng.delta());
	  for_all (fwd_p)
	    work[fwd_p] = max(morpho::min(work, fwd_p, se_plus), mask[fwd_p]);
  	  for_all (bkd_p)
	    work[bkd_p] = max(morpho::min(work, bkd_p, se_minus), mask[bkd_p]);
	  non_stability = !(level::is_equal(work, output));
	  output = work;
	}
      return output;
    }


    template<class _P, class _I1, class _I2, class _E> inline
    static
    bool exist_init_erosion(const point<_P>& _p,
			    const image<_I1>& _marker,
			    const image<_I2>& _mask,
			    const struct_elt<_E>& _se)
    {
      Exact_cref(P, p);
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(E, se);
      meta::eq<I1::dim, I2::dim>::ensure();
      meta::eq<I1::dim, E::dim>::ensure();
      meta::eq<I1::dim, P::dim>::ensure();

      Neighb(E) q(se, p);
      for_all (q)
	if (marker.hold(q) && (marker[q] > marker[p]) && (marker[q] > mask[q]))
	  return true;
      return false;
    }


    /*=processing hybrid_geodesic_reconstruction_erosion
     * ns: morpho
     * what: Geodesic reconstruction by erosion.
     * arg: const image<I1>&, marker, IN, marker image
     * arg: const image<I2>&, mask, IN, mask image
     * arg: const struct_elt<E>&, se, IN, structural element
     * ret: Concrete(I1)
     * doc:
     *  Compute the reconstruction by erosion of \var{marker} with respect
     * to the mask \var{mask} image using \var{se}
     *   as structural element. Soille p.160.  The algorithm used is the
     * one defined as hybrid
     * in Vincent(1993), Morphological grayscale reconstruction in
     *  image analysis: applications and efficient algorithms, itip, 2(2),
     * 176--201.
     * see: morpho::simple_geodesic_erosion
     * ex:
     * $ image2d<int_u8> light = load("light.pgm");
     * $ image2d<int_u8> dark = load("dark.pgm");
     * $ save(morpho::sequential_geodesic_reconstruction_erosion(light, dark, win_c8p()), "out.pgm");
     * exi: light.pgm dark.pgm
     * exo: out.pgm
     * wontcompile: fixme
     =*/

    template<class _I1, class _I2, class _N>
    Concrete(_I1)
    hybrid_geodesic_reconstruction_erosion(const image<_I1> & _marker,
					   const image<_I2> & _mask,
					   const neighborhood<_N>& _Ng)
    {
      Exact_cref(I1, marker);
      Exact_cref(I2, mask);
      Exact_cref(N, Ng);
      meta::eq<I1::dim, I2::dim>::ensure();
      meta::eq<I1::dim, N::dim>::ensure();

      precondition(marker.size() == mask.size());
      precondition(level::is_greater_or_equal(marker, mask));

      Concrete(I1) output = marker.clone();
      border::adapt_copy(output, Ng.delta());
      {
	typedef typename get_se<N::dim>::ret E;
     	E Ng_plus = get_plus_se_p(convert::ng_to_cse(Ng));
     	E Ng_minus = get_minus_se_p(convert::ng_to_cse(Ng));
     	typename I1::fwd_iter fwd_p(output);
     	typename I1::bkd_iter bkd_p(output);
     	for_all (fwd_p)
     	  output[fwd_p] = max(morpho::min(output, fwd_p, Ng_plus),
			      mask[fwd_p]);

     	std::queue< Point(I1) > fifo;
     	for_all (bkd_p)
     	  {
     	    output[bkd_p] = max(morpho::min(output, bkd_p, Ng_minus),
				mask[bkd_p]);
     	    if (exist_init_erosion(bkd_p.cur(), output, mask, Ng_minus))
     	      fifo.push(bkd_p);
     	  }
	//  	 Propagation Step
     	while (!fifo.empty())
     	  {
     	    Point(I1) p = fifo.front();
     	    fifo.pop();
     	    Neighb(N) q(Ng, p);
     	    for_all (q) if (output.hold(q))
     	      {
     		if ((output[q] > output[p]) && (mask[q] != output[q]))
     		  {
     		    output[q] = max(output[p], mask[q]);
     		    fifo.push(q);
     		  }
     	      }
     	  }
      }
      return output;
    }

  } // morpho
} // oln

#endif // OLENA_MORPHO_RECONSTRUCTION_HH
