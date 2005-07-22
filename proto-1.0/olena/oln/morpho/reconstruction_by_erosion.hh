// Copyright (C) 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_RECONSTRUCTION_BY_EROSION_HH
# define OLENA_MORPHO_RECONSTRUCTION_BY_EROSION_HH

# include <oln/canvas/reconstruction.hh>
# include <oln/morpho/tags.hh>
# include <oln/morpho/local.hh>
# include <oln/funobj/arith.hh>

namespace oln {

  namespace morpho {

    namespace impl {


      template <typename I1, typename I2, typename A, typename E>
      struct reconstruction_by_erosion
	: public canvas::reconstruction<I1, I2, A, E>
      {
	typedef canvas::reconstruction<I1, I2, A, E> super_type;

	using super_type::mask;
	using super_type::marker;
	using super_type::output;
	using super_type::bkd_p;
	using super_type::fwd_p;
	using super_type::n;
	using super_type::p;


	reconstruction_by_erosion(const abstract::image_with_nbh<I1>& marker,
				  const abstract::image<I2>& mask) :
	  super_type(marker, mask)
	{
	}


	/// Local image "and-value" for erosion on sets
	/// (based on the point and its backward neighborhood).

	oln_type_of(I1, value) bkd_and()
	{
	  if (not output[bkd_p])
	    return false;
	  p = bkd_p;
	  for_all_n_of_p(n, p)
	    if (p.bkd_less(n) and output.hold(n) and not output[n])
	      return false;

	  return true;
	}


	/// Local image "and-value" for erosion on sets
	/// (based on the point and its forward neighborhood).

	oln_type_of(I1, value) fwd_and()
	{
	  if (not output[fwd_p])
	    return false;
	  p = fwd_p;
	  for_all_n_of_p(n, p)
	    if (p.fwd_less(n) and output.hold(n) and not output[n])
	      return false;

	  return true;
	}

	void impl_bkd_loop_body()
	{
	  // FIXME: The call to value_box<>::value is needed to have
	  // f_max_alt compile.  Try to get rid of it.
	  output[bkd_p] = f_max_alt(mask[bkd_p].value(), bkd_and());
	}

	void impl_fwd_loop_body()
	{
	  // FIXME: The call to value_box<>::value is needed to have
	  // f_max_alt compile.  Try to get rid of it.
	  output[fwd_p] = f_max_alt(mask[fwd_p].value(), fwd_and());
	}

	void impl_preconditions()
	{
	  precondition(level::is_greater_or_equal(marker, mask));
	}

      };

      // Sequential version

      template<typename I1, typename I2>
      struct reconstruction <I1, I2, tag::sequential_type, tag::by_erosion_type>
	: public reconstruction_by_erosion<I1, I2, tag::sequential_type,
					   reconstruction<I1, I2, tag::sequential_type,
							  tag::by_erosion_type> >
      {
	typedef reconstruction<I1, I2, tag::sequential_type,
			       tag::by_erosion_type> self_type;
	typedef reconstruction_by_erosion<I1, I2, tag::sequential_type,
					  self_type> super_type;

	reconstruction(const abstract::image_with_nbh<I1>& marker,
		       const abstract::image<I2>& mask) :
	  super_type(marker, mask)
	{
	}
      };


      // Hybrid version

      template<typename I1, typename I2>
      struct reconstruction <I1, I2, tag::hybrid_type, tag::by_erosion_type>
	: public reconstruction_by_erosion<I1, I2, tag::hybrid_type,
	    reconstruction<I1, I2, tag::hybrid_type, tag::by_erosion_type> >
      {
	typedef reconstruction<I1, I2, tag::hybrid_type,
			       tag::by_erosion_type> self_type;
	typedef reconstruction_by_erosion<I1, I2, tag::hybrid_type,
					  self_type> super_type;

	reconstruction(const abstract::image_with_nbh<I1>& marker,
		       const abstract::image<I2>& mask) :
	  super_type(marker, mask)
	{
	}

	using super_type::mask;
	using super_type::output;
	using super_type::bkd_p;
	using super_type::n;
	using super_type::p;
	using super_type::fifo;


	void impl_fifo_loop_body()
	{
	  if ((output[n] > output[p]) and (mask[n] != output[n]))
	    {
	      // FIXME: The calls to value_box<>::value are needed to
	      // have f_min_alt compile.  Try to get rid of it.
	      output[n] = f_min_alt(output[p].value(), mask[n].value());
	      fifo.push(n);
	    }
	}

	bool impl_test_fifo_push()
	{
	  return output[n] > output[bkd_p] and output[n] > mask[n];
	}

      };

    }

  }

}


#endif // ! OLENA_MORPHO_RECONSTRUCTION_BY_EROSION_HH
