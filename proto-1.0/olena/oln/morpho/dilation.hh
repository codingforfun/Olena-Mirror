// Copyright (C) 2001, 2002, 2004, 2005  EPITA Research and Development Laboratory
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
// As a special exception, you may use this filek as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_MORPHO_DILATION_HH
# define OLENA_MORPHO_DILATION_HH

# include <oln/basics.hh>
# include <oln/morpho/stat.hh>
# include <oln/core/abstract/image_operator.hh>
# include <mlc/cmp.hh>

namespace oln {

  namespace morpho {



    namespace proc {

      /// Dilation as a procedure (do not use it; prefer morpho::dilation).

      template<typename I, typename S>
      oln_type_of(I, concrete) dilation(const abstract::image<I>& input,
					const abstract::struct_elt<S>& se)
      {
	oln_type_of(I, concrete) output(input.size());
	oln_type_of(I, fwd_piter) p(input.size());
	for_all (p)
	  output[p] = morpho::max(input, p, se);
	return output;
      }

    } // end of namespace oln::morpho::proc



    /*!
    ** \brief Processing dilation.
    **
    ** Compute the morphological dilation of input using se
    ** as structural element.\n
    **
    ** On grey-scale images, each point is replaced by the maximum value
    ** of its neighbors, as indicated by se.  On binary images,
    ** a logical or is performed between neighbors.\n
    **
    ** The morpho::fast version of this function use a different
    ** algorithm: This algorithm is described in
    ** Implementation of morphological operations from:
    ** M. Van Droogenbroeck and H. Talbot.
    ** "Fast computation of morphological operations with arbitrary
    ** structuring elements". Pattern Recognition Letters,
    ** 17(14):1451-1460, 1996.\n
    **
    ** An histogram of the value of the neighborhood indicated by
    ** se is updated while iterating over all point of the
    ** image.  Doing so is more efficient  when the
    ** structural element is large.\n
    **
    ** \param I Exact type of the input image.
    ** \param E Exact type of the neighborhood.
    **
    ** \arg input The input image.
    ** \arg se Structuring element to use.
    **
    ** \code
    ** #include <oln/basics2d.hh>
    ** #include <oln/morpho/dilation.hh>
    ** #include <oln/level/compare.hh>
    ** #include <ntg/all.hh>
    ** int main()
    ** {
    **   typedef oln::image2d<ntg::bin>	im_type;
    **
    **   im_type im1(oln::load(IMG_IN "object.pbm"));
    **   save(oln::morpho::dilation(im1, oln::win_c8p()),
    **        IMG_OUT "oln_morpho_dilation.pbm");
    ** }
    ** \endcode
    **
    ** \image html object_pbm.png
    ** \image latex object_pbm.png
    ** =>
    ** \image html oln_morpho_dilation.png
    ** \image latex oln_morpho_dilation.png
    */

    // fwd decl
    template <typename I, typename E> struct dilation_ret;

  }

  // category
  template <typename I, typename E>
  struct set_category< morpho::dilation_ret<I,E> > { typedef category::image ret; };

  // super_type
  template <typename I, typename E>
  struct set_super_type< morpho::dilation_ret<I,E> >
  {
    typedef abstract::image_unary_operator<oln_type_of(I, concrete), I, morpho::dilation_ret<I, E> > ret;
  };



  namespace morpho {

    /// Dilatation return.

    template <typename I, typename S>
    struct dilation_ret : public abstract::image_unary_operator<oln_type_of(I, concrete), I, dilation_ret<I, S> >
    {
      typedef abstract::image_unary_operator<oln_type_of(I, concrete), I, dilation_ret<I, S> > super_type;
      typedef typename super_type::output_type output_type;

      const S se;

      dilation_ret(const abstract::image<I>& input,
		   const abstract::struct_elt<S>& se) :
	super_type(input.exact()),
	se(se.exact())
	{
	}

    };


    // Implementation:

    namespace impl {

      /// Dilation generic implementation.

      template <typename I, typename S>
      struct generic_dilation : public dilation_ret<I, S>
      {
	typedef dilation_ret<I, S> super_type;
	typedef typename super_type::output_type output_type;

	generic_dilation(const abstract::image<I>& input,
			 const abstract::struct_elt<S>& se) :
	  super_type(input, se)
	{
	}

	void impl_run()
	{
	  mlc::is_true<mlc::type::eq<oln_type_of(I, size),
	                             oln_type_of(S, size)>::ret>::ensure();

	  output_type tmp(this->input.size()); // FIXME: trick
	  this->output = tmp;

	  oln_type_of(I, fwd_piter) p(this->input.size());
	  for_all (p)
	    this->output[p] = morpho::max(this->input, p, this->se);
	}
      };

    } // end of namespace oln::morpho::impl


    /// Dilation generic routine.

    template<typename I, typename S>
    dilation_ret<I, S> dilation(const abstract::image<I>& input,
				const abstract::struct_elt<S>& se)
    {
      impl::generic_dilation<I, S> tmp(input, se);
      tmp.run();
      return tmp;
    }

  } // end of morpho

} // end of oln

#endif // ! OLENA_MORPHO_DILATION_HH
