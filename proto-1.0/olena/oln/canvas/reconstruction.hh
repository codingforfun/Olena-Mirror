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
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLENA_CANVAS_RECONSTRUCTION_HH
# define OLENA_CANVAS_RECONSTRUCTION_HH

# include <queue>

# include <oln/canvas/backandforth.hh>
# include <oln/level/compare.hh>
# include <oln/utils/clone.hh>

namespace oln {

  namespace morpho {

    namespace impl {

      template <typename T, typename A, typename I1, typename I2>
      struct reconstruction {};

    }

  }

  namespace canvas {

    template <typename I1, typename I2, typename E>
    struct hybrid_reconstruction : public mlc::any<E>
    {

      typedef oln_type_of(I1, neighb) nbh_type;
      typedef oln_nbh_type_of(nbh_type, window) window_type;
      typedef oln_type_of(I1, concrete) output_type;

      bool exist_init()
      {
	for (unsigned i = 0; i < win_minus.card(); ++i)
	  {
	    q = win_minus.get_dp()[i] + (oln_type_of(I1, point))bkd_p;
	    if (this->exact().impl_exist_init())
	      return true;
	  }
	return false;
      }


      void fwd_loop_body()
      {
	this->exact().impl_fwd_loop_body();
      }

      void bkd_loop_body()
      {
	this->exact().impl_bkd_loop_body();
      }

      void fifo_loop_body()
      {
	this->exact().impl_fifo_loop_body();
      }

      void init()
      {
	output_type output_tmp(marker.size());
	output = output_tmp;
	// FIXME: We can't use `work = clone(marker)' directly here,
	// because box's op=(const abstract::image<II>& rhs) would be
	// called, which is empty (see oln/core/box.hh).
	output_type work_tmp(clone(marker));
	work = work_tmp;

	win_plus = marker.nbh_get().get_win().get_fwd_win_p();
	win_minus = marker.nbh_get().get_win().get_bkd_win_p();
      }

      void run()
      {
	init();

	for_all_p (fwd_p)
	  fwd_loop_body();

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

	    window_type win = marker.nbh_get().get_win();
	    for (unsigned i = 0; i < win.card(); ++i)
	      {
		q = win.get_dp()[i] + p;

		if (output.hold(q))
		  fifo_loop_body();
	      }
	  }
      }

      oln_type_of(I1, concrete) get_output()
      {
	return output.unbox();
      }

    protected:

      hybrid_reconstruction(const abstract::image_with_nbh<I1>& marker,
			    const abstract::image<I2>& mask) :
	marker(marker.exact()),
	mask(mask.exact()),
	bkd_p(marker.size()),
	fwd_p(marker.size())
      {
      }

      ~hybrid_reconstruction()
      {
	mlc_check_method_impl(E, bool, exist_init,	, );
	mlc_check_method_impl(E, void, bkd_loop_body,	, );
	mlc_check_method_impl(E, void, fwd_loop_body,	, );
	mlc_check_method_impl(E, void, fifo_loop_body,	, );
      }

      oln_type_of(I1, point) p;
      oln_type_of(I1, point) q;

      window_type win_plus;
      window_type win_minus;

      box<oln_type_of(I1, concrete)> work;
      box<oln_type_of(I1, concrete)> output;
      box<const I1> marker;
      box<const I2> mask;

      oln_type_of(I1, bkd_piter) bkd_p;
      oln_type_of(I1, fwd_piter) fwd_p;

      std::queue<oln_type_of(I1, point) > fifo;

    };

    template <typename I1, typename I2, typename E>
    struct sequential_reconstruction :
      public back_and_forth_until_convergence<I1, E>
    {
      typedef back_and_forth_until_convergence<I1, E> super_type;
      typedef oln_type_of(I1, neighb) nbh_type;
      typedef oln_nbh_type_of(nbh_type, window) window_type;
      typedef oln_type_of(I1, concrete) output_type;

      void impl_init()
      {
	// FIXME: We can't use `output = clone(marker)' directly here,
	// because box's op=(const abstract::image<II>& rhs) would be
	// called, which is empty (see oln/core/box.hh).
	output_type output_tmp(clone(marker));
	output = output_tmp;
	// FIXME: We can't use `work = clone(marker)' directly here,
	// because box's op=(const abstract::image<II>& rhs) would be
	// called, which is empty (see oln/core/box.hh).
	output_type work_tmp(clone(marker));
	work = work_tmp;

	win_plus = marker.nbh_get().get_win().get_fwd_win_p();
	win_minus = marker.nbh_get().get_win().get_bkd_win_p();
      }

      bool impl_is_stable() const
      {
	return level::is_equal(work, output);
      }

      void impl_re_loop()
      {
	// FIXME: We can't use `output = clone(marker)' directly here,
	// because box's op=(const abstract::image<II>& rhs) would be
	// called, which is empty (see oln/core/box.hh).
	output_type work_tmp(clone(marker));
	work = work_tmp;
      }

      oln_type_of(I1, concrete) get_output()
      {
	return output.unbox();
      }

      sequential_reconstruction(const abstract::image_with_nbh<I1>& marker,
				const abstract::image<I2>& mask) :
	super_type(marker),
	marker(marker.exact()),
	mask(mask.exact())
      {
      }

      oln_nbh_type_of(nbh_type, window) win_plus;
      oln_nbh_type_of(nbh_type, window) win_minus;

      box<oln_type_of(I1, concrete)> work;
      box<oln_type_of(I1, concrete)> output;
      box<const I1> marker;
      box<const I2> mask;

    };


  }


}


#endif // ! OLENA_CANVAS_RECONSTRUCTION_HH
