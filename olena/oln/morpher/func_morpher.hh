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



#ifndef OLENA_MORPHER_FUNC_MORPHER_HH
# define OLENA_MORPHER_FUNC_MORPHER_HH

# include <oln/morpher/generic_morpher.hh>


namespace oln {

  namespace morpher {

    template <class SrcType, class Ftor, class Exact = mlc::final>
    struct func_morpher;


  } // end of namespace morpher

  template <class SrcType, class Ftor, class Exact>
  struct image_id<oln::morpher::func_morpher<SrcType, Ftor, Exact> >
  {
    enum {dim = SrcType::dim};
    /*! <The image dimension. */
    typedef oln_impl_type(SrcType) impl_type;
    /*! <The underlying implementation.*/
    typedef oln_value_type(SrcType) value_type;
    /*! <The modified value type.*/
    typedef typename mlc::exact_vt<oln::morpher::func_morpher<SrcType, Ftor, Exact>,
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
  template <class SrcType, class Ftor, class Exact>
  struct image_traits <oln::morpher::func_morpher<SrcType, Ftor, Exact> > :
    public image_traits<oln::morpher::abstract::generic_morpher<SrcType,
								typename image_id<oln::morpher::func_morpher<SrcType, Ftor, Exact> >::exact_type> >
  {
  };

  namespace morpher {



    template <class SrcType, class Ftor, class Exact>
    struct func_morpher:
      public abstract::generic_morpher<SrcType,
				       typename oln::image_id<func_morpher<SrcType, Ftor, Exact> >::exact_type>
    {

      Ftor f;

      /// The exact type of \a this. This class can be derived.
      typedef typename oln::image_id<func_morpher<SrcType, Ftor, Exact> >::exact_type exact_type;

      /// The upper class.
      typedef abstract::generic_morpher<SrcType,
					exact_type> super_type;

      /// The value point of the resulting image.
      typedef typename image_id<exact_type>::value_type value_type;
      typedef typename image_id<exact_type>::point_type point_type;
      typedef typename image_id<exact_type>::impl_type impl_type;

      /// Construct the morpher with an image.
      func_morpher(const SrcType &ima, const Ftor f_)
	: super_type(ima)
      { f = f_;}

      /// Construct the morpher with another morpher.
      func_morpher(const func_morpher<SrcType, Ftor>& r)
	: super_type(r.get_ima())
      { f = r.f; }

      /*! Empty constructor.
      **
      ** Needed by mlc_hierarchy::any_with_diamond.
      */
      func_morpher() {}

      /// Return the value stored at \a p in the resulting image.
      const value_type
      at(const point_type& p) const
      {
	return f(ima_, p);
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
  }
}



#endif // !OLENA_MORPHER_FUNC_MORPHER_HH
