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

#ifndef OLENA_MATH_MACROS_HH
# define OLENA_MATH_MACROS_HH

# include <oln/basics.hh>

# include <ntg/basics.hh>

namespace oln {

  namespace math {


    // FIXME: I'm not proud of the code below
    // I think it could be better...

    // FIXME: this code sounds really odd. Why does the operator()
    // take value<Self> instead of Self directly ?

    // FIXME: Self should be renamed into Exact.

    // abs

    template<class Self>
    struct f_sqr
    {
      typedef Self output_t;
      const Self operator()(const ntg::value<Self>& val) const
      {
	return val * val;
      }
    };

    template<class Self>
    const Self sqr(const ntg::value<Self>& val)
    {
      f_sqr<Self> f;
      return f(val);
    }

    // abs

    template<class Self>
    struct f_abs
    {
      typedef Self output_t;
      const Self operator()(const ntg::value<Self>& val) const
      {
	return ::abs(val);
      }
    };

    template<class Self>
    const Self abs(const ntg::value<Self>& val)
    {
      f_abs<Self> f;
      return f(val);
    }

    // plus_abs
# if 0
    // FIXME: Move this to arith/ops.hh
    // Express f_plus_abs in term of plus_abs, and leave plus_abs here.

    template<class Self>
    struct f_plus_abs
    {
      typedef typename 
      internal::operator_plus_traits<Self,Self>::ret_t output_t;
      
      const output_t operator()(const ntg::value<Self>& val1,
				const ntg::value<Self>& val2) const
      {
	return math::abs(val1.self()) + math::abs(val2.self());
      }
    };

    template<class Self>
    const typename f_plus_abs<Self>::output_t
    plus_abs(const ntg::value<Self>& val1,
	     const ntg::value<Self>& val2)
    {
      f_plus_abs<Self> f;
      return f(val);
    }

    template<class Self, class T_out>
    struct f_plus_abs_to
    {
      typedef T_out output_t;
      const output_t operator()(const ntg::value<Self>& val1,
				const ntg::value<Self>& val2) const
      {
	return ntg::cast::force<T_out>(math::abs(val1.self()) 
				  + math::abs(val2.self()));
      }
    };

    template<class T_out, class Self>
    const T_out plus_abs(const ntg::value<Self>& val1,
			 const ntg::value<Self>& val2)
    {
      f_plus_abs_to<Self,T_out> f;
      return f(val);
    }

# endif

  } // end of math

} // end of oln

#endif // ! OLENA_MATH_MACROS_HH
