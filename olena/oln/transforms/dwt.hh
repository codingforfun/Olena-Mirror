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

#ifndef OLENA_TRANSFORMS_DWT_HH
# define OLENA_TRANSFORMS_DWT_HH

# include <oln/config/system.hh>
# include <oln/basics.hh>

# include <ntg/basics.hh>
# include <ntg/float.hh>
# include <ntg/utils/cast.hh>
# include <mlc/array/1d.hh>

// FIXME: this file has not been adjusted to the coding style since it
// will be completely rewritten in next release.

namespace oln {

  /*!
  ** \brief Transform algorithm implementation.
  */
  namespace transforms {

    // macros used to define all the wavelets coefficients

# define Wavelet_coeffs_definition(Name, Type, Size) \
    struct Name : public oln::internal::wavelet_coeffs_<Type, Size, Name > \
    { \
      Name()

# define Wavelet_coeffs_begin \
      h = (wc_start =

# define Wavelet_coeffs_end \
        , end); \
      init(); \
    }

    /*!
    ** \brief type of dwt to perform.
    */
    typedef enum {
      dwt_std,
      dwt_non_std
    } dwt_transform_type;

  } // end of namespace transforms

  namespace internal
  {
    /// \brief Value of ln(2).
    static const ntg::float_d ln_2_ = 0.6931471805599453092;

    /*!
    ** \brief Wavelet coefficient data structure.
    **
    ** \param T Coefficients data type.
    ** \param N Number of coefficients.
    ** \param Self Self type.
    */
    template <class T, unsigned N, class Self>
    struct wavelet_coeffs_
    {
      typedef T		value_t; ///< Type of data used.
      typedef Self	self_t; ///< Self type.

    public:

      // accessors
      /// Accessor to ith element of g.
      const value_t getG(unsigned i) const { return g[i]; }
      /// Accessor to ith element of ig.
      const value_t getInvG(unsigned i) const { return ig[i]; }
      /// Accessor to ith element of h.
      const value_t getH(unsigned i) const { return h[i]; }
      /// Accessor to ith element of ih.
      const value_t getInvH(unsigned i) const { return ih[i]; }
      /// Give the size of the arrays.
      const unsigned size() const { return size_; }

    protected:
      /*!
      ** \brief Initialization method.
      **
      ** Fill coefficients.
      */
      void init()
      {
	for (unsigned i = 0; i < size_; i += 2) {
	  g[i] = -h[size_ - 1 - i];
	  g[i + 1] = h[size_ - 2 - i];
	  ig[size_ - 1 - i] = g[i + 1];
	  ig[size_ - 2 - i] = h[i + 1];
	  ih[size_ - 1 - i] = g[i];
	  ih[size_ - 2 - i] = h[i];
	}
      }

      /*!
      ** \brief Constructor.
      */
      wavelet_coeffs_(){}

      /*!
      ** \brief Destructor.
      **
      ** Its aim is to check N is pair.
      */
      ~wavelet_coeffs_()
      {
	mlc::is_false<N % 2>::ensure();
      }

      mlc::array1d< mlc::array1d_info<N>, value_t>	h; ///< array of value_t.
      mlc::internal::array1d_start_<value_t>		wc_start; ///< First coefficient.

    private:

      value_t	g[N]; ///< Array of value_t.
      value_t	ih[N]; ///< Array of value_t.
      value_t	ig[N]; /// Array of value_t.
      enum {
	size_ = N
      };
    };

    /*!
    ** \brief Step of a dwt transform.
    **
    ** \param I Exact type of the input image.
    ** \param K type of coefficients.
    **
    ** \arg im Image to process.
    ** \arg p_ Point to work on.
    ** \arg d Component d of p.
    ** \arg n Step number.
    ** \arg coeffs Coefficients.
    */
    template <class I, class K>
    void dwt_transform_step_(abstract::image<I>& im,
			     const oln_point_type(I)& p_,
			     const unsigned d,
			     const unsigned n,
			     const K& coeffs)
    {
      assertion(n >= coeffs.size());

      const unsigned	half = n >> 1;
      unsigned		lim = n + 1 - coeffs.size();
      ntg::float_d*	tmp = new ntg::float_d[n];
      oln_point_type(I)	p(p_);
      unsigned		i, j, k;

      for (i = 0, j = 0; j < lim; j += 2, i++) {
	tmp[i] = 0;
	tmp[i + half] = 0;
	for (k = 0; k < coeffs.size(); k++) {
	  p.nth(d) = j + k;
	  tmp[i] += im[p] * coeffs.getH(k);
	  tmp[i + half] += im[p] * coeffs.getG(k);
	}
      }
      for (; i < half; j += 2, i++) {
	tmp[i] = 0;
	tmp[i + half] = 0;
	for (unsigned k = 0; k < coeffs.size(); k++) {
	  p.nth(d) = (j + k) % n;
	  tmp[i] += im[p] * coeffs.getH(k);
	  tmp[i + half] += im[p] * coeffs.getG(k);
	}
      }

      for (i = 0; i < n; i++) {
	p.nth(d) = i;
	im[p] = tmp[i];
      }

      delete[] tmp;
    }

    /*!
    ** \brief Step of a dwt invert transform.
    **
    ** \param I Exact type of the input image.
    ** \param K type of coefficients.
    **
    ** \arg im Image to process.
    ** \arg p_ Point to work on.
    ** \arg d Component d of p.
    ** \arg n Step number.
    ** \arg coeffs Coefficients.
    */
    template <class I, class K>
    void dwt_transform_inv_step_(abstract::image<I>& im,
				 const oln_point_type(I)& p_,
				 const unsigned d,
				 const unsigned n,
				 const K& coeffs)
    {
      assertion(n >= coeffs.size());

      const unsigned half = n >> 1;
      unsigned lim = coeffs.size() - 2;
      ntg::float_d* tmp = new ntg::float_d[n];
     oln_point_type(I) p(p_), q(p_);
      unsigned i, j, k, l;

      for (i = half - lim / 2, j = 0; j < lim; i++, j += 2) {
	tmp[j] = 0;
	tmp[j + 1] = 0;
	for (k = 0, l = 0; l < coeffs.size(); k++, l += 2) {
	  p.nth(d) = (i + k) % half;
	  q.nth(d) = p.nth(d) + half;
	  tmp[j] += im[p] * coeffs.getInvH(l) + im[q] * coeffs.getInvH(l + 1);
	  tmp[j + 1] += im[p] * coeffs.getInvG(l) + im[q] * coeffs.getInvG(l + 1);
	}
      }
      lim = n - 1;
      for (i = 0; j < lim; i++, j += 2) {
	tmp[j] = 0;
	tmp[j + 1] = 0;
	for (k = 0, l = 0; l < coeffs.size(); k++, l += 2) {
	  p.nth(d) = i + k;
	  q.nth(d) = p.nth(d) + half;
	  tmp[j] += im[p] * coeffs.getInvH(l) + im[q] * coeffs.getInvH(l + 1);
	  tmp[j + 1] += im[p] * coeffs.getInvG(l) + im[q] * coeffs.getInvG(l + 1);
	}
      }

      for (i = 0; i < n; i++) {
	p.nth(d) = i;
	im[p] = tmp[i];
      }

      delete[] tmp;
    }

    // Functions used to iterate over all dimensions except one
    /*!
    ** \brief Enum to know if you go in forward or backward order.
    */
    typedef enum {
      dwt_fwd,
      dwt_bwd
    } dwt_transform_dir_;

    /*!
    ** \brief Functions used to iterate over all dimensions except one.
    **
    ** \param dim Number of dimension to treat.
    ** \param skip Dimension to skip.
    ** \param current Current dimension.
    */
    template <unsigned dim, unsigned skip,
	      unsigned current = dim>
    struct dim_skip_iterate_rec_
    {
      /*!
      ** \brief Iterate over all dimensions except one.
      */
      template <class I, class K>
      static void doit(abstract::image<I>& im,
		      oln_point_type(I)& p,
		       const unsigned l1,
		       const unsigned l2,
		       const K& coeffs,
		       dwt_transform_dir_ d)
      {
	if (current != skip) {
	  for (unsigned i = 0; i < l2; i++) {
	    p.nth(current - 1) = i;
	    dim_skip_iterate_rec_<dim, skip, current - 1>::
	      doit(im, p, l1, l2, coeffs, d);
	  }
	  p.nth(current - 1) = 0;
	}
	else
	  dim_skip_iterate_rec_<dim, skip, current - 1>::
	    doit(im, p, l1, l2, coeffs, d);
      }
    };

    /*!
    ** \brief  Specialization  of  dim_skip_iterate_rec_ with  current
    ** dimension set to 0.
    **
    ** \param dim Number of dimension to process.
    ** \param skip Dimension to skip.
    */
    template <unsigned dim, unsigned skip>
    struct dim_skip_iterate_rec_<dim, skip, 0>
    {
      /*!
      ** \brief Iterate over all dimensions except one.
      **
      ** \param I Exact type of the image to process.
      ** \param K Type of coefficients.
      **
      ** It is leaf call, thus you can call a dwt transform step.
      */
      template <class I, class K>
      static void doit(abstract::image<I>& im,
		      oln_point_type(I)& p,
		       const unsigned l1,
		       const unsigned l2,
		       const K& coeffs,
		       dwt_transform_dir_ d)
      {
	unsigned n;

	switch (d) {
	case dwt_fwd:
	  for (n = l2; n >= l1; n >>= 1)
	    dwt_transform_step_(im, p, skip - 1, n, coeffs);
	  break;
	case dwt_bwd:
	  for (n = l1; n <= l2; n <<= 1)
	    dwt_transform_inv_step_(im, p, skip - 1, n, coeffs);
	  break;
	};
      }
    };

    /*!
    ** \brief Iterate over all dimensions except one.
    **
    ** \param dim Number of dimension to process.
    ** \param skip Dimension to skip.
    */
    template <unsigned dim, unsigned skip>
    struct dim_iterate_rec_
    {
      /*!
      ** \brief Iterate over all dimensions except one.
      **
      ** \param I Exact type of the image to process.
      ** \param K Type of coefficients.
      */
      template <class I, class K>
      static void doit(abstract::image<I>& im,
		      oln_point_type(I)& p,
		       const unsigned l1,
		       const unsigned l2,
		       const K& coeffs,
		       dwt_transform_dir_ d)
      {
	dim_skip_iterate_rec_<dim, skip>::doit(im, p, l1, l2, coeffs, d);
	dim_iterate_rec_<dim, skip - 1>::doit(im, p, l1, l2, coeffs, d);
      }
    };

    /*!
    ** \brief Specialization  of dim_iterate_rec_ with  skip dimension
    ** set to 0.
    **
    ** \param dim Number of dimension to process.
    */
    template <unsigned dim>
    struct dim_iterate_rec_<dim, 0>
    {

      /*!
      ** \brief Iterate over all dimensions except one.
      **
      ** \param I Exact type of the image to process.
      ** \param K Type of coefficients.
      **
      ** End of recursion.
      */
      template <class I, class K>
      static void doit(abstract::image<I>& ,
		      oln_point_type(I)& ,
		       const unsigned ,
		       const unsigned ,
		       const K& ,
		       dwt_transform_dir_ )
      {
	// end of recursion
      }
    };

    /*!
    ** \brief Internal dwt transform function.
    **
    ** \param I Exact type of the image to process.
    ** \param K Type of coefficients.
    */
    template <class I, class K>
    void dwt_transform_(abstract::image<I>& im,
			const unsigned l1,
			const unsigned l2,
			const K& coeffs,
			transforms::dwt_transform_type t)
    {
     oln_point_type(I) p;

      switch (t) {
      case transforms::dwt_std:
	dim_iterate_rec_<I::dim, I::dim>::doit(im, p, l1, l2, coeffs, dwt_fwd);
	break;
      case transforms::dwt_non_std:
	for (unsigned n = l2; n >= l1; n >>= 1)
	  dim_iterate_rec_<I::dim, I::dim>::doit(im, p, n, n, coeffs, dwt_fwd);
	break;
      }
    }

    /*!
    ** \brief Internal dwt invert transform function.
    **
    ** \param I Exact type of the image to process.
    ** \param K Type of coefficients.
    */
    template <class I, class K>
    void dwt_transform_inv_(abstract::image<I>& im,
			    const unsigned l1,
			    const unsigned l2,
			    const K& coeffs,
			    transforms::dwt_transform_type t)
    {
     oln_point_type(I) p;

      switch (t) {
      case transforms::dwt_std:
	dim_iterate_rec_<I::dim, I::dim>::doit(im, p, l1, l2, coeffs, dwt_bwd);
	break;
      case transforms::dwt_non_std:
	for (unsigned n = l1; n <= l2; n <<= 1)
	  dim_iterate_rec_<I::dim, I::dim>::doit(im, p, n, n, coeffs, dwt_bwd);
	break;
      }
    }

  } // end of namespace internal

  namespace transforms {

    /*!
    ** \brief Object to compute dwt transforms.
    **
    ** \param I Exact type of the image to process.
    ** \param K Type of coefficients.
    */
    template <class I, class K>
    class dwt
    {
    public:

      typedef I						original_im_t; ///< Exact of the image.
      typedef oln_value_type(I)				original_im_value_t; ///< Original data type of the image.
      typedef typename mute<I, ntg::float_d>::ret	trans_im_t; ///< Type of the transformed image.
      typedef typename K::self_t			coeffs_t; ///< Data type of coefficients.

      /*!
      ** \brief Constructor from an image.
      **
      ** Initialization of dwt transform.
      **
      ** \arg im Image to process.
      */
      dwt(const original_im_t& im) : original_im(im)
      {
	ntg_is_a(original_im_value_t, ntg::real)::ensure();

	im_size = im.size().nth(0);
	assertion(im_size >= coeffs.size());
	for (unsigned i = 1; i < original_im_t::dim; i++)
	  assertion(im_size == static_cast<unsigned>(im.size().nth(i)));

	max_level = static_cast<unsigned>(log(2 * im_size / coeffs.size()) /
			       internal::ln_2_);

	current_level = max_level;

	assertion(!(im_size % (1 << max_level)));

	trans_im = trans_im_t(im.size());
      }

      /*!
      ** \brief Accessor to the transformed image.
      **
      ** Const version.
      */
      const trans_im_t transformed_image() const
      {
	return trans_im;
      }

      /*!
      ** \brief Accessor to the transformed image.
      **
      ** Non const version.
      */
      trans_im_t& transformed_image()
      {
	return trans_im;
      }

      /*!
      ** \brief Compute the dwt transform.
      **
      ** \arg t Type of the transform (standard or non standard)
      ** \arg normalized Do you want a normalized result ?
      ** \arg l Level.
      */
      trans_im_t transform(dwt_transform_type t = dwt_non_std,
			   bool normalized = true, unsigned l = 0)
      {
	if (l == 0) {
	  l = max_level;
	  current_level = max_level;
	} else {
	  if (l > max_level)
	    l = max_level;
	  current_level = l;
	}

	oln_iter_type(trans_im_t) it(trans_im);

	if (normalized) {
	  norm = pow(sqrt(2), original_im_t::dim * l);
	  for_all(it)
	    trans_im[it] = original_im[it] / norm;
	}
	else {
	  norm = 1;
 	  for_all(it)
 	    trans_im[it] = original_im[it];
	}

	unsigned lim = im_size >> (l - 1);
	transform_type = t;

	internal::dwt_transform_(trans_im, lim, im_size, coeffs, t);

	return trans_im;
      }

      /*!
      ** \brief Compute the invert dwt transform.
      **
      ** \arg l Level.
      */
      trans_im_t transform_inv(unsigned l = 0)
      {
	if (l == 0)
	  l = current_level;
	else if (l > current_level)
	  l = current_level;

	trans_im_t new_im(trans_im.size());
	oln_iter_type(trans_im_t) it(trans_im);

	if (norm != 1) {
	  for_all(it)
	    new_im[it] = trans_im[it] * norm;
	}
	else
	  for_all(it)
	    new_im[it] = trans_im[it];

	unsigned lim1 = im_size >> (current_level - 1);
	unsigned lim2 = im_size >> (current_level - l);

	internal::dwt_transform_inv_(new_im, lim1, lim2, coeffs, transform_type);

	return new_im;
      }

      /*!
      ** \brief Compute the invert dwt transform.
      **
      ** \param T1 Data type you want the output image contains.
      **
      ** \arg l Level.
      */
      template <class T1>
      typename mute<I, T1>::ret transform_inv(unsigned l = 0)
      {
	ntg_is_a(T1, ntg::real)::ensure();

	trans_im_t tmp_im = transform_inv(l);
	typename mute<I, T1>::ret new_im(tmp_im.size());

	oln_iter_type(trans_im_t) it(tmp_im);

	for_all(it)
	  new_im[it] = (tmp_im[it] >= ntg_inf_val(T1) ?
			(tmp_im[it] <= ntg_sup_val(T1) ?
			 tmp_im [it] :
			 ntg_sup_val(T1)) :
			ntg_inf_val(T1));
	return new_im;
      }

    private:

      const original_im_t&	original_im; ///< The original image.
      trans_im_t		trans_im; ///< The transformed image.
      const coeffs_t		coeffs; ///< The coefficients.
      unsigned			im_size; ///< Size of the image.
      unsigned			max_level; ///< Higher level.
      unsigned			current_level; ///< Current level.
      ntg::float_d		norm; ///< Norm.
      dwt_transform_type	transform_type; ///< Type of transform wanted.
    };

  } // end of namespace transforms

} // end of namespace oln

#endif // ! OLENA_TRANSFORMS_DWT_HH
