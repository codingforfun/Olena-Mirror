// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef OLN_UTILS_CLONE_HH
# define OLN_UTILS_CLONE_HH

# include <oln/core/abstract/op.hh>
# include <oln/core/abstract/piter.hh>

namespace oln {

  // fwd decl
  namespace utils {
      template <typename I> struct clone_type;
  }

  // category
  template <typename I>
  struct set_category < utils::clone_type<I> >
  {
    typedef category::image ret;
  };

  // super_type
  template <typename I>
  struct set_super_type < utils::clone_type<I> >
  {
    typedef abstract::op<I, utils::clone_type<I> > ret;
  };



  namespace utils {

    template <typename I>
    struct clone_type : abstract::op<I, clone_type<I> >
    {
      typedef abstract::op<I, clone_type<I> > super_type;

      mlc::box<I> input_;

      clone_type(I& input) : input_(input)
      {}

      void impl_run()
      {
	I ima(input_->size());
	oln_type_of(I, fwd_piter) it(input_->size());

	for_all(it)
	  {
	    ima[it] = (*input_)[it];
	  }

	*this->image_ = ima;
      }
    };


    template <typename I>
    clone_type<I> clone(abstract::image<I>& ima)
    {
      clone_type<I> tmp(ima.exact());
      tmp.run();
      return tmp;
    }
  }
}


#endif // ! OLN_UTILS_CLONE_HH
