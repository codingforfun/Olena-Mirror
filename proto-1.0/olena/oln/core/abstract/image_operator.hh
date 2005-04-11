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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_OPERATOR_HH
# define OLENA_CORE_ABSTRACT_IMAGE_OPERATOR_HH

# include <oln/core/abstract/image_like_.hh>

namespace oln {

  // fwd decls
  namespace abstract {
    template <typename O, typename E> struct image_operator;
    template <typename O, typename I, typename E> struct image_unary_operator;
    template <typename O, typename I1, typename I2, typename E> struct image_binary_operator;
  }

  // super types
  template <typename O, typename E>
  struct set_super_type< abstract::image_operator<O, E> > { typedef abstract::image_like_<O, E> ret; };
  template <typename O, typename I, typename E>
  struct set_super_type< abstract::image_unary_operator<O, I, E> > { typedef abstract::image_operator<O, E> ret; };
  template <typename O, typename I1, typename I2, typename E>
  struct set_super_type< abstract::image_binary_operator<O, I1, I2, E> > { typedef abstract::image_operator<O, E> ret; };



  namespace abstract {


    /// Class image_operator.
    // FIXME: some methods here should be defined in a real OO abstraction

    template <typename O, typename E>
    struct image_operator : public image_like_<O, E>
    {

      /// run

      void run()
      {
	this->impl_precondition();
	this->impl_before_run();
	this->impl_run();
	this->impl_after_run();

	this->image_ = this->output;

	this->impl_postcondition();
      }

      virtual void impl_run()
      {
	// impl_run is defined here so that base operators can be instantiated
	std::cerr << "oops: nothing done!" << std::endl;
      }

      virtual void impl_before_run()
      {
	// nothing
      }

      virtual void impl_after_run()
      {
	// nothing
      }

      virtual void impl_precondition()
      {
	// none
      }

      virtual void impl_postcondition()
      {
	// none
      }

      box<O> output;
      typedef O output_type;

    protected:
      image_operator() {}

    };


    /// Class image_unary_operator.

    template <typename O, typename I, typename E>
    struct image_unary_operator : public image_operator<O, E>
    {
      box<const I> input;
      typedef I input_type;

    protected:

      image_unary_operator() {}

      image_unary_operator(const abstract::image<I>& input) :
	input(input)
      {
      }
     
    };


    /// Class image_binary_operator.

    template <typename O, typename I1, typename I2, typename E>
    struct image_binary_operator : public image_operator<O, E>
    {

      box<const I1> input1;
      typedef I1 input1_type;

      box<const I2> input2;
      typedef I2 input2_type;

    protected:

      image_binary_operator() {}

      image_binary_operator(const abstract::image<I1>& input1,
			    const abstract::image<I2>& input2) :
	input1(input1),
	input2(input2)
      {
      }

    };


  } // end of namespace oln::abstract



} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_OPERATOR_HH
