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

#ifndef PIECE_MORPHER_HH
# define PIECE_MORPHER_HH

# include <oln/morpher/generic_morpher.hh>

namespace oln {

  namespace morpher {

    template <class I, class Exact = mlc::final>
    struct piece_morpher;
    template <class I, class Exact = mlc::final>
    struct super_piece_morpher;

  } // end of namespace morpher

  /// Informations about the super piece morpher.
  template <class SrcType, class Exact>
  struct image_id< morpher::super_piece_morpher<SrcType, Exact> >
  {
    typedef typename mlc::exact_vt<
      morpher::super_piece_morpher<SrcType, Exact>,
      Exact>::ret
    exact_type;
    ///< Retrieve the exact type of the image.
  };

  /// Informations about the piece morpher.
  template <class SrcType, class Exact>
  struct image_id< morpher::piece_morpher<SrcType, Exact> >
  {
    enum {dim = SrcType::dim}; ///< The Image dimension.
    typedef oln_impl_type(SrcType) impl_type;
    ///< Underlying implementation.
    typedef oln_value_type(SrcType) value_type;
    ///< The value type of the decorated image.
    typedef typename mlc::exact_vt<morpher::piece_morpher<SrcType, Exact>,
				   Exact>::ret exact_type;
    ///< Retrieve the exact type of the image.
    typedef oln_point_type(SrcType) point_type;
    typedef oln_dpoint_type(SrcType) dpoint_type;
    typedef oln_size_type(SrcType) size_type;
    typedef oln_iter_type(SrcType) iter_type;
  };

  /// Traits for piece morpher.
  template <class SrcType, class Exact>
  struct image_traits < morpher::piece_morpher<SrcType, Exact> >
    : public
  image_traits<
    morpher::abstract::generic_morpher<
      SrcType,
      typename image_id<morpher::piece_morpher<SrcType,
					       Exact> >::exact_type
    >
  >
  {};


  namespace morpher {

    /// Abstract piece morpher class used for code factorization.
    template <class SrcType, class Exact>
    class super_piece_morpher
      : public abstract::generic_morpher<SrcType, Exact>
    {

    public:

      typedef super_piece_morpher<SrcType, Exact> self_type;
      typedef typename image_id<self_type>::exact_type exact_type;
      typedef abstract::generic_morpher<SrcType, Exact> super_type;

      typedef typename image_id<exact_type>::dpoint_type dpoint_type;
      typedef typename image_id<exact_type>::size_type size_type;

    protected:

      /*!
      ** \brief Default constructor.
      ** \arg ima will be the image.
      ** \arg p The reference point.
      ** \arg s The size of the piece of image.
      **
      ** One can not use this constructor to instantiate this class
      ** since it is protected.
      */
      super_piece_morpher(const SrcType &ima, const dpoint_type& p,
			  const size_type& s)
	: super_type(ima), size_(s), p_(p)
      {}

      const size_type size_; ///< The size of the piece of picture.
      const dpoint_type p_;
      ///< The reference point of the piece of picture.

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      super_piece_morpher()
      {}

    public:

      /// Return the size (different from the original picture).
      const size_type
      size() const
      {
	return size_;
      }

      /// Return the reference point.
      const dpoint_type
      ref_point() const
      {
	return p_;
      }

      /// Useful to debug.
      static std::string
      name()
      {
	return "super_piece_morpher<" + super_type::name() + ">";
      }

    };

    /*!
    ** \brief The default piece morpher class.
    **
    ** Using this class, a piece of picture is a picture.
    **
    ** \see oln::morpher::abstract::generic_morpher
    ** \see oln::morpher::piece_morph
    */
    template <class SrcType, class Exact>
    struct piece_morpher
      : public super_piece_morpher<
          SrcType,
          typename image_id<piece_morpher<SrcType, Exact> >::exact_type
        >
    {
      typedef piece_morpher<SrcType, Exact> self_type;
      typedef typename image_id<self_type>::exact_type exact_type;
      typedef super_piece_morpher<SrcType, exact_type> super_type;

      typedef typename image_id<exact_type>::point_type point_type;
      typedef typename image_id<exact_type>::dpoint_type dpoint_type;
      typedef typename image_id<exact_type>::size_type size_type;
      typedef typename image_id<exact_type>::value_type value_type;

      /// Construct the piece morpher with an image \a ima.
      piece_morpher(const SrcType &ima, const dpoint_type p,
		    const size_type s)
	: super_type(ima, p, s)
      {}

      /// Construct the piece morpher with another piece morpher.
      piece_morpher(const self_type& r)
	: super_type(r.get_ima(), r.ref_point(), r.size())
      {}

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      piece_morpher()
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
	  ( const_cast<SrcType &>(this->ima_)[p + p_] );
      }

      /*!
      ** \brief Return the stored value at the point.
      ** \arg p The point.
      ** \return The stored value.
      */
      const value_type
      at(const point_type& p) const
      {
	return this->ima_[p + p_];
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
	return "piece_morpher<" + super_type::name() + ">";
      }

    };

    /// The specialized version for `const' images.
    template <class SrcType, class Exact>
    struct piece_morpher<const SrcType, Exact>
      : public super_piece_morpher<
          const SrcType,
          typename image_id<piece_morpher<const SrcType,
					  Exact> >::exact_type
        >
    {
      typedef piece_morpher<const SrcType, Exact> self_type;
      typedef typename image_id<self_type>::exact_type exact_type;
      typedef super_piece_morpher<const SrcType, exact_type> super_type;

      typedef typename image_id<exact_type>::point_type point_type;
      typedef typename image_id<exact_type>::dpoint_type dpoint_type;
      typedef typename image_id<exact_type>::size_type size_type;
      typedef typename image_id<exact_type>::value_type value_type;

      /*!
      ** \brief Construct a piece morpher.
      ** \arg ima The image.
      ** \arg p The reference point.
      ** \arg s The size.
      */
      piece_morpher(const SrcType &ima, const dpoint_type p,
		    const size_type s)
	: super_type(ima, p, s)
      {}

      /// Construct a piece morpher from another one.
      piece_morpher(const self_type& r)
	: super_type(r.get_ima(), r.ref_point(), r.size())
      {}

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      piece_morpher() {}

      /*!
      ** \brief Return the stored value at the point.
      ** \arg p The point.
      ** \return The stored value.
      */
      const value_type
      at(const point_type &p) const
      {
	return this->ima_[p + p_];
      }

      /// Useful to debug.
      static std::string
      name()
      {
	return "piece_morpher<" + super_type::name() + ">";
      }

    };


    /*!
    ** \brief Instantiate a temporary read-only piece morpher.
    ** \arg ima The image.
    ** \arg p The reference's point.
    ** \arg s The size of the window.
    **
    ** A piece of the image will be viewed.
    **
    ** \code
    ** #include <oln/morpher/piece_morpher.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN  "lena.ppm");
    **   oln::save(oln::morpher::piece_morph(imc,
    **                                       oln::dpoint2d(246, 244),
    **                                       oln::image2d_size(30, 60,
    **                                        imc.border())),
    ** 	    IMG_OUT "oln_morpher_piece_morpher.pgm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_morpher_piece_morpher.png
    ** \image latex oln_morpher_piece_morpher.png
    */
    template <class I, class PointType, class SizeType>
    const piece_morpher<I>
    piece_morph(I &ima, const PointType p, const SizeType s)
    {
      return piece_morpher<I>(ima, p, s);
    }


  } // end namespace morpher

} // end namespace oln

#endif // !PIECE_MORPHER
