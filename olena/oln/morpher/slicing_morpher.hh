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

#ifndef SLICING_MORPHER_HH
# define SLICING_MORPHER_HH

# include <oln/morpher/generic_morpher.hh>

namespace oln {

  namespace morpher {

    template <class I, class Exact = mlc::final>
    struct slicing_morpher;

  } // end of namespace morpher


  template <class Image>
  struct DecDimensionImage
  {};
  template <class Type>
  struct DecDimensionImage< oln::image2d<Type> >
  {
    typedef oln::image1d<Type> ret;
  };
  template <class Type>
  struct DecDimensionImage< oln::image3d<Type> >
  {
    typedef oln::image2d<Type> ret;
  };

  /// Inherits identification's informations about the slicing morpher.
  template <class I, class Exact>
  struct image_id< morpher::slicing_morpher<I, Exact> >
    : public image_id<typename DecDimensionImage<I>::ret>
  {};

  /// Inherits identification's informations about the const slicing morpher.
  template <class I, class Exact>
  struct image_id< morpher::slicing_morpher<const I, Exact> >
    : public image_id<typename DecDimensionImage<I>::ret>
  {};

  /// Specialized version for slicing morpher.
  template <class I>
  struct image_traits< morpher::slicing_morpher<I> > :
    public image_traits<abstract::image_with_impl<typename DecDimensionImage<I>::ret::impl_type,
						  morpher::slicing_morpher<I> > >
  {};

  /// Specialized version for slicing morpher.
  template <class I>
  struct image_traits< morpher::slicing_morpher<const I> > :
    public image_traits<abstract::image_with_impl<typename DecDimensionImage<I>::ret::impl_type,
						  morpher::slicing_morpher<const I> > >
  {};

  namespace morpher {

    /// Return a size of N-1 dimension.
    oln::image1d_size* image_size_dec(const oln::image2d_size& image_size)
    {
      return new oln::image1d_size(image_size.ncols(), image_size.border());
    }

    /// Return a size of N-1 dimension.
    oln::image2d_size* image_size_dec(const oln::image3d_size& image_size)
    {
      return new oln::image2d_size(image_size.nrows(), image_size.ncols(), image_size.border());
    }

    /// Abstract piece morpher class used for code factorization.
    template <class DestType, class SrcType, class Exact>
    class super_slicing_morpher : public abstract::generic_morpher<DestType, SrcType, Exact>
    {
    public:

      typedef oln_size_type(DestType) size_type;
      typedef oln_impl_type(DestType) impl_type;
      typedef abstract::generic_morpher<DestType, SrcType, Exact> super_type;

      /// Override the size method.
      const size_type&
      size() const
      {
	return *size_;
      }

      /// Override the impl method.
      const impl_type*
      impl() const
      {
	return impl_;
      }

      /// Override the impl method.
      impl_type*
      impl()
      {
	return impl_;
      }

      ///< Return the last coordinate' value.
      coord get_slice() const
      {
	return slice_;
      }

      /// Useful to debug.
      static std::string name()
      {
	return "super_slicing_morpher<" + super_type::name() + ">";
      }

    protected:

      /*!
      ** \brief Default constructor.
      ** \arg ima will be the image.
      ** \arg slice The last coordinate.
      **
      ** One can not use this constructor to instantiate this class
      ** since it is protected.
      */
      super_slicing_morpher(const SrcType &ima, const coord slice)
	: super_type(ima), impl_(0), size_(0), slice_(slice)
      {
	size_ = image_size_dec(ima_.size());
	impl_ = new impl_type(*size_);
      }

      /// Destructor
      ~super_slicing_morpher()
      {
	delete size_;
      }

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      super_slicing_morpher() : impl_(0), size_(0), slice_(0)
      {}

      impl_type *impl_;
      size_type *size_; ///< The size of the N-1 dimension image.
      coord slice_; ///< The last coordinate.

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
    struct slicing_morpher
      : public super_slicing_morpher< typename DecDimensionImage<SrcType>::ret, SrcType, slicing_morpher<SrcType, Exact> >
    {
      typedef typename image_id< slicing_morpher<SrcType, Exact> >::exact_type exact_type;
      typedef slicing_morpher<SrcType, Exact> self_type;
      typedef typename DecDimensionImage<SrcType>::ret DestType;
      typedef super_slicing_morpher<DestType, SrcType, slicing_morpher<SrcType, Exact> > super_type;

      typedef oln_value_type(DestType) value_type;
      typedef oln_point_type(DestType) point_type;

      /*!
      ** \brief Construct a piece morpher.
      ** \arg ima The image.
      ** \arg slice The slice value.
      */
      slicing_morpher(const SrcType &ima, coord slice)
	: super_type(ima, slice) {}

      /// Construct a piece morpher from another one.
      slicing_morpher(const self_type& r)
	: super_type(r.get_ima(), r.get_slice()) {}

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      slicing_morpher() {}

      /*!
      ** \brief Return the stored value at the point.
      ** \arg p The point.
      ** \return The stored value.
      */
      value_type& at(const point_type &p)
      {
	typename SrcType::point_type tmp_p(p, slice_);
	return const_cast<value_type &>(this->ima_)[tmp_p];
      }

      const value_type at(const point_type& p) const
      {
	typename SrcType::point_type tmp_p(p, slice_);
	return this->ima_[tmp_p];
      }

      /*!
      ** \brief Perform a shallow copy from the decorated image of \a rhs
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
      operator=(DestType& rhs)
      {
	return this->exact().assign(rhs);
      }

      /// Useful to debug.
      static std::string name()
      {
	return "slicing_morpher<" + super_type::name() + ">";
      }

    };

    /// The specialized version for `const' declared images.
    template <class SrcType, class Exact>
    struct slicing_morpher<const SrcType, Exact>
      : public super_slicing_morpher<typename DecDimensionImage<SrcType>::ret, SrcType, slicing_morpher<const SrcType, Exact> >
    {
      /// The type of the object instantiated. piece morpher can be derived.
      typedef typename image_id<slicing_morpher<const SrcType, Exact> >::exact_type exact_type;
      typedef slicing_morpher<SrcType, Exact> self_type;
      typedef typename DecDimensionImage<SrcType>::ret DestType;
      typedef super_slicing_morpher<DestType, SrcType, slicing_morpher<const SrcType, Exact> > super_type;

      typedef oln_value_type(DestType) value_type;
      typedef oln_point_type(DestType) point_type;

      /*!
      ** \brief Construct a piece morpher.
      ** \arg ima The image.
      ** \arg slice The slice value.
      */
      slicing_morpher(const SrcType &ima, coord slice)
	: super_type(ima, slice)
      {}

      /// Construct a piece morpher from another one.
      slicing_morpher(const self_type& r)
	: super_type(r.get_ima(), r.get_slice())
      {}

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      slicing_morpher()
      {}

      /*!
      ** \brief Return the stored value at the point.
      ** \arg p The point.
      ** \return The stored value.
      */
      const value_type at(const point_type &p) const
      {
	typename SrcType::point_type tmp_p(p, slice_);
	return this->ima_[tmp_p];
      }

      /// Useful to debug.
      static std::string name()
      {
	return "slicing_morpher<" + super_type::name() + ">";
      }

    };

    /*!
    ** \brief Instantiate a temporary read-only slicing morpher.
    ** \arg ima The image.
    ** \arg slice The slice.
    **
    ** A slice of the image will be viewed.
    **
    ** \code
    ** #include <oln/morpher/slicing_morpher.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN "lena.ppm");
    **   oln::save(oln::morpher::slicing_morph(imc, 5),
    ** 	    IMG_OUT "oln_morpher_slicing_morpher.pgm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_morpher_slicing_morpher.png
    ** \image latex oln_morpher_slicing_morpher.png
    */
    template <class I>
    const slicing_morpher<I> slicing_morph(I &ima, coord slice)
    {
      return slicing_morpher<I>(ima, slice);
    }

  } // end of morpher namespace

} // end of oln namespace

#endif // !SLICING_MORPHER
