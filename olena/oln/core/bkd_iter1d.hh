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

#ifndef OLENA_CORE_BKD_ITER1D_HH
# define OLENA_CORE_BKD_ITER1D_HH

# include <oln/core/abstract/iter1d.hh>

namespace oln {

  template<class Exact = mlc::final>
  class bkd_iter1d; // forward declaration

  /*!
  ** \brief Traits for bkd_iter1d.
  */
  template<class Exact>
  struct iter_traits< bkd_iter1d<Exact> >: public
  iter_traits<abstract::iter1d<typename
  mlc::exact_vt<bkd_iter1d<Exact>, Exact>::ret> >
  {
    typedef point1d point_type;
    typedef dpoint1d dpoint_type;
  };

  /*!
  ** \brief Backward Iterator of 1 dimension.
  **
  ** Allow iterable object (like image, window, ...) of 1 dimension backward
  ** traversing.
  ** \see iter
  */
  template<class Exact>
  class bkd_iter1d : public abstract::iter1d<typename mlc::exact_vt<bkd_iter1d<Exact>, Exact>::ret>
  {
  public:

    typedef typename mlc::exact_vt<bkd_iter1d<Exact>, Exact>::ret exact_type;
    ///< The exact type.

    typedef abstract::iter1d<exact_type> super_type; ///< The super type.
    typedef abstract::iter<exact_type> super_iter_type;
    ///< The super iterator type.


    enum { dim = iter_traits<exact_type>::dim }; ///< Dimension.
    /*!
    ** \brief The associate image's type of point.
    ** \warning Prefer the macros oln_point_type(Pointable) and
    ** oln_point_type_(Pointable) (the same without the 'typename' keyword)
    */
    typedef typename iter_traits<exact_type>::point_type point_type;

    friend class abstract::iter<exact_type>;
    friend class abstract::iter1d<exact_type>;

    /*!
    ** \brief Construct a backward iterator (1 dimension).
    ** \arg ima The image to iterate.
    */
    template<class Image>
    bkd_iter1d(const Image& ima) :
      super_type(ima.size())
    {}

    /*!
    ** \brief Set current iterator's point.
    **
    ** Set current point of iterator to the first iterator's point.
    */
    template<class U>
    U
    operator=(U u)
    {
      return super_iter_type::operator=(u);
    }

    /*!
    ** \brief Return its type in a string.
    ** \return The type in a string.
    **
    ** Very useful to debug.
    */
    static std::string
    name()
    {
      return std::string("bkd_iter1d<") + Exact::name() + ">";
    }

  protected:

    /*!
    ** \brief Set current point to the first iterator's point.
    **
    ** Set current point of iterator to the first iterator's point.
    */
    void
    goto_begin_()
    {
      this->p_.col() = this->ncols_ - 1;
    }

    /*!
    ** \brief Set current point to the last iterator's point.
    **
    ** Set current point of iterator to the last iterator's point.
    */
    void
    goto_end_()
    {
      this->p_.col() = -1;
    }

    /*!
    ** \brief Test if iterator's current point is the last one.
    ** \return True if current point is the last one.
    */
    bool
    is_at_end_() const
    {
      return this->p_.col() == -1;
    }

    /*!
    ** \brief Go to the next iterator's point.
    */
    void
    goto_next_()
    {
      --this->p_.col();
    }

  };

} // end of oln

#endif // ! OLENA_CORE_BKD_ITER1D_HH
