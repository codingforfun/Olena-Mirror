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

#ifndef OLENA_TOPO_DIST_HH
# define OLENA_TOPO_DIST_HH

# include <oln/core/point.hh>
# include <cmath>

namespace oln {

  namespace topo {

    struct L1
    {
      typedef float output_t;

      template <class PointType>
      output_t operator()(const PointType& p1, const PointType& p2) const
      {
	float acu = 0;
	for (unsigned i = 0; i < PointType::dim; ++i)
	  acu += p1.nth(i) - p2.nth(i);
	return acu;
      }
    };

     struct L2
    {
      typedef float output_t;

      template <class PointType>
      output_t operator()(const PointType& p1, const PointType& p2) const
      {
	float acu = 0;
	for (unsigned i = 0; i < PointType::dim; ++i)
	  {
	    float d = p1.nth(i) - p2.nth(i);
	    acu += d * d;
	  }
	return sqrt(acu);
      }
    };

    struct L2_sqr
    {
      typedef float output_t;

      template <class PointType>
      output_t operator()(const PointType& p1, const PointType& p2) const
      {
	float acu = 0;
	for (unsigned i = 0; i < PointType::dim; ++i)
	  {
	    float d = p1.nth(i) - p2.nth(i);
	    acu += d * d;
	  }
	return acu;
      }
    };


    struct L_infty
    {
      typedef float output_t;

      template <class PointType>
      output_t operator()(const PointType& p1, const PointType& p2) const
      {
	float acu = 0;
	for (unsigned i = 0; i < PointType::dim; ++i)
	  acu = max(fabs(p1.nth(i), p2.nth(i)), acu);
	return acu;
      }
    };

    template<unsigned n>
    struct L
    {
      typedef float output_t;

      template <class PointType>
      output_t operator()(const PointType& p1, const PointType& p2) const
      {
	float acu = 0;
	for (unsigned i = 0; i < PointType::dim; ++i)
	  acu += ::pow(p1.nth(i) - p2.nth(i), n);
	return ::pow(acu,1.f / n);
      }
    };

    template<> struct L<1> {
      typedef L1::output_t output_t;

      template <class PointType>
      output_t operator()(const PointType& p1, const PointType& p2) const {
	L1 l1;
	return l1(p1, p2);
      }
    };

    template<> struct L<2> {
      typedef L2::output_t output_t;

      template <class PointType>
      output_t operator()(const PointType& p1, const PointType& p2) const {
	L2 l2;
	return l2(p1, p2);
      }
    };

  } // end of topo

} // end of mln

#endif // OLN_TOPO_DIST_HH
