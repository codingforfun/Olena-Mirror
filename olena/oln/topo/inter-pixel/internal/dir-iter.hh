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

#ifndef OLENA_TOPO_INTER_PIXEL_INTERNAL_DIR_ITER_HH
# define OLENA_TOPO_INTER_PIXEL_INTERNAL_DIR_ITER_HH

# include <mlc/objs.hh>
# include <oln/core/iter.hh>
# include <oln/topo/inter-pixel/internal/dir.hh>

namespace oln {

  namespace topo {

    namespace inter_pixel {

      namespace internal {

	template<unsigned Dim, class Exact>
	class _dir_iter : public virtual iter<Exact>
	{
	protected:
	  typedef typename dir_traits<Dim>::ret dir_t;

	  _dir_iter() : _cur(to_exact(this)->begin()), _cnt(0) {}
	  _dir_iter(dir_t i) : _cur(i), _cnt(0) {}

	public:
	  operator dir_t()
	  {
	    return _cur;
	  }

	  void operator++()
	  {
	    precondition(_cnt != Dim * 2);
	    _cur = to_exact(this)->next();
	    ++_cnt;
	  }

	  bool operator==(mlc::_end) const
	  {
	    return _cnt == Dim * 2;
	  }
	  bool operator!=(mlc::_end) const
	  {
	    return _cnt != Dim * 2;
	  }

	  mlc::_begin operator=(mlc::_begin b)
	  {
	    _cur = to_exact(this)->begin();
	    _cnt = 0;
	    return b;
	  }
	  mlc::_end operator=(mlc::_end e)
	  {
	    _cur = to_exact(this)->begin();
	    _cnt = Dim * 2;
	    return e;
	  }

	protected:
	  dir_t _cur;

	private:
	  unsigned _cnt;
	};

      } // end internal

    } // end inter_pixel

  } // end topo

} // end oln

#endif // !OLENA_TOPO_INTER_PIXEL_INTERNAL_DIR_ITER_HH
