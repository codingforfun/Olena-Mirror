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

      template <typename I1, typename I2, typename A, typename E>
      struct generic_reconstruction_canvas;

      template <typename I1, typename I2, typename E>
      struct generic_reconstruction_canvas<I1, I2, tag::sequential, E> :
	public reconstruction_ret<I1, I2>
      {
	typedef reconstruction_ret<I1, I2> super_type;
	typedef oln_type_of(I1, neighb) nbh_type;
	typedef oln_type_of(I1, concrete) output_type;

	E& exact__()
	{
	  return *(E*)(void*)(this);
	}

	void bkd_loop_body()
	{
	  this->exact__().impl_bkd_loop_body();
	}
	void fwd_loop_body()
	{
	  this->exact__().impl_fwd_loop_body();
	}

	void preconditions()
	{
	  mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
	  precondition(this->marker.size() == this->mask.size());
	  this->exact__().impl_preconditions();
	}

	void init()
	{
	  // no call to impl_init here because this canvas can't be generalized yet.
	  this->output = utils::clone(this->marker);
	  this->work = utils::clone(this->marker);

	  win_plus = get_plus_win_p(convert::nbh_to_cse(this->marker.nbh_get()));
	  win_minus = get_minus_win_p(convert::nbh_to_cse(this->marker.nbh_get()));
	}

	bool is_stable()
	{
	  // same explanation as above
	  return level::is_equal(this->work, this->output);
	}

	void impl_run()
	{
	  this->preconditions();
	  this->init();
	  for (;;)
	    {
	      for_all_p (fwd_p)
		this->fwd_loop_body();
	      for_all_p (bkd_p)
		this->bkd_loop_body();
	      if (this->is_stable())
		return;
	      work = utils::clone(this->output);
	    }
	}

      protected:

	generic_reconstruction_canvas(const abstract::image_with_nbh<I1>& marker,
				      const abstract::image<I2>& mask) :
	  super_type(marker, mask),
	  bkd_p(marker.size()),
	  fwd_p(marker.size())
	{
	}


	oln_type_of(I1, bkd_piter) bkd_p;
	oln_type_of(I1, fwd_piter) fwd_p;

	oln_nbh_type_of(nbh_type, window) win_plus;
	oln_nbh_type_of(nbh_type, window) win_minus;

	box<oln_type_of(I1, concrete)> work;
      };

      template <typename I1, typename I2, typename E>
      struct generic_reconstruction_canvas<I1, I2, tag::hybrid, E> :
	public reconstruction_ret<I1, I2>
      {
	typedef reconstruction_ret<I1, I2> super_type;
	typedef oln_type_of(I1, neighb) nbh_type;
	typedef oln_nbh_type_of(nbh_type, window) window_type;
	typedef oln_type_of(I1, concrete) output_type;

	E& exact__()
	{
	  return *(E*)(void*)(this);
	}


	bool exist_init()
	{
	  for (unsigned i = 0; i < win_minus.card(); ++i)
	    {
	      q = win_minus.get_dp()[i] + (oln_type_of(I1, point))bkd_p;
	      if (this->exact__().impl_exist_init())
		return true;
	    }
	  return false;
	}

	void bkd_loop_body()
	{
	  this->exact__().impl_bkd_loop_body();
	}
	void fwd_loop_body()
	{
	  this->exact__().impl_fwd_loop_body();
	}

	void fifo_loop_body()
	{
	  this->exact__().impl_fifo_loop_body();
	}

	void preconditions()
	{
	  mlc::eq<oln_type_of(I1, grid), oln_type_of(I2, grid)>::ensure();
	  precondition(this->marker.size() == this->mask.size());
	  this->exact__().impl_preconditions();
	}

	void init()
	{
	  output_type tmp(this->marker.size());

	  this->output = tmp;
	  this->work = utils::clone(this->marker);

	  win_plus = get_plus_win_p(convert::nbh_to_cse(this->marker.nbh_get()));
	  win_minus = get_minus_win_p(convert::nbh_to_cse(this->marker.nbh_get()));
	}

	void impl_run()
	{
	  this->preconditions();
	  this->init();

	  for_all_p (fwd_p)
	    this->fwd_loop_body();

	  for_all_p (bkd_p)
	    {
	      bkd_loop_body();
	      if (exist_init())
		fifo.push(bkd_p);
	    }
	  // Propagation Step
	  while (!fifo.empty())
	    {
	      p = fifo.front();
	      fifo.pop();

	      window_type win = convert::nbh_to_se(this->marker.nbh_get());
	      for (unsigned i = 0; i < win.card(); ++i)
		{
		  q = win.get_dp()[i] + p;

		  if (this->output.hold(q))
		    fifo_loop_body();
		}
	    }
	}

      protected:

	generic_reconstruction_canvas(const abstract::image_with_nbh<I1>& marker,
				      const abstract::image<I2>& mask) :
	  super_type(marker, mask),
	  bkd_p(marker.size()),
	  fwd_p(marker.size())
	{
	}


	oln_type_of(I1, bkd_piter) bkd_p;
	oln_type_of(I1, fwd_piter) fwd_p;

	oln_type_of(I1, point) p;
	oln_type_of(I1, point) q;

	window_type win_plus;
	window_type win_minus;

	box<oln_type_of(I1, concrete)> work;

	std::queue<oln_type_of(I1, point) > fifo;

      };

    } // end of namespace impl

  } // end of namespace morpho

} // end of namespace oln
