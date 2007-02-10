// Copyright (C) 2004  EPITA Research and Development Laboratory
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


#ifndef OLENA_MORPHER_COLOR_MORPHER_HH
# define OLENA_MORPHER_COLOR_MORPHER_HH

# include <oln/morpher/generic_morpher.hh>

namespace oln {

  namespace morpher {

    template <class I, class Exact = mlc::final>
    class color_morpher;

  } // end of namespace morpher


  /*! \brief Retrieve types and dimension of the color_morpher.
  **
  ** \param I The type of the decorated image.
  **
  ** \param Exact The exact type of the object.
  */
  template <class I, class Exact>
  struct image_id<morpher::color_morpher<I, Exact> >
  {
    enum {dim = I::dim};
    /*! <The Image dimension.*/
    typedef oln_impl_type(I) impl_type;
    /*! <Underlying implementation.*/
    typedef ntg_comp_type(oln_value_type(I)) value_type;
    /*! <The value type of the decorated image. Here the
    ** component type of the source image value_type is
    ** retrieved.
    */
    typedef typename mlc::exact_vt<morpher::color_morpher<I, Exact>,
				   Exact>::ret exact_type;
    /*! <Retrieve the exact type of the image. It depends on
    ** the value of Exact.
    */

    typedef oln_point_type(I) point_type;
    typedef oln_iter_type(I) iter_type;
  };


  /*! \brief Specialized version for color_morpher.
  **
  ** \param SrcType The type of the decorated image.
  **
  ** \param Exact The exact type of the object.
  */
  template <class SrcType, class Exact>
  struct image_traits <morpher::color_morpher<SrcType, Exact> > :
    public image_traits<morpher::abstract::generic_morpher<SrcType,
							   typename image_id<morpher::color_morpher<SrcType, Exact> >::exact_type> >
  {

  };

  namespace morpher {

    /*! \brief Abstract color_morpher class used for
    ** code factorization.
    **
    ** \param SrcType Input type decorated.
    **
    ** \param Exact Exact type
    */
    template <class SrcType, class Exact>
    class super_color_morpher :
      public abstract::generic_morpher<SrcType, Exact>
    {
    protected:

      /*! The component to return.
      **
      ** If \a n is equal to 0, the red component is returned.
      ** If \a n is equal to 1, the green component is returned.
      ** If \a n is equal to 2, the blue component is returned.
      */
      unsigned n_;

      /*! Default constructor.
      **
      ** \a ima will be the decorated image.
      ** One can not use this constructor to instantiate this class
      ** since it is protected.
      */
      super_color_morpher(const SrcType &ima, unsigned n) : super_type(ima)
      {
	assert(n == ntg::rgb_R || n == ntg::rgb_G || n == ntg::rgb_B);
	n_ = n;
      }

      /*! Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      super_color_morpher()
      {}

    public:
      typedef abstract::generic_morpher<SrcType, Exact> super_type;
      typedef typename image_id<Exact>::impl_type impl_type;

      /// Return the number of the image component to retrieve.
      unsigned
      get_comp() const
      {
	return n_;
      }

      /// Return the image implementation.
      impl_type*
      impl()
      {
	return this->ima_.impl();
      }

      /// Return the image implementation.
      const impl_type*
      impl() const
      {
	return this->ima_.impl();
      }



      static std::string
      name()
      {
	return "super_color_morpher<" + super_type::name() + ">";
      }


    };

    /*! The default color_morpher class.
    **
    ** Using this class, an rgb image can be viewed
    ** according to one of its component.
    **
    ** \param SrcType Input Type decorated.
    **
    ** \param Exact Exact type.
    **
    ** \see oln::morpher::abstract::generic_morpher
    */
    template <class SrcType, class Exact>
    struct color_morpher :
      public super_color_morpher<SrcType,
				 typename image_id<color_morpher<SrcType, Exact> >::exact_type>
    {
      /// The type of the object instantiated. color_morpher can be derived.
      typedef typename image_id<color_morpher<SrcType, Exact> >::exact_type exact_type;


      typedef color_morpher<SrcType, Exact> self_type;
      typedef typename image_id<exact_type>::iter_type iter_type;
      /*! <Type of the class iterator.*/
      typedef typename image_id<exact_type>::point_type point_type;
      /*! <Type of the class point.*/
      typedef typename image_id<exact_type>::value_type value_type;
      /*! <The value type of the decorated image.*/
      typedef super_color_morpher<SrcType, exact_type> super_type;
      /*! <The upper class. */

      /// Construct the color_morpher with an image \a ima and a component \a n.
      color_morpher(const SrcType &ima, unsigned n) : super_type(ima, n)
      {}

      /// Construct the color_morpher with another color_morpher.
      color_morpher(const color_morpher<SrcType, Exact>& r) : super_type(r.get_ima(), r.get_comp())
      {}

      /*! Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      color_morpher()
      {}

      /*! Return a reference to the \a n_th component of the
      ** rgb value stored at \a p.
      **
      ** \warning This method should not be used directly.
      ** Prefer operator[].
      */
      value_type&
      at(const point_type& p)
      {
        return const_cast<SrcType &>(this->ima_)[p][this->n_];
      }

      /*! Return the \a n_th component of the rgb value stored at \a p.
      **
      ** \warning This method should not be used directly.
      ** Prefer operator[].
      */
      const value_type
      at(const point_type& p) const
      {
        return this->ima_[p][this->n_];
      }

      /*! Perform a shallow copy from the decorated image of \a rhs
      ** to the current decorated image. The points will be shared
      ** by the two images.
      */
      self_type&
      assign(self_type& rhs)
      {
        oln_iter_type(SrcType)	it(rhs);

        for_all(it)
          this->at(it) = rhs[it];
        return this->exact();
      }

      static std::string
      name()
      {
	return "color_morpher<" + SrcType::name() + "," + Exact::name() + ">";
      }

    };

    /*! \brief The specialized version for `const' declared images.
    **
    ** \param SrcType Input type decorated.
    **
    ** \param Exact Exact Type.
    **
    ** \see oln::morpher::abstract::generic_morpher
    */
    template <class SrcType, class Exact>
    struct color_morpher<const SrcType, Exact> :
      public super_color_morpher<const SrcType,
				 typename image_id<color_morpher<const SrcType,
								 Exact> >::exact_type>
    {
      /// The type of the object instantiated. color_morpher can be derived.
      typedef typename image_id<color_morpher<const SrcType, Exact> >::exact_type exact_type;

      typedef typename image_id<exact_type>::point_type point_type;
      /*! <The type of the class point.*/
      typedef typename image_id<exact_type>::iter_type iter_type;
      /*! <The type of the class iterator.*/
      typedef typename image_id<exact_type>::value_type value_type;
      /*! <The value of the decorated image.*/

      typedef super_color_morpher<const SrcType, exact_type> super_type;
      /*! <The upper class.*/

      /// Construct the color_morpher with an image \a ima and a component \a n.
      color_morpher(const SrcType &ima, unsigned n) : super_type(ima, n)
      {}

      /// Construct the color_morpher with another color_morpher.
      color_morpher(const color_morpher<const SrcType, Exact>& r) :
      super_type(r.get_ima(), r.get_comp())
      {}

      /*! Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      color_morpher()
      {}

      /*! Return the \a n_th component of the rgb value stored at \a p.
      **
      ** \warning This method should not be used directly.
      ** Prefer operator[].
      */
      const value_type
      at(const point_type& p) const
      {
	return this->ima_[p][this->n_];
      }

      static std::string
      name()
      {
	return "color_morpher<" + SrcType::name() + ">";
      }

    };


    /*! \brief Instantiate a temporary read-only
    ** color_morpher.
    **
    ** The image will be viewed according to its red layer.
    **
    ** \code
    ** #include <oln/morpher/color_morpher.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN  "lena.ppm");
    **   oln::save(oln::morpher::rmorph(imc),
    ** 	    IMG_OUT "oln_morpher_red_morpher.pgm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_morpher_red_morpher.png
    ** \image latex oln_morpher_red_morpher.png
    */
    template <class I>
    const color_morpher<I> rmorph(I &ima)
    {
      return color_morpher<I>(ima, ntg::rgb_R);
    }

    /*! \brief Instantiate a temporary read-only
    ** color_morpher.
    **
    ** The image will be viewed according to its green layer.
    **
    ** \code
    ** #include <oln/morpher/color_morpher.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN  "lena.ppm");
    **   oln::save(oln::morpher::gmorph(imc),
    ** 	    IMG_OUT "oln_morpher_green_morpher.pgm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_morpher_green_morpher.png
    ** \image latex oln_morpher_green_morpher.png
    */
    template <class I>
    const color_morpher<I> gmorph(I &ima)
    {
      return color_morpher<I>(ima, ntg::rgb_G);
    }

    /*! \brief Instantiate a temporary read-only
    ** color_morpher.
    **
    ** The image will be viewed according to its blue layer.
    **
    ** \code
    ** #include <oln/morpher/color_morpher.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN  "lena.ppm");
    **   oln::save(oln::morpher::bmorph(imc),
    ** 	    IMG_OUT "oln_morpher_blue_morpher.pgm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_morpher_blue_morpher.png
    ** \image latex oln_morpher_blue_morpher.png
    */
    template <class I>
    const color_morpher<I> bmorph(I &ima)
    {
      return color_morpher<I>(ima, ntg::rgb_B);
    }

  } // end of namespace morpher

} // end of namespace oln

#endif // !OLENA_MORPHER_COLOR_MORPHER_HH
