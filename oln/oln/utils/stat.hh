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

#ifndef OLENA_UTILS_STAT_HH
# define OLENA_UTILS_STAT_HH

# include <oln/basics.hh>
# include <oln/types/predecls.hh>
# include <oln/types/optraits.hh>

namespace oln {

  namespace utils {

    template< class T >
    struct f_minmax : std::unary_function< const T&, void >
    {
      f_minmax()
      {
        reset();
      }

      void operator()(const T& val)
      {
        if (! valued())
	  _min = _max = val;
	else if (val < _min) // FIXME: use a ftor instead of > and <
          _min = val;
        else if (val > _max)
	  _max = val;
	++_count;
      }

      void reset()
      {
        _count = 0;
      }

      bool valued() const
      {
        return _count;
      }

      size_t count() const
      {
        return _count;
      }

      const T min() const
      {
        assertion(valued());
        return _min;
      }

      const T max() const
      {
        assertion(valued());
        return _max;
      }

    protected:
      size_t _count;
      T _min;
      T _max;
    };

    template< class T, class C = sfloat >
    struct f_moments : f_minmax< T >
    {
      typedef f_minmax< T > super;

      void operator()(const T& val)
      {
	if (! valued()) {
	  _sum1 = optraits<C>::zero();
	  _sum2 = optraits<C>::zero();
	} else {
	  _sum1 += val;
	  _sum2 += C(val) * val;
	}
	super::operator()(val);
      }

      const C mean() const
      {
        assertion(valued());
        return _sum1 / C(count());
      }

      const C variance() const
      {
        assertion(valued());
        return _sum2 / C(count()) - mean() * mean();
      }

      protected:
	C _sum1;
	C _sum2;
    };

  } // utils
} // oln

#endif // OLENA_STAT_HISTOGRAM_HH
