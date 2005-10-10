// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_HH
# define OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_HH

# include <ntg/decls.hh>
# include <oln/core/abstract/image.hh>


//                         image
//
//                           ^
//                           |
//         -------------------------------------------------
//        |              |            |           |         |
//
// greylevel_image  color_image  label_image  data_image   ...
//
//                                    ^
//                                    |
//
//                               binary_image



// fwd decls
namespace oln {
  namespace abstract {
    template <typename E> struct greylevel_image;
    template <typename E> struct color_image;
    template <typename E> struct label_image;
    template <typename E> struct binary_image;
    template <typename E> struct not_binary_image;
    template <typename E> struct data_image;
  }
}


/// Macro.
# define oln_typeness_of(T) typename typeness::tag_< typeness::of<T>::val >::ret



/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /*! \namespace oln::typeness
  ** \brief oln::typeness namespace.
  */
  namespace typeness {

    enum {
      _greylevel_tag = 1,
      _color_tag     = 2,
      _label_tag     = 3,
      _binary_tag    = 4,
      _data_tag      = 5
    };

    struct greylevel_tag;
    struct color_tag;
    struct label_tag;
    struct binary_tag;
    struct data_tag;

    template <int tag> struct tag_;
    template <> struct tag_ <_greylevel_tag> { typedef greylevel_tag ret; };
    template <> struct tag_ <_color_tag>     { typedef color_tag ret; };
    template <> struct tag_ <_label_tag>     { typedef label_tag ret; };
    template <> struct tag_ <_binary_tag>    { typedef binary_tag ret; };
    template <> struct tag_ <_data_tag>      { typedef data_tag ret; };

    template <typename T>
    struct of
    {
    protected:

      static T* makeT();
      template <int tag> struct tag_is { char c[tag]; };

      template <typename E>
      static tag_is<_color_tag>     selector(ntg::color_value<E>*);

      template <typename E>
      static tag_is<_label_tag>     selector(ntg::enum_value<E>*);

      template <typename E>
      static tag_is<_greylevel_tag> selector(ntg::real_value<E>*);

      static tag_is<_greylevel_tag> selector(char*);
      static tag_is<_greylevel_tag> selector(unsigned char*);

      static tag_is<_binary_tag>    selector(bool*);
      static tag_is<_binary_tag>    selector(ntg::bin*);
      template <class b>
      static tag_is<_binary_tag>    selector(ntg::int_u<1,b>*);
      template <class b>
      static tag_is<_binary_tag>    selector(ntg::int_s<1,b>*);

      static tag_is<_data_tag>      selector(...); // default

    public:

      enum { val = sizeof(selector(makeT())) };
    };


    // specializations:

    template <class T, class interval>
    struct of < ntg::cycle<T, interval> >
    {
      enum { val = of<T>::val };
    };

    template <class T, class interval, class behavior>
    struct of < ntg::range<T, interval, behavior> >
    {
      enum { val = of<T>::val };
    };


    // inheritance mechanism:

    template <typename tag, typename E>
    struct inheritance_case;

    // case: greylevel
    template <typename E>
    struct inheritance_case <greylevel_tag, E> :
      public abstract::greylevel_image<E>,
      public abstract::not_binary_image<E>
    {};
    // case: color
    template <typename E> struct inheritance_case <color_tag, E> :
      public abstract::color_image<E>,
      public abstract::not_binary_image<E>
    {};
    // case: label
    template <typename E> struct inheritance_case <label_tag, E> :
      public abstract::label_image<E>,
      public abstract::not_binary_image<E>
    {};
    // case: binary
    template <typename E> struct inheritance_case <binary_tag, E> :
      public abstract::binary_image<E>
    {};
    // case: data
    template <typename E> struct inheritance_case <data_tag, E> :
      public abstract::data_image<E>,
      public abstract::not_binary_image<E>
    {};

    // switch
    template <typename E>
    struct inheritance_switch :
      public inheritance_case < typename tag_< of<oln_type_of(E, value)>::val >::ret, E >
    {
    };


  } // end of namespace oln::typeness




  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {


    /*! \class abstract::greylevel_image<E>
    **
    ** Class of greylevel images.  An image that contains values whose
    ** type is 'int' or 'unsigned char', or 'ntg::int_u<8>' (...) is
    ** NOT a greylevel image.  FIXME: say more.
    */

    template <typename E>
    struct greylevel_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      greylevel_image() {}
    };



    /*! \class abstract::color_image<E>
    **
    ** Class of color images.
    */

    template <typename E>
    struct color_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      color_image() {}
    };



    /*! \class abstract::label_image<E>
    **
    ** Class of images whose data are labels.  Such images do not
    ** support arithmetics.
    */

    template <typename E>
    struct label_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      label_image() {}
    };



    /*! \class abstract::binary_image<E>
    **
    ** Class of images whose data are Boolean values.  Such images are
    ** abstract::label_image.
    */

    template <typename E>
    struct binary_image : public label_image<E>
    {
    protected:
      /// Constructor (protected, empty).
      binary_image() {}
    };

    /*! \class abstract::not_binary_image<E>
    **
    ** Class of images whose data are NOT Boolean values.  However
    ** such images can derive from abstract::label_image (we can have
    ** labels that are not binary ones).
    */

    template <typename E>
    struct not_binary_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      not_binary_image() {}
    };



    /*! \class abstract::data_image<E>
    **
    ** Class of images whose data cannot be qualified of greylevels,
    ** colors, or labels.
    */

    template <typename E>
    struct data_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      data_image() {}
    };



  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_TYPENESS_HH
