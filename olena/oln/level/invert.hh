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

#ifndef OLENA_LEVEL_INVERT_HH
# define OLENA_LEVEL_INVERT_HH

# include <oln/basics.hh>

# include <ntg/bin.hh>
# include <ntg/float.hh>

# include <functional>

namespace oln {

  namespace level {

    /*! \brief Fctor to invert a value
    **
    ** \see invert.
    ** \todo FIXME: the specialisation is done within the class.
    */
    template<class T>
    struct f_invert : public std::unary_function<const ntg::value<T>&, T>
    {
      typedef f_invert self;

      const typename self::result_type
      operator()(typename self::argument_type val) const
      {
	return doit(val.exact());
      }

      // FIXME: It sounds unnatural to specialize here.
      // Inversion ought to be an operation defined on value types and
      // specialized there. -- adl

      template<unsigned N, class B>
      static const ntg::int_s<N, B>
      doit(const ntg::int_s<N, B>& val)
      {
	return - val;
      }

      static const ntg::float_d
      doit(const ntg::float_d& val)
      {
	return - val;
      }

      static const ntg::float_s
      doit(const ntg::float_s& val)
      {
	return - val;
      }

      template<unsigned N, class B>
      static const ntg::int_u<N, B>
      doit(const ntg::int_u<N, B>& val)
      {
	typedef ntg::int_u<N, B> tmp;
	return ntg_max_val(tmp) - val;
      }

      static const ntg::bin
      doit(ntg::bin val)
      {
	return val == true ? false : true;
      }
    };

    /*! \brief Return the image inverted. */
    template<class I>
    inline oln_concrete_type(I)
    invert(const abstract::image<I>& input)
    {
      return apply(f_invert<oln_value_type(I)>(), input);
    }

    /*! \brief Invert an image. */
    template<class I>
    inline void
    invert_self(abstract::image<I>& input)
    {
      apply_self(f_invert<oln_value_type(I)>(), input);
    }

  } // end of namespace level

} // end of namespace oln

#endif // ! OLENA_LEVEL_INVERT_HH
