// Copyright (C) 2001  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_WITH_DIM_HH
# define OLENA_CORE_ABSTRACT_IMAGE_WITH_DIM_HH

# include <mlc/contract.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/coord.hh>
# include <oln/core/point1d.hh>
# include <oln/core/image1d_size.hh>
# include <oln/core/point2d.hh>
# include <oln/core/image2d_size.hh>
# include <oln/core/point3d.hh>
# include <oln/core/image3d_size.hh>
//# include <oln/io/readable.hh>
# include <stdlib.h>

namespace oln {

  // fwd decl
  template<class Exact>
  struct fwd_iter1d;
  template<class Exact>
  struct bkd_iter1d;
  // fwd decl
  template<class Exact>
  struct fwd_iter2d;
  template<class Exact>
  struct bkd_iter2d;
  // fwd decl
  template<class Exact>
  struct fwd_iter3d;
  template<class Exact>
  struct bkd_iter3d;


  namespace abstract {

    // fwd_decl
    template <unsigned Dim, class Exact>
    class image_with_dim;

  } // end of namespace abstract


    template <unsigned Dim, class Exact>
    struct image_traits<abstract::image_with_dim<Dim, Exact> >: public image_traits<abstract::image<Exact> >
    {
      enum {dim = Dim};
      typedef pointnd<Dim> point_type;
      //FIXME what about nd-iterators ?
    };

    template <class Exact>
    struct image_traits<abstract::image_with_dim<1, Exact> >: public image_traits<abstract::image<Exact> >
    {
      enum {dim = 1};
      typedef point1d point_type;
      typedef dpoint1d dpoint_type;
      typedef fwd_iter1d<mlc::final> iter_type;
      typedef fwd_iter1d<mlc::final> fwd_iter_type;
      typedef bkd_iter1d<mlc::final> bkd_iter_type;
      typedef image1d_size size_type;
    };

    template <class Exact>
    struct image_traits<abstract::image_with_dim<2, Exact> >: public image_traits<abstract::image<Exact> >
    {
      enum {dim = 2};
      typedef point2d point_type;
      typedef dpoint2d dpoint_type;
      typedef fwd_iter2d<mlc::final> iter_type;
      typedef fwd_iter2d<mlc::final> fwd_iter_type;
      typedef bkd_iter2d<mlc::final> bkd_iter_type;
      typedef image2d_size size_type;
    };

    template <class Exact>
    struct image_traits<abstract::image_with_dim<3, Exact> >: public image_traits<abstract::image<Exact> >
    {
      enum {dim = 3};
      typedef point3d point_type;
      typedef dpoint3d dpoint_type;
      typedef fwd_iter3d<mlc::final> iter_type;
      typedef fwd_iter3d<mlc::final> fwd_iter_type;
      typedef bkd_iter3d<mlc::final> bkd_iter_type;
      typedef image3d_size size_type;
    };


  namespace abstract {

    // one-dimensional specialization
    template <class Exact>
    class image_with_dim<1, Exact>: public virtual abstract::image<Exact>
    {
    public:
      typedef typename image_traits<Exact>::point_type point_type;
      typedef typename image_traits<Exact>::point_type dpoint_type;
      typedef typename image_traits<Exact>::iter_type iter_type;
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<Exact>::value_type value_type;
      typedef typename image_traits<Exact>::size_type size_type;

      typedef abstract::image<Exact> super_type;
      typedef image_with_dim<1, Exact> self_type;
      typedef Exact exact_type;

      coord ncols() const
      {
	return size().ncols();
      }

      // FIXME: size_t ???
      size_t npoints_() const
      {
	return size_t(ncols());
      }

      const value_type operator()(coord col) const
      {
	return to_exact(*this)[point_type(col)];
	//          super_type::operator[](point_type(col));
      }

      value_type& operator()(coord col)
      {
	return to_exact(*this)[point_type(col)];
	//return super_type::operator[](point_type(col));
      }

      using abstract::image<Exact>::hold;

      bool hold(coord col) const
      {
	return hold(point_type(col));
      }

      exact_type& operator=(self_type rhs)
      {
	return to_exact(this)->assign(to_exact(rhs));
      }

      static std::string name()
      {
	return
	  std::string("abstract::image_with_dim<")
	  + Dim + ","
	  + Exact::name() + ">";
      }

    protected:
      image_with_dim() {}

    }; // end of one-dimensional specialization

    // bi-dimensional specialization
    template <class Exact>
    class image_with_dim<2, Exact>: public virtual abstract::image<Exact>
    {
    public:
      typedef typename image_traits<Exact>::point_type point_type;
      typedef typename image_traits<Exact>::dpoint_type dpoint_type;
      typedef typename image_traits<Exact>::iter_type iter_type;
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<Exact>::value_type value_type;
      typedef typename image_traits<Exact>::size_type size_type;


      typedef image_with_dim<2, Exact> self_type;
      typedef Exact exact_type;
      typedef abstract::image<Exact> super_type;

      coord nrows() const
      {
	return size().nrows();
      }

      coord ncols() const
      {
	return size().ncols();
      }

      // FIXME: size_t ???
      size_t npoints_() const
      {
	return size_t(nrows()) * size_t(ncols());
      }

      const value_type operator()(coord row, coord col) const
      {
	return to_exact(*this)[point_type(row, col)];
	//return super_type::operator[](point_type(row, col));
      }

      value_type& operator()(coord row, coord col)
      {
	return to_exact(*this)[point_type(row, col)];
	//return super_type::operator[](point_type(row, col));
      }

      using abstract::image<Exact>::hold;

      bool hold(coord row, coord col) const
      {
	return hold(point_type(row, col));
      }

      exact_type& operator=(self_type rhs)
      {
	return to_exact(this)->assign(to_exact(rhs));
      }

      static std::string name()
      {
	return
	  std::string("abstract::image_with_dim<")
	  + Dim + ","
	  + Exact::name() + ">";
      }

    protected:
      image_with_dim() {}

    }; // end of bi-dimensional specialization

    // tri-dimensional specialization
    template <class Exact>
    class image_with_dim<3, Exact>: public virtual abstract::image<Exact>
    {
    public:
      typedef typename image_traits<Exact>::point_type point_type;
      typedef typename image_traits<Exact>::point_type dpoint_type;
      typedef typename image_traits<Exact>::iter_type iter_type;
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<Exact>::value_type value_type;
      typedef typename image_traits<Exact>::size_type size_type;
      typedef abstract::image<Exact> super_type;

      typedef image_with_dim<3, Exact> self_type;
      typedef Exact exact_type;

      coord nslices() const
      {
	return size().nslices();
      }

      coord nrows() const
      {
	return size().nrows();
      }

      coord ncols() const
      {
	return size().ncols();
      }

      size_t npoints_() const
      {
	return size_t(nslices()) * size_t(nrows()) * size_t(ncols());
      }

      const value_type operator()(coord slice, coord row, coord col) const
      {
	return to_exact(*this)[point_type(slice, row, col)];
	// return super_type::operator[](point_type(slice, row, col));
      }

      value_type& operator()(coord slice, coord row, coord col)
      {
	return to_exact(*this)[point_type(slice, row, col)];
	// return super_type::operator[](point_type(slice, row, col));
      }

      bool hold(coord slice, coord row, coord col) const
      {
	return hold(point_type(slice, row, col));
      }

      exact_type& operator=(self_type rhs)
      {
	return to_exact(this)->assign(to_exact(rhs));
      }

      static std::string name()
      {
	return
	  std::string("abstract::image_with_dim<")
	  + Dim + ","
	  + Exact::name() + ">";
      }

    protected:
      image_with_dim(){}

    }; // end of tri-dimensional specialization


  } // end of namespace abstract
} // end of namespace oln



template<class Exact> inline std::ostream&
operator<<(std::ostream& o, const oln::abstract::image_with_dim<1, Exact>& ima)
{
  if (to_exact(ima).impl() == 0)
    return o << "null";
  for (oln::coord col = 0; col < ima.ncols(); ++col)
    o << ima(col) << ' ';
  o << std::endl;
  return o;
}

template<class Exact> inline std::ostream&
operator<<(std::ostream& o, const oln::abstract::image_with_dim<2, Exact>& ima)
{
  typedef typename oln::image_id<Exact>::value_type value_type;
  if (to_exact(ima).impl() == 0)
    return o << "null";

  for (oln::coord row = 0; row < ima.nrows(); ++row)
    {
      for (oln::coord col = 0; col < ima.ncols(); ++col)
	{
	  o.width(ntg_max_print_width(value_type));
	  o << ima(row,col) << ' ';
	}
      o << std::endl;
    }
  return o;
}

template<class Exact> inline std::ostream&
operator<<(std::ostream& o, const oln::abstract::image_with_dim<3, Exact>& ima)
{
  typedef typename oln::image_id<Exact>::value_type value_type;

  if (to_exact(ima).impl() == 0)
    return o << "null";
  for (oln::coord slice = 0; slice < ima.nslices(); ++slice) {
    o << "### " << slice << std::endl;
    for (oln::coord row = 0; row < ima.nrows(); ++row) {
      for (oln::coord col = 0; col < ima.ncols(); ++col)
	{
	  o.width(ntg_max_print_width(value_type));
	  o << ima(slice, row, col) << ' ';
	}
      o << std::endl;
    }
  }
  return o;
}




#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_DIM_HH
