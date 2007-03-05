// Copyright (C) 2007 EPITA Research and Development Laboratory.
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef ITERATOR_HH_
# define ITERATOR_HH_

# include "../local/scoop.hh"

namespace oln
{

  // abstract world

  /*!
  ** \class Iterator
  ** \brief top abstract class for Iterator
  **
  ** declare base methods and properties for a Iterator
  ** methods:
  ** void start()      : initialize the Iterator
  ** void next()       : set iterator to the next state
  ** void invalidate() : set iterator to an invalid state
  ** bool is_valid()   : return true if the iterator is in a valid state
  **
  */
  template <typename Exact>
  struct Iterator : public any <Exact>
  {
    stc_typename(value);	/*!< value type of the iterator*/


    // base method
    void start() { this->exact().impl_start(); }
    void next() { this->exact().impl_next(); }
    void invalidate() { this->exact().impl_invalidate(); }
    bool is_valid() const { return this->exact().impl_is_valid(); }
  };

# include "../local/undefs.hh"

# define current   Iterator_on_Points<Exact>
# define super     Iterator <Exact>
# define templ      template <typename Exact>
# define classname  Iterator_on_Points

  /*!
  ** \class Iterator_on_Points
  ** \brief abstract class of iterator class which is specialized for iterate on a Point_set
  **
  ** methods:
  ** operator point() const         : cast an Iterator on point to the current point
  ** point to_point() const         : return the current point
  ** point const* point_adr() const : return the adr of the current point
  **
  */
  template <typename Exact>
  struct Iterator_on_Points : public super
  {
    stc_typename(point);	/*!< point type of the iterator*/

    typedef Iterator_on_Points<Exact> self;

    operator point() const { return this->exact().impl_cast(); }
    point to_point() const { return this->exact().impl_to_point(); }
    point const* point_adr() const { return this->exact().impl_point_adr(); }
    operator point const ()  { return this->exact().impl_to_point(); }
  };

# include "../local/undefs.hh"


  // Concret world


# define current    simple_iter<C>
# define super      top <current>
# define templ      template <typename C>
# define classname  simple_iter


  stc_Header;
  typedef stc::is<Iterator> category;
  typedef C value;
  typedef oln::point2d_<C> point;
  stc_End;

  /*!
  ** \class simple_iter
  ** \brief class for test,  nothing interessting.
  **
  */
  template < typename C >
  class simple_iter : public super
  {
  public:
    typedef stc_type(simple_iter<C>, point) point;

    simple_iter(const point& init) : i_(0)
    {
      for (int i = 0; i < 15; ++i)
	v_[i] = init;
    }

    void impl_start() { i_ = 0; }
    void impl_next() { ++i_; }
    void impl_invalidate() { i_ = 42; }
    bool impl_is_valid() const { return i_ < 15; }
    point impl_to_point() const { return (point) *this; }
    point const* point_adr() const { return v_; }

    operator point const()
    {
      if (i_ < 16)
	return v_[i_];
      return (v_[0]);
    }

  protected:
    int		  i_;
    point v_[15];
  };

# include "../local/undefs.hh"

  template <typename P>
  class box_;



# define current    box_iterator_<P>
# define super      top  <current>
# define templ      template <typename P>
# define classname  box_iterator_



  stc_Header;
  typedef stc::is<Iterator_on_Points> category; //box iterator is an Iterator_on_Points
  typedef P point;
  typedef P value;
  stc_End;


  template <typename P>
  class box_;

  /*!
  ** \class box_iterator_
  ** \brief concrete Iterator fox box_ class
  **
  ** P is the type of Iterator points
  */
  template < typename P >
  class box_iterator_ : public super
  {
  public:
    typedef P point;

    //ctor
    box_iterator_(const box_<point>& box) : bb_(box)
    {
      nop_ = box.pmax;
      ++nop_[0];
    }

    //method implementation
    void impl_start() { p_ = bb_.pmin; }
    void impl_next();
    void impl_invalidate() { p_ = nop_; }
    bool impl_is_valid() const { return p_ != nop_; }
    point impl_to_point() const { return p_; }
    point const* point_adr() const { return &p_; }

  protected:
    point p_;			/*!< current point*/
    box_<point> bb_;		/*!< box associated with the iterator*/
    point nop_;			/*!< point which is not into the box */
  };



  //// methode implementation

  /*!
  ** set the iterator to the next point
  **
  */
  template < typename P >
  void box_iterator_< P >::impl_next()
  {
    typedef typename box_iterator_<P>::point point;
    for (int i = point::n - 1; i >= 0; --i)
      if (p_[i] == bb_.pmax[i])
      	p_[i] = bb_.pmin[i];
      else
      {
	++p_[i];
	break;
      }

    if (p_ == bb_.pmin)
      p_ = nop_;
  }

  // sugar
  typedef box_iterator_< point2d_<int> > iter2d;
  typedef box_iterator_< point1d_<int> > iter1d;

# include "../local/undefs.hh"

# define classname subset_image_1_iterator_
# define current subset_image_1_iterator_<I, F>
# define super  top  <current>
# define templ template <typename I, typename F>

  stc_Header;
  typedef stc::is<Iterator_on_Points> category;
  typedef typename I::point point;
  typedef typename I::value value;
  stc_End;


  /*!
  ** \class subset_image_1_iterator_
  ** \brief concrete Iterator for subset_image_1 class
  **
  */
  template < typename I, typename F >
  class subset_image_1_iterator_ : public super
  {
  public:

    //properties
    stc_using(point);
    stc_using(value);
    typedef typename I::iter iter;
    typedef typename I::box  box;

    // base methods implementation
    void impl_invalidate() { p_.invalidate(); }
    bool impl_is_valid() const {  return p_.is_valid(); }
    point impl_to_point() const { return p_.to_point(); }
    point const* point_adr() const { return p_.point_adr(); }
    operator point const() { return p_.to_point(); }
    void impl_start()
    {
      p_.start();
      while (p_.is_valid() && !(f_(p_)))
	p_.next();
    }
    void impl_next()
    {
      do
	p_.next();
      while (p_.is_valid() && !(f_(p_)));
    }

    //ctor
    subset_image_1_iterator_(const box& bb) : p_(bb) {}

  protected:
    iter p_;			/*!< iterator of subset_image delegatee */
    F f_;			/*!< functions that define the subset_image domain */
  };

# include "../local/undefs.hh"

# define classname subset_image_2_iterator_
# define current subset_image_2_iterator_<I>
# define super  top  <current>
# define templ template <typename I>

  stc_Header;
  typedef stc::is<Iterator_on_Points> category;
  typedef typename I::point point;
  typedef typename I::value value;
  stc_End;



  /*!
  ** \class subset_image_2_iterator_
  ** \brief iterator for subset_image_2 class
  **
  */
  template < typename I >
  class subset_image_2_iterator_ : public super
  {
  public:
    // properties
    stc_using(value);
    stc_using(point);
    typedef typename I::box  box;
    typedef typename box::iter iter;

    //methods implementation
    void impl_invalidate() { p_.invalidate(); }
    bool impl_is_valid() const {  return p_.is_valid(); }
    point impl_to_point() const { return p_.to_point(); }
    point const* point_adr() const { return p_.point_adr(); }
    operator point const() { return p_.to_point(); }
    void impl_start()
    {
      p_.start();
      while (p_.is_valid() && !(ima_.owns_(p_)))
	p_.next();
    }
    void impl_next()
    {
      do
	p_.next();
      while (p_.is_valid() && !(ima_.owns_(p_)));
    }

    //ctor
    subset_image_2_iterator_(const I& ima) : ima_(ima), p_(ima.bbox()) {}

  protected:
    I ima_;			/*!< delegatee associated with the subset_image*/
    iter p_;			/*!< iterator on the subset_image domain */
  };

# include "../local/undefs.hh"


  template <typename P>
  class pset_std;

# define current    pset_std_iterator_<P>
# define super      top  <current>
# define templ      template <typename P>
# define classname  pset_std_iterator_

  stc_Header;
  typedef stc::is<Iterator> category;
  typedef P point;
  typedef P value;
  stc_End;

  /*!
  ** \class pset_std_iterator_
  ** \brief iterator on pset_std class
  **
  */
  template < typename P >
  class pset_std_iterator_ : public super
  {
  public:
    typedef stc_type(pset_std_iterator_, point) point;

    pset_std_iterator_(std::set<P> &box) : bb_(box) {}

    void impl_start() { iter = bb_.begin(); }
    void impl_next() { ++iter;  }
    void impl_invalidate() { iter = bb_.end(); }
    bool impl_is_valid() const { return *iter != bb_.end(); }
    point impl_to_point() const { return *iter; }
    point const* point_adr() const { return iter; }

  protected:
    typename std::set<P> &bb_;
    typename std::set< P >::iterator iter;
  };


# include "../local/undefs.hh"
}

#endif /* !ITERATOR_HH_ */
