// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_NEIGHBORHOOD_HH
# define OLENA_CORE_NEIGHBORHOOD_HH

# include <oln/core/type.hh>

namespace oln {

  template<class Exact>
  struct neighborhood : public type::any<Exact>
  {

    static std::string name()
    {
      return std::string("neighborhood<") + Exact::name() + ">";
    }

  protected:
    neighborhood() {}
  };

  template<class N_>
  inline 
  Exact(N_) inter(const neighborhood<N_> &_lhs, 
		  const neighborhood<N_> &_rhs)
  {
    Exact_cref(N, lhs);
    Exact_cref(N, rhs);
    
    Exact(N_) neighb;
    for (unsigned j = 0; j < rhs.card(); ++j)
      if (lhs.has(rhs.dp(j)) && ! neighb.has(rhs.dp(j)))
	neighb.add(rhs.dp(j));
    return neighb;
  }

  template<class N_>
  inline
  Exact(N_) uni(const neighborhood<N_> &_lhs, 
		const neighborhood<N_> &_rhs)
  {
    Exact_cref(N, lhs);
    Exact_cref(N, rhs);
    
    std::cout << lhs.name() << std::endl;
    
    Exact(N_) neighb;
    for (unsigned j = 0; j < rhs.card(); ++j)
      if (! neighb.has(rhs.dp(j)))
	neighb.add(rhs.dp(j));
    for (unsigned j = 0; j < lhs.card(); ++j)
      if (! neighb.has(lhs.dp(j)))
	neighb.add(lhs.dp(j));
    return neighb;
  }

} // end of oln


#endif // OLENA_CORE_NEIGHBORHOOD_HH
