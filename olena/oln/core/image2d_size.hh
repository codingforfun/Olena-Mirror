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

#ifndef OLENA_CORE_IMAGE2D_SIZE_HH
# define OLENA_CORE_IMAGE2D_SIZE_HH

# include <mlc/contract.hh>
# include <oln/core/abstract/image_size.hh>
# include <oln/core/coord.hh>

namespace oln {

  struct image2d_size;

  /*! \class image_size_traits<image2d_size>
  **
  ** The specialized version for image2d_size.
  */
  
  template<>
  struct image_size_traits<image2d_size>
  {
    enum { dim = 2 };
  };

  
  /*! \class image2d_size
  **
  ** Size_type for image2d.
  */

  struct image2d_size : public abstract::image_size<image2d_size >
  {
    /*! \brief Image2d_size constructor.
    **
    ** \arg nrows The number of rows in 
    ** the image is set to \a nrows.
    ** 
    ** \arg ncols The number of columns in
    ** the image is set to \a ncols.
    **
    ** \arg border The border width of the image
    ** is set to border.
    */
    
    image2d_size(coord nrows, coord ncols, coord border)
    {
      nth(0) = nrows;
      nth(1) = ncols;
      border_ = border;
    }

    /// Return the number of rows in the image.
    
    coord 
    nrows() const
    {
      invariant(nth(0) > 0);
      return nth(0);
    }

    /// Return a reference to the number of rows in the image.

    coord& 
    nrows() 
    {
      invariant(nth(0) > 0);
      return nth(0);
    }

    /// Return the number of columns in the image.
    
    coord 
    ncols() const
    {
      invariant(nth(1) > 0);
      return nth(1);
    }

    /// Return a reference to the number of columns in the image.
    
    coord& 
    ncols() 
    {
      invariant(nth(1) > 0);
      return nth(1);
    }

    static std::string 
    name() 
    { 
      return "image2d_size"; 
    }

  };

} // end of oln



#endif // ! OLENA_CORE_IMAGE2D_SIZE_HH
