// Copyright (C) 2007 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef MLN_LEVEL_TAKE_SPE_HH
# define MLN_LEVEL_TAKE_SPE_HH

/*! \file mln/level/take.spe.hh
 *
 * \brief Specializations for mln::level::take.
 */

# ifndef MLN_LEVEL_TAKE_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LEVEL_TAKE_HH

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>

# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {

    namespace impl
    {

      namespace generic
      {
	template <typename A, typename I>
	void take_(const I& input, A& a);
      }

      template <typename A, typename I>
      inline
      void take_(trait::image::speed::any, const I& input, A& a)
      {
	generic::take_(input, a);
      }

      template <typename A, typename I>
      inline
      void take_(trait::image::speed::fastest, const I& input, A& a)
      {
	trace::entering("level::impl::take");

	mln_pixter(const I) pxl(input);
	for_all(pxl)
	  a.take(pxl.val());

	trace::exiting("level::impl::take");
      }

    } // end of namespace mln::level::impl

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY

#endif // ! MLN_LEVEL_TAKE_HH
