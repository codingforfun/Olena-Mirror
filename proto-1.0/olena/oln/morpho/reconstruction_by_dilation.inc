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

namespace oln {

  namespace morpho {

    namespace impl {

      // Sequential version

      template<typename I1, typename I2>
      struct generic_reconstruction <tag::by_dilation, tag::sequential, I1, I2>
	: public generic_reconstruction_canvas<I1, I2, tag::sequential,
	    generic_reconstruction<tag::by_dilation, tag::sequential, I1, I2> >
      {
	typedef generic_reconstruction<tag::by_dilation,
				       tag::sequential, I1,I2> self_type;
	typedef generic_reconstruction_canvas<I1, I2, tag::sequential,
					      self_type> super_type;

	generic_reconstruction(const abstract::image_with_nbh<I1>& marker,
			       const abstract::image<I2>& mask) :
	  super_type(marker, mask)
	{
	}


	void impl_bkd_loop_body()
	{
	  this->output[this->bkd_p] = ntg::min(morpho::max(this->output,
							   this->bkd_p,
							   this->win_minus),
					       this->mask[this->bkd_p].value());
	}

	void impl_fwd_loop_body()
	{
	  this->output[this->fwd_p] = ntg::min(morpho::max(this->output,
							   this->fwd_p,
							   this->win_plus),
					       this->mask[this->fwd_p].value());
	}

	void impl_preconditions()
	{
	  precondition(level::is_greater_or_equal(this->mask, this->marker));
	}

      };


      // Hybrid version

      template<typename I1, typename I2>
      struct generic_reconstruction <tag::by_dilation, tag::hybrid, I1, I2>
	: public generic_reconstruction_canvas<I1, I2, tag::hybrid,
	    generic_reconstruction<tag::by_dilation, tag::hybrid, I1, I2> >
      {
	typedef generic_reconstruction<tag::by_dilation,
				       tag::hybrid, I1,I2> self_type;
	typedef generic_reconstruction_canvas<I1, I2, tag::hybrid,
					      self_type> super_type;

	generic_reconstruction(const abstract::image_with_nbh<I1>& marker,
			       const abstract::image<I2>& mask) :
	  super_type(marker, mask)
	{
	}



	void impl_bkd_loop_body()
	{
	  this->output[this->bkd_p] = ntg::min(morpho::max(this->work,
							   this->bkd_p,
							   this->win_minus),
					       this->mask[this->bkd_p].value());
	}

	void impl_fwd_loop_body()
	{
	  this->output[this->fwd_p] = ntg::min(morpho::max(this->work,
							   this->fwd_p,
							   this->win_plus),
					       this->mask[this->fwd_p].value());
	}

	void impl_fifo_loop_body()
	{
	  if ((this->output[this->q] < this->output[this->p]) &&
	      (this->mask[this->q] != this->output[this->q]))
	    {
	      this->output[this->q] = ntg::min(this->output[this->p].value(),
					       this->mask[this->q].value());
	      this->fifo.push(this->q);
	    }
	}

	bool impl_exist_init()
	{
	  return this->output.hold(this->q) &&
	    (this->output[this->q] < this->output[this->bkd_p]) &&
	    (this->output[this->q] < this->mask[this->q]);
	}

	void impl_preconditions()
	{
	  precondition(level::is_greater_or_equal(this->mask, this->marker));
	}

      };

    }

  }

}
