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

#ifndef OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ZETA_HH
# define OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ZETA_HH

namespace oln {

  namespace topo {

    namespace combinatorial_map {

      namespace internal {

	/* zeta function.
	**
	** Tabular of 4 elements.
	**
	** \todo FIXME: Add doc.
	*/
	struct zeta
	{
	  zeta() : empty_(true)
	  {
	    for (unsigned i = 0; i < 4; ++i)
	      darts_[i] = 0;
	  }

	  bool
	  empty() const
	  {
	    return empty_;
	  }

	  unsigned
	  operator[](unsigned i) const
	  {
	    return darts_[i];
	  }
	  unsigned&
	  operator[](unsigned i)
	  {
	    empty_ = false;
	    return darts_[i];
	  }

	private:
	  unsigned darts_[4];
	  bool empty_;
	};

# define oln_zeta_type(ImgType)		\
typename oln::mute< ImgType, oln::topo::combinatorial_map::internal::zeta>::ret

      } // end of namespace internal

    } // end of namespace combinatorial_map

  } // end of namespace topo

} // end of namespace oln

#endif // ! OLENA_TOPO_COMBINATORIAL_MAP_INTERNAL_ZETA_HH
