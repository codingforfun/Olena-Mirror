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

#ifndef OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_SIGMA_HH
# define OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_SIGMA_HH

# include <oln/topo/combinatorial-map/internal/anyfunc.hh>
# include <oln/topo/combinatorial-map/internal/alpha.hh>

# include <mlc/contract.hh>

# include <vector>

namespace oln {

  namespace topo {

    namespace combinatorial_map {

      namespace internal {

	/* sigma function
	**
	** \todo Add doc.
	*/
	template <class U>
	class sigma : public anyfunc< U, U, sigma<U> >
	{
	public:
	  static std::string
	  name()
	  {
	    return "sigma";
	  }

	  void
	  resize_(unsigned n)
	  {
	    this->f_.resize(n+1);
	    this->f_1_.resize(n+1);
	  }

	  void
	  assign_(const U & i, const U & e)
	  {
	    assertion(e < this->f_1_.size());

	    this->f_[i] = e;
	    this->f_1_[e] = i;
	  }

	  void
	  erase_(const U & d)
	  {
	    this->f_[this->f_1_[d]] = this->f_[d];
	    this->f_1_[this->f_[d]] = this->f_1_[d];
	    this->f_[d] = this->f_1_[d] = 0;

	    unsigned d_ = alpha<U>::result(d);
	    this->f_[this->f_1_[d_]] = this->f_[d_];
	    this->f_1_[this->f_[d_]] = this->f_1_[d_];
	    this->f_[d_] = this->f_1_[d_] = 0;
	  }

	private:
	  std::vector<U> f_1_;
	};

      } // end of namespace internal

    } // end of namespace combinatorial_map

  } // end of namespace topo

} // end of namespace oln

#endif // ! OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_SIGMA_HH
