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

#ifndef OLENA_MORPHO_SPLITSE_HH
# define OLENA_MORPHO_SPLITSE_HH

# include "basics.hh"

namespace oln {
  namespace morpho {

    template<class _E>
    Exact(_E) get_plus_se_only(const struct_elt<_E>& _se)
    {
      Exact_cref(E, se);
      Iter(E) dp(se);
      E out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < E::dim; ++n)
	    if (dp.cur().nth(n) < 0) {
	      out.add(dp);
	      break;
	    } else if (dp.cur().nth(n) > 0) {
	      break;
	    }
	}
      return out;
    }

    template<class _E>
    Exact(_E) get_plus_se_p(const struct_elt<_E>& _se)
    {
      Exact_cref(E, se);
      Iter(E) dp(se);
      E out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < E::dim; ++n)
	    if (dp.cur().nth(n) < 0) {
	      out.add(dp);
	      break;
	    } else if (dp.cur().nth(n) > 0) {
	      break;
	    }
	  // All p.nth(n) are 0.
	  if (n == E::dim)
	    out.add(dp);
	}
      return out;
    }

    template<class _E>
    Exact(_E) get_minus_se_only(const struct_elt<_E>& _se)
    {
      Exact_cref(E, se);
      Iter(E) dp(se);
      E out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < E::dim; ++n)
	    if (dp.cur().nth(n) > 0) {
	      out.add(dp);
	      break;
	    } else if (dp.cur().nth(n) < 0) {
	      break;
	    }
	}
      return out;
    }

    template<class _E>
    Exact(_E) get_minus_se_p(const struct_elt<_E>& _se)
    {
      Exact_cref(E, se);
      Iter(E) dp(se);
      E out;

      for_all (dp)
	{
	  unsigned n;
	  for (n = 0; n < E::dim; ++n)
	    if (dp.cur().nth(n) > 0) {
	      out.add(dp);
	      break;
	    } else if (dp.cur().nth(n) < 0) {
	      break;
	    }
	  // All p.nth(n) are 0.
	  if (n == E::dim)
	    out.add(dp);
	}
      return out;
    }

  } // morpho
} // oln

#endif // OLENA_MORPHO_SPLITSE_HH
