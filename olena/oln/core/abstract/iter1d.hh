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

#ifndef OLENA_CORE_ABSTRACT_ITER1D_HH
# define OLENA_CORE_ABSTRACT_ITER1D_HH

# include <oln/core/coord.hh>
# include <oln/core/point1d.hh>
# include <oln/core/dpoint1d.hh>
# include <oln/core/image1d_size.hh>
# include <oln/core/abstract/iter.hh>

# include <mlc/contract.hh>
# include <mlc/type.hh>
# include <mlc/objs.hh>

namespace oln {

  namespace abstract {
    template<class Exact>
    class iter1d; // forward declaration
  } // end of abstract

  /*!
  ** \brief Traits for iter::iter1d.
  */
  template<class Exact>
  struct iter_traits<abstract::iter1d<Exact> >: public
  iter_traits<abstract::iter<Exact> >
  {
    enum { dim = 1 }; ///< The dimension of the image traversed.
    typedef point1d point_type; ///< The type of point of the image.
    typedef dpoint1d dpoint_type; ///< The type of dpoint of the image.
  };

  namespace abstract {

    /*!
    ** \brief Iterator on image of 1 dimension.
    **
    ** Allow iterable object (like image, window, ...) of 1 dimension
    ** traversing.
    ** \see iter
    */
    template<class Exact>
    class iter1d : public iter< Exact >
    {
    public:

      typedef iter<Exact> super_type; ///< The exact type of the object.

      friend class iter<Exact>;

      /*!
      ** \brief Get the coordinates of iterator's current point.
      **
      ** On this kind of image, all point are on the same line. So
      ** you are able to get the column number by this way (and never the
      ** line number).
      */
      coord
      col() const
      {
	return this->p_.col();
      }

      /// Return the name of the type.
      static std::string name()
      {
	return std::string("_iter1d<") + Exact::name() + ">";
      }

    protected:

      const coord ncols_; ///< The number of columns of the image you are iterating

      /*!
      ** \brief Get the current point viewed by the iterator.
      ** \return The point (1 dimension) viewed by the iterator.
      ** \pre Instance != end.
      */
      point1d
      to_point() const
      {
	precondition(*this != end);
	invariant(this->p_.col() >= 0 &&
		  this->p_.col() < ncols_);
	return this->p_;
      }

      /*!
      ** \brief Constructor
      */
      iter1d() : super_type(), ncols_(0)
      {}

      /*!
      ** \brief Constructor
      ** \arg size The size of the image to iterate.
      ** \pre size.ncols() > 0.
      **
      ** Construct an iterator (1d) on an image (1d).
      */
      iter1d(const image1d_size& size) :
	super_type(), ncols_(size.ncols())
      {
	precondition(size.ncols() > 0);
	this->exact().goto_begin_();
      }
    };


  } // end of abstract

} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_ITER1D_HH
