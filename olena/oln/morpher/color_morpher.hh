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


#ifndef COLOR_MORPHER_HH
# define COLOR_MORPHER_HH

# include <oln/morpher/generic_morpher.hh>

namespace oln {

  namespace morpher {

    template <class I, class Exact = mlc::final>
    class color_morpher;

  } // end of namespace morpher


  /// Retrieve types and dimension of the color_morpher.
  template <class I, class Exact>
  struct image_id<morpher::color_morpher<I, Exact> >
  {
    enum {dim = I::dim};
    typedef oln_impl_type(I) impl_type;
    typedef ntg_comp_type(oln_value_type(I)) value_type;
    typedef typename mlc::exact_vt<morpher::color_morpher<I, Exact>,
				   Exact>::ret exact_type;
  };

  /// Specialized version for color_morpher.
  template <class I>
  struct image_traits <morpher::color_morpher<I> > :
    public image_traits<abstract::image_with_impl<oln_impl_type(I),
						  morpher::color_morpher<I> > >
  {
  };

  namespace morpher {

    /// Abstract color_morpher class used for code factorization.
    template <class DestType, class SrcType, class Exact>
    class super_color_morpher : public abstract::generic_morpher<DestType, SrcType, Exact>
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
      typedef abstract::generic_morpher<DestType, SrcType, Exact> super_type;
      typedef oln_impl_type(SrcType) impl_type;

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
	return ima_.impl();
      }

      /// Return the image implementation.
      const impl_type*
      impl() const
      {
	return ima_.impl();
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
    ** \see oln::morpher::abstract::generic_morpher
    */
    template <class SrcType, class Exact>
    struct color_morpher :
      public super_color_morpher<typename oln::mute<oln_exact_type(SrcType),
						    ntg_comp_type(oln_value_type(SrcType))>::ret,
				 SrcType,
				 color_morpher<SrcType, Exact> >
    {
      /// The type of the object instantiated. color_morpher can be derived.
      typedef typename image_id<color_morpher<SrcType, Exact> >::exact_type exact_type;

      /// The image will be viewed as a Destype image.
      typedef typename oln::mute<oln_exact_type(SrcType),
	ntg_comp_type(oln_value_type(SrcType))>::ret DestType;

      typedef color_morpher<SrcType, Exact> self_type;
      typedef oln_iter_type(SrcType) iter_type;
      typedef oln_point_type(SrcType) point_type;
      typedef oln_fwd_iter_type(SrcType) fwd_iter_type;
      typedef oln_size_type(SrcType) size_type;
      typedef ntg_comp_type(oln_value_type(SrcType)) value_type;
      typedef oln_impl_type(SrcType) impl_type;
      typedef super_color_morpher<typename oln::mute<oln_exact_type(SrcType),
	ntg_comp_type(oln_value_type(SrcType))>::ret,
	SrcType,
	color_morpher<SrcType, Exact> > super_type;

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
      */
      value_type&
      at(const point_type& p)
      {
        return const_cast<SrcType &>(this->ima_)[p][this->n_];
      }

      /// Return the \a n_th component of the rgb value stored at \a p.
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

      /*! Perform a shallow from \a rhs to the current decorated
      ** image. The points will be shared by the two images.
      */
      self_type&
      assign(DestType& rhs)
      {
        oln_iter_type(SrcType)	it(rhs);

	for_all(it)
	  this->at(it) = rhs[it];
	return this->exact();
      }

      /*! This operator= assigns rhs to the current image.
      **
      ** \todo FIXME: This operator= doesn't work if declared in
      ** the generic_morpher.
      */
      self_type&
      operator=(DestType& rhs)
      {
	return this->exact().assign(rhs);
      }

      static std::string
      name()
      {
	return "color_morpher<" + SrcType::name() + ">";
      }

    };

    /// The specialized version for `const' declared images.
    template <class SrcType, class Exact>
    struct color_morpher<const SrcType, Exact> :
      public super_color_morpher<typename oln::mute<oln_exact_type(SrcType),
						    ntg_comp_type(oln_value_type(SrcType))>::ret,
				 SrcType,
				 color_morpher<const SrcType, Exact> >
    {
      /// The type of the object instantiated. color_morpher can be derived.
      typedef typename image_id<color_morpher<SrcType, Exact> >::exact_type exact_type;

      typedef oln_point_type(SrcType) point_type;
      typedef oln_fwd_iter_type(SrcType) fwd_iter_type;
      typedef oln_size_type(SrcType) size_type;
      typedef ntg_comp_type(oln_value_type(SrcType)) value_type;
      typedef oln_impl_type(SrcType) impl_type;

      typedef super_color_morpher<typename oln::mute<oln_exact_type(SrcType),
	ntg_comp_type(oln_value_type(SrcType))>::ret,
	SrcType,
	color_morpher<const SrcType, Exact> > super_type;

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

      /// Return the \a n_th component of the rgb value stored at \a p.
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

#endif // !COLOR_MORPHER_HH
