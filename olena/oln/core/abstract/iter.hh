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

#ifndef OLENA_CORE_ABSTRACT_ITER_HH
# define OLENA_CORE_ABSTRACT_ITER_HH

# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/dpoint.hh>
# include <mlc/type.hh>
# include <mlc/objs.hh>
# include <oln/core/macros.hh>

namespace oln {

  /*! \brief Abstract class implementation.
  */
  namespace abstract {
    template<class Exact>
    struct iter; // forward declaration
  } // end of abstract

  template<class Exact>
  struct iter_traits;

  /*!
  ** \brief Traits for abstract::iter.
  */
  template<class Exact>
  struct iter_traits<abstract::iter<Exact> >
  {

  };

  // shortcuts
  using mlc::begin;
  using mlc::end;

  namespace abstract
  {
    /*!
    ** \brief Iterator.
    **
    ** Allow iterable object (like image, window, ...) traversing.
    ** \warning To know the type of iterator you need for an iterable object,
    ** use the macro oln_iter_type(Iterable) or oln_iter_type_(Iterable) (same
    ** without 'typename' keyword) rather than Iterable::iter_type.
    **
    ** Simple use of iterators:
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** #include <iostream>
    ** #include <assert.h>
    ** using namespace oln;
    ** using namespace ntg;
    ** int main(int argc, char **argv)
    ** {
    **   image2d<bin> image1 = load(IMG_IN "se.pbm");
    **   assert(image1.has_impl());
    **   oln_iter_type_(image2d<bin>) i(image1);
    **   for_all(i)
    **   {
    **     std::cout << image1[i] << std::endl;
    **   }
    ** }
    ** \endcode
    **
    ** This code is equivalent to the previous one but DEPRECATED (prefer to
    ** use the for_all macro):
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** #include <iostream>
    ** #include <assert.h>
    ** using namespace oln;
    ** using namespace ntg;
    ** int main(int argc, char **argv)
    ** {
    **   image2d<bin> image1 = load(IMG_IN "se.pbm");
    **   assert(image1.has_impl());
    **   for (int row = 0; row < image1.nrows(); ++row)
    **     for (int col = 0; col < image1.ncols(); ++col)
    **       std::cout << image1(row, col) << std::endl;
    ** }
    ** \endcode
    **
    **
    ** You can use the same iterator on several image if they have the same size.
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** #include <iostream>
    ** #include <assert.h>
    ** using namespace oln;
    ** using namespace ntg;
    ** int main(int argc, char **argv)
    ** {
    **   image2d<bin> image1 = load(IMG_IN "se.pbm");
    **   image2d<bin> image2 = load(IMG_IN "se.pbm");
    **   assert(image1.has_impl());
    **   assert(image2.has_impl());
    **   oln_iter_type_(image2d<bin>) i(image1);
    **   for_all(i)
    **   {
    **     std::cout << "image1:" << image1[i] << std::endl;
    **     std::cout << "image2:" << image2[i] << std::endl;
    **   }
    **   return 0;
    ** }
    ** \endcode
    **
    ** You can iterate not only image but windows. This example make a binary
    ** dilatation to show you how to use them:
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** #include <iostream>
    ** #include <assert.h>
    ** using namespace oln;
    ** using namespace ntg;
    ** int main(int argc, char **argv)
    ** {
    **   image2d<bin> image1 = load(IMG_IN "object.pbm");
    **   assert(image1.has_impl());
    **   image2d<bin> image1_out(image1.size());
    **   oln_iter_type_(image2d<bin>) i(image1);
    **   for_all(i)
    **   {
    **     image1_out[i] = image1[i];
    **     if (!image1[i])
    **     {
    **       window2d win = win_c8_only();
    **       oln_iter_type_(window2d) j(win);
    **       bool change_color = false;
    **       for_all(j)
    **       {
    **         if (image1[i + j])
    **           change_color = true;
    **       }
    **       image1_out[i] = change_color;
    **     }
    **   }
    **   save(image1_out, IMG_OUT "oln_abstract_iter.pbm");
    **   return 0;
    ** }
    ** \endcode
    ** \image html object_pbm.png
    ** \image latex object_pbm.png
    ** =>
    ** \image html oln_abstract_iter.png
    ** \image latex oln_abstract_iter.png
    **
    */
    template<class Exact>
    struct iter : public mlc_hierarchy::any<Exact>
    {
      /*!
      ** \brief The associate image's type of point.
      ** \warning Prefer the macros oln_point_type(Pointable) and
      ** oln_point_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename iter_traits<Exact>::point_type point_type;
      /*!
      ** \brief The associate image's type of dpoint (move point).
      ** \warning Prefer the macros oln_dpoint_type(Pointable) and
      ** oln_dpoint_type_(Pointable) (the same without the 'typename' keyword)
      */
      typedef typename iter_traits<Exact>::dpoint_type dpoint_type;

      /*!
      ** \brief Accessor to current iterator's point.
      **
      ** Just return the current point of the iterator which
      ** is traversing an image.
      */
      const point_type&
      point_ref() const
      {
	return p_;
      }

      /*!
      ** \brief Compare with the current iterator's point.
      ** \arg p The iterator's point to be compared to the current point.
      **
      ** Compare the current iterator's point with p (his argument). If
      ** successful, return true.
      */
      bool
      operator==(const abstract::point<point_type>& p) const
      {
	return p_ == p.exact();
      }

      /*!
      ** \brief Compare with the current iterator's point.
      ** \arg p The iterator's point to be compared to the current point.
      **
      ** Compare the current iterator's point with p (his argument). If
      ** they are different, return true.
      */
      bool
      operator!=(const abstract::point<point_type>& p) const
      {
	return p_ != p.exact();
      }

      /*!
      ** \brief Sum a move point to the current point.
      ** \arg p The move point.
      ** \return The sum.
      ** \pre Instance != end.
      **
      ** Sum the current iterator's point and the move coordinates.
      */
      point_type
      operator+(const abstract::dpoint<dpoint_type>& dp) const
      {
	precondition(*this != end);
	return p_ + dp.exact();
      }

      /*!
      ** \brief Minor a move to the current point.
      ** \arg p The move.
      ** \return The minoration.
      ** \pre Instance != end.
      **
      ** Minor the current point and the move coordinates.
      */
      point_type
      operator-(const abstract::dpoint<dpoint_type>& dp) const
      {
	precondition(*this != end);
	return p_ - dp.exact();
      }

      /*!
      ** \brief Cast to exact point type.
      ** \return The exact point type.
      **
      ** Return the exact point type by calling sub-classes methods.
      */
      operator point_type() const
      {
	return this->exact().to_point();
      }

      /*!
      ** \brief Syntax improvement
      **
      ** It's convenient to type `it.cur()' instead of `(point)it' when
      ** necessary.
      */
      point_type
      cur() const
      {
	return *this;
      }

      // deferred methods are:
      //
      //   void _goto_begin();
      //   void _goto_end();
      //   bool _is_at_end() const;
      //   void _goto_next();

      /*!
      ** \brief Set current point to the first iterator's point.
      **
      ** Set current point of iterator to the first iterator's point.
      */
      mlc::begin_type
      operator=(mlc::begin_type b)
      {
	this->exact().goto_begin_();
	return b;
      }

      /*!
      ** \brief Set current point to the last iterator's point.
      **
      ** Set current point of iterator to the last iterator's point.
      */
      mlc::end_type
      operator=(mlc::end_type e)
      {
	this->exact().goto_end_();
	return e;
      }

      /*!
      ** \brief Compare current point and last point.
      ** \return True if they are the same.
      **
      ** Compare current point with last iterator's point.
      */
      bool
      operator==(mlc::end_type) const
      {
	return this->exact().is_at_end_();
      }

      /*!
      ** \brief Go to the next iterator's point.
      ** \pre Instance != end.
      */
      void
      operator++()
      {
	precondition(*this != end);
	this->exact().goto_next_();
      }

//       typename mlc::exact<self>::ret operator++(int)
//       {
// 	precondition(*this != end);
// 	typename mlc::exact<self>::ret tmp = this->exact();
// 	this->operator++();
// 	return tmp;
//       }

      // deduced methods:

      /*!
      ** \brief Compare current point and last point.
      ** \return True if they are different.
      **
      ** Compare current point with last iterator's point.
      */
      bool
      operator!=(mlc::end_type e) const
      {
	return ! this->operator==(e);
      }

      /// Return the name of the type.
      static std::string
      name()
      {
	return std::string("iter<") +
	  Exact::name() + ">";
      }

    protected:
      point_type p_; ///< The iterator's current point.

      /*!
      ** \brief Constructor
      **
      ** Do nothing, used only by sub-classes
      */
      iter()
      {}
    };

  } // end of abstract

} // end of oln


#endif // ! OLENA_CORE_ABSTRACT_ITER_HH
