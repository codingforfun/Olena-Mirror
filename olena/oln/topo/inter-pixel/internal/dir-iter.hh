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
# include <oln/core/abstract/iter1d.hh>
# include <oln/core/abstract/iter2d.hh>
# include <oln/core/abstract/iter3d.hh>
# include <oln/topo/inter-pixel/internal/dir.hh>

namespace oln {

  namespace topo {
    namespace inter_pixel {
      namespace internal {
	template<unsigned Dim, class Exact>
	class _dir_iter; // fwd_decl
      }
    }
  }

  template<class Exact>
  struct iter_traits<topo::inter_pixel::internal::_dir_iter<1, Exact> >: public
  iter_traits<abstract::iter1d<Exact> >
  {
    typedef abstract::iter1d<Exact> super_type;
    typedef point1d point_type;
    typedef dpoint1d dpoint_type;
  };

  template<class Exact>
  struct iter_traits<topo::inter_pixel::internal::_dir_iter<2, Exact> >: public
  iter_traits<abstract::iter2d<Exact> >
  {
    typedef abstract::iter2d<Exact> super_type;
    typedef point2d point_type;
    typedef dpoint2d dpoint_type;
  };

  template<class Exact>
  struct iter_traits<topo::inter_pixel::internal::_dir_iter<3, Exact> >: public
  iter_traits<abstract::iter3d<Exact> >
  {
    typedef abstract::iter3d<Exact> super_type;
    typedef point3d point_type;
    typedef dpoint3d dpoint_type;
  };

  namespace topo {

    namespace inter_pixel {

      namespace internal {

	template<unsigned Dim, class Exact>
	class _dir_iter : public iter_traits<_dir_iter<Dim, Exact> >::super_type
	{
	protected:
	  typedef typename dir_traits<Dim>::ret dir_t;
	  typedef typename iter_traits<_dir_iter<Dim, Exact> >::super_type super_type;

	  _dir_iter() : super_type(), _cur(this->exact().begin()), _cnt(0) {}
	  _dir_iter(dir_t i) : super_type(), _cur(i), _cnt(0) {}

	public:
	  operator dir_t()
	  {
	    return _cur;
	  }

	  dir_t to_dir() const
	  {
	    return _cur;
	  }

	  void operator++()
	  {
	    precondition(_cnt != Dim * 2);
	    _cur = this->exact().next();
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
	    _cur = this->exact().begin();
	    _cnt = 0;
	    return b;
	  }
	  mlc::_end operator=(mlc::_end e)
	  {
	    _cur = this->exact().begin();
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

template<unsigned Dim, class Exact>
inline 
std::ostream&
operator<<(std::ostream& o, const oln::topo::inter_pixel::internal::_dir_iter<Dim, Exact>& it)
{
  return o << it.to_dir();
}

#endif // !OLENA_TOPO_INTER_PIXEL_INTERNAL_DIR_ITER_HH
