// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

# include <oln/basics.hh>
# include <ntg/core/optraits.hh>
# include <vector>

namespace oln {

  namespace utils {

    template< typename T, typename U>
    class histogram;

    template< typename T, typename U >
    T min(const histogram<T, U>& hist);

    template< typename T, typename U >
    T max(const histogram<T, U>& hist);


    template< typename T, typename U = unsigned>
    class histogram
    {
    public:

      histogram() : _values(0)
      {
	unsigned size = unsigned(ntg::optraits<T>::max()
				 - ntg::optraits<T>::min()
				 + ntg::optraits<T>::unit());
	_values = new U[size];
	for (unsigned i = 0; i < size; ++i)
	  _values[i] = 0;
      }

      ~histogram()
      {
	if (_values)
	  {
	    delete[] _values;
	    _values = 0;
	  }
      }

      U& operator[](const T& i)
      {
	return _values[unsigned(i.val() - ntg::optraits<T>::min())];
      }

      friend T min<T, U>(const histogram<T, U>& hist);
      friend T max<T, U>(const histogram<T, U>& hist);

      template <class I_>
      void init(const image<I_> &_img)
      {
	Exact_cref(I, img);
	Iter(I) p(img);

	for_all (p)
	  _values[unsigned(img[p].val())]++;
      }
    protected:
      U *_values;
    };

    template< typename T, typename U >
    inline
    T min(const histogram<T, U>& hist)
    {
      unsigned i;
      for (i = 0; i < unsigned(ntg::optraits<T>::max() - ntg::optraits<T>::min()); ++i)
	if (hist._values[i] > 0)
	  break;
      return T(ntg::optraits<T>::min() + i);
    }


    template< typename T, typename U >
    inline
    T max(const histogram<T, U>& hist)
    {
      unsigned i;
      for (i = unsigned(ntg::optraits<T>::max() - ntg::optraits<T>::min()); i > 0; --i)
	if (hist._values[i] > 0)
	  break;
      return T(ntg::optraits<T>::min() + i);
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

    template< typename T, typename U = unsigned>
    class histogram_minmax : public histogram<T, U>
    {
    protected:
      void adjust(unsigned idx)
      {
	if (idx > _max)
	  _max = idx;
	if (idx < _min)
	  _min = idx;
      }

    public:

      histogram_minmax() :
	histogram<T,U>(),
	_min(0), _max(unsigned(ntg::optraits<T>::max() - ntg::optraits<T>::min())) {}

      U& operator[](const T& i)
      {
	unsigned idx = unsigned(i.val() - ntg::optraits<T>::min());
	adjust(idx);
	return _values[idx];
      }

      U min()
      {
	unsigned i;
	for (i = _min; i < unsigned(ntg::optraits<T>::max() - ntg::optraits<T>::min()); ++i)
	  if (_values[i] > 0)
	    break;
	_min = i;
	return T(ntg::optraits<T>::min() + i);
      }

      T max()
      {
	unsigned i;
	for (i = _max; i > 0; --i)
	  if (_values[i] > 0)
	    break;
	_max = i;
	return T(ntg::optraits<T>::min() + i);
      }

    protected:
      unsigned _min, _max;	// indices of min and max elements
    };

    template< typename T, typename U = unsigned>
    class histogram_min : public histogram<T, U>
    {
    protected:
      void adjust(unsigned idx)
      {
	if (idx < _min)
	  _min = idx;
      }

    public:

      histogram_min() : histogram<T,U>(), _min(0) {}

      U& operator[](const T& i)
      {
	unsigned idx = unsigned(i.val() - ntg::optraits<T>::min());
	adjust(idx);
	return _values[idx];
      }

      U min()
      {
	unsigned i;
	for (i = _min; i < unsigned(ntg::optraits<T>::max() - ntg::optraits<T>::min()); ++i)
	  if (_values[i] > 0)
	    break;
	_min = i;
	return T(ntg::optraits<T>::min() + i);
      }

      T res()
      {
	return min();
      }

    protected:
      unsigned _min;	// index of min element
    };

    template< typename T, typename U = unsigned>
    class histogram_max : public histogram<T, U>
    {
    protected:
      void adjust(unsigned idx)
      {
	if (idx > _max)
	  _max = idx;
      }

    public:

      histogram_max() :
	histogram<T,U>(),_max(unsigned(ntg::optraits<T>::max() - ntg::optraits<T>::min())) {}

      U& operator[](const T& i)
      {
	unsigned idx = unsigned(unsigned(i) - unsigned(ntg::optraits<T>::min()));
	adjust(idx);
	return _values[idx];
      }

      T max()
      {
	unsigned i;
	for (i = _max; i > 0; --i)
	  if (_values[i] > 0)
	    break;
	_max = i;
	return T(ntg::optraits<T>::min() + i);
      }

      T res()
      {
	return max();
      }

    protected:
      unsigned _max;	// index of max element
    };


    template< typename T, typename U >
    inline
    T min(histogram_minmax<T, U>& hist)
    {
      return hist.min();
    }
    template< typename T, typename U >
    inline
    T min(histogram_min<T, U>& hist)
    {
      return hist.min();
    }
    template< typename T, typename U >
    inline
    T max(histogram_minmax<T, U>& hist)
    {
      return hist.max();
    }
    template< typename T, typename U >
    inline
    T max(histogram_max<T, U>& hist)
    {
      return hist.max();
    }


    template<class I_>
    void distrib_sort(const image<I_>& _im, std::vector<Point(I_)> &v)
    {
      Exact_cref(I, im);
      typedef Value(I) val;

      // check the size
      precondition(v.size() == unsigned(im.nrows() * im.ncols()));

      // calculate the histogram of the image
      utils::histogram< Value(I) > histo;
      histo.init(im);

      // initialize the array of pointer to the point in the result
      // with the histogram we can know the number of each color and
      // then calculate an array of pointer for quick access to each
      // value of the image
      std::vector< Point(I)* > ptr(ntg::optraits<val>::max() + 1);
      ptr[0] = &(v[0]);
      for (int i = ntg::optraits<val>::min() + 1; i <= ntg::optraits<val>::max(); i++)
	ptr[unsigned(i)] = ptr[unsigned(i - 1)] + histo[i - 1];

      // Now iterate on the image to sort point in the order of their
      // level
      Iter(I) p(im);
      for_all(p)
	*(ptr[unsigned(im[p].val())]++) = p;
    }

  } // utils

} // oln

#endif // OLENA_UTILS_HISTOGRAM_HH
