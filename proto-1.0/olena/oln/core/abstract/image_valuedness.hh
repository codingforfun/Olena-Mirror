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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_VALUEDNESS_HH
# define OLENA_CORE_ABSTRACT_IMAGE_VALUEDNESS_HH

# include <ntg/decls.hh>
# include <oln/core/abstract/image.hh>


//                                            image
//
//                                              ^
//                                              |
//           -------------------------------------------------------------------------
//          |                    |                   |                  |             |
//
// scalar_valued_image  vector_valued_image  label_valued_image  data_valued_image   ...




// fwd decls
namespace oln {
  namespace abstract {
    template <typename E> struct scalar_valued_image;
    template <typename E> struct vector_valued_image;
    template <typename E> struct label_valued_image;
    template <typename E> struct data_valued_image;
  }
}
namespace std {
  template <class T, class A> class vector;
  template <class T> class complex;
}




/*! \namespace oln
** \brief oln namespace.
*/
namespace oln {


  /*! \namespace oln::valuedness
  ** \brief oln::valuedness namespace.
  */
  namespace valuedness {

    enum {
      scalar_tag = 1,
      vector_tag = 2,
      label_tag  = 3,
      data_tag   = 4
    };


    template <typename T>
    struct of
    {
    protected:

      static T* makeT();
      template <int tag> struct tag_is { char c[tag]; };

      // scalar

      template <typename E>
      static tag_is<scalar_tag>   selector(ntg::float_value<E>*);
      template <typename E>
      static tag_is<scalar_tag>   selector(ntg::int_value<E>*);

      static tag_is<scalar_tag>   selector(  signed int*);
      static tag_is<scalar_tag>   selector(unsigned int*);
      static tag_is<scalar_tag>   selector(  signed char*);
      static tag_is<scalar_tag>   selector(unsigned char*);
      static tag_is<scalar_tag>   selector(  signed short*);
      static tag_is<scalar_tag>   selector(unsigned short*);
      static tag_is<scalar_tag>   selector(  signed long*);
      static tag_is<scalar_tag>   selector(unsigned long*);
      static tag_is<scalar_tag>   selector(float*);
      static tag_is<scalar_tag>   selector(double*);

      // vector

      template <unsigned n, typename V>
      static tag_is<vector_tag>   selector(V (*)[n]);
      template <class V, class A>
      static tag_is<vector_tag>   selector(std::vector<V,A>*);
      template <typename E>
      static tag_is<vector_tag>   selector(ntg::vect_value<E>*);
      template <class V>
      static tag_is<vector_tag>   selector(std::complex<V>*);
      template <ntg::cplx_representation R, class V>
      static tag_is<vector_tag>   selector(ntg::cplx<R,V>*);

      // label

      template <typename E>
      static tag_is<label_tag>    selector(ntg::enum_value<E>*);
      static tag_is<label_tag>    selector(bool*);
      static tag_is<label_tag>    selector(ntg::bin*);
      template <class b>
      static tag_is<label_tag>    selector(ntg::int_u<1,b>*);
      template <class b>
      static tag_is<label_tag>    selector(ntg::int_s<1,b>*);

      // data

      static tag_is<data_tag>     selector(...); // default

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

    template <unsigned tag, typename E>
    struct inheritance_case;

    template <typename E> struct inheritance_case <scalar_tag, E> : public abstract::scalar_valued_image<E> {};
    template <typename E> struct inheritance_case <vector_tag, E> : public abstract::vector_valued_image<E> {};
    template <typename E> struct inheritance_case <label_tag, E> : public abstract::label_valued_image<E> {};
    template <typename E> struct inheritance_case <data_tag, E> : public abstract::data_valued_image<E> {};

    template <typename E>
    struct inheritance_switch : public inheritance_case < of< oln_type_of(E, value) >::val, E >
    {
    };


  } // end of namespace oln::valuedness




  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {


    /*! \class abstract::scalar_image<E>
    **
    ** Class of images containing scalar values.
    */

    template <typename E>
    struct scalar_valued_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      scalar_valued_image() {}
    };



    /*! \class abstract::vector_valued_image<E>
    **
    ** Class of images containing vectorial values.
    */

    template <typename E>
    struct vector_valued_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      vector_valued_image() {}
    };



    /*! \class abstract::label_valued_image<E>
    **
    ** Class of images containing label values.
    */

    template <typename E>
    struct label_valued_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      label_valued_image() {}
    };



    /*! \class abstract::data_valued_image<E>
    **
    ** Class of images containing values that cannot be qualified of scalars,
    ** vectors, or labels.
    */

    template <typename E>
    struct data_valued_image : public virtual image<E>
    {
    protected:
      /// Constructor (protected, empty).
      data_valued_image() {}
    };



  } // end of namespace oln::abstract


} // end of namespace oln



#endif // ! OLENA_CORE_ABSTRACT_IMAGE_VALUEDNESS_HH
