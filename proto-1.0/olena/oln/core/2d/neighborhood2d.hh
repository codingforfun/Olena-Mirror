// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_NEIGHBORHOOD2D_HH
# define OLENA_CORE_NEIGHBORHOOD2D_HH

# include <oln/core/coord.hh>
# include <oln/core/abstract/neighborhood.hh>
# include <oln/core/2d/dpoint2d.hh>
# include <oln/core/2d/size2d.hh>
# include <oln/core/2d/window2d.hh>


namespace oln {

  class neighborhood2d; // forward declaration

  // super_type
  template <>
  struct set_super_type< neighborhood2d > { typedef abstract::neighborhood< neighborhood2d > ret; };

  template <>
  struct set_props< category::neighborhood, neighborhood2d >
  {
    typedef dpoint2d dpoint_type;
    typedef size2d   size_type;
    typedef window2d window_type;
  };


  class neighborhood2d : public abstract::neighborhood< neighborhood2d >
  {

  public:

    typedef abstract::neighborhood< neighborhood2d > super_type;

    /*!
    ** \brief Construct a neighborhood of 2 dimensions.
    */
    neighborhood2d() :
      super_type()
    {
    }

    /*!
    ** \brief Construct a neighborhood of 2 dimensions.
    ** \arg size The number of element.
    */
    neighborhood2d(unsigned size) :
      super_type(size)
    {
    }

    /*!
    ** \brief Construct a neighborhood of 2 dimensions from several points.
    ** \arg n The number of element.
    ** \arg crd The coordinates of the elements
    */
    neighborhood2d(unsigned n, const coord_t crd[]) :
      super_type(n)
    {
      for (unsigned i = 0; i < 2 * n; i += 2)
        this->add(dpoint2d(crd[i], crd[i+1]));
    }

    neighborhood2d&
    add(const dpoint2d& dp)
    {
      return this->exact().impl_add(dp);
    }

    neighborhood2d&
    add(coord_t row, coord_t col)
    {
      dpoint2d dp(row, col);
      return add(dp);
    }


    /// Return the name of the type.
    static std::string
    name()
    {
      return std::string("neighborhood2d");
    }

    coord_t
    impl_delta_update(const dpoint2d& dp)
    {
      delta_(abs(dp.row()));
      delta_(abs(dp.col()));
      return delta_;
    }

  };


  inline const neighborhood2d&
  neighb_c4()
  {
    static const coord_t crd[] = { 0,1,  1,0 };
    static const neighborhood2d neighb(2, crd);
    return neighb;
  }

  inline const neighborhood2d&
  neighb_c8()
  {
    static const coord_t crd[] = { 0,1,  1,1,  1,0,  1,-1 };
    static const neighborhood2d neighb(4, crd);
    return neighb;
  }

} // end of oln

#endif // OLENA_CORE_NEIGHBORHOOD2D_HH
