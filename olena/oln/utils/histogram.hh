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

#ifndef OLENA_UTILS_HISTOGRAM_HH
# define OLENA_UTILS_HISTOGRAM_HH


# include <oln/core/image1d.hh>

# include <oln/core/image3d.hh>

# include <oln/convert/value_to_point.hh>

# include <oln/core/abstract/image.hh>

# include <ntg/core/pred_succ.hh>

# include <vector>

/// Return the underlying counter type of \a H.
# define oln_cpt_type(H) mlc_exact_type(H)::cpt_type
/// Return the underlying counter type of \a H.
# define oln_cpt_type_(H) mlc_exact_type_(H)::cpt_type

namespace oln {
  /// \brief Utilities, such as statistics.
  namespace utils {

    /// \brief Internal purpose only.
    namespace internal {
      //! Return the size of the space needed to explore the type T.
      template <typename T>
      struct img_max_size
      {
      private:
	typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type
	                                                        ensure_type;
      public:
	typedef image1d_size size_type;
	typedef T comp_type;

	image1d_size
	operator()()
	{
	  size_type s(ntg_max_val(comp_type) - ntg_min_val(comp_type) + 1,
		      1);
	  return s;
	}
      };


      template <unsigned Qbits, template <unsigned> class S>
      struct img_max_size<ntg::color<3, Qbits, S> >
      {
      public:
	typedef image3d_size size_type;
	typedef typename ntg::color<3, Qbits, S>::comp_type comp_type;

	image3d_size
	operator()()
	{
	  size_type s(ntg_max_val(comp_type) - ntg_min_val(comp_type) + 1,
		      ntg_max_val(comp_type) - ntg_min_val(comp_type) + 1,
		      ntg_max_val(comp_type) - ntg_min_val(comp_type) + 1,
		      1);
	  return s;
	}
      };
    } // end of namespace internal

    /// Traits for oln::utils::abstract::histogram hierarchy.
    template <class Exact>
    struct hist_traits
    {
    };


    /// \brief Abstract utilities.
    namespace abstract
    {
      /*! Abstract base class for histogram.
      **
      ** \see oln::histogram
      */
      template<class Exact = mlc::final>
      class histogram: public mlc_hierarchy::any<Exact>
      {
      public:
	typedef typename mlc::exact_vt<histogram<Exact>,
				       Exact>::ret exact_type;
	typedef typename hist_traits<exact_type>::cpt_type cpt_type;
	typedef typename hist_traits<exact_type>::value_type value_type;

	/// Put the number of occurrence of every value to zero.
	void
	clear()
	{
	  this->exact().clear_impl();
	}
	/// Read the number of occurrence of \a v.
	const cpt_type
	operator[](const value_type &v)const
	{
	  return this->exact().at(v);
	}
	/// Read or write the number of occurrence of \a v.
	cpt_type&
	operator[](const value_type &v)
	{
	  return this->exact().at(v);
	}
	/*! Build the histogram of an image.
	**
	** \attention The histogram is not cleared.
	*/
	template <class I>
	void
	init(const oln::abstract::image<I> &img)
	{
	  return this->exact().init_impl(img.exact());
	}
      };
    } // end of namespace abstract

    template<typename T,
	     typename CPT = unsigned,
	     class V2P = oln::convert::value_to_point<T>,
	     class Exact = mlc::final>
    class histogram;

    /// Traits for oln::utils::abstract::histogram hierarchy.
    template <typename T,
	      typename CPT,
	      class V2P,
	      class Exact>
    struct hist_traits<histogram<T, CPT, V2P, Exact> >
    {
      typedef CPT cpt_type;		///< Counter type
      typedef T value_type;		///< Value of the input image
      typedef V2P value_to_point_type;	///< Class that convert a value to
					/// a point
      typedef typename value_to_point_type::result_type point_type;
					///< Point type of the internal image.
      enum {dim = point_type::dim};	///< Dimension of the histogram.
      typedef typename dim_traits<dim, CPT>::img_type img_type;
					 ///< Type of the internal image.
    };

    /*! Histogram.
    **
    ** This histogram uses an image of unsigned to store the value.
    ** For example the histogram of an image<int_u8> will store the
    ** number of occurrences in an image1d; an image<rgb_8> will store the
    ** number of occurrences an image3d (because rgb_8 has 3 components).
    **
    ** \todo FIXME:  An image is inside the histogram. This is incorrect
    **  because it is not exactly an image (no border needed).
    **
    ** \param T Type of the image.
    ** \param CPT Type used to count the occurrences (unsigned).
    ** \param V2P Conversion class to convert a value T to a point.
    ** \param Exact Exact type of the histogram.
    **
    ** \see oln::abstract::histogram
    **
    ** \code
    **  #include <oln/basics2d.hh>
    **  #include <oln/utils/histogram.hh>
    **  #include <ntg/all.hh>
    **  #include <iostream>
    **
    **  int main()
    **  {
    **    oln::image2d<ntg::rgb_8> in = oln::io::load(IMG_IN "lena.ppm");
    **
    **    oln::utils::histogram<ntg::rgb_8> h(in);
    **
    **    ntg::rgb_8 pink(215, 129, 113);
    **
    **    // h[pink] = 14
    **    std::cout << "Number of occurrences of the color (213, 129, 135): "
    **  	    <<  h[pink] << std::endl;
    **  }
    ** \endcode
    */
    template<typename T,
	     typename CPT,
	     class V2P,
	     class Exact>
    class histogram: public abstract::histogram
      <typename mlc::exact_vt<histogram<T, CPT, V2P, Exact>, Exact>::ret>
    {
    public:
      typedef typename mlc::exact_vt<histogram<T, CPT, V2P, Exact>,
				     Exact>::ret exact_type;
      typedef typename hist_traits<exact_type>::value_type value_type;
      typedef typename hist_traits<exact_type>::cpt_type cpt_type;
      typedef typename hist_traits<exact_type>::value_to_point_type
						value_to_point_type;
      typedef typename hist_traits<exact_type>::point_type point_type;
      enum {dim = hist_traits<exact_type>::dim};
      typedef typename hist_traits<exact_type>::img_type img_type;

      /*! \brief Empty histogram.
      **
      ** \note The function init(image) should be used after this
      ** constructor.
      */
      histogram(const value_to_point_type & c2p = value_to_point_type()):
	v2p_(c2p), img_(internal::img_max_size<value_type>()())
      {
	clear();
      }

      /// This compute the histogram of an image.
      template <class I>
      histogram(const oln::abstract::image<I> & input,
		const value_to_point_type & v2p = value_to_point_type()):
	v2p_(v2p), img_(internal::img_max_size<value_type>()())
      {
	clear();
	init(input);
      }

      /// clear() should be called.
      void
      clear_impl()
      {
	typename img_type::iter_type it(img_ );
	for_all(it)
	img_[it] = ntg_zero_val(cpt_type);
      }

      /// operator[] should be called.
      const cpt_type
      at(const T &v)const
      {
	return img_[v2p_(v)];
      }
      /// operator[] should be called.
      cpt_type&
      at(const value_type &v)
      {
	return img_[v2p_(v)];
      }
      /// impl() should be called.
      template <class I>
      void
      init_impl(const oln::abstract::image<I> &img)
      {
	oln_iter_type(I) it_img(img);

	for_all(it_img)
	++img_[v2p_(img[it_img])];
      }

      /// Return the image of occurrence.
      const img_type &
      image() const
      {
	return img_;
      }

    protected:
      const value_to_point_type v2p_;
      img_type img_;
    };

    /*! Minimum value of an histogram.
    **
    ** Return the smaller value within the image used to build the
    ** histogram.
    **
    ** \note It can be slow when the histogram is sparse because it iterates
    ** over a large range of 0. Use \a histogram_min or \a histogram_minmax
    ** instead.
    ** \see histogram_min
    */
    template<class T>
    inline oln_value_type(T)
    min(const abstract::histogram<T>& hist)
    {
      typedef typename ntg_is_a(oln_value_type(T),
				ntg::non_vectorial)::ensure_type ensure_type;

      oln_value_type(T) i;
      for (i = ntg_min_val(oln_value_type(T));
	   i != ntg_max_val(oln_value_type(T));
	   i = ntg::succ(i))
	if (hist[i] > ntg_zero_val(oln_cpt_type(T)))
	  break;
      return i;
    }
    /*! Maximum value of an histogram.
    **
    ** Return the higher value within the image used to build the
    ** histogram.
    **
    ** \note It can be slow when the histogram is sparse because it
    ** iterates over a large range of 0. Use histogram_max or
    ** histogram_minmax instead.
    ** \see histogram_max
    */
    template<class T>
    inline oln_value_type(T)
    max(const abstract::histogram<T>& hist)
    {
      typedef typename ntg_is_a(oln_value_type(T),
				ntg::non_vectorial)::ensure_type ensure_type;

      oln_value_type(T) i;
      for (i = ntg_max_val(oln_value_type(T));
	   i != ntg_min_val(oln_value_type(T));
	   i = ntg::pred(i))
	if (hist[i] > ntg_zero_val(oln_cpt_type(T)))
	  break;
      return i;
    }

    template< typename T,
	      typename CPT = unsigned,
	      class V2P = convert::value_to_point<T>,
	      class Exact = mlc::final>
    class histogram_minmax;

    template <typename T,
	      typename CPT,
	      class V2P,
	      class Exact>
    struct hist_traits<histogram_minmax<T, CPT, V2P, Exact> >
      : public hist_traits<histogram<T, CPT, V2P, Exact> >
    {};
    /*! Build the histogram and has quick min and max functions.
    **
    ** The idea behind the min- and max-specialized histogram is to
    ** maintain worst min and max bounds while updating histogram.  It
    ** does not maintain _exact_ min and max bounds, because this would
    ** involve some costly computation when removing values from the
    ** histogram and maybe this time will be lost if the removed value is
    ** reinserted before max() or min() is called.\n
    ** So it updates the _worst_ min and max bounds whenever the histogram
    ** value are accessed, and delay the _real_ min and max computation
    ** until min() or max() is called.
    ** \see histogram
    ** \see histogram_min
    ** \see histogram_max
    */
    template< typename T,
	      typename CPT,
	      class V2P,
	      class Exact>
    class histogram_minmax : public histogram
      <T, CPT, V2P,
       typename mlc::exact_vt<histogram_minmax<T, CPT, V2P, Exact>,
			      Exact>::ret>
    {
    private:
      typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type
							ensure_type;
    public:
      typedef typename mlc::exact_vt<histogram_minmax<T, CPT, V2P, Exact>,
				     Exact>::ret exact_type;
      typedef typename hist_traits<exact_type>::value_type value_type;
      typedef typename hist_traits<exact_type>::cpt_type cpt_type;
      typedef typename hist_traits<exact_type>::value_to_point_type
						 value_to_point_type;
      typedef typename hist_traits<exact_type>::point_type point_type;
      enum {dim = hist_traits<exact_type>::dim};
      typedef typename hist_traits<exact_type>::img_type img_type;
      typedef histogram<T, CPT, V2P, exact_type> upper_type;

      histogram_minmax(const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(v2p),
	min_(ntg_min_val(value_type)), max_(ntg_max_val(value_type)) {}

      template <class I>
      histogram_minmax(const oln::abstract::image<I> & input,
		       const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(input, v2p),
	min_(ntg_min_val(value_type)), max_(ntg_max_val(value_type)) {}

      /// operator[] should be called.
      const cpt_type
      at(const value_type& i) const
      {
	adjust(i);
	return img_[v2p_(i)];
      }

      /// operator[] should be called.
      cpt_type&
      at(const value_type& i)
      {
	adjust(i);
	return img_[v2p_(i)];
      }

      /// Quick function min.
      value_type
      min()
      {
	for (; min_ != ntg_max_val(value_type); min_ = ntg::succ(min_))
  	  if (img_[v2p_(min_)] > ntg_zero_val(cpt_type))
     	    break;
	return min_;
      }
      /// Quick function max.
      value_type
      max()
      {
	for (; max_ != ntg_min_val(value_type); max_ = ntg::pred(max_))
  	  if (img_[v2p_(max_)] > ntg_zero_val(cpt_type))
   	    break;
	return max_;
      }

    protected:
      /// Maintain the worst min and max.
      void
      adjust(const value_type &idx)
      {
	if (idx > max_)
	  max_ = idx;
	if (idx < min_)
	  min_ = idx;
      }
      value_type min_, max_;	///< Indices of min and max elements.
    };


    template< typename T,
	      typename CPT = unsigned,
	      class V2P = convert::value_to_point<T>,
	      class Exact = mlc::final>
    class histogram_min;

    template <typename T,
	      typename CPT,
	      class V2P,
	      class Exact>
    struct hist_traits<histogram_min<T, CPT, V2P, Exact> >
      : public hist_traits<histogram<T, CPT, V2P, Exact> >
    {};

    /*! Build the histogram and has quick min function.
    **
    ** \see histogram_minmax
    */
    template< typename T,
	      typename CPT,
	      class V2P,
	      class Exact>
    class histogram_min : public histogram<T, CPT, V2P,
	  typename mlc::exact_vt<histogram_min<T, CPT, V2P, Exact>,
				 Exact>::ret>
    {
    private:
      typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type
							 ensure_type;

    public:
      typedef typename mlc::exact_vt<histogram_min<T, CPT, V2P, Exact>,
				     Exact>::ret exact_type;
      typedef typename hist_traits<exact_type>::value_type value_type;
      typedef typename hist_traits<exact_type>::cpt_type cpt_type;
      typedef typename hist_traits<exact_type>::value_to_point_type
						value_to_point_type;
      typedef typename hist_traits<exact_type>::point_type point_type;
      enum {dim = hist_traits<exact_type>::dim};
      typedef typename hist_traits<exact_type>::img_type img_type;
      typedef histogram<T, CPT, V2P, exact_type> upper_type;

      histogram_min(const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(v2p), min_(ntg_min_val(value_type)) {}

      template <class I>
      histogram_min(const oln::abstract::image<I> & input,
		    const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(input, v2p), min_(ntg_min_val(value_type)) {}

      /// operator[] should be called.
      const cpt_type
      at(const value_type& i) const
      {
	return img_[v2p_(i)];
      }

      /// operator[] should be called.
      cpt_type&
      at(const value_type& i)
      {
	adjust(i);
	return img_[v2p_(i)];
      }
      /// Quick function min.
      value_type
      min()
      {
	for (; min_ != ntg_max_val(value_type); min_ = succ(min_))
  	  if (img_[v2p_(min_)] > ntg_zero_val(cpt_type))
   	    break;
	return min_;
      }
      /// Return the min.
      value_type
      res()
      {
	return min();
      }

    protected:
      /// Maintain the worst min.
      void
      adjust(const value_type& idx)
      {
	if (idx < min_)
	  min_ = idx;
      }
      value_type min_;	///< Index of the worst min element.
    };

    template< typename T,
	      typename CPT = unsigned,
	      class V2P = convert::value_to_point<T>,
	      class Exact = mlc::final>
    class histogram_max;

    template <typename T,
	      typename CPT,
	      class V2P,
	      class Exact>
    struct hist_traits<histogram_max<T, CPT, V2P, Exact> >
      : public hist_traits<histogram<T, CPT, V2P, Exact> >
    {};

    /*! Build the histogram and has quick max function.
    **
    ** \see histogram_minmax
    */
    template< typename T,
	      typename CPT,
	      class V2P,
	      class Exact>
    class histogram_max : public histogram<T, CPT, V2P,
	  typename mlc::exact_vt<histogram_max<T, CPT, V2P, Exact>,
				 Exact>::ret>
    {
    private:
      typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type
							ensure_type;

    public:
      typedef typename mlc::exact_vt<histogram_max<T, CPT, V2P, Exact>,
				     Exact>::ret exact_type;
      typedef typename hist_traits<exact_type>::value_type value_type;
      typedef typename hist_traits<exact_type>::cpt_type cpt_type;
      typedef typename hist_traits<exact_type>::value_to_point_type
						value_to_point_type;
      typedef typename hist_traits<exact_type>::point_type point_type;
      enum {dim = hist_traits<exact_type>::dim};
      typedef typename hist_traits<exact_type>::img_type img_type;
      typedef histogram<T, CPT, V2P, exact_type> upper_type;

      histogram_max(const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(v2p),max_(ntg_max_val(value_type)) {}

      template <class I>
      histogram_max(const oln::abstract::image<I> & input,
		    const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(input, v2p),max_(ntg_max_val(value_type)) {}

      /// operator[] should be called.
      const cpt_type
      at(const value_type& i) const
      {
	return img_[v2p_(i)];
      }

      /// operator[] should be called.
      cpt_type&
      at(const value_type& i)
      {
	adjust(i);
	return img_[v2p_(i)];
      }
      /// Quick function max.
      value_type
      max()
      {
	for (; max_ != ntg_min_val(value_type); max_ = ntg::pred(max_))
	  if (img_[v2p_(max_)] > ntg_zero_val(cpt_type))
  	    break;
	return max_;
      }
      /// Return the max.
      value_type
      res()
      {
	return max();
      }

    protected:

      /// Maintain the worst max.
      void
      adjust(const value_type& idx)
      {
	if (idx > max_)
	  max_ = idx;
      }
      value_type max_;	///< Index of the worst max element.
    };

    /// Minimum non-zero value of an histogram.
    template< typename T, typename CPT, class V2P, class Exact >
    inline T
    min(histogram_minmax<T, CPT, V2P, Exact>& hist)
    {
      return hist.min();
    }
    /// Minimum non-zero value of an histogram.
    template< typename T, typename CPT, class V2P, class Exact >
    inline T
    min(histogram_min<T, CPT, V2P, Exact>& hist)
    {
      return hist.min();
    }
    /// Maximum non-zero value of an histogram.
    template< typename T, typename CPT, class V2P, class Exact >
    inline T
    max(histogram_minmax<T, CPT, V2P, Exact>& hist)
    {
      return hist.max();
    }
    /// Maximum non-zero value of an histogram.
    template< typename T, typename CPT, class V2P, class Exact >
    inline T
    max(histogram_max<T, CPT, V2P, Exact>& hist)
    {
      return hist.max();
    }
    /*! Sort the values of an image, and store the result in a vector
    **
    ** This sort is efficient.
    ** \arg im Image non_vectorial.
    ** \arg v sorted vector at the end of the function.
    ** \pre precondition(v.size() == im.npoints());
    ** \pre A histogram of the image has to be possible.
    */
    template<class I>
    void
    distrib_sort(const oln::abstract::image<I>& im,
		 std::vector<oln_point_type(I)> &v)
    {
      typedef oln_value_type(I) val;

      typedef typename ntg_is_a(val, ntg::non_vectorial)::ensure_type
							 ensure_type;

      // check the size
      precondition(v.size() == im/*.npoints()*/);

      // calculate the histogram of the image
      utils::histogram<val> histo(im);

      // Initialize the array of pointer to the point in the result.
      // With the histogram the number of each color can be deduced and
      // then it calculates an array of pointer for quick access to each
      // value of the image.
      const ntg_cumul_type(val) card = ntg_max_val(val)
					- ntg_min_val(val) + 1;
      std::vector<oln_point_type(I)* > ptr(card);
      ptr[0] = &(v[0]);
      for (ntg_cumul_type(val) i = 1; i < card; ++i)
	ptr[i] = ptr[i - 1] + histo[i - 1 + ntg_min_val(val)];

      // Now iterate on the image to sort point in the order of their
      // level
      oln_iter_type(I) p(im);
      for_all(p)
	*(ptr[unsigned(im[p] - ntg_min_val(val))]++) = p;
    }

    /*! Inverted sort of the values of an image, and store the result
    ** in a vector
    **
    ** This sort is efficient.
    ** \arg im Image non_vectorial.
    ** \arg v sorted vector at the end of the function.
    ** \pre precondition(v.size() == im.npoints());
    ** \pre A histogram of the image has to be possible.
    */
    template<class I>
    void
    distrib_sort_inv(const oln::abstract::image<I>& im,
		     std::vector<oln_point_type(I)> &v)
    {
      typedef oln_value_type(I) val;

      typedef typename ntg_is_a(val, ntg::non_vectorial)::ensure_type
							ensure_type;

      precondition(v.size() == im.npoints());

      utils::histogram<val> histo(im);

      const ntg_cumul_type(val) card = ntg_max_val(val)
					- ntg_min_val(val) + 1;
      std::vector<oln_point_type(I)* > ptr(card);
      ptr[card - 1] = &(v[0]);

      for (ntg_signed_cumul_type(val) i = card - 2; i >= 0; --i)
	ptr[i] = ptr[i + 1] + histo[i + 1 + ntg_min_val(val)];

      oln_iter_type(I) p(im);
      for_all(p)
	*(ptr[unsigned(im[p] - ntg_min_val(val))]++) = p;
    }

    /*! Select statically the good distrib_sort.
    **
    ** \param reverse If the sort should be reverted or not.
    */
    template <bool reverse>
    struct select_distrib_sort
    {
      template <class I>
      void
      operator ()(const oln::abstract::image<I>& im,
		  std::vector<oln_point_type(I)> &v)
      {
	distrib_sort_inv(im, v);
      }
    };

    template <>
    struct select_distrib_sort<true>
    {
      template <class I>
      void
      operator ()(const oln::abstract::image<I>& im,
		  std::vector<oln_point_type(I)> &v)
      {
	distrib_sort(im, v);
      }
    };
  } // end of namespace utils

} // end of namespace oln

#endif // ! OLENA_UTILS_HISTOGRAM_HH
