// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_TO_POINT
# define OLENA_VALUE_TO_POINT

# include <ntg/basics.hh>

# include <oln/core/1d/point1d.hh>
# include <oln/core/3d/point3d.hh>

// FIXME: waiting for a coherent convert.
// FIXME: this is file is awful...

namespace oln {

  namespace convert {
    /*! Convert a value of pixel to a point.
    **
    ** For example, transform an RGB color to a 3D point
    ** (ntg::rgb_8 => oln::point3d).
    ** This function is useful to build the histogram. \n
    ** Example:
    ** \verbatim
    ** f(oln::convert::value_to_point<ntg::rgb_8>()(ntg::rgb_8(1,6,64)));
    ** // is equivalent to:
    ** f(oln::point3d(1, 6, 64));
    ** \endverbatim
    */

    namespace internal {

      template <typename ValueType>
      struct value_to_point
      {
        typedef ValueType value_type;
        typedef point1d point_type;

        value_to_point(const value_type& v) 
        : v_(v)
        {}

        operator point_type()
        {
          return point1d(v_ - ntg_min_val(value_type));
        }

        const value_type& v_;
      };

      template <unsigned Qbits, template <unsigned> class S>
      struct value_to_point< ntg::color<3, Qbits, S> >
      {
        typedef ntg::color<3, Qbits, S> value_type; 
        typedef point3d point_type;

        value_to_point(const value_type& v) 
        : v_(v)
        {}

        operator point_type()
        {
          return point_type(coord_t(v_[0]), coord_t(v_[1]), coord_t(v_[2]));
        }

        const value_type& v_;
      }; 


    }

    template <typename ValueType>
    typename internal::value_to_point<ValueType>::point_type
    value_to_point(const ValueType& v)
    {
      return internal::value_to_point<ValueType>(v);
    }

  } // convert

} // oln

#endif
