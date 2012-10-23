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

/// \file
///
/// \brief Return the equivalent domain in Kn from a domain in
/// K0.

#ifndef MLN_WORLD_KN_DOMAIN_FROM_K0_HH
# define MLN_WORLD_KN_DOMAIN_FROM_K0_HH

# include <mln/core/concept/box.hh>
# include <mln/world/kn/internal/immerse_point.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace internal
      {

	/// \brief Return the equivalent domain in Kn from a domain in
	/// K0.
	template <typename B>
	B
	domain_from_k0(const Box<B>& b, const unsigned n,
		       unsigned inner_border_thickness);

	/// \overload
	/// \p inner_border_thickness is set to 0.
	template <typename B>
	B
	domain_from_k0(const Box<B>& b, const unsigned n);


# ifndef MLN_INCLUDE_ONLY


	template <typename B>
	B
	domain_from_k0(const Box<B>& b_, const unsigned n,
		       unsigned inner_border_thickness)
	{
	  mln_precondition(exact(b_).is_valid());
	  const B& b = exact(b_);

	  mln_deduce(B, site, delta) one;
	  one.set_all(1);
	  B bout = B(immerse_point(b.pmin(), n, inner_border_thickness)
		     - (inner_border_thickness * 2 * one) - one,
		     immerse_point(b.pmax(), n, inner_border_thickness)
		     + (std::pow(2,n) - 1) * one + (inner_border_thickness * 2 * one));
	  return bout;
	}


	template <typename B>
	B
	domain_from_k0(const Box<B>& b, const unsigned n)
	{
	  return domain_from_K0(b, n, 0);
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::work::kn::internal

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_DOMAIN_FROM_K0_HH


