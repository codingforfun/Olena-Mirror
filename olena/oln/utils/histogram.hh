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

# include <vector>

namespace oln {

  namespace utils {

    namespace internal {
      //! Return the size of the space needed to explore the type T
      template <typename T>
      struct img_max_size
      {
      private:
	typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type ensure_type;
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

    namespace abstract
    {
      // FIXME: An image is inside the histogram. This is incorrect
      // because it is not exactly an image (we do not need any
      // border).
      template<class T,
	       typename CPT,
	       class Exact = mlc::final>
      class histogram: public mlc_hierarchy::any<Exact>
      {
      public:
	typedef histogram<T, CPT, Exact> self_type;
	typedef Exact exact_type;
	typedef CPT cpt_type;

	void
	clear()
	{
	  this->exact().clear_impl();
	}

	const CPT&
	operator[](const T &v)const
	{
	  return this->exact().at(v);
	}

	CPT&
	operator[](const T &v)
	{
	  return this->exact().at(v);
	}

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
    class histogram: public abstract::histogram
      <T, CPT, typename mlc::exact_vt<histogram<T, CPT, V2P, Exact>, Exact>::ret>
    {
    public:
      typedef histogram<T, CPT, V2P, Exact> self_type;
      typedef Exact exact_type;
      typedef T input_type;
      typedef V2P value_to_point_type;
      typedef CPT cpt_type;
      typedef typename value_to_point_type::result_type point_type;
      enum {dim = value_to_point_type::result_type::dim};
      typedef abstract::histogram<T, CPT, self_type> upper_type;
      typedef typename dim_traits<dim, CPT>::img_type img_type;

      histogram(const value_to_point_type & c2p = value_to_point_type()):
	v2p_(c2p), img_(internal::img_max_size<input_type>()())
      {
	clear();
      }

      template <class I>
      histogram(const oln::abstract::image<I> & input,
		const value_to_point_type & v2p = value_to_point_type()):
	v2p_(v2p), img_(internal::img_max_size<input_type>()())
      {
	clear();
	init(input);
      }

      void
      clear_impl()
      {
	typename img_type::iter_type it(img_ );
	for_all(it)
	img_[it] = ntg_zero_val(CPT);
      }

      const CPT&
      at(const T &v)const
      {
	return img_[v2p_(v)];
      }

      CPT&
      at(const T &v)
      {
	return img_[v2p_(v)];
      }

      template <class I>
      void
      init_impl(const oln::abstract::image<I> &img)
      {
	oln_iter_type(I) it_img(img);

	for_all(it_img)
	++img_[v2p_(img[it_img])];
      }

      const img_type &
      image() const
      {
	return img_;
      }

    protected:
      const value_to_point_type v2p_;
      img_type img_;
    };

    //Note: If there is no min an assertion will fail at the end of the loop.
    template< typename T, typename CPT, class Exact>
    inline T
    min(const abstract::histogram<T, CPT, Exact>& hist)
    {
      typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type ensure_type;

      T i;
      for (i = ntg_min_val(T); i <= ntg_max_val(T); ++i)
	if (hist[i] > ntg_zero_val(CPT))
	  break;
      return i;
    }
    //Note: If there is no max an assertion will fail at the end of the loop.
    template< typename T, typename CPT, class Exact>
    inline T
    max(const abstract::histogram<T, CPT, Exact>& hist)
    {
      typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type ensure_type;

      T i;
      for (i = ntg_max_val(T); i >= ntg_min_val(T); --i)
	if (hist[i] > ntg_zero_val(CPT))
	  break;
      return i;
    }

/* The two functions above can be slow when the histogram is sparse
   because they iterate over a large range of 0.

   It's inefficient to call them repeatedly while updating the
   histogram (that's typically the case in the morpho::fast::
   algorithms).  Better use the specialized histograms below.

   ---

   The idea behind the min- and max-specialized histogram is to
   maintain worst min and max bounds while updating histogram.  We
   don't maintain _exact_ min and max bounds, because this would
   involve some costly computation when removing values from the
   histogram and maybe this time will be lost if the removed value is
   reinsterted before max() or min() is called.

   So we update the _worst_ min and max bounds whenever the histogram
   value are accessed, and delay the _real_ min and max compuation
   until min() or max() is called.  */

    template< typename T,
	      typename CPT = unsigned,
	      class V2P = convert::value_to_point<T>,
	      class Exact = mlc::final>
    class histogram_minmax : public histogram
      <T, CPT, V2P,
       typename mlc::exact_vt<histogram_minmax<T, CPT, V2P, Exact>, Exact>::ret>
    {
    private:
      typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type ensure_type;
    protected:
      void
      adjust(const T &idx)
      {
	if (idx > max_)
	  max_ = idx;
	if (idx < min_)
	  min_ = idx;
      }
    public:
      typedef histogram_minmax<T, CPT, V2P, Exact> self_type;
      typedef Exact exact_type;
      typedef histogram<T, CPT, V2P, histogram_minmax<T, CPT, V2P, Exact> > upper_type;
      typedef V2P value_to_point_type;

      histogram_minmax(const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(v2p),
	min_(ntg_min_val(T)), max_(ntg_max_val(T)) {}

      template <class I>
      histogram_minmax(const oln::abstract::image<I> & input,
		       const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(input, v2p),
	min_(ntg_min_val(T)), max_(ntg_max_val(T)) {}


      const CPT&
      at(const T& i) const
      {
	adjust(i);
	return img_[v2p_(i)];
      }

      CPT&
      at(const T& i)
      {
	adjust(i);
	return img_[v2p_(i)];
      }

      T
      min()
      {
	for (; min_ <= ntg_max_val(T); ++min_)
  	  if (img_[v2p_(min_)] > ntg_zero_val(CPT))
     	    break;
	return min_;
      }

      T
      max()
      {
	for (; max_ > 0; --max_)
  	  if (img_[v2p_(max_)] > ntg_zero_val(CPT))
   	    break;
	return max_;
      }

    protected:
      T min_, max_;	// indices of min and max elements
    };

    template< typename T,
	      typename CPT = unsigned,
	      class V2P = convert::value_to_point<T>,
	      class Exact = mlc::final>
    class histogram_min : public histogram<T, CPT, V2P,
	  typename mlc::exact_vt<histogram_min<T, CPT, V2P, Exact>,
				 Exact>::ret>
    {
    private:
      typedef typename ntg_is_a(T, ntg::non_vectorial)::ensure_type ensure_type;
    protected:
      void
      adjust(const T& idx)
      {
	if (idx < min_)
	  min_ = idx;
      }
    public:
      typedef histogram_min<T, CPT, V2P, Exact> self_type;
      typedef Exact exact_type;
      typedef V2P value_to_point_type;
      typedef histogram<T, CPT, V2P, histogram_min<T, CPT, V2P, Exact> > upper_type;


      histogram_min(const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(v2p), min_(ntg_min_val(T)) {}

      template <class I>
      histogram_min(const oln::abstract::image<I> & input,
		    const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(input, v2p), min_(ntg_min_val(T)) {}


      const CPT&
      at(const T& i) const
      {
	return img_[v2p_(i)];
      }

      CPT&
      at(const T& i)
      {
	adjust(i);
	return img_[v2p_(i)];
      }

      T
      min()
      {
	for (; min_ <= ntg_max_val(T); ++min_)
  	  if (img_[v2p_(min_)] > ntg_zero_val(CPT))
   	    break;
	return min_;
      }

      T
      res()
      {
	return min();
      }

    protected:
      T min_;	// index of min element
    };

    template< typename T,
	      typename CPT = unsigned,
	      class V2P = convert::value_to_point<T> ,
	      class Exact = mlc::final>
    class histogram_max : public histogram<T, CPT, V2P,
	  typename mlc::exact_vt<histogram_max<T, CPT, V2P, Exact>,
				 Exact>::ret>
    {
    protected:
      void
      adjust(const T& idx)
      {
	if (idx > max_)
	  max_ = idx;
      }

    public:
      typedef histogram_max<T, CPT, V2P, Exact> self_type;
      typedef Exact exact_type;
      typedef V2P value_to_point_type;
      typedef histogram<T, CPT, V2P, histogram_max<T, CPT, V2P, Exact> > upper_type;

      histogram_max(const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(v2p),max_(ntg_max_val(T)) {}

      template <class I>
      histogram_max(const oln::abstract::image<I> & input,
		    const value_to_point_type & v2p = value_to_point_type()) :
	upper_type(input, v2p),max_(ntg_max_val(T)) {}

      const CPT&
      at(const T& i) const
      {
	return img_[v2p_(i)];
      }

      CPT&
      at(const T& i)
      {
	adjust(i);
	return img_[v2p_(i)];
      }

      T
      max()
      {
	for (; max_ > 0; --max_)
	  if (img_[v2p_(max_)] > ntg_zero_val(CPT))
  	    break;
	return max_;
      }

      T
      res()
      {
	return max();
      }

    protected:
      T max_;	// index of max element
    };

    template< typename T, typename CPT, class V2P, class Exact >
    inline T
    min(histogram_minmax<T, CPT, V2P, Exact>& hist)
    {
      return hist.min();
    }
    template< typename T, typename CPT, class V2P, class Exact >
    inline T
    min(histogram_min<T, CPT, V2P, Exact>& hist)
    {
      return hist.min();
    }
    template< typename T, typename CPT, class V2P, class Exact >
    inline T
    max(histogram_minmax<T, CPT, V2P, Exact>& hist)
    {
      return hist.max();
    }
    template< typename T, typename CPT, class V2P, class Exact >
    inline T
    max(histogram_max<T, CPT, V2P, Exact>& hist)
    {
      return hist.max();
    }

    template<class I>
    void
    distrib_sort(const oln::abstract::image<I>& im,
		 std::vector<oln_point_type(I)> &v)
    {
      typedef oln_value_type(I) val;

      typedef typename ntg_is_a(val, ntg::non_vectorial)::ensure_type ensure_type;

      // check the size
      precondition(v.size() == im.npoints());

      // calculate the histogram of the image
      utils::histogram<val> histo(im);

      // initialize the array of pointer to the point in the result
      // with the histogram we can know the number of each color and
      // then calculate an array of pointer for quick access to each
      // value of the image
      const ntg_cumul_type(val) card = ntg_max_val(val) - ntg_min_val(val) + 1;
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

    template<class I>
    void
    distrib_sort_inv(const oln::abstract::image<I>& im,
		     std::vector<oln_point_type(I)> &v)
    {
      typedef oln_value_type(I) val;

      typedef typename ntg_is_a(val, ntg::non_vectorial)::ensure_type ensure_type;

      precondition(v.size() == im.npoints());

      utils::histogram<val> histo(im);

      const ntg_cumul_type(val) card = ntg_max_val(val) - ntg_min_val(val) + 1;
      std::vector<oln_point_type(I)* > ptr(card);
      ptr[card - 1] = &(v[0]);

      for (ntg_signed_cumul_type(val) i = card - 2; i >= 0; --i)
	ptr[i] = ptr[i + 1] + histo[i + 1 + ntg_min_val(val)];

      oln_iter_type(I) p(im);
      for_all(p)
	*(ptr[unsigned(im[p] - ntg_min_val(val))]++) = p;
    }

    // to select staticly the good distrib_sort
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
