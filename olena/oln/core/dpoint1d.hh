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

#ifndef OLENA_CORE_DPOINT1D_HH
# define OLENA_CORE_DPOINT1D_HH

# include <oln/config/system.hh>
# include <ntg/core/predecls.hh>
# include <oln/core/coord.hh>
# include <oln/core/abstract/dpoint.hh>
# include <iostream>

namespace oln {

  // fwd decl
  class point1d;

  class dpoint1d; //fwd_decl

  /*! \class dpoint_traits<dpoint1d>
  **
  ** The specialized version for dpoint1d.
  */
  
  template <>
  struct dpoint_traits<dpoint1d>: public
  dpoint_traits<abstract::dpoint<dpoint1d> >
  {
    enum { dim = 1 };
    typedef point1d point_type;
  };

  /*! \class dpoint1d
  **
  ** Subclass of abstract::dpoint, declaration of dpoint
  ** for image1d. To instantiate a dpoint1d on an 
  ** oln::image1d<ntg::rgb_8> for example, use the
  ** macro oln_dpoint_type(I).\n
  ** oln_dpoint_type(oln::image1d<ntg::rgb_8>) p();\n
  ** or\n
  ** oln_dpoint_type(oln::image1d<ntg::rgb_8>) p(1);
  */

  class dpoint1d : public abstract::dpoint<dpoint1d>
  {

  public:

    typedef abstract::dpoint<dpoint1d> super_type;

    friend class abstract::dpoint<dpoint1d>;

    
    dpoint1d();

    /// The coordinate of the dpoint1d is set to \a c.

    dpoint1d(coord c);

    /// The coordinate of the dpoint1d is set to the \p coordinate.
        
    explicit dpoint1d(const point1d& p);

    /// Return the value of the dpoint1d coordinate.
    coord 
    col() const;
    
    /// Return a reference to the dpoint1d coordinate.
    
    coord& 
    col();

    static std::string 
    name() 
    { 
      return "dpoint1d"; 
    }

  protected:

    /*! \brief Return a dpoint1d whose coordinate is equal to 
    ** \a dp coordinate plus the current dpoint1d coordinate.
    **
    */
    
    dpoint1d 
    plus_dp(const dpoint1d& dp) const;
    
    /*! \brief Return a dpoint1d whose coordinate is equal to 
    ** the opposite of the current dpoint1d coordinate.
    */
    
    dpoint1d 
    minus() const;

    /*! \brief Return a dpoint1d whose coordinate is equal to
    ** the current dpoint1d coordinate minus 'dp' coordinate.
    */
    
    dpoint1d 
    minus_dp(const dpoint1d& dp) const;

    /*! \brief Return a reference to the current dpoint1d 
    ** plus 'dp'.
    */
    
    dpoint1d& 
    plus_assign_dp(const dpoint1d& dp);
    
    /*! \brief Return a reference to the current dpoint1d
    ** minus 'dp'.
    */
    
    dpoint1d& 
    minus_assign_dp(const dpoint1d& dp);

  };

    
  namespace internal
  {

    /*! \class default_less<dpoint1d>
    ** 
    ** The specialized version for dpoint1d.
    */

    template<>
    struct default_less<dpoint1d> :
      public default_less<dpoint1d::super_type>
    {
    };

  } // end of internal
} // end of oln


/// Write on an output stream \a o the coordinate of the dpoint1d \a dp.

inline std::ostream&
operator<<(std::ostream& o, const oln::dpoint1d& dp);

# include <oln/core/dpoint1d.hxx>

#endif // ! OLENA_CORE_DPOINT1D_HH
