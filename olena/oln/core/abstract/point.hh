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

#ifndef OLENA_CORE_ABSTRACT_POINT_HH
# define OLENA_CORE_ABSTRACT_POINT_HH

# include <mlc/type.hh>
# include <ntg/utils/debug.hh>
# include <oln/core/coord.hh>

# include <iostream>
# include <sstream>

namespace oln
{
  namespace abstract {
    template<class Exact>
    struct point; // fwd declaration

    template<class Exact>
    struct dpoint; // fwd declaration
  } // end of abstract 

  template<class Exact>
  struct point_traits; // fwd declaration

  /*! \class point_traits<abstract::point<Exact> >
  **
  ** The specialized implementation for abstract::point.
  */
  
  template<class Exact>
  struct point_traits<abstract::point<Exact> >
  {

  };
  
  namespace abstract {
    
    /*! \class point
    **
    ** The abstract class from whom derives all the others
    ** point classes.
    */
    
    template<class Exact>
    struct point : public mlc_hierarchy::any<Exact>
    {

    public:
      
      typedef point<Exact> self_type;
      typedef Exact exact_type;      
      typedef typename point_traits<Exact>::dpoint_type dpoint_type; 

      enum { dim = point_traits<Exact>::dim };
      
      /// Return the current point casted in exact_type.

      const exact_type& 
      point_ref() const
      {
	return this->exact();
      }

      /// Give the value of the nth coordinate of the point.

      coord 
      nth(const unsigned dim) const
      {
	return coord_[dim];
      }
      
      /// Return a reference to the nth coordinate of the point.

      coord& 
      nth(const unsigned dim)
      {
	return coord_[dim];
      }

      /*! \brief Add a delta point \a dp to the current point
      **
      ** \return The current point plus \a dp.
      */
      
      exact_type& 
      operator+=(const abstract::dpoint<dpoint_type>& dp)
      {
	return this->exact().plus_assign_dp(dp.exact());
      }
      
      /*! \brief Subtract a delta point \a dp from the current point.
      **
      ** \return The current point minus \a dp.
      */ 
      
      
      exact_type& 
      operator-=(const abstract::dpoint<dpoint_type>& dp)
      {
	return this->exact().minus_assign_dp(dp.exact());
      }

      /*! \brief Subtract a point \a p from the current point.
      ** 
      ** \return A reference to this current point minus \a p.
      */
      
      dpoint_type 
      operator-(const self_type& p) const
      {
	return this->exact().minus_p(p.exact());
      }

      
      /*! \brief Give the result of the addition of
      ** a delta point \a dp and the current point
      **
      ** \return The result of the addition of \a dp and the current point.
      */
      
      exact_type 
      operator+(const abstract::dpoint<dpoint_type>& dp) const
      {
	return this->exact().plus_dp(dp.exact());
      }

      /*! \brief Give the result of the subtraction of
      ** a delta point \a dp and the current point.
      **
      ** \return The result of the subtraction of \a dp and the current point.
      */
      
      
      exact_type 
      operator-(const abstract::dpoint<dpoint_type>& dp) const
      {
	return this->exact().minus_dp(dp.exact());
      }

      /*! \brief Return a point whose coordinates are 
      ** the opposite of the current point coordinates
      */
      
      exact_type 
      operator-() const
      {
	return this->exact().minus();
      }

      
      /*! \brief Test if \a p and the current point have the same coordinates
      **
      ** \return True if \a p and the current point have the same coordinates,
      ** false otherwise.
      */

      bool 
      operator==(const self_type& p) const
      {
	for (unsigned i = 0; i < dim; ++i)
	  if (p.nth(i) != nth(i))
	    return false;
	return true;
      }
      
        
      /*! \brief Test if \a p and the current point do 
      ** not have the same coordinates.
      **
      ** \return False if \a p and the current point have
      ** the same coordinates, true otherwise.
      */
      
      bool 
      operator!=(const self_type& p) const
      {
	for (unsigned i = 0; i < dim; ++i)
	  if (p.nth(i) != nth(i))
	    return true;
	return false;
      }


      static std::string name() 
      { 
	return std::string("point<") + Exact::name() + ">"; 
      }

    protected:
      
      point() 
      {}

    private:

      /*! \brief The coordinates of the point are stored 
      ** in this array.
      */
      
      coord coord_[dim];

    };
    
  } // end of abstract

    
  namespace internal
  {
    
    /*! \class default_less< abstract::point<Exact> >
    **
    ** The specialized version for abstract::point.
    */
    
    template<class Exact>
    struct default_less< abstract::point<Exact> >
    {
      
      /*! \brief Test if the coordinates of a point l
      ** are not greater than the coordinates of a point r.
      ** 
      ** \arg l A point.
      **
      ** \arg r Another point.
      **
      ** \return True if the coordinates of l are not greater
      ** than the coordinates of r.
      */
      
      bool operator()(const abstract::point<Exact>& l,
		      const abstract::point<Exact>& r) const
      {
	for (unsigned i = 0; i < abstract::point<Exact>::dim; ++i)
	  if (l.nth(i) < r.nth(i))
	    return true;
	  else if (l.nth(i) > r.nth(i))
	    return false;
	return false;
      }
    };
    
  } // internal

} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_POINT_HH
