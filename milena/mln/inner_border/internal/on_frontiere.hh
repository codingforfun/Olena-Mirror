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

#ifndef MLN_INNER_BORDER_ON_FRONTIERE_HH
# define MLN_INNER_BORDER_ON_FRONTIERE_HH

/// \file
///
/// \brief Returns true if a site is included in the inner border of
/// the image.

# include <mln/core/image/image2d.hh>
# include <mln/data/paste.hh>


namespace mln
{

  namespace inner_border
  {

    namespace internal
    {

      /// \brief Returns true if a site is included in the inner
      /// border of the image.
      template <typename P>
      bool
      on_frontiere(const mln_site(box<P>)& p, const box<P>& b,
		   def::coord inner_border_size);


# ifndef MLN_INCLUDE_ONLY

      // Implementation

      inline
      bool
      on_frontiere_2d(const point2d& p, const box2d& b,
		      def::coord inner_border_size)
      {
	for (int d = 0; d < 2; ++d)
	  if ((p[d] >= b.pmin()[d]
	       && p[d] < (b.pmin()[d] + inner_border_size))
	      ||
	      (p[d] <= b.pmax()[d]
	       && p[d] > b.pmax()[d] - inner_border_size))
	    return true;
	return false;
      }

      // Dispatch

      inline
      bool
      on_frontiere_dispatch(const point2d& p, const box2d& b,
			    def::coord inner_border_size)
      {
	return on_frontiere_2d(p, b, inner_border_size);
      }

      template <typename P>
      bool
      on_frontiere_dispatch(const mln_site(box<P>)& p, const box<P>& b,
			    def::coord inner_border_size)
      {
	mlc_abort(P)::check(); // Not implemented.
	return false;
      }


      // Facade

      template <typename P>
      bool
      on_frontiere(const mln_site(box<P>)& p, const box<P>& b,
		   def::coord inner_border_size)
      {
	return on_frontiere_dispatch(p, b, inner_border_size);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::inner_border::internal

  } // end of namespace mln::inner_border

} // end of namespace mln

#endif // ! MLN_INNER_BORDER_ON_FRONTIERE_HH

