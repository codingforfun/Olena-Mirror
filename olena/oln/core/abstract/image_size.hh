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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_SIZE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_SIZE_HH

# include <mlc/type.hh>
# include <mlc/contract.hh>
# include <oln/core/coord.hh>
# include <ntg/utils/debug.hh>
# include <sstream>



namespace oln {

  namespace abstract {



    template<class Exact>
    struct image_size; // fwd_decl

  } // end of abstract

  template<class Exact>
  struct image_size_traits;

  /*! \class image_size_traits<abstract::image_size<Exact> >
  **
  ** A class specialized for each image type
  ** which gives the dimension of the template
  ** parameter.
  */

  template<class Exact>
  struct image_size_traits<abstract::image_size<Exact> >
  {
  };


  namespace abstract {


    /*! \class image_size
    **
    ** The class that defines the image size
    ** according to its dimension.
    */

    template<class Exact>
    struct image_size : public mlc_hierarchy::any< Exact >
    {

    public:

      enum { dim = image_size_traits<Exact>::dim };

      /*! \brief Return the number of coordinates
      ** in the nth section of the image.
      */

      coord
      nth(unsigned n) const
      {
	assertion(n < dim);
	return coord_[n];
      }

      /*! \brief Return a reference to the number of coordinates
      ** in the nth section of the image.
      */


      coord&
      nth(unsigned n)
      {
	assertion(n < dim);
	return coord_[n];
      }

      /// Return the value border width of the current image.
      coord
      border() const
      {
	return border_;
      }

      /// Return a reference to  the border width of the current image.

      coord&
      border()
      {
	return border_;
      }

      /*! \brief Test if two images
      ** have compatible size.
      **
      ** \return True if the two images have compatible size, false otherwise.
      */

      template< class S >
      bool
      operator==(const image_size<S>& size) const
      {
	for (unsigned i = 0; i < dim; ++i)
	  if (coord_[i] != size.coord_[i])
	    return false;
	return true;
      }

      /*! \brief Test if two images do not have compatible size
      **
      ** \return False if the two images have compatible size, true otherwise.
      */

      template< class S >
      bool
      operator!=(const image_size<S>& size) const
      {
	for (unsigned i = 0; i < dim; ++i)
	  if (coord_[i] != size.coord_[i])
	    return true;
	return false;
      }



      static std::string
      name()
      {
	return std::string("image_size<") +
	  Exact::name() + ">";
      }

      image_size()
      {}

    protected:

      /*! border_ represents the width of the image border
      ** such a mecanism allow algorithm to perform operation
      ** on the points at the edge of the image as if they were
      ** normally surrounded by points
      */

      coord border_;

    private:


      /*! \brief An array that contains the number
      ** of coordinates for each dimension.
      */

      coord coord_[dim];

    };

  } // end of abstract

} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_SIZE_HH
