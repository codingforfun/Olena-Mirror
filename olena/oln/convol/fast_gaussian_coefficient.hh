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

#ifndef OLENA_CONVOL_FAST_GAUSSIAN_COEFFICIENT_H__
# define OLENA_CONVOL_FAST_GAUSSIAN_COEFFICIENT_H__

//
// Gaussian filter implementation from
// "Recursively implementing the gaussian and its derivatives"
// Deriche 93 INRIA REPORT
//

//// ------------------------------------------------------------ ////
//// Compute the coefficient d_ii and n_ii from the formulae (21) ////
//// and (22)                                                     ////
//// Respect of index number is aimed to adjust the article	  ////
//// (to be discussed ...) FIXME
//// ------------------------------------------------------------ ////


# include <ntg/core/predecls.hh>
# include <vector>

namespace oln {
  namespace convol {
    namespace fast {
      namespace internal {

	/*!
	** \brief Data structure for coefficients used for a recursive
	** filter call.
	*/
	template < class FloatT >
	struct recursivefilter_coef_
	{
	  enum FilterType { DericheGaussian,
			    DericheGaussianFirstDerivative,
			    DericheGaussianSecondDerivative };

	  /*!
	  ** \brief Constructor.
	  */
	  recursivefilter_coef_(FloatT a0, FloatT a1,
			       FloatT b0, FloatT b1,
			       FloatT c0, FloatT c1,
			       FloatT w0, FloatT w1,
			       ntg::float_d s, FilterType filter_type);
	  std::vector<FloatT>	n, d, nm, dm;
	  FloatT		sumA, sumC;
	};

	/*
	** Define out of the struct so the compiler don't even attempt
	** to inline this.
	*/
	template < class FloatT >
	recursivefilter_coef_<FloatT>
	::recursivefilter_coef_(FloatT a0, FloatT a1,
			       FloatT b0, FloatT b1,
			       FloatT c0, FloatT c1,
			       FloatT w0, FloatT w1,
			       ntg::float_d s, FilterType filter_type)
	{
	  n.reserve(5);
	  d.reserve(5);
	  nm.reserve(5);
	  dm.reserve(5);

	  b0 /= s;
	  b1 /= s;
	  w0 /= s;
	  w1 /= s;

	  FloatT sin0	= sin(w0);
	  FloatT sin1	= sin(w1);
	  FloatT cos0	= cos(w0);
	  FloatT cos1	= cos(w1);

	  switch (filter_type) {

	  case DericheGaussian :
	    {
	      sumA  =
		(2.0 * a1 * exp( b0 ) * cos0 * cos0 - a0 * sin0 * exp( 2.0 * b0 )
		 + a0 * sin0 - 2.0 * a1 * exp( b0 )) /
		(( 2.0 * cos0 * exp( b0 ) - exp( 2.0 * b0 ) - 1 ) * sin0);

	      sumC  =
		(2.0 * c1 * exp( b1 ) * cos1 * cos1 - c0 * sin1 * exp( 2.0 * b1 )
		 + c0 * sin1 - 2.0 * c1 * exp( b1 ))
		/ (( 2.0 * cos1 * exp( b1 ) - exp( 2.0 * b1 ) - 1 ) * sin1);
	      break;
	    }

	  case DericheGaussianFirstDerivative :
	    {
	      sumA  = -2.f *
		(a0 * cos0 - a1 * sin0 + a1 * sin0 * exp( 2.0 * b0 )
		 + a0 * cos0 * exp( 2.0 * b0 ) - 2.0 * a0 * exp( b0 ))
		* exp( b0 )
		/ (exp( 4.0 * b0 ) - 4.0 * cos0 * exp( 3.0 * b0 )
		   + 2.0 * exp( 2.0 * b0 ) + 4.0 * cos0 * cos0 * exp( 2.0 * b0 )
		   + 1.0 - 4.0 * cos0 * exp( b0 ));
	      sumC  = - 2.f *
		(c0 * cos1 - c1 * sin1 + c1 * sin1 * exp( 2.0 * b1 )
		 + c0 * cos1 * exp( 2.0 * b1 ) - 2.0 * c0 * exp( b1 ))
		* exp( b1 ) /
		(exp( 4.0 * b1 ) - 4.0 * cos1 * exp( 3.0 * b1 )
		 + 2.0 * exp( 2.0 * b1 ) + 4.0 * cos1 * cos1 * exp( 2.0 * b1 )
		 + 1.0 - 4.0 * cos1 * exp( b1 ));
	      break;
	    }

	  case DericheGaussianSecondDerivative :
	    {
	      FloatT aux;
	      aux   =
		12.0 * cos0 * exp( 3.0 * b0 ) - 3.0 * exp( 2.0 * b0 )
		+ 8.0 * cos0 * cos0 * cos0 * exp( 3.0 * b0 ) - 12.0 * cos0 * cos0 *
		exp( 4.0 * b0 )
		- (3.0 * exp( 4.0 * b0 ))
		+ 6.0 * cos0 * exp( 5.0 * b0 ) -  exp( 6.0 * b0 ) + 6.0 * cos0 * exp
		( b0 )
		- ( 1.0 + 12.0 * cos0 * cos0 * exp( 2.0 * b0 ) );
	      sumA  =
		4.0 * a0 * sin0 * exp( 3.0 * b0 ) + a1 * cos0 * cos0 * exp( 4.0 * b0 )
		- ( 4.0 * a0 * sin0 * exp( b0 ) + 6.0 * a1 * cos0 * cos0 * exp( 2.0 * b0 ) )
		+ 2.0 * a1 * cos0 * cos0 * cos0 * exp( b0 ) - 2.0 * a1 * cos0 * exp(b0)
		+ 2.0 * a1 * cos0 * cos0 * cos0 * exp( 3.0 * b0 ) - 2.0 * a1 * cos0
		* exp( 3.0 * b0 )
		+ a1 * cos0 * cos0 - a1 * exp( 4.0 * b0 )
		+ 2.0 * a0 * sin0 * cos0 * cos0 * exp( b0 ) - 2.0 * a0 * sin0 * cos0
		* cos0 * exp( 3.0 * b0 )
		- ( a0 * sin0 * cos0 * exp( 4.0 * b0 ) + a1 )
		+ 6.0 * a1 * exp( 2.0 * b0 ) + a0 * cos0 * sin0
		* 2.0 * exp( b0 ) / ( aux * sin0 );
	      aux   =
		12.0 * cos1 * exp( 3.0 * b1 ) - 3.0 * exp( 2.0 * b1 )
		+ 8.0 * cos1 * cos1 * cos1 * exp( 3.0 * b1 ) - 12.0 * cos1 * cos1 *
		exp( 4.0 * b1 )
		- 3.0 * exp( 4.0 * b1 )
		+ 6.0 * cos1 * exp( 5.0 * b1 ) -  exp( 6.0 * b1 ) + 6.0 * cos1 * exp
		( b1 )
		- ( 1.0 + 12.0 * cos1 * cos1 * exp( 2.0 * b1 ) );
	      sumC  = 4.0 * c0 * sin1 * exp( 3.0 * b1 ) + c1 * cos1 * cos1 * exp( 4.0 * b1 )
		- ( 4.0 * c0 * sin1 * exp( b1 ) + 6.0 * c1 * cos1 * cos1 * exp( 2.0 * b1 ) )
		+ 2.0 * c1 * cos1 * cos1 * cos1 * exp( b1 ) - 2.0 * c1 * cos1 * exp( b1 )
		+ 2.0 * c1 * cos1 * cos1 * cos1 * exp( 3.0 * b1 ) - 2.0 * c1 * cos1
		* exp( 3.0 * b1 )
		+ c1 * cos1 * cos1 - c1 * exp( 4.0 * b1 )
		+ 2.0 * c0 * sin1 * cos1 * cos1 * exp( b1 ) - 2.0 * c0 * sin1 * cos1
		* cos1 * exp( 3.0 * b1 )
		- ( c0 * sin1 * cos1 * exp( 4.0 * b1 ) + c1 )
		+ 6.0 * c1 * exp( 2.0 * b1 ) + c0 * cos1 * sin1
		* 2.0 * exp( b1 ) / ( aux * sin1 );
	      sumA /= 2;
	      sumC /= 2;
	      break;
	    }
	  }

	  a0 /= (sumA + sumC);
	  a1 /= (sumA + sumC);
	  c0 /= (sumA + sumC);
	  c1 /= (sumA + sumC);

	  n[3] =
	    exp( -b1 - 2*b0 ) * (c1 * sin1 - cos1 * c0) +
	    exp( -b0 - 2*b1 ) * (a1 * sin0 - cos0 * a0);
	  n[2] =
	    2 * exp(-b0 - b1) * ((a0 + c0) * cos1 * cos0 -
				 cos1 * a1 * sin0 -
				 cos0 * c1 * sin1) +
	    c0 * exp(-2 * b0) + a0 * exp(-2 * b1);
	  n[1] =
	    exp(-b1) * (c1 * sin1 - (c0 + 2*a0) * cos1) +
	    exp(-b0) * (a1 * sin0 - (2*c0 + a0) * cos0);
	  n[0] =
	    a0 + c0;

	  d[4] = exp(-2 * b0 - 2 * b1);
	  d[3] =
	    -2 * cos0 * exp(-b0 - 2*b1) -
	    2 * cos1 * exp(-b1 - 2*b0);
	  d[2] =
	    4 * cos1 * cos0 * exp(-b0 - b1) +
	    exp(-2*b1) + exp(-2*b0);
	  d[1] =
	    -2*exp(-b1) * cos1 - 2 * exp(-b0) * cos0;

	  switch (filter_type) {
	  case DericheGaussian :
	  case DericheGaussianSecondDerivative :
	    {
	      for (unsigned i = 1; i <= 3; ++i)
		{
		  dm[i] = d[i];
		  nm[i] = n[i] - d[i] * n[0];
		}
	      dm[4] = d[4];
	      nm[4] = -d[4] * n[0];
	      break;
	    }
	  case DericheGaussianFirstDerivative :
	    {
	      for (unsigned i = 1; i <= 3; ++i)
		{
		  dm[i] = d[i];
		  nm[i] = - (n[i] - d[i] * n[0]);
		}
	      dm[4] = d[4];
	      nm[4] = d[4] * n[0];
	      break;
	    }
	  }
	}


      } // internal
    } // fast
  } // convol
} // oln


#endif // OLENA_CONVOL_FAST_GAUSSIAN_COEFFICIENT_H__
