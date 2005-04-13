// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_GEODESIC_EROSION_HH
# define OLENA_MORPHO_GEODESIC_EROSION_HH

# include <mlc/cmp.hh>

# include <oln/core/abstract/image_operator.hh>
# include <oln/morpho/stat.hh>
# include <oln/level/compare.hh>
# include <oln/convert/nbh_to_se.hh>
# include <oln/arith/max.hh>

# include <oln/core/abstract/neighborhood.hh>
# include <oln/morpho/erosion.hh>

namespace oln {

  namespace morpho {

    // fwd declaration
    template <class I1, class I2> struct geodesic_erosion_ret;

    namespace proc {

      template<class I1, class I2>
      oln_type_of(I1, concrete)
        geodesic_erosion(const abstract::image_with_nbh<I1>& marker,
			 const abstract::image<I2>& mask)
      {
	mlc::eq<oln_type_of(I1, size), oln_type_of(I2, size)>::ensure();
	precondition(marker.size() == mask.size());
	precondition(level::is_greater_or_equal(marker, mask));
        oln_type_of(I1, concrete) output(marker.size());
	marker.border_adapt_copy(marker.nbh_get().delta());
        oln_type_of(I1, piter) p(marker);
        for_all (p)
          output[p] = arith::max(morpho::min(marker, p, convert::nbh_to_cse(marker.nbh_get())),
				 mask[p]);
        return output;
      }

    } // end of namespace oln::morpho::proc

  } // end of namespace oln::morpho


  // super_type
  template <class I1, class I2>
  struct set_super_type< morpho::geodesic_erosion_ret<I1, I2> >
  {
    typedef abstract::image_binary_operator<oln_type_of(I1, concrete), I1, I2, morpho::geodesic_erosion_ret<I1, I2> > ret;
  };


  namespace morpho {

    template <class I1, class I2>
    struct geodesic_erosion_ret : public abstract::image_binary_operator<oln_type_of(I1, concrete), I1, I2, morpho::geodesic_erosion_ret<I1, I2> >
    {
      typedef abstract::image_binary_operator<oln_type_of(I1, concrete), I1, I2, geodesic_erosion_ret<I1, I2> > super_type;

      geodesic_erosion_ret(const abstract::image_with_nbh<I1>& marker,
			   const abstract::image<I2>& mask) :
	super_type(marker, mask)
      {}
    };


    namespace safe {

      template <class I1, class I2>
      struct geodesic_erosion : public geodesic_erosion_ret<I1, I2>
      {
	typedef geodesic_erosion_ret<I1, I2> super_type;

	geodesic_erosion(const abstract::image_with_nbh<I1>& marker,
			 const abstract::image<I2>& mask) :
	  super_type(marker, mask)
	{}

	void impl_run()
	{
	  mlc::eq<oln_type_of(I1, size), oln_type_of(I2, size)>::ensure();
	  precondition((this->input1).size() == (this->input2).size());
	  precondition(level::is_greater_or_equal(this->input2, this->input1));
	  this->output = arith::max(erosion(this->input1.unbox(), this->input1.unbox().nbh_get()).output.unbox(), this->input2.unbox()).output;
	}
      };

    } // end of namespace oln::morpho::safe

    template<class I1, class I2>
    geodesic_erosion_ret<I1, I2>
    geodesic_erosion(const abstract::image_with_nbh<I1>& marker,
		     const abstract::image<I2>& mask)
    {
      safe::geodesic_erosion<I1, I2> tmp(marker, mask);
      tmp.run();
      return tmp;
    }

  } // end of namespace oln::morpho

} // end of namespace oln

#endif // OLENA_MORPHO_GEODESIC_EROSION_HH
