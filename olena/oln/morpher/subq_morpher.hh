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



#ifndef OLENA_MORPHER_SUBQ_MORPHER_HH
# define OLENA_MORPHER_SUBQ_MORPHER_HH

# include "generic_morpher.hh"

namespace oln {

  namespace morpher {

    template <class SrcType, unsigned N, class Exact = mlc::final>
    struct subq_morpher;


    /*! Change the color depth of \a T.
    **
    ** For Example, calling color_mute with
    ** color<3, 8, rgb_traits>, 5 will give
    ** the type : color<3, 5, rgb_traits>.
    **
    ** \param T The data type of the image.
    **
    ** \param N The new number of bits by component.
    **
    */

    template <class T, unsigned N>
    struct color_mute
    {
    };

    /// Specialized version for ntg::color.
    template <unsigned nbcomps_,
	      unsigned nbits_,
	      template <unsigned> class color_system,
	      unsigned N>
    struct color_mute<ntg::color<nbcomps_, nbits_, color_system>, N>
    {
      typedef ntg::color<nbcomps_, N, color_system> ret;
      /*! <The new value type.*/
      enum { nbcomps = nbcomps_ };
      /*! <The number of components */
    };



  } // end of namespace morpher

  /*! Retrieve types and dimension of the subq_morpher.
  **
  ** \param SrcType Input type decorated.
  **
  ** \param N The new number of bits by component.
  **
  ** \param Exact The exact type of the morpher.
  */
  template <class SrcType, unsigned N, class Exact>
  struct image_id<oln::morpher::subq_morpher<SrcType, N, Exact> >
  {
    enum {dim = SrcType::dim};
    /*! <The image dimension. */
    typedef oln_impl_type(SrcType) impl_type;
    /*! <The underlying implementation.*/
    typedef typename oln::morpher::color_mute<oln_value_type(SrcType), N>::ret value_type;
    /*! <The modified value type.*/
    typedef typename mlc::exact_vt<oln::morpher::subq_morpher<SrcType, N, Exact>,
				    Exact>::ret exact_type;

    typedef oln_point_type(SrcType) point_type;
  };

  /*! Specialized version for subq_morpher.
  **
  ** \param SrcType Input type decorated.
  **
  ** \param N The new number of bits by components.
  **
  ** \param Exact The exact type of the morpher.
  */
  template <class SrcType, unsigned N, class Exact>
  struct image_traits <oln::morpher::subq_morpher<SrcType, N, Exact> > :
    public image_traits<oln::morpher::abstract::generic_morpher<SrcType,
								typename image_id<oln::morpher::subq_morpher<SrcType, N, Exact> >::exact_type> >
  {
  };

  namespace morpher {



    /*! \brief Sub quantify an image.
    **
    ** By using this class, an image can be viewed
    ** as an image with a lower color depth.
    ** subq_morpher<image2d<rgb_8>, 5> is
    ** the same as image2d<rgb_5>.
    **
    ** \param SrcType The input type decorated.
    **
    ** \param N The new numbers of bits by component.
    **
    ** \param Exact The exact type of the morpher.
    */
    template <class SrcType, unsigned N, class Exact>
    struct subq_morpher:
      public abstract::generic_morpher<SrcType,
				       typename oln::image_id<subq_morpher<SrcType, N, Exact> >::exact_type>
    {

      /// The exact type of \a this. This class can be derived.
      typedef typename oln::image_id<subq_morpher<SrcType, N, Exact> >::exact_type exact_type;


      /// The upper class.
      typedef abstract::generic_morpher<SrcType,
					exact_type> super_type;

      /// The value point of the resulting image.
      typedef typename image_id<exact_type>::value_type value_type;
      typedef typename image_id<exact_type>::point_type point_type;
      typedef typename image_id<exact_type>::impl_type impl_type;
      enum { nbcomps = color_mute<oln_value_type(SrcType), N>::nbcomps };

      /// Construct the morpher with an image.
      subq_morpher(const SrcType &ima)
	: super_type(ima)
      {}

      /// Construct the morpher with another morpher.
      subq_morpher(const subq_morpher<SrcType, N>& r)
	: super_type(r.get_ima())
      {}

      /*! Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      subq_morpher() {}

      /// Return the value stored at \a p in the resulting image.
      const value_type
      at(const point_type& p) const
      {
        value_type	tmp;
        unsigned int	i;

        for (i = 0; i < nbcomps; i++)
	  {
	    tmp[i] = this->ima_[p][i] / ((ntg_max_val(ntg_comp_type(oln_value_type(SrcType))) + 1)
					 / (ntg_max_val(ntg_comp_type(value_type)) + 1));
	  }
	return tmp;
      }

      /// Return the implementation.
      const impl_type*
      impl() const
      {
        return ima_.impl();
      }

      static std::string
      name()
      {
        return "subq_morpher<" + SrcType::name() + ">";
      }

    };

    /*! \brief Instantiate a temporary read-only subq_morpher.
    **
    ** \param S Indicate the color depth of the resulting image.
    ** It can't be higher than the color depth of \a ima.
    **
    ** \code
    ** #include <oln/morpher/subq_morpher.hh>
    ** #include <oln/basics2d.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   oln::image2d<ntg::rgb_8> imc = oln::load(IMG_IN  "lena.ppm");
    **   oln::save(oln::morpher::sqmorph<3>(imc),
    ** 	    IMG_OUT "oln_morpher_subq_morpher.ppm");
    ** }
    ** \endcode
    ** \image html lena_ppm.png
    ** \image latex lena_ppm.png
    ** =>
    ** \image html oln_morpher_subq_morpher.png
    ** \image latex oln_morpher_subq_morpher.png
    */

    template <unsigned S, class I>
    const subq_morpher<I, S> sqmorph(I &ima)
    {
      return subq_morpher<I, S>(ima);
    }

  } // end of namespace morpher

} // end of namespace morpher




#endif // !OLENA_MORPHER_SUBQ_MORPHER_HH

