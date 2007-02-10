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

#ifndef NTG_CONFIG_MATH_HH
# define NTG_CONFIG_MATH_HH

# include <ntg/config/system.hh>

# include <cmath>

# ifndef USE_C_LIMITS
#  include <limits>
#  define NTG_DOUBLE_INFINITY (std::numeric_limits<double>::infinity())
#  define NTG_FLOAT_INFINITY (std::numeric_limits<float>::infinity())
# else
#  ifdef HUGE_VAL
#   define NTG_DOUBLE_INFINITY HUGE_VAL
#   ifdef HUGE_VALF
#    define NTG_FLOAT_INFINITY HUGE_VALF
#   else
#    define NTG_FLOAT_INFINITY ((float)HUGE_VAL)
#   endif
#  else
#   error Cannot define Infinity in this configuration.
#  endif
# endif

# ifndef M_PI
#  define M_PI 3.14159265358979323846264338327
# endif

# ifdef NEED_SQRTF
static inline float sqrtf(float f)
{
  return (float)sqrt((double)f);
}
# endif

# ifdef NEED_FLOORF
static inline float floorf(float f)
{
  return (float)floor((double)f);
}
# endif

# ifdef NEED_ROUND
static inline double round(double f)
{
  return (f < 0.) ? -floor(-f + 0.5) : floor(f + 0.5);
}
# endif
# ifdef NEED_ROUNDF
static inline float roundf(float f)
{
  return (f < 0.f) ? -floorf(-f + 0.5f) : floorf(f + 0.5f);
}
# endif

#endif // !NTG_CONFIG_MATH_HH
