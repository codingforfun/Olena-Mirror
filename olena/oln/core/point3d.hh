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

#ifndef OLENA_CORE_POINT3D_HH
# define OLENA_CORE_POINT3D_HH

# include <oln/core/abstract/point.hh>

namespace oln {

  struct point3d;

  template <>
  struct props <point3d>
  {
    typedef dim3d dim_type;
  };

  struct point3d : public abstract::point< point3d >
  {
    point3d()
    {
      slice() = 0;
      row() = 0;
      col() = 0;
      this->exact_ptr = this;
    }

    point3d(coord slice, coord row, coord col)
    {
      this->slice() = slice;
      this->row() = row;
      this->col() = col;
      this->exact_ptr = this;
    }

    point3d(const point3d& rhs)
    {
      this->slice() = rhs.slice();
      this->row() = rhs.row();
      this->col() = rhs.col();
      this->exact_ptr = this;
    }

    point3d& operator=(const point3d& rhs)
    {
      if (&rhs == this)
	return *this;
      this->slice() = rhs.slice();
      this->row() = rhs.row();
      this->col() = rhs.col();
      return *this;
    }

    bool op_eq_impl(const point3d& rhs) const
    {
      // FIXME: pb when abstract::point inherits from
      // any__best_memory
//       std::cout << this->row_ << ' '
// 		<< rhs.row_ << ' '
// 		<< this->col_ << ' '
// 		<< rhs.col_ << std::endl;
//       std::cout << this->row() << ' '
// 		<< rhs.row() << ' '
// 		<< this->col() << ' '
// 		<< rhs.col() << std::endl;
      return this->slice() == rhs.slice()
	&& this->row() == rhs.row()
	&& this->col() == rhs.col();
    }

    bool op_less_impl(const point3d& rhs) const
    {
      return
	this->slice() < rhs.slice() || ((this->slice() == rhs.slice()) &&
	(this->row() < rhs.row() ||
	 (this->row() == rhs.row() && this->col() < rhs.col())));
    }

    const coord slice() const { return this->data_[0]; }
    const coord row() const { return this->data_[1]; }
    const coord col() const { return this->data_[2]; }

    coord &slice() { return this->data_[0]; }
    coord &row() { return this->data_[1]; }
    coord &col() { return this->data_[2]; }
  };

} // end of namespace oln


// FIXME: I had to specialize ntg::props; it is strange that
// ntg intervenes!!!
namespace ntg {

  template <> struct props < oln::point3d > {
    enum { tag_value = ntg::vector_tag };
  };

} // end of namespace ntg


std::ostream& operator<<(std::ostream& ostr, const oln::point3d& p)
{
  return ostr << '(' << p.row() << ',' << p.col() << ')';
}


// # include <ntg/core/predecls.hh>
// # include <oln/core/coord.hh>
// # include <oln/core/point2d.hh>
// # include <iostream>


// namespace oln {


//   // fwd decl
//   class dpoint3d;
//   class point3d;

//   /*! \class point_traits<point3d>
//   **
//   ** The specialized version for point3d.
//   */

//   template<>
//   struct point_traits<point3d>: public point_traits<abstract::point<point3d> >
//   {
//     enum { dim = 3 };
//     typedef dpoint3d dpoint_type;
//   };

//   /*! \class point3d
//   **
//   ** Subclass of abstract::point, declaration of point
//   ** for image3d. To instantiate a point3d on an
//   ** oln::image3d<ngt::rgb_8> for example, use the
//   ** macro oln_point_type(I).\n
//   ** oln_point_type(oln::image3d<ngt::rgb_8>) p();\n
//   ** or\n
//   ** oln_point_type(oln::image3d<ngt::rgb_8>) p(1, 2, 3);
//   */

//   class point3d : public abstract::point< point3d >
//   {
//   public:

//     typedef abstract::point< point3d >	super_type;
//     typedef point_traits<point3d>::dpoint_type dpoint_type;

//     friend class abstract::point< point3d >;


//     point3d();

//     /// The coordinates of the point3d are set to \a slice, \a row, and \a col.

//     point3d(coord slice, coord row, coord col);

//     /// The coordinates of the point3d are set to \a p, and \a slice.

//     point3d(const point2d& p, coord slice);

//     /// Return the value of the point3d slice coordinate.

//     coord
//     slice() const;

//     /// Return a reference to the value of the point3d slice coordinate.

//     coord&
//     slice();

//     /// Return the value of the point3d row coordinate.

//     coord
//     row() const;

//     /// Return a reference to the point3d row coordinate.

//     coord&
//     row();

//     /// Return the value of the point3d col coordinate.

//     coord
//     col() const;

//     /// Return a reference to the point3d col coordinate.

//     coord&
//     col();

//     static std::string
//     name()
//     {
//       return "point3d";
//     }

//   protected:

//     /*! \brief Return a point3d whose coordinates are equal to
//     ** \a dp coordinates plus the current point3d coordinates.
//     */

//     point3d
//     plus_dp(const dpoint3d& dp) const;

//     /*! \brief Return a point3d whose coordinates are equal to
//     ** the current point3d coordinates minus \a dp coordinates.
//     */

//     point3d
//     minus_dp(const dpoint3d& dp) const;

//     /*! \brief Return a reference to the current point3d
//     ** plus \a dp.
//     */

//     point3d&
//     plus_assign_dp(const dpoint3d& dp);

//     /*! \brief Return a reference to the current point3d
//     ** minus \a dp.
//     */

//     point3d&
//     minus_assign_dp(const dpoint3d& dp);

//     /*! \brief Return a dpoint3d whose coordinates are equal
//     ** to the current point3d coordinates minus \a p coordinates.
//     */

//     dpoint3d
//     minus_p(const point3d& p) const;

//     /*! \brief Return a point3d whose coordinates are equal to
//     ** the opposite of the current point3d coordinates.
//     */

//     point3d
//     minus() const;

//   };

//   namespace internal
//   {

//     /*! \class default_less<point3d>
//     **
//     ** The specialized version for point3d.
//     */

//     template<>
//     struct default_less<point3d> : public default_less<point3d::super_type>
//     {
//     };

//   } // end of internal

// } // end of oln

// /// Write on an output stream \a o the coordinates of the point3d \a p.

// inline std::ostream&
// operator<<(std::ostream& o, const oln::point3d& p);

// # include <oln/core/point3d.hxx>

#endif // ! OLENA_CORE_POINT3D_HH
