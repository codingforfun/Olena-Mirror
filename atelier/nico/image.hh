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

#ifndef IMAGE_HH_
# define IMAGE_HH_

# include <cassert>
# include <iostream>

# include "../local/scoop.hh"



# include "forward.hh"
# include "point.hh"
# include "plug.hh"
# include "impl_deleg.hh"
# include "array.hh"
# include "proxy.hh"
# include "tracked_ptr.hh"


namespace oln
{

  /////////////////////////////////////////////////////////////
  //all Grid class

  // usefull to determine grid type of a Image
  template < class Exact >
  struct Grid  {};

  struct grid1d : public Grid< grid1d > {};

  struct grid2d : public Grid< grid2d >{};


  //End Grid
  ////////////////////////////////////////////////////////////

  ////////////////////////////////////////////////
  // Image

  // Properties

  // Abstract world

# define current    Image<Exact>
# define super      any<Exact>
# define templ      template <typename Exact>
# define classname  Image

  /*!
  ** \class Image
  ** \brief top abstract class for image hierarchy
  **
  ** defines base methods and properties for an image
  ** methods:
  ** bool has_data() const      : return true if image has data
  ** box bbox() const           : return the box associated with the image
  ** operator box const() const : return the box associated with the image
  ** (internal method) bool owns_(const psite& p) const : return true if p belong to the image
  */
  template <typename Exact>
  struct Image : public virtual super, automatic::get_impl<Image, Exact>
  {
    stc_typename(point);	/*!< Image point type*/
    stc_typename(value);	/*!< Image value type*/
    stc_typename(box);		/*!< Image box type*/
    stc_typename(iter);		/*!< Image iterator type (depend on box type) */
    stc_typename(grid);		/*!< Image grid type*/
    stc_typename(data);		/*!< Image data type (structure that contains value)*/
    stc_typename(rvalue);	/*!< right value type of the image (type of right operande in affectation)*/
    stc_typename(psite);	/*!< Image psite type (is able to access to image value)*/

    //method declaration
    rvalue operator() (const psite& p) const
    {
      assert(owns_(p));
      return this->exact().impl_rvalue_access(p);
    }
    bool owns_(const psite& p) const { return this->exact().impl_owns(p); }
    bool has_data() const { return this->exact().impl_has_data(); }
    box bbox() const { return this->exact().impl_bbox(); }
    operator box const() const  { return this->exact().impl_bbox(); }
  };

# include "../local/undefs.hh"


# define current    Signal<Exact>1
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Signal

  /*!
  ** \class Signal
  ** \brief specialisation of Image class for images in one dimension
  */
  template <typename Exact>
  struct Signal : public virtual super, automatic::get_impl<Signal, Exact> {};

# include "../local/undefs.hh"

# define current    Image2d<Exact>
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Image2d

  /*!
  ** \class Image2d
  ** \brief Specialization of Image for images in two dimension
  */
  template <typename Exact>
  struct Image2d : public virtual super, automatic::get_impl<Image2d, Exact>
  {
    stc_typename(coord);	/*!< type of coordinate of point*/
  };

# include "../local/undefs.hh"


# define current    Mutable_Image<Exact>
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Mutable_Image


  /*!
  ** \class Mutable_Image
  ** \brief Specialization of Image for images which have values that can be modified
  **
  ** method:
  ** lvalue operator() (const psite& p) : return value associated to psite p
  */
  template <typename Exact>
  struct Mutable_Image : public virtual super, automatic::get_impl<Mutable_Image, Exact>
  {
    stc_using(psite);
    stc_typename(lvalue);	/*!< type of left operand in affectation*/

    lvalue operator() (const psite& p)
    {
      assert(owns_(p));
      return this->exact().impl_lvalue_access(p);
    }

  };

# include "../local/undefs.hh"

# define current    Point_Wise_Accessible_Image<Exact>
# define super      Image<Exact>
# define templ      template <typename Exact>
# define classname  Point_Wise_Accessible_Image

  /*!
  ** \class Point_Wise_Accessible_Image
  ** \brief Specialization of Image for point wise accessible image
  **
  ** method:
  ** bool has(const point& pt) : true if image has pt
  */
  template <typename Exact>
  struct Point_Wise_Accessible_Image : public virtual super, automatic::get_impl<Point_Wise_Accessible_Image, Exact>
  {
    stc_using(point);

    bool has(const point& pt) { return this->exact().impl_has(pt); }
  };

# include "../local/undefs.hh"
  // Concret World

# define current    image_base<Exact>
# define super      top <Exact>
# define templ      template <typename Exact>
# define classname  image_base

  stc_Header;
  typedef stc::is<Image> category;
  typedef stc::abstract point;
  typedef stc::abstract grid;
  typedef stc::abstract box;
  typedef stc::abstract data;
  stc_End;


  /*!
  ** \class image_base
  ** \brief top concret image class
  **
  */
  template <typename Exact>
  class image_base : public super
  {
  public:
    //properties
    stc_using(point);
    stc_using(value);
    stc_using(box);
    stc_using(iter);
    stc_using(grid);

    stc_lookup(data);

    //methods implementaion
    typedef data data_t;
    bool impl_has_data() const { return data_ != 0; }

  protected:
    image_base() { data_ = 0; }
    internal::tracked_ptr<data_t> data_; /*!< data of image (structure which contains value of the image) */
  };

# include "../local/undefs.hh"


# define current    primitive_image<Exact>
# define super      image_base <Exact>
# define templ      template <typename Exact>
# define classname  primitive_image

  stc_Header;
  stc_End;

  /*!
  ** \class primitive_image
  ** \brief simple image class
  **
  */
  template <typename Exact>
  class primitive_image : public super
  {
  protected:
    primitive_image() {}
  };

# include "../local/undefs.hh"

# define classname  image_morpher
# define current    image_morpher<Exact>
# define super      image_base <Exact>
# define templ      template <typename Exact>

  stc_Header;
  typedef stc::abstract delegatee;
  typedef stc::not_delegated data;
  stc_End;

  /*!
  ** \class image_morpher
  ** \brief morpher image class
  **
  */
  template <typename Exact>
  class classname : public super
  {
  public:
    stc_typename(delegatee);	/*!< delegatee type */
  protected:
    image_morpher() {}
  };

# include "../local/undefs.hh"



# define classname  single_image_morpher
# define current    single_image_morpher<Exact>
# define super      image_morpher<Exact>
# define templ      template <typename Exact>

  stc_Header;
  stc_End;

  /*!
  ** \class single_image_morpher
  ** \brief has just one delegatee
  **
  */
  template <typename Exact>
  class single_image_morpher : public super
  {
  public:
    stc_using(delegatee);

    delegatee& image() { assert(this->has_data());
      return this->exact().impl_image(); }
    delegatee image() const { assert(this->has_data());
    return this->exact().impl_image(); }
  };

# include "../local/undefs.hh"


# define classname  image_extansion
# define current    image_extansion<Exact>
# define super      single_image_morpher<Exact>
# define templ      template <typename Exact>

  stc_Header;
  typedef behavior::identity behavior;
  stc_End;

  /*!
  ** \class image_extansion
  ** \brief extension of simple image
  **
  */
  template <typename Exact>
  struct image_extansion : public super
  {
    stc_typename(behavior);
  };

# include "../local/undefs.hh"




# define classname  multiple_image_morpher
# define current    multiple_image_morpher<Exact>
# define super      image_morpher<Exact>
# define templ      template <typename Exact>

  stc_Header;
  stc_End;

  /*!
  ** \class multiple_image_morpher
  ** \brief has several delegatees
  **
  */
  template <typename Exact>
  class multiple_image_morpher : public super
  {
  public:
    stc_using(delegatee);

    delegatee& image(unsigned i) { assert(/*this->has_data() &&*/ i < n);
    return this->exact().impl_image(i); }
    delegatee image(unsigned i) const {assert(/*this->has_data() &&*/ i < n);
    return this->exact().impl_image(i); }
  protected:
    unsigned n;
  };

# include "../local/undefs.hh"


# define classname  images_extansion
# define current    images_extansion<Exact>
# define super      multiple_image_morpher<Exact>
# define templ      template <typename Exact>

  stc_Header;
  typedef behavior::mult_identity behavior;
  stc_End;

  /*!
  ** \class image_extansions
  ** \brief extension of simples images
  **
  */
  template <typename Exact>
  struct images_extansion : public super
  {
    stc_typename(behavior);
  };

# include "../local/undefs.hh"

# define classname  polite_image
# define current    polite_image<I>
# define super      image_extansion<current>
# define templ      template <typename I>

  stc_Header;
  typedef I delegatee;
  typedef singleton<I> data;
  stc_End;

  /*!
  ** \class polite_image
  **
  ** void talk() : say Nice
  */
  template <typename I>
  class polite_image : public super
  {
  public:
    stc_using(delegatee);
    stc_using(data);

    polite_image(I& ima) { this->data_ = new data (ima); }

    void talk() const { std::cout << "Nice" << std::endl; }

    delegatee& impl_image() { return this->data_->value_; }
    delegatee impl_image() const { return this->data_->value_; }
  };

# include "../local/undefs.hh"

# define classname  subset_image_1
# define current    subset_image_1<I, F>
# define super      image_extansion<current>
# define templ      template <typename I, typename F>

  stc_Header;
  typedef I delegatee;
  typedef singleton<I> data;
  typedef subset_image_1_iterator_<I, F> iter;
  stc_End;


  /*!
  ** \class subset_image_1
  ** \brief use a function point to bool F to define a subset image of image I
  **
  */
  template <typename I, typename F>
  class subset_image_1 : public super
  {
  public:
    //properties
    stc_using(delegatee);
    stc_using(data);
    stc_using(psite);
    stc_using(lvalue);
    stc_using(rvalue);

    //ctors
    subset_image_1(I& ima, F fun) : fun_(fun) { this->data_ = new data (ima); }


    // method implementation
    rvalue impl_rvalue_access(const psite& p) const { assert(fun_(p));
    return this->exact().image()(p); }

    lvalue impl_rvalue_access(const psite& p) { assert(fun_(p));
    return this->exact().image()(p) ;}

    bool impl_has(const psite& p) const { return fun_(p) && this->exact().image().has(p) ;}
    bool impl_owns(const psite& p) const { return fun_(p) && this->exact().image().owns_(p) ;}

    delegatee& impl_image() { return this->data_->value_; }
    delegatee impl_image() const { return this->data_->value_; }
  protected:
    F fun_;			/*!< function fp2b that define a subset of delegatee*/
  };

# include "../local/undefs.hh"

# define classname  subset_image_2
# define current    subset_image_2<I, Ps>
# define super      image_extansion<current>
# define templ      template <typename I, typename Ps>

  stc_Header;
  typedef I delegatee;
  typedef singleton<I> data;
  typedef Ps box;
  typedef subset_image_2_iterator_<Image < subset_image_2<I, box> > >  iter;
  stc_End;


  /*!
  ** \class subset_image_2
  ** \brief define a subset image with a new point_set
  **
  */
  template <typename I, typename Ps>
  class subset_image_2 : public super
  {
  public:
    //properties
    stc_using(delegatee);
    stc_using(data);
    stc_using(psite);
    stc_using(lvalue);
    stc_using(rvalue);
    stc_using(box);

    //ctor
    subset_image_2(I& ima, Ps ps) : ps_(ps) { this->data_ = new data (ima); }


    //method implemention
    rvalue impl_rvalue_access(const psite& p) const { assert(ps_.includes(p));
    return this->exact().image()(p); }

    lvalue impl_rvalue_access(const psite& p) { assert(ps_.includes(p));
    return this->exact().image()(p) ;}

    bool impl_has(const psite& p) const { return ps_.includes(p) && this->exact().image().has(p) ;}
    bool impl_owns(const psite& p) const { return ps_.includes(p) ;}
    Ps impl_bbox() const { return ps_;}

    delegatee& impl_image() { return this->data_->value_; }
    delegatee impl_image() const { return this->data_->value_; }
  protected:
    Ps ps_;			/*!< new point set */
  };

# include "../local/undefs.hh"


  /*!
  ** redefinition of operator |
  **
  ** @param ima image
  ** @param fun fp2b function
  **
  ** @return subset_image ima with fun
  */
  template <typename I, typename F>
  subset_image_1<I, F> operator| (I& ima, F& fun)
  {
    subset_image_1<I, F> masked(ima, fun);
    return (masked);
  }



# define classname  value_cast_image
# define current    value_cast_image<T, I>
# define super      image_extansion<current>
# define templ      template <typename T, typename I>

  stc_Header;
  typedef T rvalue;
  typedef T value;
  typedef stc::not_delegated lvalue;
  typedef I delegatee;
  typedef singleton<I> data;
  stc_End;

  template <typename T, typename I>
  class value_cast_image : public super
  {
  public:
    stc_using(delegatee);
    stc_using(data);
    stc_using(rvalue);
    stc_using(psite);

    value_cast_image(I& ima) { this->data_ = new data (ima); }

   //  value_cast_image<T, I> value_cast(const Image<I>& input)
//     {
//       value_cast< T, I> tmp(input.exact());
//       return tmp;
//     }

    rvalue impl_rvalue_access(const psite& p) const { return this->data_->value_(p); }
    delegatee& impl_image() { return this->data_->value_; }
    delegatee impl_image() const { return this->data_->value_; }
//     protected:
//     delegatee& delegatee_;
  };




# include "../local/undefs.hh"

# define classname  image_stack
# define current    image_stack<n, I>
# define super      images_extansion<current>
# define templ      template <unsigned n, typename I>

  stc_Header;
  typedef vec<n, I> data;
  typedef I delegatee;
  typedef vec<n, typename I::value> value;
  typedef const value rvalue;
  typedef value_proxy_< image_stack<n, I> > lvalue;
  stc_End;

  /*!
  ** \class image_stack
  ** \brief make a stack of different image
  **
  ** n is number of images into the stack
  ** I is the type of image belong to the stack
  ** Note: this class use a value proxy to differentiate read and write operation
  */
  template <unsigned n, typename I>
  class image_stack : public super
  {
  public:
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(value);
    stc_using(delegatee);
    stc_using(psite);
    stc_using(data);

    //Ctor
    image_stack() { this->data_ = 0;
    this->n = n; }

    //Methods
    lvalue impl_lvalue_access(const psite& p)
    {
      value_proxy_< image_stack<n, I> > tmp (*this, p);
      return tmp;
    }
    rvalue impl_rvalue_access(const psite& p) const { return this->read_(p); }
    delegatee& impl_image(unsigned i) { return this->delegatee_[i]; }
    delegatee impl_image(unsigned i) const { return this->delegatee_[i]; }

    //Internal methods
    rvalue read_(const psite& p) const
    {
      value v;

      for (unsigned i = 0; i < n; ++i)
	v[i] = this->delegatee_[i](p);
      return v;
    }

    lvalue write_(const psite& p)
    {
      vec<n, typename I::value&>& v;

      for (unsigned i = 0; i < n; ++i)
	v[i] = this->delegatee_[i](p);
      return v;
    }
  protected:
    vec<n, I> delegatee_;	/*!< images vector which represent stack*/
  };

# include "../local/undefs.hh"

# define current    image2d<T>
# define super      primitive_image<current>
# define templ      template <typename T>
# define classname  image2d

  stc_Header;
  typedef point2d point;
  typedef point::coord coord;
  typedef point psite;
  typedef array2d<T, coord>  data;
  typedef T value;
  typedef const value& rvalue;
  typedef value& lvalue;
  typedef grid2d grid;
  typedef box2d box;
  typedef box::iter iter;
  stc_End;


  /*!
  ** \class image2d
  ** \brief Simple image 2d
  **
  */
  template <typename T>
  class image2d : public super
  {
  public:
    stc_using(point);
    stc_using(value);
    stc_using(box);
    stc_using(iter);
    stc_using(grid);
    stc_using(coord);
    stc_using(data);
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(psite);

    image2d() {}
    image2d(box &box_ref) : bb_(box_ref) {
      this->data_ = new data (box_ref.pmin.row_, box_ref.pmin.col_, box_ref.pmax.row_, box_ref.pmax.col_); }

    bool impl_owns(const psite& p) const { return  bb_.includes(p); }
    bool impl_has(const point& p) { return  bb_.includes(p); }
    lvalue impl_lvalue_access(const psite& p) { return (this->data_.ptr_)->operator()(p.row_, p.col_); }
    rvalue impl_rvalue_access(const psite& p) const  { return (this->data_.ptr_)->operator()(p.row_, p.col_); }
    box impl_bbox() const {return bb_;}
  protected:
    box bb_;			/*!< box associated with the image2d*/
  };

# include "../local/undefs.hh"


  //break, don't use this class
# define current    signal<T>
# define super      primitive_image<current>
# define templ      template <typename T>
# define classname  signal

  stc_Header;
  typedef T value;
  typedef const value& rvalue;
  typedef value& lvalue;
  typedef iter1d iter;
  typedef T*  data;
  typedef point1d point;
  typedef point psite;
  typedef point::coord coord;
  typedef grid1d grid;
  typedef box1d box;
  stc_End;


  /*!
  ** \class signal
  ** \brief simple image in one dimension
  **
  */
  template <typename T>
  class signal : public super
  {
  public:
    stc_using(point);
    stc_using(value);
    stc_using(box);
    stc_using(iter);
    stc_using(grid);
    stc_using(psite);

    signal(box &box_ref) : bb_(box_ref) { }

    bool imp_owns(const psite& p) const  { return  bb_.includes(p); }
    //  value imp_value_acces(const point& p) { return thisdata_[p.col]; }
    box impl_bbox() const {return bb_;}
  protected:
    box &bb_;
  };

# include "../local/undefs.hh"


  /*!
  ** print point and value of a image
  **
  ** @param ima image to print
  */
  template <typename I>
  void print(I& ima)
  {
    typename I::iter it (ima);

    for (it.start(); it.is_valid(); it.next())
    {
      std::cout << (typename I::point) it << std::endl;
  //     std::cout << ima.operator()(it) << std::endl;
      //ima.affect(it) = 5;
      //std::cout << (typename I::rvalue) ima(it) << std::endl;
      std::cout << ima(it) << std::endl;
    }
  }

}

#endif /* !IMAGE_HH_ */
