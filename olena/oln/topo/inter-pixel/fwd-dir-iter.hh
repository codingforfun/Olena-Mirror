
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

#ifndef OLENA_TOPO_INTER_PIXEL_FWD_DIR_ITER_HH
# define OLENA_TOPO_INTER_PIXEL_FWD_DIR_ITER_HH

# include <oln/core/abstract/iter.hh>
# include <oln/topo/inter-pixel/internal/dir-iter.hh>

namespace oln {

  namespace topo {

    namespace inter_pixel {
      template<unsigned Dim, class Exact = mlc::final>
      class fwd_dir_iter;
    }

  }

  template<unsigned Dim, class Exact>
  struct iter_traits<topo::inter_pixel::fwd_dir_iter<Dim, Exact> >: public
  iter_traits<topo::inter_pixel::internal::_dir_iter<Dim, typename
  mlc::exact_vt<topo::inter_pixel::fwd_dir_iter<Dim, Exact>, Exact>::ret> >
  {

  };

  namespace topo {

    namespace inter_pixel {

      template<unsigned Dim, class Exact>
      class fwd_dir_iter : public internal::_dir_iter<Dim, typename mlc::exact_vt<fwd_dir_iter<Dim, Exact>, Exact>::ret>
      {
      private:
	typedef internal::_dir_iter<Dim, typename mlc::exact_vt<fwd_dir_iter<Dim, Exact>, Exact>::ret> super;
	typedef typename super::dir_t dir_t;

      public:
	fwd_dir_iter() : super(), _begin(internal::dir_traits<Dim>::first()) {}
	fwd_dir_iter(dir_t i) : super(i), _begin(i) {}

	template<class U> U operator=(U u) { return super::operator=(u); }

	dir_t next() { return internal::dir_traits<Dim>::next(_cur); }

	dir_t begin() { return _begin; }

      private:
	dir_t _begin;
      };

# define FwdDirIter(ImgType)		\
fwd_dir_iter< ImgType::dim >

    } // end inter_pixel

  } // end topo

} // end oln

#endif // !OLENA_TOPO_INTER_PIXEL_FWD_DIR_ITER_HH
