// Copyright 2001  EPITA Research and Development Laboratory
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


#ifndef OLENA_CONVOL_CONVOLUTION_HH__
# define OLENA_CONVOL_CONVOLUTION_HH__

# include <oln/basics.hh>
# include <oln/core/point.hh>
# include <oln/value/all.hh>
# include <oln/meta/array.hh>

namespace oln {

  namespace convol {

    namespace output {
      
      // FIXME

      template<class T_in, class T_win>
      struct output_traits;
      
      template<unsigned nbits> 
      struct output_traits<int_u<nbits>,  int> { 
	typedef int_s<nbits+2> ret;
      };
      
      template<unsigned nbits>
      struct output_traits<int_u<nbits>,  float> {
	typedef float_p1 ret;
      };
      
      template<unsigned nbits>
      struct output_traits<int_s<nbits>,  int> {
	typedef int_s<nbits+2> ret;
      };
      
      template<unsigned nbits>
      struct output_traits<int_s<nbits>,  float> {
	typedef float_p1 ret;
      };
      
      template<unsigned prec>
      struct output_traits<float_p<prec>, int> {
	typedef float_p<prec> ret;
      };
      
      template<unsigned prec>
      struct output_traits<float_p<prec>, float> {
	typedef float_p<prec> ret;
      }; 
    
    } // end namespace output
    
    namespace slow {
      
      template<class _T_in, class _T_win>
      typename mute <_T_in, typename output::output_traits< Value(_T_in), Weight(_T_win) >::ret >::ret
      convolve(const image < _T_in >& _input, const w_window< _T_win >& _win)
      {
	Exact_cref(T_in,input);
	Exact_cref(T_win,win);
	typedef typename output::output_traits<typename T_in::value, typename T_win::weight>::ret output_value;
	typedef typename mute < T_in, output_value>::ret T_out;
	
	T_out output(input.size());
	border::adapt_copy(input, win.delta());
	typename T_in::iter p(input);
	for (p = begin; p != end; ++p)
	  {
	    typename internal::cumul_traits<typename T_in::value>::ret_t sum = 0;
	    for (unsigned i = 0; i < win.card(); ++i)
	      sum += win.w(i) * input[p + win.dp(i)];
	    output[p] = cast::force<typename T_out::value>(sum);
	  }
	return output;
      }

      // FIXME : What about 1D and 3D ??
      template<class _T_in, class Info, class _T_arr>
      typename mute <_T_in, typename output::output_traits< Value(_T_in), _T_arr>::ret >::ret
      convolve(const image < _T_in >& _input, const meta::array2d< Info, _T_arr >& _arr)
      {
	return convolve(_input, static_cast< w_window2d<_T_arr> >(_arr)); // FIXME -> Should be w_window<_T_arr>
      }
      
    } // end namespace slow
    
  } // end namespace convol
  
} // end namespace oln

#endif // OLENA_CONVOL_CONVOLUTION_HH__
