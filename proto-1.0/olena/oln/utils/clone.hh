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

# include <oln/basics.hh>

namespace oln {

  // fwd decl
  namespace utils {
    namespace impl {
      template <typename I> struct clone_type;
    }
  }

  // category
  template <typename I>
  struct set_category < utils::impl::clone_type<I> >
  {
    typedef category::image ret;
  };

  // super_type
  template <typename I>
  struct set_super_type < utils::impl::clone_type<I> >
  {
    typedef abstract::image_unary_operator<I, I, utils::impl::clone_type<I> > ret;
  };



  namespace utils {

    namespace impl {

      template <typename I>
      struct clone_type : public abstract::image_unary_operator<I, I, clone_type<I> >
      // FIXME: use concrete_type; Cf. erosion.hh
      {
	typedef abstract::image_unary_operator<I, I, clone_type<I> > super_type;

	clone_type(const abstract::image<I>& input) :
	  super_type(input)
	{
	}

	void impl_run()
	{
	  I tmp(this->input.size()); // FIXME: trick
	  this->output = tmp;

	  oln_type_of(I, fwd_piter) p(this->input.size());
	  for_all(p)
	    this->output[p] = this->input[p];
	}
      };

    } // end of namespace oln::utils::impl


    template <typename I>
    impl::clone_type<I> clone(const abstract::image<I>& ima)
    {
      impl::clone_type<I> tmp(ima.exact());
      tmp.run();
      return tmp;
    }

  } // end of namespace oln::utils

} // end of namespace oln


#endif // ! OLN_UTILS_CLONE_HH
