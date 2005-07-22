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
# include <oln/morpho/tags.hh>

namespace oln {

  namespace morpho {

    namespace impl {

      template <typename T, typename A, typename I1, typename I2>
      struct reconstruction {};

    }

  }

  namespace canvas {

    // fwd decl

    template <typename I1, typename I2, typename A, typename E>
    struct reconstruction;

    template <typename I1, typename I2, typename E>
    struct reconstruction<I1, I2, morpho::tag::hybrid_type, E>
      : public mlc::any<E>
    {

      typedef oln_type_of(I1, neighb) nbh_type;
      typedef oln_type_of(I1, point) point_type;
      typedef oln_nbh_type_of(nbh_type, window) window_type;
      typedef oln_type_of(I1, concrete) output_type;

      bool test_fifo_push()
      {
	p = bkd_p;
	for_all_n_of_p(n, bkd_p)
	  if (p.bkd_less(n) and output.hold(n) and this->exact().impl_test_fifo_push())
	    return true;

	return false;
      }

      void preconditions()
      {
	this->exact().impl_preconditions();
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
	// FIXME: We can't use `output = clone(marker)' directly here,
	// because box's op=(const abstract::image<II>& rhs) would be
	// called, which is empty (see oln/core/box.hh).
	output_type output_tmp(clone(marker));
	output = output_tmp;
      }

      void run()
      {
	preconditions();
	init();

	for_all_p (fwd_p)
	  fwd_loop_body();

	for_all_p (bkd_p)
	  {
	    bkd_loop_body();
	    if (test_fifo_push())
	      fifo.push(bkd_p);
	  }

	// Propagation Step
	while (!fifo.empty())
	  {
	    p = fifo.front();
	    fifo.pop();

	    for_all_n_of_p(n, p)
	      {
		if (output.hold(n))
		  fifo_loop_body();
	      }
	  }
      }

      oln_type_of(I1, concrete) get_output()
      {
	return output.unbox();
      }

    protected:

      reconstruction(const abstract::image_with_nbh<I1>& marker,
		     const abstract::image<I2>& mask) :
	marker(marker.exact()),
	mask(mask.exact()),
	bkd_p(marker.size()),
	fwd_p(marker.size()),
	n(marker)
      {
      }

      ~reconstruction()
      {
	mlc_check_method_impl(E, bool, test_fifo_push,	, );
	mlc_check_method_impl(E, void, bkd_loop_body,	, );
	mlc_check_method_impl(E, void, fwd_loop_body,	, );
	mlc_check_method_impl(E, void, fifo_loop_body,	, );
      }

      box<const I1> marker;
      box<const I2> mask;

      oln_type_of(I1, bkd_piter) bkd_p;
      oln_type_of(I1, fwd_piter) fwd_p;

      oln_type_of(I1, point) p;
      oln_type_of(I1, niter) n;

      box<oln_type_of(I1, concrete)> output;

      std::queue<oln_type_of(I1, point) > fifo;

    };




    template <typename I1, typename I2, typename E>
    struct reconstruction<I1, I2, morpho::tag::sequential_type, E> :
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
	// FIXME: We can't use `save = clone(marker)' directly here,
	// because box's op=(const abstract::image<II>& rhs) would be
	// called, which is empty (see oln/core/box.hh).
	output_type save_tmp(clone(marker));
	save = save_tmp;

      }

      bool impl_is_stable() const
      {
	return level::is_equal(save, output);
      }

      void impl_re_loop()
      {
	// FIXME: We can't use `save = clone(output)' directly here,
	// because box's op=(const abstract::image<II>& rhs) would be
	// called, which is empty (see oln/core/box.hh).
	save.unbox() = clone(output);
      }

      oln_type_of(I1, concrete) get_output()
      {
	return output.unbox();
      }

      reconstruction(const abstract::image_with_nbh<I1>& marker,
		     const abstract::image<I2>& mask) :
	super_type(marker),
	marker(marker.exact()),
	mask(mask.exact()),
	n(marker)
      {
      }

      box<oln_type_of(I1, concrete)> save;
      box<oln_type_of(I1, concrete)> output;
      box<const I1> marker;
      box<const I2> mask;
      oln_type_of(I1, point) p;
      oln_type_of(I1, niter) n;

    };


  }


}


#endif // ! OLENA_CANVAS_RECONSTRUCTION_HH
