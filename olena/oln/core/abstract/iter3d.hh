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

#ifndef OLENA_CORE_ABSTRACT_ITER3D_HH
# define OLENA_CORE_ABSTRACT_ITER3D_HH

# include <oln/core/coord.hh>
# include <oln/core/point3d.hh>
# include <oln/core/dpoint3d.hh>
# include <oln/core/image3d_size.hh>
# include <oln/core/abstract/iter.hh>

# include <mlc/contract.hh>
# include <mlc/type.hh>
# include <mlc/objs.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    class iter3d; // forward declaration
  } // end of abstract

  /*!
  ** \brief Traits for abstract::iter3d
  */
  template<class Exact>
  struct iter_traits<abstract::iter3d<Exact> >: public
  iter_traits<abstract::iter<Exact> >
  {
    enum { dim = 3 }; ///< The dimension of the image traversed.
    typedef point3d point_type; ///< The type of point of the image.
    typedef dpoint3d dpoint_type; ///< The type of dpoint of the image.
  };

  namespace abstract {

    /*!
    ** Iterator on image of 3 dimensions.
    **
    ** Allow iterable object (like image, window, ...) of 3 dimensions
    ** traversing.
    ** \see iter
    */
    template<class Exact>
    class iter3d : public iter< Exact >
    {
    public:

      typedef iter<Exact> super_type; ///< The exact type of the object.

      friend class iter<Exact>;

      /*!
      ** \brief Get the coordinates (slice) of iterator's current point.
      ** \return The slice number.
      **
      ** On this kind of image (i.e. 3 dimensions), you are able to get
      ** the column number, the row number and the slice number.
      */
      coord
      slice() const
      {
	return this->p_.slice();
      }

      /*!
      ** \brief Get the coordinates (row) of iterator's current point.
      ** \return The row number.
      **
      ** On this kind of image (i.e. 3 dimensions), you are able to get
      ** the column number, the row number and the slice number.
      */
      coord
      row() const
      {
	return this->p_.row();
      }

      /*!
      ** \brief Get the coordinates (col) of iterator's current point.
      ** \return The col number.
      **
      ** On this kind of image (i.e. 3 dimensions), you are able to get
      ** the column number, the row number and the slice number.
      */
      coord
      col() const
      {
	return this->p_.col();
      }

      /// Return the name of the type.
      static std::string name()
      {
	return std::string("_iter3d<") + Exact::name() + ">";
      }

    protected:

      const coord nslices_; ///< The number of slices of the image you are iterating.
      const coord nrows_; ///< The number of rows of the image you are iterating.
      const coord ncols_; ///< The number of columns of the image you are iterating.

      /*!
      ** \brief Get the current point viewed by the iterator.
      ** \return The point (3 dimensions) viewed by the iterator.
      ** \pre Instance != end.
      */
      point3d
      to_point() const
      {
	precondition(*this != end);
	invariant(this->p_.slice() >=0
		  && this->p_.slice() < nslices_
		  && this->p_.row() >= 0
		  && this->p_.row() < nrows_
		  && this->p_.col() >= 0
		  && this->p_.col() < ncols_);
	return this->p_;
      }

      /*!
      ** \brief Constructor
      */
      iter3d() : super_type(), nslices_(0), nrows_(0), ncols_(0)
      {}

      /*!
      ** \brief Construct an iterator (3d) on an image (3d).
      ** \arg size The size of the image to iterate.
      ** \pre size.ncols() > 0.
      ** \pre size.nrows() > 0.
      ** \pre size.nslices() > 0.
      */
      iter3d(const image3d_size& size) :
	super_type(),
	nslices_(size.nslices()),
	nrows_(size.nrows()),
	ncols_(size.ncols())
      {
	precondition(size.nslices() > 0
		     && size.nrows() > 0
		     && size.ncols() > 0);
	this->exact().goto_begin_();
      }
    };

  } // end of abstract

} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_ITER3D_HH
