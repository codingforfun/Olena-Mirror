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

#ifndef OLENA_ARITH_MIN_HH
# define OLENA_ARITH_MIN_HH

# include <oln/basics.hh>
# include <oln/core/abstract/image_operator.hh>

# include <ntg/all.hh>

namespace oln {

  namespace arith {
        // fwd decl
    namespace impl {
      template <typename I> struct min_type;
    }

  }

  // category
  template <typename I>
  struct set_category< arith::impl::min_type<I> >
  {
    typedef category::image ret;
  };

  // super_type
  template <typename I>
  struct set_super_type< arith::impl::min_type<I> >
  {
    typedef abstract::image_binary_operator<I, I, I, arith::impl::min_type<I> > ret;
  };

  namespace arith {

    namespace impl {

      template <class I>
      struct min_type : public abstract::image_binary_operator<I, I, I, min_type<I> >
      {
	typedef abstract::image_binary_operator<I, I, I, min_type<I> > super_type;

	min_type(const abstract::non_vectorial_image<I>& input1,
		 const abstract::non_vectorial_image<I>& input2) :
	  super_type(input1.exact(), input2.exact())
	{}

	void impl_run()
	{
	  precondition(this->input1.size() == this->input2.size());
	  I output(this->input1.size());
	  oln_type_of(I, fwd_piter) p(this->input1.size());

	  for_all(p)
	    output[p] = ntg::min(this->input1[p].value(), this->input2[p].value());

	  this->output = output;
	}

      };

    }

    // FIXME: replace non_vectorial_image by scalar_image
    // FIXME: cause arithmetics is not defined for Booleans and labels...
    template <typename I>
    impl::min_type<I> min(const abstract::non_vectorial_image<I>& input1,
			  const abstract::non_vectorial_image<I>& input2)
    {
      impl::min_type<I> tmp(input1, input2);
      tmp.run();
      return tmp;
    }

  }

} // end of namespace oln


#endif // ! OLENA_ARITH_MIN_HH
