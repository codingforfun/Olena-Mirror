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

#ifndef OLENA_CANVAS_FORTH_HH
# define OLENA_CANVAS_FORTH_HH

# include <oln/canvas/until_convergence.hh>
# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/piter.hh>


namespace oln {


  namespace canvas {


    template <typename I, typename E>
    struct forth_until_convergence : public until_convergence<E>
    {
      // Abstract methods.

      // Do something like 'work -> output'.
      void fwd_loop_body()
      {
	this->exact().impl_fwd_loop_body();
      }

      // Concrete method.
      void impl_loop_body()
      {

	// back
	for_all_p (fwd_p)
	  fwd_loop_body();
      }


    protected:

      // Ctor.
      forth_until_convergence(const abstract::image<I>& input) :
	fwd_p(input.size())
      {
      }

      // Attributes.
      oln_type_of(I, fwd_piter) fwd_p;

      // Check for impls..
      ~forth_until_convergence()
      {
	mlc_check_method_impl(E, void, fwd_loop_body, , );
      }

    };


    template <typename I, typename E>
    struct forth : public mlc::any<E>
    {
      // Abstract methods.

      void init()
      {
	this->exact().impl_init();
      }

      void preconditions()
      {
	this->exact().impl_preconditions();
      }

      // Do something like 'work -> output'.
      void fwd_loop_body()
      {
	this->exact().impl_fwd_loop_body();
      }

      // Concrete method.
      void run()
      {
	preconditions();
	init();

	// forth
	for_all_p (fwd_p)
	  fwd_loop_body();

      }


    protected:

      // Ctor.
      forth(const abstract::image<I>& input) :
	fwd_p(input.size())
      {
      }

      // Attributes.
      oln_type_of(I, fwd_piter) fwd_p;

      // Check for impls..
      ~forth()
      {
	mlc_check_method_impl(E, void, init,          , );
	mlc_check_method_impl(E, void, fwd_loop_body, , );
	mlc_check_method_impl(E, void, preconditions, , );
      }

    };

  } // end of namespace oln::canvas


} // end of namespace oln


#endif // ! OLENA_CANVAS_FORTH_HH
