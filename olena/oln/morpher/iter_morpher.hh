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

#ifndef ITER_MORPHER_HH
# define ITER_MORPHER_HH

# include <oln/morpher/generic_morpher.hh>

namespace oln {

  namespace morpher {

    template <class SrcType, class IterType, class Exact = mlc::final>
    struct iter_morpher;

  } // end of namespace morpher

  /// Inherits identification's informations about the iter morpher.
  template <class SrcType, class IterType, class Exact>
  struct image_id< morpher::iter_morpher<SrcType, IterType, Exact> > : public image_id<SrcType>
  {};

  /// Inherits identification's informations about the const iter morpher.
  template <class SrcType, class IterType, class Exact>
  struct image_id< morpher::iter_morpher<const SrcType, IterType, Exact> > : public image_id<SrcType>
  {};

  /// Traits for iter morpher.
  template <class SrcType, class IterType>
  struct image_traits < morpher::iter_morpher<SrcType, IterType> > :
    public image_traits<abstract::image_with_impl<oln_impl_type(SrcType),
						  morpher::iter_morpher<SrcType, IterType> > >
  {
    typedef IterType iter_type;
  };

  /// Traits for const iter morpher.
  template <class SrcType, class IterType>
  struct image_traits < morpher::iter_morpher<const SrcType, IterType> > :
    public image_traits<abstract::image_with_impl<oln_impl_type(SrcType),
						  morpher::iter_morpher<const SrcType, IterType> > >
  {
    typedef IterType iter_type;
  };

  namespace morpher {

    template <class SrcType, class IterType, class Exact>
    struct iter_morpher
      : public abstract::generic_morpher< SrcType, SrcType, iter_morpher<SrcType, IterType, Exact> >
    {
      /// The type of the object instantiated. iter morpher can be derived.
      typedef typename image_id< iter_morpher<SrcType, IterType, Exact> >::exact_type exact_type;
      typedef iter_morpher<SrcType, IterType, Exact> self_type;
      typedef IterType iter_type;
      typedef abstract::generic_morpher< SrcType, SrcType, iter_morpher<SrcType, IterType, Exact> > super_type;

      /// Construct the iter morpher with an image \a ima.
      iter_morpher(const SrcType &ima)
	: super_type(ima)
      {}

      /// Construct the iter morpher with another iter morpher.
      iter_morpher(const self_type& r)
	: super_type(r.get_ima())
      {}

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      iter_morpher() {}

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

      /// Useful to debug.
      static std::string name()
      {
	return "iter_morpher<" + super_type::name() + ">";
      }

    };

    /// The specialized version for `const' declared images.
    template <class SrcType, class IterType, class Exact>
    struct iter_morpher<const SrcType, IterType, Exact>
      : public abstract::generic_morpher< SrcType, SrcType, iter_morpher<const SrcType, IterType, Exact> >
    {
      /// The type of the object instantiated. iter morpher can be derived.
      typedef typename image_id< iter_morpher<SrcType, IterType, Exact> >::exact_type exact_type;
      typedef iter_morpher<const SrcType, IterType, Exact> self_type;
      typedef IterType iter_type;
      typedef abstract::generic_morpher<SrcType, SrcType, iter_morpher<const SrcType, IterType, Exact> > super_type;

      /// Construct the iter morpher with an image \a ima.
      iter_morpher(const SrcType &ima)
	: super_type(ima)
      {}

      /// Construct the iter morpher with another iter morpher.
      iter_morpher(const iter_morpher<const SrcType, IterType>& r)
	: super_type(r.get_ima()) {}

      /*!
      ** \brief Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      iter_morpher()
      {}

     /// Useful to debug.
      static std::string name()
      {
	return "iter_morpher<" + super_type::name() + ">";
      }

    };

    /*!
    ** \brief Instantiate a temporary read-only iter morpher.
    **
    ** The image will be viewed according to its iterator type.
    ** For example, the foo function will print the size of the picture
    ** (the bkd_iter_type is used transparently).
    **
    ** \code
    ** #include <oln/morpher/iter_morpher.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** template <class E>
    ** void foo(const oln::abstract::image<E>& img)
    ** {
    **   oln_iter_type(oln::abstract::image<E>) it(img);
    **   for_all(it)
    **   {
    **     std::cout << it.row() << " " << it.col() << std::endl;
    **     break;
    **   }
    ** }
    ** int main()
    ** {
    **   const oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN "lena.ppm");
    **   assert(imc.has_impl());
    **   foo(oln::morpher::iter_morph<oln_bkd_iter_type_(oln::image2d<ntg::rgb_8>)>(imc));
    ** }
    ** \endcode
    */
    template <class IterType, class I>
    const iter_morpher<I, IterType> iter_morph(I &ima)
    {
      return iter_morpher<I, IterType>(ima);
    }

  } // End of namespace morpher.

} // End of namespace oln.

#endif // !ITER_MORPHER
