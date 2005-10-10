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

#ifndef OLENA_MORPHO_LOCAL_HH
# define OLENA_MORPHO_LOCAL_HH

# include <mlc/is_a.hh>
# include <ntg/core/macros.hh>
# include <oln/basics.hh>
# include <oln/funobj/accum.hh>
# include <oln/funobj/infsup.hh>


namespace oln {

  namespace morpho {


    /// Local image "inf-value" for erosion on functions (based on the neighborhood).

    template <typename I>
    oln_type_of(I, value) local_inf_value(const abstract::image<I>&    input,
					  const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::not_binary_image)::ensure();

      typedef oln_type_of(I, value) value_type;
      typedef f_::accum_with_init<f_::inf_<value_type>, value_type> accum_type;
      accum_type val(ntg_sup_val(value_type));

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n))
	  val(input[n]);

      return val;
    }


    /// Local image "inf-value" for erosion on sets - based on the
    /// neighborhood points that have already been processed when
    /// performing a forward scan.

    template <typename I>
    oln_type_of(I, value) local_inf_value_fwd(const abstract::image<I>&    input,
					      const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::not_binary_image)::ensure();

      typedef oln_type_of(I, value) value_type;
      typedef f_::accum_with_init<f_::inf_<value_type>, value_type> accum_type;
      accum_type val(ntg_sup_val(value_type));

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and p.fwd_less(n))
	  val(input[n]);

      return val;
    }


    /// Local image "inf-value" for erosion on sets - based on the
    /// neighborhood points that have already been processed when
    /// performing a forward scan.

    template <typename I>
    oln_type_of(I, value) local_inf_value_bkd(const abstract::image<I>&    input,
					      const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::not_binary_image)::ensure();

      typedef oln_type_of(I, value) value_type;
      typedef f_::accum_with_init<f_::inf_<value_type>, value_type> accum_type;
      accum_type val(ntg_sup_val(value_type));

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and p.bkd_less(n))
	  val(input[n]);

      return val;
    }


    /// Local image "sup-value" for dilation on functions (based on the neighborhood).

    template <typename I>
    oln_type_of(I, value) local_sup_value(const abstract::image<I>& input,
					  const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::not_binary_image)::ensure();

      typedef oln_type_of(I, value) value_type;
      typedef f_::accum_with_init<f_::sup_<value_type>, value_type> accum_type;
      accum_type val(ntg_inf_val(value_type));

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n))
	  val(input[n]);

      return val;
    }


    /// Local image "sup-value" for erosion on sets - based on the
    /// neighborhood points that have already been processed when
    /// performing a forward scan.

    template <typename I>
    oln_type_of(I, value) local_sup_value_fwd(const abstract::image<I>&    input,
					      const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::not_binary_image)::ensure();

      typedef oln_type_of(I, value) value_type;
      typedef f_::accum_with_init<f_::sup_<value_type>, value_type> accum_type;
      accum_type val(ntg_inf_val(value_type));

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and p.fwd_less(n))
	  val(input[n]);

      return val;
    }


    /// Local image "sup-value" for erosion on sets - based on the
    /// neighborhood points that have already been processed when
    /// performing a forward scan.

    template <typename I>
    oln_type_of(I, value) local_sup_value_bkd(const abstract::image<I>&    input,
					      const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::not_binary_image)::ensure();

      typedef oln_type_of(I, value) value_type;
      typedef f_::accum_with_init<f_::sup_<value_type>, value_type> accum_type;
      accum_type val(ntg_inf_val(value_type));

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and p.bkd_less(n))
	  val(input[n]);

      return val;
    }


    /// Local image "and-value" for erosion on sets (based on the neighborhood).

    template <typename I>
    oln_type_of(I, value) local_and_value(const abstract::image<I>& input,
					  const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::binary_image)::ensure();

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and not input[n])
	  return false;

      return true;
    }


    /// Local image "and-value" for erosion on sets - based on the
    /// neighborhood points that have already been processed when
    /// performing a forward scan.

    template <typename I>
    oln_type_of(I, value) local_and_value_fwd(const abstract::image<I>& input,
					      const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::binary_image)::ensure();

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and p.fwd_less(n) and not input[n])
	  return false;

      return true;
    }


    /// Local image "and-value" for erosion on sets - based on the
    /// neighborhood points that have already been processed when
    /// performing a backward scan.

    template <typename I>
    oln_type_of(I, value) local_and_value_bkd(const abstract::image<I>& input,
					      const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::binary_image)::ensure();

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and p.bkd_less(n) and not input[n])
	  return false;

      return true;
    }



    /// Local image "or-value" for dilation on sets (based on the neighborhood).

    template <typename I>
    oln_type_of(I, value) local_or_value(const abstract::image<I>& input,
					 const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::binary_image)::ensure();

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and input[n])
	  return true;

      return false;
    }


    /// Local image "or-value" for erosion on sets - based on the
    /// neighborhood points that have already been processed when
    /// performing a forward scan.

    template <typename I>
    oln_type_of(I, value) local_or_value_fwd(const abstract::image<I>& input,
					      const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::binary_image)::ensure();

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and p.fwd_less(n) and input[n])
	  return true;

      return false;
    }


    /// Local image "or-value" for erosion on sets - based on the
    /// neighborhood points that have already been processed when
    /// performing a backward scan.

    template <typename I>
    oln_type_of(I, value) local_or_value_bkd(const abstract::image<I>& input,
					      const oln_type_of(I, point)& p)
    {
      mlc_is_a(I, abstract::image_with_nbh)::ensure();
      mlc_is_a(I, abstract::binary_image)::ensure();

      oln_type_of(I, niter) n(input);
      for_all_n_of_p(n, p)
	if (input.hold(n) and p.bkd_less(n) and input[n])
	  return true;

      return false;
    }



    /// Image "inf-value" in a window for erosion on functions.

    template <typename I, typename W>
    oln_type_of(I, value) win_inf_value(const abstract::image<I>&    input,
					const oln_type_of(I, point)& p,
					const abstract::window<W>&   win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
      mlc_is_a(I, abstract::not_binary_image)::ensure();

      typedef oln_type_of(I, value) value_type;
      typedef f_::accum_with_init<f_::inf_<value_type>, value_type> accum_type;
      accum_type val(ntg_sup_val(value_type));

      oln_wn_type_of(W, fwd_iter) q(win);
      for_all_q_of_p (q, p)
	if (input.hold(q))
	  val(input[q]);

      return val;
    }

    /// Image "sup-value" in a window for dilation on functions.

    template <typename I, typename W>
    oln_type_of(I, value) win_sup_value(const abstract::image<I>&    input,
					const oln_type_of(I, point)& p,
					const abstract::window<W>&   win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
      mlc_is_a(I, abstract::not_binary_image)::ensure();

      typedef oln_type_of(I, value) value_type;
      typedef f_::accum_with_init<f_::sup_<value_type>, value_type> accum_type;
      accum_type val(ntg_inf_val(value_type));

      oln_wn_type_of(W, fwd_iter) q(win);
      for_all_q_of_p (q, p)
	if (input.hold(q))
	  val(input[q]);

      return val;
    }

    /// Image "and-value" in a window for erosion on sets.

    template <typename I, typename W>
    oln_type_of(I, value) win_and_value(const abstract::image<I>&    input,
					const oln_type_of(I, point)& p,
					const abstract::window<W>&   win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
      mlc_is_a(I, abstract::binary_image)::ensure();

      oln_wn_type_of(W, fwd_iter) q(win);
      for_all_q_of_p (q, p)
	if (input.hold(q) and not input[q])
	  return false;

      return true;
    }

    /// Image "or-value" in a window for erosion on sets.

    template <typename I, typename W>
    oln_type_of(I, value) win_or_value(const abstract::image<I>&    input,
				       const oln_type_of(I, point)& p,
				       const abstract::window<W>&   win)
    {
      mlc::eq<oln_type_of(I, grid), oln_wn_type_of(W, grid)>::ensure();
      mlc_is_a(I, abstract::binary_image)::ensure();

      oln_wn_type_of(W, fwd_iter) q(win);
      for_all_q_of_p (q, p)
	if (input.hold(q) and input[q])
	  return true;

      return false;
    }



  } // end of namespace morpho

} // end of namespace oln

#endif // ! OLENA_MORPHO_LOCAL_HH
