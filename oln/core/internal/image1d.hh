// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_INTERNAL_IMAGE1D_HH
# define OLENA_CORE_INTERNAL_IMAGE1D_HH

# include <oln/core/contract.hh>
# include <oln/core/image.hh>
# include <oln/core/coord.hh>
# include <oln/core/point1d.hh>
# include <oln/core/image1d_size.hh>
# include <oln/core/regular_image.hh>
//# include <oln/io/readable.hh>
# include <stdlib.h>

namespace oln {


  // fwd decl
  struct fwd_iter1d;
  struct bkd_iter1d;


  namespace internal
  {

    // _image1d is a base class for every 1d image type

    template<class T, class Inferior = type::bottom>
    class _image1d : public regular_image< 1, _image1d<T,Inferior> >
    {
    public:

      typedef Inferior inferior;

      typedef T value;
      typedef fwd_iter1d iter;
      typedef fwd_iter1d fwd_iter;
      typedef bkd_iter1d bkd_iter;

      // the following methods are deduced from the method data() that
      // has to be provided by sub-classes

      // every kind of 1d images thus inherits these methods

      // nota bene: there is no assertion about data() return type
      // except that it holds several methods such as ncols(), etc.

      coord ncols() const
      {
	assertion(_has_data());
	return to_exact(this)->data()->ncols();
      }

      coord border() const
      {
	assertion(_has_data());
	return to_exact(this)->data()->border();
      }

      const image1d_size size() const
      {
	assertion(_has_data());
	return image1d_size(to_exact(this)->data()->ncols());
      }

      size_t npoints() const
      {
	return size_t(ncols());
      }

      bool hold(coord col) const
      {
	assertion(_has_data());
	return to_exact(this)->data()->hold(col);
      }

      bool hold(const point1d& p) const
      {
	return hold(p.col());
      }

      const T operator[](const point1d& p) const
      {
	assertion(_has_data());
	return to_exact(this)->data()->at(p.col());
      }

      T& operator[](const point1d& p)
      {
	assertion(_has_data());
	return to_exact(this)->data()->at(p.col());
      }

      const T operator()(coord col) const
      {
	assertion(_has_data());
	return to_exact(this)->data()->at(col);
      }

      T& operator()(coord col)
      {
	assertion(_has_data());
	return to_exact(this)->data()->at(col);
      }

      static std::string name()
      {
	return
	  std::string("_image1d<")
	  + T::name() + ","
	  + Inferior::name() + ">";
      }

    protected:

      _image1d()
      {
      }

      bool _has_data() const
      {
	return to_exact(this)->data() != 0;
      }
    };



  } // end of internal

} // end of oln


#endif // ! OLENA_CORE_INTERNAL_IMAGE1D_HH
