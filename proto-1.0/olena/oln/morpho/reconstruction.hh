// Copyright (C) 2001, 2002, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_RECONSTRUCTION_HH
# define OLENA_MORPHO_RECONSTRUCTION_HH

# include <queue>

# include <mlc/cmp.hh>
# include <mlc/contract.hh>

# include <oln/convert/nbh_to_se.hh>

# include <oln/core/abstract/image_operator.hh>
# include <oln/core/abstract/neighborhood.hh>

# include <oln/level/compare.hh>

# include <oln/morpho/splitse.hh>
# include <oln/morpho/stat.hh>

# include <oln/utils/clone.hh>

// FIXME: ADD TESTS !!!!

namespace oln {

  namespace morpho {

    // fwd decl
    template <typename I, typename E> struct reconstruction_ret;

  }

  // category
  template <typename I, typename E>
  struct set_category< morpho::reconstruction_ret<I,E> > { typedef category::image ret; };

  // super_type
  template <typename I, typename E>
  struct set_super_type< morpho::reconstruction_ret<I,E> >
  {
    typedef abstract::image_binary_operator<I, I, I, morpho::reconstruction_ret<I, E> > ret;
    // FIXME: see below
  };


  namespace morpho {

    template <typename I, typename N>
    struct reconstruction_ret : public abstract::image_binary_operator<I, I, I, reconstruction_ret<I, N> >
      // FIXME: abstract::image_binary_operator<oln_type_of(I, concrete), ...
    {
      typedef abstract::image_binary_operator<I, I, I, reconstruction_ret<I, N> > super_type;

      const N nbh;

      reconstruction_ret(const abstract::image<I>& input1,
			 const abstract::image<I>& input2,
			 const abstract::neighborhood<N>& nbh) :
	super_type(input1.exact(), input2.exact()),
	nbh(nbh.exact())
      {}

    };

    namespace sequential {

      namespace impl {

	template <typename I, typename N, typename E>
	struct reconstruction_sequential_ret : public reconstruction_ret<I, N>
	{
	  typedef reconstruction_ret<I, N> super_type;

	  void fwd_loop_body()
	  {
	    static_cast<E*>((void*)this)->fwd_loop_body_impl();
	  }

	  void bkd_loop_body()
	  {
	    static_cast<E*>((void*)this)->bkd_loop_body_impl();
	  }

	  void preconditions()
	  {
	    precondition(this->input1.size() == this->input2.size());
	    static_cast<E*>((void*)this)->preconditions_impl();
	  }

	  void impl_run()
	  {
	    mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();

	    I output;
	    output = utils::clone(this->input1);
	    bool non_stability = true;
	    while (non_stability)
	      {
		work.unbox() = utils::clone(output);
		for_all_p (fwd_p)
		  fwd_loop_body();
		for_all_p (bkd_p)
		  bkd_loop_body();

		non_stability = !(level::is_equal(work, output));
		output = work.unbox();
	      }
	    this->output = output;
	  }

	protected:
	  reconstruction_sequential_ret(const abstract::image<I>& input1, //marker
					const abstract::image<I>& input2, //mask
					const abstract::neighborhood<N>& nbh)
	    : super_type(input1, input2, nbh),
	      fwd_p(input1.size()),
	      bkd_p(input1.size())
	  {
	    se_plus = get_plus_se_p(convert::nbh_to_cse(this->nbh));
	    se_minus = get_minus_se_p(convert::nbh_to_cse(this->nbh));
	  }

	  oln_type_of(N, window) se_plus;
	  oln_type_of(N, window) se_minus;
	  oln_type_of(I, fwd_piter) fwd_p;
	  oln_type_of(I, bkd_piter) bkd_p;
	  box<I> work;

	};

	template <typename I, typename N>
	struct reconstruction_dilation_ret :
	public reconstruction_sequential_ret<I, N, reconstruction_dilation_ret<I, N> >
	{
	  typedef reconstruction_sequential_ret<I, N, reconstruction_dilation_ret<I, N> > super_type;

	  reconstruction_dilation_ret(const abstract::image<I>& input1, //marker
				      const abstract::image<I>& input2, //mask
				      const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  void fwd_loop_body_impl()
	  {
	    this->work[this->fwd_p] = ntg::min(morpho::max(this->work.unbox(),
							   this->fwd_p,
							   this->se_plus),
					       this->input2[this->fwd_p].value());
	  }

	  void bkd_loop_body_impl()
	  {
	    this->work[this->bkd_p] = ntg::min(morpho::max(this->work.unbox(),
							   this->bkd_p,
							   this->se_minus),
					       this->input2[this->bkd_p].value());
	  }

	  void preconditions_impl()
	  {
	    precondition(level::is_greater_or_equal(this->input2, this->input1));
	  }

	};


	template <typename I, typename N>
	struct reconstruction_erosion_ret :
	public reconstruction_sequential_ret<I, N, reconstruction_erosion_ret<I, N> >
	{
	  typedef reconstruction_sequential_ret<I, N, reconstruction_erosion_ret<I, N> > super_type;

	  reconstruction_erosion_ret(const abstract::image<I>& input1, //marker
				     const abstract::image<I>& input2, //mask
				     const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  void fwd_loop_body_impl()
	  {
	    this->work[this->fwd_p] = ntg::max(morpho::min(this->work.unbox(),
							   this->fwd_p,
							   this->se_plus),
					       this->input2[this->fwd_p].value());
	  }

	  void bkd_loop_body_impl()
	  {
	    this->work[this->bkd_p] = ntg::max(morpho::min(this->work.unbox(),
							   this->bkd_p,
							   this->se_minus),
					       this->input2[this->bkd_p].value());
	  }

	  void preconditions_impl()
	  {
	    precondition(level::is_greater_or_equal(this->input1, this->input2));
	  }

	};

      }

      template<class I, class N>
      reconstruction_ret<I, N>
      geodesic_reconstruction_dilation(const abstract::image<I> & marker,
				       const abstract::image<I> & mask,
				       const abstract::neighborhood<N>& nbh)
      {
	impl::reconstruction_dilation_ret<I, N> tmp(marker, mask, nbh);
	tmp.run();
	return tmp;
      }

      template<class I, class N>
      reconstruction_ret<I, N>
      geodesic_reconstruction_erosion(const abstract::image<I> & marker,
				      const abstract::image<I> & mask,
				      const abstract::neighborhood<N>& nbh)
      {
	impl::reconstruction_erosion_ret<I, N> tmp(marker, mask, nbh);
	tmp.run();
	return tmp;
      }

    }// sequential


    namespace hybrid {

      namespace impl {

	template <typename I, typename N, typename E>
	struct reconstruction_hybrid_ret : public reconstruction_ret<I, N>
	{
	  typedef reconstruction_ret<I, N> super_type;

	  bool exist_init()
	  {
	    // FIXME: to many changes => rewrite!
// 	    typedef oln_type_of(N, window) se_type;
// 	    oln_type_of(se_type, fwd_qiter) dp(se_minus);
// 	    for_all (dp)
// 	      {
// 		q = (oln_type_of(se_type, dpoint))dp +
// 		  (oln_type_of(I, point))bkd_p;
// 		if (static_cast<E*>((void*)this)->exist_init_impl())
// 		  return true;
// 	      }
	    return false;
	  }

	  void fwd_loop_body()
	  {
	    static_cast<E*>((void*)this)->fwd_loop_body_impl();
	  }

	  void bkd_loop_body()
	  {
	    static_cast<E*>((void*)this)->bkd_loop_body_impl();
	  }

	  void fifo_loop_body()
	  {
	    static_cast<E*>((void*)this)->fifo_loop_body_impl();
	  }

	  void preconditions()
	  {
	    precondition(this->input1.size() == this->input2.size());
	    static_cast<E*>((void*)this)->preconditions_impl();
	  }

	  void impl_run()
	  {
	    mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();
	    preconditions();

	    this->output.unbox() = utils::clone(this->input1);

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
		// FIXME: AWFUL commented cause too many changes!
// 		typedef oln_type_of(N, window) window_type;
// 		window_type w = convert::nbh_to_se(this->nbh);
// 		oln_wn_type_of(window_type, fwd_iter) q(w);

// 		for_all_q_of_p (q)
// 		  {
// 		    if (this->output.hold(q))
// 		      fifo_loop_body();
// 		  }
	      }
	  }

	protected:

	  reconstruction_hybrid_ret(const abstract::image<I>& input1, //marker
				    const abstract::image<I>& input2, //mask
				    const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh),
	      fwd_p(input1.size()),
	      bkd_p(input1.size())
	  {
	    se_plus = get_plus_se_p(convert::nbh_to_cse(this->nbh));
	    se_minus = get_minus_se_p(convert::nbh_to_cse(this->nbh));
	  }

	  oln_type_of(N, window) se_plus;
	  oln_type_of(N, window) se_minus;
	  oln_type_of(I, fwd_piter) fwd_p;
	  oln_type_of(I, bkd_piter) bkd_p;
	  oln_type_of(I, point) p;
	  oln_type_of(I, point) q;
	  std::queue<oln_type_of(I, point) > fifo;


	};


	template <typename I, typename N>
	struct reconstruction_dilation_ret :
	  public reconstruction_hybrid_ret<I, N, reconstruction_dilation_ret<I, N> >
	{
	  typedef reconstruction_hybrid_ret<I, N, reconstruction_dilation_ret<I, N> > super_type;

	  reconstruction_dilation_ret(const abstract::image<I>& input1, //marker
				      const abstract::image<I>& input2, //mask
				      const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  void fwd_loop_body_impl()
	  {
	    this->output[this->fwd_p] = ntg::min(morpho::max(this->output.unbox(),
							     this->fwd_p,
							     this->se_plus),
						 this->input2[this->fwd_p].value());
	  }

	  void bkd_loop_body_impl()
	  {
	    this->output[this->bkd_p] = ntg::min(morpho::max(this->output.unbox(),
							     this->bkd_p,
							     this->se_minus),
						 this->input2[this->bkd_p].value());
	  }

	  void fifo_loop_body_impl()
	  {
	    if ((this->output[this->q] < this->output[this->p]) &&
		(this->input2[this->q] != this->output[this->q]))
	      {
		this->output[this->q] = ntg::min(this->output[this->p].value(),
						 this->input2[this->q].value());
		this->fifo.push(this->q);
	      }
	  }

	  bool exist_init_impl()
	  {
	    return this->output.hold(this->q) &&
	      (this->output[this->q] < this->output[this->bkd_p]) &&
	      (this->output[this->q] < this->input2[this->q]);
	  }

	  void preconditions_impl()
	  {
	    precondition(level::is_greater_or_equal(this->input2, this->input1));
	  }

	};



	template <typename I, typename N>
	struct reconstruction_erosion_ret :
	  public reconstruction_hybrid_ret<I, N, reconstruction_erosion_ret<I, N> >
	{
	  typedef reconstruction_hybrid_ret<I, N, reconstruction_erosion_ret<I, N> > super_type;

	  reconstruction_erosion_ret(const abstract::image<I>& input1, //marker
				      const abstract::image<I>& input2, //mask
				      const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  void fwd_loop_body_impl()
	  {
	    this->output[this->fwd_p] = ntg::max(morpho::min(this->output.unbox(),
							     this->fwd_p,
							     this->se_plus),
						 this->input2[this->fwd_p].value());
	  }

	  void bkd_loop_body_impl()
	  {
	    this->output[this->bkd_p] = ntg::max(morpho::min(this->output.unbox(),
							     this->bkd_p,
							     this->se_minus),
						 this->input2[this->bkd_p].value());
	  }

	  void fifo_loop_body_impl()
	  {
	    if ((this->output[this->q] > this->output[this->p]) &&
		(this->input2[this->q] != this->output[this->q]))
	      {
		this->output[this->q] = ntg::max(this->output[this->p].value(),
						 this->input2[this->q].value());
		this->fifo.push(this->q);
	      }
	  }

	  bool exist_init_impl()
	  {
	    return this->output.hold(this->q) &&
		    (this->output[this->q] > this->output[this->bkd_p]) &&
		    (this->output[this->q] > this->input2[this->q]);
	  }

	  void preconditions_impl()
	  {
	    precondition(level::is_greater_or_equal(this->input1, this->input2));
	  }

	};

      }

      template<class I, class N>
      reconstruction_ret<I, N>
      geodesic_reconstruction_dilation(const abstract::image<I> & marker,
				       const abstract::image<I> & mask,
				       const abstract::neighborhood<N>& nbh)
      {
	impl::reconstruction_dilation_ret<I, N> tmp(marker, mask, nbh);
	tmp.run();
	return tmp;
      }

      template<class I, class N>
      reconstruction_ret<I, N>
      geodesic_reconstruction_erosion(const abstract::image<I> & marker,
				      const abstract::image<I> & mask,
				      const abstract::neighborhood<N>& nbh)
      {
	impl::reconstruction_erosion_ret<I, N> tmp(marker, mask, nbh);
	tmp.run();
	return tmp;
      }
    }// hybrid

  }

}

#endif // ! OLENA_MORPHO_RECONSTRUCTION_HH
