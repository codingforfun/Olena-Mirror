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
	class dir_iter_; // fwd_decl

      }
    }
  }

  /// Traits for iterator for 1D directions.
  template<class Exact>
  struct iter_traits<topo::inter_pixel::internal::dir_iter_<1, Exact> >
    : public iter_traits<abstract::iter1d<Exact> >
  {
    typedef abstract::iter1d<Exact> super_type;
    typedef point1d point_type;
    typedef dpoint1d dpoint_type;
  };

  /// Traits for iterator for 2D directions.
  template<class Exact>
  struct iter_traits<topo::inter_pixel::internal::dir_iter_<2, Exact> >
    : public iter_traits<abstract::iter2d<Exact> >
  {
    typedef abstract::iter2d<Exact> super_type;
    typedef point2d point_type;
    typedef dpoint2d dpoint_type;
  };

  /// Traits for iterator for 3D directions.
  template<class Exact>
  struct iter_traits<topo::inter_pixel::internal::dir_iter_<3, Exact> >
    : public iter_traits<abstract::iter3d<Exact> >
  {
    typedef abstract::iter3d<Exact> super_type;
    typedef point3d point_type;
    typedef dpoint3d dpoint_type;
  };

  namespace topo {

    namespace inter_pixel {

      namespace internal {

	template<unsigned Dim, class Exact>
	class dir_iter_ : public iter_traits<dir_iter_<Dim, Exact> >::super_type
	{
	protected:
	  typedef typename dir_traits<Dim>::ret dir_type;
	  typedef typename iter_traits<dir_iter_<Dim, Exact> >::super_type super_type;

	  dir_iter_() : super_type(), cur_(this->exact().begin()), cnt_(0) {}
	  dir_iter_(dir_type i) : super_type(), cur_(i), cnt_(0) {}

	public:
	  operator dir_type()
	  {
	    return cur_;
	  }

	  dir_type
	  to_dir() const
	  {
	    return cur_;
	  }

	  void
	  operator++()
	  {
	    precondition(cnt_ != Dim * 2);
	    cur_ = this->exact().next();
	    ++cnt_;
	  }

	  bool
	  operator==(mlc::end_type) const
	  {
	    return cnt_ == Dim * 2;
	  }
	  bool
	  operator!=(mlc::end_type) const
	  {
	    return cnt_ != Dim * 2;
	  }

	  mlc::begin_type
	  operator=(mlc::begin_type b)
	  {
	    cur_ = this->exact().begin();
	    cnt_ = 0;
	    return b;
	  }
	  mlc::end_type
	  operator=(mlc::end_type e)
	  {
	    cur_ = this->exact().begin();
	    cnt_ = Dim * 2;
	    return e;
	  }

	protected:
	  dir_type cur_;

	private:
	  unsigned cnt_;
	};

      } // end of namespace internal

    } // end of namespace inter_pixel

  } // end of namespace topo

} // end of namespace oln

template<unsigned Dim, class Exact>
inline
std::ostream&
operator<<(std::ostream& o, const oln::topo::inter_pixel::internal::dir_iter_<Dim, Exact>& it)
{
  return o << it.to_dir();
}

#endif // ! OLENA_TOPO_INTER_PIXEL_INTERNAL_DIR_ITER_HH
