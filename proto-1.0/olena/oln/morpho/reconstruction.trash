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

#ifndef OLENA_MORPHO_RECONSTRUCTION_HH
# define OLENA_MORPHO_RECONSTRUCTION_HH

# include <queue>

# include <mlc/cmp.hh>
# include <mlc/contract.hh>

# include <oln/convert/nbh_to_se.hh>

# include <oln/core/abstract/image_operator.hh>
# include <oln/core/abstract/neighborhood.hh>

# include <oln/level/compare.hh>

# include <oln/morpho/splitse.hh>
# include <oln/morpho/stat.hh>

# include <oln/utils/clone.hh>

namespace oln {


  namespace tag {

    template <typename Op> struct oper {};

    struct by_dilation : public oper< by_dilation > {};
    struct by_erosion  : public oper< by_erosion > {};


    template <typename A> struct algo {};

    struct sequential : public algo< sequential > {};
    struct hybrid     : public algo< hybrid > {};

  } // end of namespace oln::morpho::tag



  namespace morpho {
    template <typename I1, typename I2> struct reconstruction_ret;
  } // end of namespace oln::morpho

  // super_type

  template <typename I1, typename I2>
  struct set_super_type< morpho::reconstruction_ret<I1, I2> >
  {
    typedef oln_type_of(I1, concrete) output_type;

    typedef morpho::reconstruction_ret<I1,I2> self_type;
    typedef abstract::image_binary_operator<output_type, I1, I2, self_type > ret;
  };

  namespace morpho {

    // Reconstruction as a 'classical' procedure returning an image (do not
    // use it; prefer morpho::reconstruction).

    namespace proc {

      // FIXME: ...

    } // end of namespace oln::morpho::proc


    template <typename I1, typename I2>
    struct reconstruction_ret :
      // FIXME: oln_super_of_
      public oln::internal::get_super_type< reconstruction_ret<I1,I2> >::ret
    {
      typedef reconstruction_ret<I1, I2> self_type;
      typedef typename oln::internal::get_super_type<self_type>::ret super_type;

      box<const I1> marker;
      box<const I2> mask;

      reconstruction_ret(const abstract::image_with_nbh<I1>& marker,
			 const abstract::image<I2>& mask) :
	super_type(marker, mask),
	marker(marker),
	mask(mask)
      {
      }

      const oln_type_of(I1, neighb)& impl_nbh_get() const
      {
	return marker.nbh_get();
      }

    };

  } // end of namespace morpho

} // end of namespace oln

# include <oln/morpho/reconstruction_canvas.inc>

namespace oln {

  namespace morpho {

    namespace impl {

      template<typename Op, typename A, typename I1, typename I2>
      struct generic_reconstruction;

    } // end of namespace impl

  } // end of namespace morpho

} // end of namespace oln

# include <oln/morpho/reconstruction_by_dilation.inc>
# include <oln/morpho/reconstruction_by_erosion.inc>

namespace oln {

  namespace morpho {

    namespace impl {

      // Generic implementation of reconstruction (routine).

      template<typename Op, typename A, typename I1, typename I2>
      reconstruction_ret<I1,I2>
      reconstruction(const abstract::image_with_nbh<I1>& marker,
		     const abstract::image<I2>&          mask)
      {
	generic_reconstruction<Op, A, I1, I2> tmp(marker, mask);
	tmp.run();
	return tmp;
      }

    } // end of namespace impl

    /// Generic reconstruction (facade).

    template<typename Op, typename I1, typename I2, typename A>
    reconstruction_ret<I1,I2>
    reconstruction(const tag::oper<Op>&			oper_,
		   const abstract::image_with_nbh<I1>&	marker,
		   const abstract::image<I2>&		mask,
		   const tag::algo<A>&			algo_)
    {
      return impl::reconstruction<Op,A>(marker.exact(), mask.exact());
    }

    // by dilation

    template<typename I1, typename I2, typename A>
    reconstruction_ret<I1,I2>
    reconstruction_by_dilation(const abstract::image_with_nbh<I1>&	marker,
			       const abstract::image<I2>&		mask,
			       const tag::algo<A>&			algo_)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());
      return reconstruction(tag::by_dilation(), marker, mask, algo_);
    }

    template<typename I1, typename I2>
    reconstruction_ret<I1,I2>
    reconstruction_by_dilation(const abstract::image_with_nbh<I1>& marker,
			       const abstract::image<I2>&          mask)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());
      return reconstruction(tag::by_dilation(), marker, mask, tag::hybrid());
    }

    // by erosion

    template<typename I1, typename I2, typename A>
    reconstruction_ret<I1,I2>
    reconstruction_by_erosion(const abstract::image_with_nbh<I1>&	marker,
			       const abstract::image<I2>&		mask,
			       const tag::algo<A>&			algo_)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());
      return reconstruction(tag::by_erosion(), marker, mask, algo_);
    }

    template<typename I1, typename I2>
    reconstruction_ret<I1,I2>
    reconstruction_by_erosion(const abstract::image_with_nbh<I1>& marker,
			       const abstract::image<I2>&          mask)
    {
      mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
      precondition(marker.size() == mask.size());
      return reconstruction(tag::by_erosion(), marker, mask, tag::hybrid());
    }

  } // end of namespace oln::morpho

} // end of namespace oln

#endif // ! OLENA_MORPHO_RECONSTRUCTION_HH
