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
# include <oln/core/properties.hh>

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

	template <typename I, typename N>
	struct reconstruction_sequential_ret : public reconstruction_ret<I, N>
	{
	  typedef reconstruction_ret<I, N> super_type;


	  virtual const oln_type_of(I, value) process(const I& work,
						      const oln_type_of(I, point)& p,
						      const oln_type_of(N, window)& se,
						      const oln_type_of(I, value)& v)
	  {
	    std::cerr << "oops in " << __func__ << std::endl;
	    return oln_type_of(I, value)();
	  }

	  void impl_run()
	  {
	    mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();
	    precondition(this->input1.size() == this->input2.size());
	    precondition(level::is_greater_or_equal(this->input2, this->input1));

	    // Conversion of neighborhood into a SE.
	    oln_type_of(N, window) se_plus = get_plus_se_p(convert::nbh_to_cse(this->nbh));
	    oln_type_of(N, window) se_minus = get_minus_se_p(convert::nbh_to_cse(this->nbh));

	    I output;
	    output = utils::clone(this->input1);
	    bool non_stability = true;
	    oln_type_of(I, fwd_piter) fwd_p(output.size());
	    oln_type_of(I, bkd_piter) bkd_p(output.size());
	    while (non_stability)
	      {
		I work;
		work = utils::clone(output);
		for_all (fwd_p)
		  work[fwd_p] = this->process(work, fwd_p, se_plus, this->input2[fwd_p].value());
		for_all (bkd_p)
		  work[bkd_p] = this->process(work, bkd_p, se_minus, this->input2[bkd_p].value());

		non_stability = !(level::is_equal(work, output));
		output = work;
	      }
	    this->output = output;
	  }

	protected:
	  reconstruction_sequential_ret(const abstract::image<I>& input1, //marker
					const abstract::image<I>& input2, //mask
					const abstract::neighborhood<N>& nbh)
	    : super_type(input1, input2, nbh)
	  {}


	};


	template <typename I, typename N>
	struct reconstruction_dilation_ret : public reconstruction_sequential_ret<I, N>
	{
	  typedef reconstruction_sequential_ret<I, N> super_type;

	  reconstruction_dilation_ret(const abstract::image<I>& input1, //marker
				      const abstract::image<I>& input2, //mask
				      const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  const oln_type_of(I, value) process(const I& work,
					      const oln_type_of(I, point)& p,
					      const oln_type_of(N, window)& se,
					      const oln_type_of(I, value)& v)
	  {
	    return ntg::min(morpho::max(work, p, se), v);
	  }

	};


	template <typename I, typename N>
	struct reconstruction_erosion_ret : public reconstruction_sequential_ret<I, N>
	{
	  typedef reconstruction_sequential_ret<I, N> super_type;

	  reconstruction_erosion_ret(const abstract::image<I>& input1, //marker
				     const abstract::image<I>& input2, //mask
				     const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  const oln_type_of(I, value) process(const I& work,
					      const oln_type_of(I, point)& p,
					      const oln_type_of(N, window)& se,
					      const oln_type_of(I, value)& v) const
	  {
	    return ntg::max(morpho::min(work, p, se), v);
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

	template <typename I, typename N>
	struct reconstruction_hybrid_ret : public reconstruction_ret<I, N>
	{
	  typedef reconstruction_ret<I, N> super_type;

	  reconstruction_hybrid_ret(const abstract::image<I>& input1, //marker
				    const abstract::image<I>& input2, //mask
				    const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  virtual const oln_type_of(I, value) process(const I& work,
						      const oln_type_of(I, point)& p,
						      const oln_type_of(N, window)& se,
						      const oln_type_of(I, value)& v) const
	  {
	    std::cerr << "oops in " << __func__ << std::endl;
	    return oln_type_of(I, value)();
	  }

	  virtual void loop_body(const oln_type_of(I, point)& p,
				 const oln_type_of(I, point)& q,
				 oln_type_of(I, concrete)& output,
				 std::queue<oln_type_of(I, point) >& fifo)
	  {
	    std::cerr << "oops in " << __func__ << std::endl;
	  }

	  virtual bool exist_init(const oln_type_of(I, point)& p,
				  const oln_type_of(I, concrete)& output,
				  const oln_type_of(N, window)& se) const
	  {
	    std::cerr << "oops in " << __func__ << std::endl;
	    return true;
	  }


	  void impl_run()
	  {
	    mlc::eq<oln_type_of(I, size), oln_type_of(N, size)>::ensure();
	    precondition(this->input1.size() == this->input2.size());
	    precondition(level::is_greater_or_equal(this->input2, this->input1));

	    oln_type_of(I, concrete) output;
	    output = utils::clone(this->input1);
	    {
	      oln_type_of(N, window) se_plus = get_plus_se_p(convert::nbh_to_cse(this->nbh));
	      oln_type_of(N, window) se_minus = get_minus_se_p(convert::nbh_to_cse(this->nbh));
	      oln_type_of(I, fwd_piter) fwd_p(output.size());
	      oln_type_of(I, fwd_piter) bkd_p(output.size());

	      for_all (fwd_p)
		output[fwd_p] = this->process(output, fwd_p, se_plus, this->input2[fwd_p].value());

	      std::queue<oln_type_of(I, point) > fifo;
	      for_all (bkd_p)
		{
		  output[bkd_p] = this->process(output, bkd_p, se_minus, this->input2[bkd_p].value());
		  if (this->exist_init((oln_type_of(I, point))bkd_p, output, se_minus))
		    fifo.push(bkd_p);
		}
	      // Propagation Step
	      while (!fifo.empty())
		{
		  oln_type_of(I, point) p = fifo.front();
		  fifo.pop();
		  typedef oln_type_of(N, window) window_type;
		  window_type w = convert::nbh_to_se(this->nbh);
		  oln_type_of(window_type, fwd_witer) dp(w);

		  for_all (dp)
		    {
		      oln_type_of(I, point) q = (oln_type_of(window_type, dpoint))dp + p;
		      this->loop_body(p, q, output, fifo);
		    }
		}
	    }
	    this->output = output;
	  }
	};


	template <typename I, typename N>
	struct reconstruction_dilation_ret : public reconstruction_hybrid_ret<I, N>
	{
	  typedef reconstruction_hybrid_ret<I, N> super_type;

	  reconstruction_dilation_ret(const abstract::image<I>& input1, //marker
				      const abstract::image<I>& input2, //mask
				      const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  const oln_type_of(I, value) process(const I& work,
					      const oln_type_of(I, point)& p,
					      const oln_type_of(N, window)& se,
					      const oln_type_of(I, value)& v) const
	  {
	    return ntg::min(morpho::max(work, p, se), v);
	  }

	  virtual void loop_body(const oln_type_of(I, point)& p,
				 const oln_type_of(I, point)& q,
				 oln_type_of(I, concrete)& output,
				 std::queue<oln_type_of(I, point) >& fifo)
	  {
	    if (output.hold(q))
	      {
		if ((output[q] < output[p]) &&
		    (this->input2[q] != output[q]))
		  {
		    output[q] = ntg::min(output[p].value(),
					 this->input2[q].value());
		    fifo.push(q);
		  }
	      }

	  }


	  virtual bool exist_init(const oln_type_of(I, point)& p,
				  const oln_type_of(I, concrete)& marker,
				  const oln_type_of(N, window)& se) const
	  {
	    typedef oln_type_of(N, window) se_type;
	    oln_type_of(se_type, fwd_witer) dp(se);
	    for_all (dp)
	      {
		oln_type_of(I, point) q = (oln_type_of(se_type, dpoint))dp + p;
		if (marker.hold(q) && (marker[q] < marker[p]) &&
		    (marker[q] < this->input2[q]))
		  return true;
	      }
	    return false;
	  }

	};



	template <typename I, typename N>
	struct reconstruction_erosion_ret : public reconstruction_hybrid_ret<I, N>
	{
	  typedef reconstruction_hybrid_ret<I, N> super_type;

	  reconstruction_erosion_ret(const abstract::image<I>& input1, //marker
				      const abstract::image<I>& input2, //mask
				      const abstract::neighborhood<N>& nbh)

	    : super_type(input1, input2, nbh)
	  {}

	  const oln_type_of(I, value) process(const I& work,
					      const oln_type_of(I, point)& p,
					      const oln_type_of(N, window)& se,
					      const oln_type_of(I, value)& v) const
	  {
	    return ntg::max(morpho::min(work, p, se), v);
	  }

	  virtual void loop_body(const oln_type_of(I, point)& p,
				 const oln_type_of(I, point)& q,
				 oln_type_of(I, concrete)& output,
				 std::queue<oln_type_of(I, point) >& fifo)
	  {
	    if (output.hold(q))
	      {
		if ((output[q] > output[p]) &&
		    (this->input2[q] != output[q]))
		  {
		    output[q] = ntg::max(output[p].value(),
					 this->input2[q].value());
		    fifo.push(q);
		  }
	      }
	  }


	  virtual bool exist_init(const oln_type_of(I, point)& p,
				  const oln_type_of(I, concrete)& marker,
				  const oln_type_of(N, window)& se) const
	  {
	    typedef oln_type_of(N, window) se_type;
	    oln_type_of(se_type, fwd_witer) dp(se);
	    for_all (dp)
	      {
		oln_type_of(I, point) q = (oln_type_of(se_type, dpoint))dp + p;
		if (marker.hold(q) && (marker[q] > marker[p]) &&
		    (marker[q] > this->input2[q]))
		  return true;
	      }
	    return false;
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
