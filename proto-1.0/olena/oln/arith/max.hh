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

#ifndef OLENA_ARITH_MAX_HH
# define OLENA_ARITH_MAX_HH

# include <oln/basics.hh>
# include <ntg/all.hh>

namespace oln {

  namespace arith {
        // fwd decl
    namespace impl {
      template <typename I> struct max_type;
    }

  }

  // category
  template <typename I>
  struct set_category< arith::impl::max_type<I> >
  {
    typedef category::image ret;
  };

  // super_type
  template <typename I>
  struct set_super_type< arith::impl::max_type<I> >
  {
    typedef abstract::op<I, arith::impl::max_type<I> > ret;
  };

  namespace arith {

    namespace impl {

      template <class I>
      struct max_type : abstract::op<I, max_type<I> >
      {
	box<const I> input1_;
	box<const I> input2_;

	max_type(const abstract::non_vectorial_image<I>& input1,
		 const abstract::non_vectorial_image<I>& input2) :
	  input1_(input1.exact()),
	  input2_(input2.exact())
	{}

	void impl_run()
	{
	  precondition(input1_.size() == input2_.size());
	  I output(input1_.size());
	  oln_type_of(I, fwd_piter) p(input1_.size());

	  for_all(p)
	    output[p] = ntg::max(input1_[p].value(), input2_[p].value());

	  *this->image_ = output; // FIXME: remove * when image_ is oln::box
	}

      };

    }

    template <typename I>
    impl::max_type<I> max(const abstract::non_vectorial_image<I>& input1,
			  const abstract::non_vectorial_image<I>& input2)
    {
      impl::max_type<I> tmp(input1, input2);
      tmp.run();
      return tmp;
    }

  }

} // end of namespace oln


#endif // ! OLENA_ARITH_MAX_HH
