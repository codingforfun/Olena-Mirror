// Copyright (C) 2001, 2003, 2004  EPITA Research and Development Laboratory
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

# include <cstdlib>



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

  
  /*! \class image_traits<abstract::image_with_dim<1, Exact> > 
  **
  ** The specialized version for 1d image.
  */
  
  template <class Exact>
  struct image_traits<abstract::image_with_dim<1, Exact> > 
    : public image_traits<abstract::image<Exact> >
  {
    
    enum {dim = 1};
    /*!< Image1d dimension = 1. */
    typedef point1d point_type; 
    /*!< Image1d point_type is point1d. */
    typedef dpoint1d dpoint_type; 
    /*!< Image1d dpoint_type is dpoint1d. */
    typedef fwd_iter1d<mlc::final> iter_type; 
    /*!< Image1d iter_type is fwd_iter1d<mlc::final>. */
    typedef fwd_iter1d<mlc::final> fwd_iter_type; 
    /*!< Image1d fwd_iter_type is fwd_iter1d<mlc::final>. */
    typedef bkd_iter1d<mlc::final> bkd_iter_type; 
    /*!< Image1d bkd_iter_type is bkd_iter1d<mlc::final>. */
    typedef image1d_size size_type; 
    /*!< Image1d size_type is image1d_size. */
  };
  
  
  /*! \class image_traits<abstract::image_with_dim<2, Exact> >
  **
  ** The specialized version for 2d image.
  **
  */

  template <class Exact>
  struct image_traits<abstract::image_with_dim<2, Exact> >
    : public image_traits<abstract::image<Exact> >
  {
    enum {dim = 2}; 
    /*!< Imaged2 dimension = 2. */ 
    typedef point2d point_type; 
    /*!< Imaged2 point_type is point2d. */
    typedef dpoint2d dpoint_type; 
    /*!< Image2d dpoint_type is dpoint2d. */
    typedef fwd_iter2d<mlc::final> iter_type;
    /*!< Image2d iter_type is fwd_iter2d<mlc::final>. */
    typedef fwd_iter2d<mlc::final> fwd_iter_type;
    /*!< Image2d fwd_iter_type is fwd_iter2d<mlc::final>. */
    typedef bkd_iter2d<mlc::final> bkd_iter_type;
    /*!< Image2d bkd_iter_type is bkd_iter2d<mlc::final>. */
    typedef image2d_size size_type;
    /*!< Image2d size_type is image2d_size. */
  };
  
  /*! \class image_traits<abstract::image_with_dim<3, Exact> >
  **
  ** The specialized version for 3d image.
  */

  template <class Exact>
  struct image_traits<abstract::image_with_dim<3, Exact> >
    : public image_traits<abstract::image<Exact> >
  {
    enum {dim = 3};
    /*!< Image3d dimension = 3. */ 
    typedef point3d point_type;
    /*!< Image3d point_type is point3d. */
    typedef dpoint3d dpoint_type;
    /*!< Image3d dpoint_type is dpoint3d. */
    typedef fwd_iter3d<mlc::final> iter_type; 
    /*!< Image3d iter_type is fwd_iter3d<mlc::final>. */
    typedef fwd_iter3d<mlc::final> fwd_iter_type;
    /*!< Image3d fwd_iter_type is fwd_iter3d<mlc::final>. */
    typedef bkd_iter3d<mlc::final> bkd_iter_type;
    /*!< Image3d bkd_iter_type is bkd_iter3d<mlc::final>. */
    typedef image3d_size size_type;
    /*!< Image3d size_type is image3d_size. */
  };


  namespace abstract {


    /*! \class image_with_dim<1, Exact>: virtual public image<Exact>
    **
    ** The specialized version for image1d.
    */
    
    template <class Exact>
    class image_with_dim<1, Exact>: virtual public image<Exact>
    {
    public:
      typedef typename image_traits<Exact>::point_type point_type; 
      /*!< Prefer the macro oln_point_type(I) to retrieve the point_type
      ** of an image.
      **
      ** \see oln::point1d
      */
      typedef typename image_traits<Exact>::point_type dpoint_type; 
      /*!< Prefer the macro oln_dpoint_type(I) to retrieve the dpoint_type
      ** of an image.
      **
      ** \see oln::dpoint1d
      */
      typedef typename image_traits<Exact>::iter_type iter_type; 
      /*!< Prefer the macro oln_iter_type(I) to retrieve the iter_type 
      ** of an image.
      **
      ** \see iter1d
      */
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type;
      /*!< Forward iterator type. */
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type; 
      /*!< Backward iterator type. */
      typedef typename image_traits<Exact>::value_type value_type; 
      /*!< Prefer the macro oln_value_type(I) to retrieve the value_type
      ** of an image.
      */
      typedef typename image_traits<Exact>::size_type size_type; 
      /*!< Indicate how the image size is handled.
      **
      ** \see oln::image1d_size
      */

      typedef image<Exact> super_type;
      typedef image_with_dim<1, Exact> self_type;
      typedef Exact exact_type;
   
      friend class image<exact_type>; 
      
      
      /// Return the number of columns in the current image.
      
      coord 
      ncols() const
      {
	return this->size().ncols();
      }

      /*! \brief Return the value stored at \a col coordinate
      ** in the current image.
      */
      
      const value_type 
      operator()(coord col) const
      {
	return this->exact()[point_type(col)];
      }

      /*! \brief Return a reference to the value stored
      ** at \a col coordinate in the current image.
      */
      
      value_type& 
      operator()(coord col)
      {
	return this->exact()[point_type(col)];
      }

      using super_type::hold;

      /*! \brief Test if a point belongs to the current image.
      **
      ** \arg col Column coordinate of the point.
      **
      ** \return True if the point belongs to the image, false otherwise.
      */
      
      bool 
      hold(coord col) const
      {
	return hold(point_type(col));
      }
      
      /*! \brief Perform a shallow copy from \a rhs to
      ** the current image, the points are not duplicated
      ** but shared between the two images
      **
      ** \see image::clone()
      */

      exact_type& 
      operator=(self_type rhs)
      {
	return this->exact().assign(rhs.exact());
      }

      static std::string 
      name()
      {
	return
	  std::string("abstract::image_with_dim<")
	  + self_type::Dim + ","
	  + Exact::name() + ">";
      }

    protected:

      /// Return the total number of points in the current image.
      size_t 
      npoints_() const
      {
	return size_t(ncols());
      }

      image_with_dim() : super_type()
      {}

    }; // end of one-dimensional specialization


    
    /*! \class image_with_dim<2, Exact>: virtual public image<Exact>
    **
    ** The specialized version for image2d.
    */
    

    template <class Exact>
    class image_with_dim<2, Exact>: virtual public image<Exact>
    {
    public:

      typedef typename image_traits<Exact>::point_type point_type;
      /*!< Prefer the macro oln_point_type(I) to retrieve the point_type
      ** of an image.
      **
      ** \see oln::point2d
      */
      typedef typename image_traits<Exact>::point_type dpoint_type; 
      /*!< Prefer the macro oln_dpoint_type(I) to retrieve the dpoint_type
      ** of an image.
      **
      ** \see oln::dpoint2d
      */
      typedef typename image_traits<Exact>::iter_type iter_type; 
      /*!< Prefer the macro oln_iter_type(I) to retrieve the iter_type
      ** of an image.
      **
      ** \see iter2d
      */
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type; 
      /*!< Forward iterator type. */
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type; 
      /*!< Backward iterator type. */
      typedef typename image_traits<Exact>::value_type value_type; 
      /*!< Prefer the macro oln_value_type(I) to retrieve the value_type
      ** of an image.
      */
      typedef typename image_traits<Exact>::size_type size_type; 
      /*!< Indicate how the image size is handled. 
      **
      ** \see oln::image2d_size
      */
      
      typedef image<Exact> super_type;   
      typedef image_with_dim<2, Exact> self_type;
      typedef Exact exact_type;

      friend class image<exact_type>;

          
      /// Return the number of rows in the current image.
      coord 
      nrows() const
      {
	return this->size().nrows();
      }

          
      /// Return the number of columns in the current image.

      coord 
      ncols() const
      {
	return this->size().ncols();
      }

      /*! \brief Return the value stored at \a row, \a col
      ** coordinates on the current image.
      */
      
      const value_type 
      operator()(coord row, coord col) const
      {
	return this->exact()[point_type(row, col)];
      }
      
      /*! \brief Return a reference to the value stored
      ** at \a row, \a col coordinates on the current image.
      */  

      value_type& 
      operator()(coord row, coord col)
      {
	return this->exact()[point_type(row, col)];
      }

      using super_type::hold;

      /*! \brief Test if a point belongs to the current image.
      **
      ** \arg row Row coordinate of the point. 
      **
      ** \arg col Column coordinate of the point.
      **
      ** \return True if the point belongs to the image, false otherwise.
      */
      
      bool 
      hold(coord row, coord col) const
      {
	return hold(point_type(row, col));
      }

      /*! \brief Perform a shallow copy from \a rhs
      ** to the current image, the points are not
      ** duplicated but shared between the two
      ** image.
      **
      ** \see image::clone()
      */
      
      exact_type& 
      operator=(self_type rhs)
      {
	return this->exact().assign(rhs.exact());
      }

      static std::string 
      name()
      {
	return
	  std::string("abstract::image_with_dim<")
	  + self_type::Dim + ","
	  + Exact::name() + ">";
      }

    protected:

      
      /// Return the total number of points in the current image.
      
      size_t 
      npoints_() const
      {
	return size_t(nrows()) * size_t(ncols());
      }

      image_with_dim() : super_type() 
      {}
 
    }; // end of bi-dimensional specialization
    
      
    /*! \class image_with_dim<3, Exact>: virtual public image<Exact>
    **
    ** The specialized version for image3d.
    */

    template <class Exact>
    class image_with_dim<3, Exact>: virtual public image<Exact>
    {
    public:
      typedef typename image_traits<Exact>::point_type point_type; 
      /*!< Prefer the macro oln_point_type(I) to retrieve the point_type
      ** of an image.
      **
      ** \see oln::point3d
      */
      typedef typename image_traits<Exact>::point_type dpoint_type; 
      /*!< Prefer the macro oln_dpoint_type(I) to retrieve the dpoint_type
      ** of an image.
      **
      ** \see oln::dpoint3d
      */
      typedef typename image_traits<Exact>::iter_type iter_type; 
      /*!< Prefer the macro oln_iter_type(I) to retrieve the iter_type
      ** of an image.
      **
      ** \see iter3d
      */
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type; 
      /*!< Forward iterator type. */
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type; 
      /*!< Backward iterator type. */
      typedef typename image_traits<Exact>::value_type value_type; 
      /*!< Prefer the macro oln_value_type(I) to retrieve the value_type
      ** of an image.
      */
      typedef typename image_traits<Exact>::size_type size_type; 
      /*!< Indicate how the image size is handled.
      **
      ** \see oln::image3d_size
      */
      
      typedef image<Exact> super_type; 
      typedef image_with_dim<3, Exact> self_type;
      typedef Exact exact_type;

      friend class image<exact_type>;

             
      
      /// Return the number of slices in the current image.
      
      coord 
      nslices() const
      {
	return this->size().nslices();
      }

             
      /// Return the number of rows in the current image.
      
      coord 
      nrows() const
      {
	return this->size().nrows();
      }
       
      /// Return the number of columns in the image.
      */
      
      coord 
      ncols() const
      {
	return this->size().ncols();
      }
      
      /*! \brief Return the value stored at \a slice, \a row
      ** and \a col coordinates on the current image.
      */
      
      const value_type 
      operator()(coord slice, coord row, coord col) const
      {
	return this->exact()[point_type(slice, row, col)];
      }
      
      /*! \brief Return a reference to the value stored 
      ** at \a slice, \a row and \a col coordinates on 
      ** the current image.
      */

      value_type& 
      operator()(coord slice, coord row, coord col)
      {
	return this->exact()[point_type(slice, row, col)];
      }

      using super_type::hold;

      /*! \brief Test if a point belongs to the current image.
      **
      ** \arg slice Slice coordinate of the point.
      **
      ** \arg row Row coordinate of the point.
      **
      ** \arg col Column coordinate of the point.
      **
      ** \return True if the point belongs to the image, false otherwise.
      */

      bool 
      hold(coord slice, coord row, coord col) const
      {
	return hold(point_type(slice, row, col));
      }

      /*! \brief Perform a shallow copy from \a rhs
      ** to the current image, the points are not
      ** duplicated but shared between the two 
      ** image.
      **
      ** \see image::clone()
      */
      
      exact_type& 
      operator=(self_type rhs)
      {
	return this->exact().assign(rhs.exact());
      }

      static std::string 
      name()
      {
	return
	  std::string("abstract::image_with_dim<")
	  + self_type::Dim + ","
	  + Exact::name() + ">";
      }

    protected:

          
      
      /// Return the total number of points in the current image.
      
      size_t 
      npoints_() const
      {
	return size_t(nslices()) * size_t(nrows()) * size_t(ncols());
      }

      
      image_with_dim() : super_type() 
      {}
 
    }; // end of tri-dimensional specialization


  } // end of namespace abstract
} // end of namespace oln


/// Print all the values contained in the image on an output stream.

template<class Exact> 
inline std::ostream&
operator<<(std::ostream& o, const oln::abstract::image_with_dim<1, Exact>& ima)
{
  if (ima.exact().impl() == 0)
    return o << "null";
  for (oln::coord col = 0; col < ima.ncols(); ++col)
    o << ima(col) << ' ';
  o << std::endl;
  return o;
}


/// Print all the values contained in the image on an output stream.

template<class Exact> 
inline std::ostream&
operator<<(std::ostream& o, const oln::abstract::image_with_dim<2, Exact>& ima)
{
  typedef typename oln::image_id<Exact>::value_type value_type;
  if (ima.exact().impl() == 0)
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

/// Print all the values contained in the image on an output stream.

template<class Exact> 
inline std::ostream&
operator<<(std::ostream& o, const oln::abstract::image_with_dim<3, Exact>& ima)
{
  typedef typename oln::image_id<Exact>::value_type value_type;

  if (ima.exact().impl() == 0)
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
