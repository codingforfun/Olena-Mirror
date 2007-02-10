// Copyright (C) 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_SNAKES_SNAKES_BASE_HXX
# define OLENA_SNAKES_SNAKES_BASE_HXX

#include <oln/morpho/gradient.hh>

namespace oln {

  namespace snakes {

    template <class algorithm>
    snake<algorithm>::snake(const image_type&		image,
			    std::list<point_type>	initial_contour,
			    ntg::float_s		alpha,
			    ntg::float_s		beta,
			    ntg::float_s		gamma,
			    ntg::float_s		khi = 0.0f) :
      s(initial_contour), a(alpha, beta, gamma, khi),
      alpha(alpha), beta(beta), gamma(gamma), khi(khi)
    {
      gradient = morpho::fast::beucher_gradient(image, win_c8p());
    };

    template <class algorithm>
    std::list<typename snake<algorithm>::point_type>
    snake<algorithm>::contour(void) const
    {
      return s.contour();
    }

    template <class algorithm>
    ntg::float_s
    snake<algorithm>::energy(void) const
    {
      return s.energy(gradient);
    }

    template <class algorithm>
    inline
    int
    snake<algorithm>::update_snake(void)
    {
      return a.update_snake(gradient, *this);
    }

    template <class algorithm>
    inline
    void
    snake<algorithm>::converge(void)
    {
      a.converge(gradient, *this);
    }

  } // end snakes

} // end oln

#endif // !OLENA_SNAKES_SNAKES_BASE_HXX
