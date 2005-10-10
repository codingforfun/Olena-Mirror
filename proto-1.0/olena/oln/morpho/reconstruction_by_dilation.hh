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

#ifndef OLENA_MORPHO_RECONSTRUCTION_BY_DILATION_HH
# define OLENA_MORPHO_RECONSTRUCTION_BY_DILATION_HH

# include <oln/canvas/reconstruction.hh>
# include <oln/morpho/tags.hh>
# include <oln/morpho/local.hh>
# include <oln/funobj/arith.hh>

namespace oln {

  namespace morpho {

    namespace impl {


      // BINARY

      template <typename I1, typename I2, typename A, typename E>
      struct binary_reconstruction_by_dilation
	: public canvas::reconstruction<I1, I2, A, E>
      {
	typedef canvas::reconstruction<I1, I2, A, E> super_type;

	using super_type::mask;
	using super_type::marker;
	using super_type::output;
	using super_type::bkd_p;
	using super_type::fwd_p;
	using super_type::n;
	using super_type::p;


	void impl_bkd_loop_body()
	{
	  output[bkd_p] = mask[bkd_p] and
	    (output[bkd_p] or local_or_value_bkd(join(output, marker.nbh_get()), bkd_p));
	}

	void impl_fwd_loop_body()
	{
	  output[fwd_p] = mask[fwd_p] and
	    (output[fwd_p] or local_or_value_fwd(join(output, marker.nbh_get()), fwd_p));
	}

	binary_reconstruction_by_dilation(const abstract::binary_image<I1>& marker,
					  const abstract::binary_image<I2>& mask) :
	  super_type(marker, mask)
	{
	  mlc_is_a(I1, abstract::image_with_nbh)::ensure();
	}

      };

      // Hybrid version

      template<typename I1, typename I2>
      struct binary_reconstruction <I1, I2, tag::hybrid_type, tag::by_dilation_type>
	: public binary_reconstruction_by_dilation<I1, I2, tag::hybrid_type,
		binary_reconstruction<I1, I2, tag::hybrid_type, tag::by_dilation_type> >
      {
	typedef binary_reconstruction<I1, I2, tag::hybrid_type,
				      tag::by_dilation_type> self_type;
	typedef binary_reconstruction_by_dilation<I1, I2, tag::hybrid_type,
						  self_type> super_type;

	using super_type::mask;
	using super_type::marker;
	using super_type::output;
	using super_type::bkd_p;
	using super_type::n;
	using super_type::p;
	using super_type::fifo;


	void impl_preconditions() const
	{
	  precondition(level::is_greater_or_equal(mask, marker));
	}

	void impl_fifo_loop_body()
	{
	  // output[n] < output[p] doesn't make sense with binary types.
	  if ((output[n] == false and output[p] == true) and (mask[n] != output[n]))
	    {
	      output[n] = output[p] and mask[n];
	      fifo.push(n);
	    }
	}

	bool impl_test_fifo_push()
	{
	  return (output[n] == false and output[bkd_p] == true) and
	    (output[n] == false and mask[n] == true);
	}


	binary_reconstruction(const abstract::binary_image<I1>& marker,
			      const abstract::binary_image<I2>& mask) :
	  super_type(marker, mask)
	{
	  mlc_is_a(I1, abstract::image_with_nbh)::ensure();
	}

      };


      // Sequential version

      template<typename I1, typename I2>
      struct binary_reconstruction <I1, I2, tag::sequential_type, tag::by_dilation_type>
	: public binary_reconstruction_by_dilation<I1, I2, tag::sequential_type,
						   binary_reconstruction<I1, I2, tag::sequential_type,
									 tag::by_dilation_type> >
      {
	typedef binary_reconstruction<I1, I2, tag::sequential_type,
			       tag::by_dilation_type> self_type;
	typedef binary_reconstruction_by_dilation<I1, I2, tag::sequential_type,
					  self_type> super_type;

	using super_type::mask;
	using super_type::marker;

	void impl_preconditions() const
	{
	  precondition(level::is_greater_or_equal(mask, marker));
	}


	binary_reconstruction(const abstract::binary_image<I1>& marker,
			      const abstract::binary_image<I2>& mask) :
	  super_type(marker, mask)
	{
	  mlc_is_a(I1, abstract::image_with_nbh)::ensure();
	}

      };


      // Parallel version

      template <typename I1, typename I2>
      struct binary_reconstruction<I1, I2, tag::parallel_type, tag::by_dilation_type>
	: public canvas::reconstruction<I1, I2, tag::parallel_type,
		binary_reconstruction<I1, I2, tag::parallel_type, tag::by_dilation_type> >
      {
	typedef binary_reconstruction<I1, I2, tag::parallel_type,
				      tag::by_dilation_type> self_type;
	typedef canvas::reconstruction<I1, I2, tag::parallel_type,
				       self_type> super_type;

	using super_type::mask;
	using super_type::marker;
	using super_type::save;
	using super_type::output;
	using super_type::fwd_p;

	void impl_first_step()
	{
	  output[fwd_p] = save[fwd_p] or
	    local_or_value(join(save, marker.nbh_get()), fwd_p);
	}

	void impl_second_step()
	{
	  output[fwd_p] = mask[fwd_p] and output[fwd_p];
	}

	void impl_preconditions() const
	{
	  precondition(level::is_greater_or_equal(mask, marker));
	}

	binary_reconstruction(const abstract::binary_image<I1>& marker,
			      const abstract::binary_image<I2>& mask) :
	  super_type(marker, mask)
	{
	  mlc_is_a(I1, abstract::image_with_nbh)::ensure();
	}

      };



      // GREYLEVEL

      template <typename I1, typename I2, typename A, typename E>
      struct greylevel_reconstruction_by_dilation
	: public canvas::reconstruction<I1, I2, A, E>
      {
	typedef canvas::reconstruction<I1, I2, A, E> super_type;
	typedef oln_type_of(I1, value) value_type;
	typedef f_::accum_with_init<f_::sup_<value_type>, value_type> accum_type;

	using super_type::mask;
	using super_type::marker;
	using super_type::output;
	using super_type::bkd_p;
	using super_type::fwd_p;
	using super_type::n;
	using super_type::p;

	void impl_bkd_loop_body()
	{
	  value_type nbh_max =
	    f_max_alt(output[bkd_p].value(),
		      local_sup_value_bkd(join(output, marker.nbh_get()), bkd_p));
	  output[bkd_p] = f_min_alt(mask[bkd_p].value(), nbh_max);
	}

	void impl_fwd_loop_body()
	{
	  value_type nbh_max =
	    f_max_alt(output[fwd_p].value(),
		      local_sup_value_fwd(join(output, marker.nbh_get()), fwd_p));
	  output[fwd_p] = f_min_alt(mask[fwd_p].value(), nbh_max);
	}

	greylevel_reconstruction_by_dilation(const abstract::greylevel_image<I1>& marker,
					     const abstract::greylevel_image<I2>& mask) :
	  super_type(marker, mask)
	{
	  mlc_is_a(I1, abstract::image_with_nbh)::ensure();
	}

      };

      // Hybrid version

      template<typename I1, typename I2>
      struct greylevel_reconstruction <I1, I2, tag::hybrid_type, tag::by_dilation_type>
	: public greylevel_reconstruction_by_dilation<I1, I2, tag::hybrid_type,
		greylevel_reconstruction<I1, I2, tag::hybrid_type, tag::by_dilation_type> >
      {
	typedef greylevel_reconstruction<I1, I2, tag::hybrid_type,
					 tag::by_dilation_type> self_type;
	typedef greylevel_reconstruction_by_dilation<I1, I2, tag::hybrid_type,
						     self_type> super_type;

	using super_type::mask;
	using super_type::marker;
	using super_type::output;
	using super_type::bkd_p;
	using super_type::n;
	using super_type::p;
	using super_type::fifo;


	void impl_preconditions() const
	{
	  precondition(level::is_greater_or_equal(mask, marker));
	}

	void impl_fifo_loop_body()
	{
	  if ((output[n] < output[p]) and (mask[n] != output[n]))
	    {
	      output[n] = f_min_alt(output[p].value(), mask[n].value());
	      fifo.push(n);
	    }
	}

	bool impl_test_fifo_push()
	{
	  return (output[n] < output[bkd_p]) and (output[n] < mask[n]);
	}


	greylevel_reconstruction(const abstract::greylevel_image<I1>& marker,
				 const abstract::greylevel_image<I2>& mask) :
	  super_type(marker, mask)
	{
	  mlc_is_a(I1, abstract::image_with_nbh)::ensure();
	}

      };


      // Sequential version

      template<typename I1, typename I2>
      struct greylevel_reconstruction <I1, I2, tag::sequential_type, tag::by_dilation_type>
	: public greylevel_reconstruction_by_dilation<I1, I2, tag::sequential_type,
						      greylevel_reconstruction<I1, I2, tag::sequential_type,
									       tag::by_dilation_type> >
      {
	typedef greylevel_reconstruction<I1, I2, tag::sequential_type,
					 tag::by_dilation_type> self_type;
	typedef greylevel_reconstruction_by_dilation<I1, I2, tag::sequential_type,
						     self_type> super_type;

	using super_type::mask;
	using super_type::marker;

	void impl_preconditions() const
	{
	  precondition(level::is_greater_or_equal(mask, marker));
	}


	greylevel_reconstruction(const abstract::greylevel_image<I1>& marker,
				 const abstract::greylevel_image<I2>& mask) :
	  super_type(marker, mask)
	{
	  mlc_is_a(I1, abstract::image_with_nbh)::ensure();
	}

      };


      // Parallel version

      template <typename I1, typename I2>
      struct greylevel_reconstruction<I1, I2, tag::parallel_type, tag::by_dilation_type>
	: public canvas::reconstruction<I1, I2, tag::parallel_type,
		greylevel_reconstruction<I1, I2, tag::parallel_type, tag::by_dilation_type> >
      {
	typedef greylevel_reconstruction<I1, I2, tag::parallel_type,
				      tag::by_dilation_type> self_type;
	typedef canvas::reconstruction<I1, I2, tag::parallel_type,
				       self_type> super_type;
	typedef oln_type_of(I1, value) value_type;
	typedef f_::accum_with_init<f_::sup_<value_type>, value_type> accum_type;


	using super_type::mask;
	using super_type::marker;
	using super_type::save;
	using super_type::output;
	using super_type::fwd_p;


	void impl_first_step()
	{
	  output[fwd_p] =
	    f_max_alt(output[fwd_p].value(),
		      local_sup_value(join(output, marker.nbh_get()), fwd_p));
	}

	void impl_second_step()
	{
	  output[fwd_p] = f_min_alt(mask[fwd_p].value(), output[fwd_p].value());
	}

	void impl_preconditions() const
	{
	  precondition(level::is_greater_or_equal(mask, marker));
	}

	greylevel_reconstruction(const abstract::greylevel_image<I1>& marker,
				 const abstract::greylevel_image<I2>& mask) :
	  super_type(marker, mask)
	{
	  mlc_is_a(I1, abstract::image_with_nbh)::ensure();
	}

      };

    }

  }

}


#endif // ! OLENA_MORPHO_RECONSTRUCTION_BY_DILATION_HH
