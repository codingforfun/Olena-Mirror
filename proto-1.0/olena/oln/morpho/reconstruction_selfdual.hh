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

#ifndef OLENA_MORPHO_RECONSTRUCTION_SELF_DUAL_HH
# define OLENA_MORPHO_RECONSTRUCTION_SELF_DUAL_HH

# include <oln/canvas/reconstruction.hh>
# include <oln/morpho/tags.hh>

namespace oln {


  namespace morpho {


    namespace impl {


      template<typename I1, typename I2>
      struct reconstruction <I1, I2, tag::selfdual_type, tag::none_type>
	: public canvas::hybrid_reconstruction<I1, I2,
	    reconstruction<I1, I2, tag::selfdual_type, tag::none_type> >
      {
	typedef reconstruction<I1, I2, tag::selfdual_type,
			       tag::none_type> self_type;
	typedef canvas::hybrid_reconstruction<I1, I2,
					      self_type> super_type;

	reconstruction(const abstract::image_with_nbh<I1>& marker,
		       const abstract::image<I2>& mask) :
	  super_type(marker, mask)
	{
	}

	using super_type::mask;
	using super_type::marker;
	using super_type::work;
	using super_type::output;
	using super_type::fwd_p;
	using super_type::bkd_p;
	using super_type::win_plus;
	using super_type::win_minus;
	using super_type::p;
	using super_type::q;
	using super_type::fifo;


	void impl_bkd_loop_body()
	{
	  // FIXME: Shouldn't be .value() !
	  if (work[bkd_p] < mask[bkd_p])
	    output[bkd_p] = ntg::min(local_max(work, bkd_p, win_minus),
				     mask[bkd_p].value());
	  else
	    output[bkd_p] = ntg::max(local_min(work, bkd_p, win_minus),
				     mask[bkd_p].value());
	}

	void impl_fwd_loop_body()
	{
	  // FIXME: Shouldn't be .value() !
	  if (work[fwd_p] < mask[fwd_p])
	    output[fwd_p] = ntg::min(local_max(work, fwd_p, win_plus),
				     mask[fwd_p].value());
	  else
	    output[fwd_p] = ntg::max(local_min(work, fwd_p, win_plus),
				     mask[fwd_p].value());
	}

	void impl_fifo_loop_body()
	{
	  if (output[q] < mask[p])
	    {
	      if (output[q] < output[p] && mask[q] != output[q])
		{
		  output[q] = ntg::min(output[p].value(), mask[q].value());
		  fifo.push(q);
		}
	    }
	  else
	    if (output[q] > output[p] && mask[q] != output[q])
	      {
		output[q] = ntg::max(output[p].value(), mask[q].value());
		fifo.push(q);
	      }
	}

	bool impl_exist_init()
	{
	  return output.hold(q) &&
	    ((output[q] < output[bkd_p] && output[q] < mask[q]) ||
	     (output[q] > output[bkd_p] && output[q] > mask[q]));
	}

	void impl_preconditions()
	{
	}

      };

    }

  }

}


#endif // ! OLENA_MORPHO_RECONSTRUCTION_BY_DILATION_HH
