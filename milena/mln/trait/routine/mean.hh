// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_TRAIT_ROUTINE_MEAN_HH
# define MLN_TRAIT_ROUTINE_MEAN_HH

# define mln_trait_routine_mean(n,T) typename mln::trait::routine::mean< n,T >::ret
# define mln_trait_routine_mean_(n,T) mln::trait::routine::mean< n,T >::ret

namespace mln
{

  // Forward Declaration
  namespace value { template <unsigned n> class intsub; }

  namespace trait
  {

    namespace routine
    {

      // mean

      template <unsigned nvalues, typename T>
      struct mean
      {
	// undef
      };


      // specializations

      template <unsigned nvalues>
      struct mean< nvalues, int >
      {
	typedef mln::value::intsub<nvalues> ret;
      };

      template <unsigned nvalues>
      struct mean< nvalues, float >
      {
	typedef float ret;
      };

      template <unsigned nvalues>
      struct mean< nvalues, double >
      {
	typedef double ret;
      };

    } // end of namespace mln::trait::routine

  } // end of namespace mln::routine

} // end of namespace mln

#endif // ! MLN_TRAIT_ROUTINE_MEAN_HH
