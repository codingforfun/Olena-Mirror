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

#ifndef OLENA_CORE_DPOINT3D_HH
# define OLENA_CORE_DPOINT3D_HH

# include <oln/core/point3d.hh>

namespace oln {

  struct dpoint3d;

  template <>
  struct props <dpoint3d>
  {
    typedef dim3d dim_type;
  };

  struct dpoint3d : public abstract::dpoint< dpoint3d >
  {
    dpoint3d()
    {
      slice() = 0;
      row() = 0;
      col() = 0;
      this->exact_ptr = this;
    }

    dpoint3d(coord slice, coord row, coord col)
    {
      this->slice() = slice;
      this->row() = row;
      this->col() = col;
      this->exact_ptr = this;
    }

    dpoint3d(const dpoint3d& rhs)
    {
      this->slice() = rhs.slice();
      this->row() = rhs.row();
      this->col() = rhs.col();
      this->exact_ptr = this;
    }

    dpoint3d& operator=(const dpoint3d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->slice() = rhs.slice();
      this->row() = rhs.row();
      this->col() = rhs.col();
      return *this;
    }

    coord delta_impl() const
    {
      return std::max(std::abs(slice()),
		      std::max(std::abs(row()),
			       std::abs(col())));
    }

    bool op_eq_impl(const dpoint3d& rhs) const
    {
      return this->slice() == rhs.slice()
	&& this->row() == rhs.row()
	&& this->col() == rhs.col();
    }

    bool op_less_impl(const dpoint3d& rhs) const
    {
      return
	this->slice() < rhs.slice() || ((this->slice() == rhs.slice()) &&
	(this->row() < rhs.row() ||
	 (this->row() == rhs.row() && this->col() < rhs.col())));
    }

    const dpoint3d op_minus_impl() const
    {
      dpoint3d tmp(-slice(), -row(), -col());
      return tmp;
    }

    const coord slice() const { return this->data_[0]; }
    const coord row() const { return this->data_[1]; }
    const coord col() const { return this->data_[2]; }

    coord &slice() { return this->data_[0]; }
    coord &row() { return this->data_[1]; }
    coord &col() { return this->data_[2]; }
  };


  inline
  const point3d op_plus_impl(const point3d& p, const dpoint3d& dp)
  {
    point3d tmp(p.slice() + dp.slice(), p.row() + dp.row(), p.col() + dp.col());
    return tmp;
  }


} // end of namespace oln


std::ostream& operator<<(std::ostream& ostr, const oln::dpoint3d& p)
{
  return ostr << '(' << p.slice() << ',' << p.row() << ',' << p.col() << ')';
}


// # include <oln/core/coord.hh>
// # include <oln/core/abstract/dpoint.hh>
// # include <iostream>

// namespace oln {

//   // fwd decl
//   class point3d;
//   class dpoint3d;

//   /*! \class dpoint_traits<dpoint3d>
//   **
//   ** The specialized version for dpoint3d.
//   */

//   template<>
//   struct dpoint_traits<dpoint3d>: public
//   dpoint_traits<abstract::dpoint<dpoint3d> >
//   {
//     enum { dim = 3 };
//     typedef point3d point_type;
//   };

//   /*! \class dpoint3d
//   **
//   ** Subclass of abstract::dpoint, declaration of dpoint
//   ** for image3d. To instantiate a dpoint3d on an
//   ** oln::image3d<ntg::rgb_8> for example, use the
//   ** macro oln_dpoint_type(I).\n
//   ** oln_dpoint_type(oln::image3d<ntg::rgb_8>) dp();\n
//   ** or\n
//   ** oln_dpoint_type(oln::image3d<ntg::rgb_8>) dp(1, 2, 3);
//   */

//   class dpoint3d : public abstract::dpoint< dpoint3d >
//   {

//   public:

//     typedef abstract::dpoint< dpoint3d > super_type;

//     friend class abstract::dpoint< dpoint3d >;

//     dpoint3d();

//     /// The coordinates of the dpoint3d are set to \a slice, \a row, and \a col.

//     dpoint3d(coord slice, coord row, coord col);

//     /// The coordinates of the dpoint3d are set to the \a p coordinates.

//     explicit dpoint3d(const point3d& p);


//     /// Return the value of the dpoint3d slice coordinate.

//     coord
//     slice() const;

//     /// Return a reference to the dpoint3d slice coordinate.

//     coord&
//     slice();

//     /// Give the value of the dpoint3d row coordinate.

//     coord
//     row() const;

//     /*! \brief Return a reference to the dpoint3d row coordinate.
//     **
//     */

//     coord&
//     row();

//     /// Return the value of the dpoint3d column coordinate.

//     coord
//     col() const;

//     /// Return a reference to the dpoint3d column coordinate.

//     coord&
//     col();

//     static std::string
//     name()
//     {
//       return "dpoint3d";
//     }

//   protected:

//     /*! \brief Return a dpoint3d whose coordinates are equal to
//     ** \a dp coordinates plus the current dpoint3d coordinates.
//     **
//     */

//     dpoint3d
//     plus_dp(const dpoint3d& dp) const;

//     /*! \brief Return a dpoint3d whose coordinates are equal to
//     ** the opposite of the current dpoint3d coordinates.
//     */

//     dpoint3d
//     minus() const;

//     /*! \brief Return a dpoint3d whose coordinates are equal to
//     ** the current dpoint3d coordinates minus \a dp coordinates.
//     */

//     dpoint3d
//     minus_dp(const dpoint3d& dp) const;

//     /*! \brief Return a reference to the current dpoint3d
//     ** plus \a dp.
//     */

//     dpoint3d&
//     plus_assign_dp(const dpoint3d& dp);

//     /*! \brief Return a reference to the current dpoint3d
//     ** minus 'dp'.
//     */

//     dpoint3d&
//     minus_assign_dp(const dpoint3d& dp);

//   };
//   namespace internal
//   {
//     /*! \class default_less<dpoint3d>
//     **
//     ** The specialized version for dpoint3d.
//     */

//     template<>
//     struct default_less<dpoint3d> :
//       public default_less<dpoint3d::super_type>
//     {
//     };
//   } // end of internal

// } // end of oln

// /// Write on an output stream \a o the coordinates of the dpoint3d \a dp.

// inline std::ostream&
// operator<<(std::ostream& o, const oln::dpoint3d& dp);

// # include <oln/core/dpoint3d.hxx>


#endif // ! OLENA_CORE_DPOINT3D_HH
