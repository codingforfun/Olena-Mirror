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

#ifndef BORDER_MORPHER_HH
# define BORDER_MORPHER_HH

# include <oln/morpher/generic_morpher.hh>
# include <oln/core/abstract/behavior.hh>
# include <oln/core/behavior.hh>

namespace oln {

  namespace morpher {

    template <class I, class BehaviorType, class Exact = mlc::final>
    struct border_morpher;
    template <class I, class BehaviorType, class Exact = mlc::final>
    struct super_border_morpher;

  } // end of namespace morpher

  /// Informations about the super border morpher.
  template <class SrcType, class BehaviorType, class Exact>
  struct image_id< morpher::super_border_morpher<SrcType, BehaviorType, Exact> >
  {
    typedef typename mlc::exact_vt<
      morpher::super_border_morpher<SrcType, BehaviorType, Exact>,
      Exact>::ret
    exact_type;
    ///< Retrieve the exact type of the image.
  };

  /// Informations about the border morpher.
  template <class SrcType, class BehaviorType, class Exact>
  struct image_id< morpher::border_morpher<SrcType, BehaviorType, Exact> >
  {
    enum {dim = SrcType::dim}; ///< The Image dimension.
    typedef oln_impl_type(SrcType) impl_type;
    ///< Underlying implementation.
    typedef oln_value_type(SrcType) value_type;
    ///< The value type of the decorated image.
    typedef typename mlc::exact_vt<morpher::border_morpher<SrcType, BehaviorType, Exact>,
				   Exact>::ret exact_type;
    ///< Retrieve the exact type of the image.
    typedef oln_point_type(SrcType) point_type;
    typedef oln_dpoint_type(SrcType) dpoint_type;
    typedef oln_size_type(SrcType) size_type;
    typedef oln_iter_type(SrcType) iter_type;
  };

  /// Traits for border morpher.
  template <class SrcType, class BehaviorType, class Exact>
  struct image_traits < morpher::border_morpher<SrcType, BehaviorType, Exact> >
    : public
  image_traits<
    morpher::abstract::generic_morpher<
      SrcType,
      typename image_id<morpher::border_morpher<SrcType,
						BehaviorType,
						Exact> >::exact_type
    >
  >
  {};


  namespace morpher {

    /// Abstract border morpher class used for code factorization.
    template <class SrcType, class BehaviorType, class Exact>
    class super_border_morpher
      : public abstract::generic_morpher<SrcType, Exact>
    {

    public:

      typedef super_border_morpher<SrcType, BehaviorType, Exact> self_type;
      typedef typename image_id<self_type>::exact_type exact_type;
      typedef abstract::generic_morpher<SrcType, Exact> super_type;

      typedef typename image_id<exact_type>::dpoint_type dpoint_type;
      typedef typename image_id<exact_type>::size_type size_type;

    protected:

      /*!
      ** \brief Default constructor.
      ** \arg ima will be the image.
      ** \arg width The width of the border you want to see.
      ** \arg be The behavior of the border.
      */
      super_border_morpher(const SrcType &ima,
			   const coord width,
			   const BehaviorType& be)
	: super_type(ima), width(width), be(be)
      {
	be.adapt_border(ima, width + get_ima().border());
	for (unsigned i = 0; i < image_id<exact_type>::dim; ++i)
	{
	  size_.nth(i) = get_ima().size().nth(i) + 2 * width;
	  dp_.nth(i) = -width;
	}
 	size_.border() = get_ima().size().border();
      }

      const coord width; ///< The width of the border.
      const BehaviorType be; ///< The behavior of the border.
      size_type size_;
      dpoint_type dp_;

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      super_border_morpher() : width(0)
      {}

    public:

      /// Return the size (different from the original picture).
      const size_type
      size() const
      {
	return size_;
      }

      /// Return the point (-width, -width, ...)
      const dpoint_type
      get_dp() const
      {
	return dp_;
      }

      /// Return the behavior of the border.
      const BehaviorType
      get_behavior() const
      {
	return be;
      }

      /// Return width, the width of the border.
      const coord
      get_width() const
      {
	return width;
      }

      /// Useful to debug.
      static std::string
      name()
      {
	return "super_border_morpher<" + super_type::name() + ">";
      }

    };

    /*!
    ** \brief The default border morpher class.
    **
    ** Using this class, a border of picture is a picture.
    **
    ** \see oln::morpher::abstract::generic_morpher
    ** \see oln::morpher::border_morph
    */
    template <class SrcType, class BehaviorType, class Exact>
    struct border_morpher
      : public super_border_morpher<
          SrcType,
          BehaviorType,
          typename image_id<border_morpher<SrcType, BehaviorType, Exact> >::exact_type
        >
    {
      typedef border_morpher<SrcType, BehaviorType, Exact> self_type;
      typedef typename image_id<self_type>::exact_type exact_type;
      typedef super_border_morpher<SrcType, BehaviorType, exact_type> super_type;

      typedef typename image_id<exact_type>::point_type point_type;
      typedef typename image_id<exact_type>::dpoint_type dpoint_type;
      typedef typename image_id<exact_type>::size_type size_type;
      typedef typename image_id<exact_type>::value_type value_type;

      /// Construct the border morpher with an image \a ima.
      border_morpher(const SrcType &ima,
		     const coord width,
		     const BehaviorType& be)
	: super_type(ima, width, be)
      {}

      /// Construct the border morpher with another border morpher.
      border_morpher(const self_type& r)
	: super_type(r.get_ima(), r.get_width(), r.get_behavior())
      {}

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      border_morpher()
      {}

      /*!
      ** \brief Return the stored value at the point.
      ** \arg p The point.
      ** \return The stored value.
      */
      value_type&
      at(const point_type& p)
      {
	return const_cast<value_type &>
	  ( const_cast<SrcType &>(this->ima_)[p + get_dp()] );
      }

      /*!
      ** \brief Return the stored value at the point.
      ** \arg p The point.
      ** \return The stored value.
      */
      const value_type
      at(const point_type& p) const
      {
	return this->ima_[p + get_dp()];
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

      /*!
      ** \brief This operator= assigns rhs to the current image.
      */
      self_type&
      operator=(SrcType& rhs)
      {
	return this->exact().assign(rhs);
      }

      /// Useful to debug.
      static std::string
      name()
      {
	return "border_morpher<" + super_type::name() + ">";
      }

    };

    /// The specialized version for `const' images.
    template <class SrcType, class BehaviorType, class Exact>
    struct border_morpher<const SrcType, BehaviorType, Exact>
      : public super_border_morpher<
          const SrcType,
          BehaviorType,
          typename image_id<border_morpher<const SrcType,
					   BehaviorType,
					   Exact> >::exact_type
        >
    {
      typedef border_morpher<const SrcType, BehaviorType, Exact> self_type;
      typedef typename image_id<self_type>::exact_type exact_type;
      typedef super_border_morpher<const SrcType, BehaviorType, exact_type> super_type;

      typedef typename image_id<exact_type>::point_type point_type;
      typedef typename image_id<exact_type>::dpoint_type dpoint_type;
      typedef typename image_id<exact_type>::size_type size_type;
      typedef typename image_id<exact_type>::value_type value_type;

      /*!
      ** \brief Construct a border morpher.
      ** \arg ima The image.
      ** \arg width Width
      */
      border_morpher(const SrcType &ima,
		     const coord width,
		     const BehaviorType& be)
	: super_type(ima, width, be)
      {}

      /// Construct a border morpher from another one.
      border_morpher(const self_type& r)
	: super_type(r.get_ima(), r.get_width(), r.get_behavior())
      {}

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      border_morpher() {}

      /*!
      ** \brief Return the stored value at the point.
      ** \arg p The point.
      ** \return The stored value.
      */
      const value_type
      at(const point_type &p) const
      {
	return this->ima_[p + get_dp()];
      }

      /// Useful to debug.
      static std::string
      name()
      {
	return "border_morpher<" + super_type::name() + ">";
      }

    };


    /*!
    ** \brief Instantiate a temporary read-only border morpher.
    ** \arg ima The image.
    ** \arg width The width of the border.
    ** \arg be The behavior of the border.
    **
    ** A border of the image will be viewed.
    **
    ** \code
    ** #include <oln/morpher/border_morpher.hh>
    ** #include <oln/basics2d.hh>
    ** int main()
    ** {
    **   oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN  "lena.ppm");
    **   assert(imc.has_impl());
    **   oln::save(oln::morpher::border_morph(imc, 100, oln::mirror_bhv()),
    **             IMG_OUT "oln_morpher_border_morpher.pgm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_morpher_border_morpher.png
    ** \image latex oln_morpher_border_morpher.png
    */
    template <class I, class BehaviorType>
    const border_morpher<I, BehaviorType>
    border_morph(I &ima,
		 const coord width,
		 const BehaviorType& be)
    {
      return border_morpher<I, BehaviorType>(ima, width, be);
    }


  } // end namespace morpher

} // end namespace oln

#endif // !BORDER_MORPHER
