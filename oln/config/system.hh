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

#ifndef OLENA_CONFIG_SYSTEM_HH
# define OLENA_CONFIG_SYSTEM_HH

/* The GNU libc will not declare round, roundf, etc. unless this is set...  */
# define _ISOC99_SOURCE 1

# include <oln/config/pconf.hh>
# include <cmath>

# ifndef M_PI
#  define M_PI 3.14159265358979323846264338327
# endif

# if HAVE_LIMITS
#  include <limits>
#  define OLN_FLOAT_INFINITY (std::numeric_limits<float>::infinity())
#  define OLN_DOUBLE_INFINITY (std::numeric_limits<double>::infinity())
# else
#  include <math.h>
#  ifdef HUGE_VAL
#   ifdef HUGE_VALF
#    define OLN_FLOAT_INFINITY HUGE_VALF
#   else
#    define OLN_FLOAT_INFINITY ((float)HUGE_VAL)
#   endif
#   define OLN_DOUBLE_INFINITY HUGE_VAL
#  else
#   error Do not know how to define infinity on this host.
#  endif
# endif

/* The STL used by g++ versions < 3 have namespaces disabled,
   because these g++ versions do not honor `std::' (unless requested).

   This means that all STL entities are declared in the global namespace,
   even the template operators that normally lie in std::rel_ops.  This
   is harmful because a global template operator have priority over
   any herited operator (these involve a derived-to-base-class conversion).
   Consider:

   #include <utility> // this defines the rel_ops operators.
   #include <iostream>

   struct A
   {
     bool operator>(const A& a) const { return true; }
     bool operator<(const A& a) const { return false; }
   };

   struct B : public A { };

   int main()
   {
     B b1, b2;
     cout << (b1 > b2) << endl;
   }

   The above example will print `0' instead of `1' because the template
   `operator>' defined by STL is used instead of our `A::operator>'.

   Because of this, and other similar errors hard to spot,
   we tweak the STL definitions so that these operators
   are declared in the `rel_ops' namespace.  (The standard requires
   `std::rel_ops' but since g++ < 3 doesn't honor `std' it makes sense
   to use only `rel_ops'.)
*/
#if (defined __GNUC__) && (__GNUC__ < 3) && HAVE_STL_CONFIG_H
#  include <stl_config.h>
#  undef __STL_BEGIN_RELOPS_NAMESPACE
#  define __STL_BEGIN_RELOPS_NAMESPACE namespace rel_ops {
#  undef __STL_END_RELOPS_NAMESPACE
#  define __STL_END_RELOPS_NAMESPACE }
#endif

#endif // OLENA_CONFIG_SYSTEM_HH
