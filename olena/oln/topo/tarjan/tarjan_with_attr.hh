// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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

#ifndef OLENA_TOPO_TARJAN_TARJAN_WITH_ATTR_HH
# define OLENA_TOPO_TARJAN_TARJAN_WITH_ATTR_HH

# include <oln/topo/tarjan/tarjan.hh>
# include <oln/morpho/attributes.hh>
# include <ntg/basics.hh>
# include <oln/level/fill.hh>
# include <oln/utils/histogram.hh>

# include <vector>
# include <map>

namespace oln {

  namespace topo {
    /// \brief Implementation of tarjan set.
    namespace tarjan {
      /// Abstract classes for tarjan based algorithms.
      namespace abstract {

	template<class Exact>
	struct tarjan_with_attr: public tarjan<mlc_exact_vt_type(tarjan_with_attr<Exact>, Exact) >
	{
	  typedef oln_tarjan_input_type(Exact)		input_type;
	  typedef oln_point_type(input_type)		point_type;
	  typedef oln_value_type(input_type)		data_type;

	  typedef oln_concrete_type(input_type)		image_type;
	  typedef oln_tarjan_output_type(Exact)		image_out_type;
	  typedef oln_tarjan_attr_type(Exact)		attr_type;
	  typedef attr_env_type(attr_type)		env_type;
	  typedef oln_value_type(image_out_type)	comp_type;

	  tarjan_with_attr(const image_type &ima,
			   const env_type &env):
	    capacity_chunk((ima.npoints() < 100) ? ima.npoints() : (ima.npoints() / 100)),
	    capacity(capacity_chunk),
	    input_(ima),
	    parent_(),
	    to_comp_(ima.size()),
	    comp_value_(),
	    env_(env)
	  {
	    parent_.reserve(capacity);
	    comp_value_.reserve(capacity);
	    data_.reserve(capacity);
	  }

	  // abstract methods
	  std::vector<point_type> get_processing_order()
	  {
	    mlc_dispatch(get_processing_order)();
	  }

	  void
	  mark_set(const point_type &x)
	  {
	    mlc_dispatch(mark_set)(x);
	  }

	  void
	  uni(const point_type& n, const point_type& p)
	  {
	    mlc_dispatch(uni)(n, p);
	  }

	  // tells if a point has been proceded
	  bool is_proc(const point_type &p) const
	  {
	    return to_comp_[p] != ntg_sup_val(comp_type);
	  };

	  comp_type
	  find_root(const comp_type& x) const
	  {
	    if (parent_[x] != x)
	      return parent_[x] = find_root(parent_[x]);
	    return x;
	  }

	  // bool closing = true -> a closing is performed,
	  // an opening otherwise.
	  template<class N>
	  image_out_type
	  get_compute(const oln::abstract::neighborhood<N> &Ng)
	  {
	    std::vector<point_type>	I(get_processing_order());

	    level::fill(to_comp_, ntg_sup_val(comp_type));
	    to_comp_.border_adapt_assign(Ng.delta(), ntg_sup_val(comp_type));
	    ncomps_ = 0;
	    parent_.push_back(0);
	    comp_value_.push_back(0);
	    data_.push_back(attr_type());

	    // We are ready to perform stuff
	    for (unsigned int p = 0; p < I.size(); ++p)
	      {
		point_type p_p = I[p];
		mark_set(p_p);

		oln_neighb_type(N) Q_prime(Ng, p_p);
		for_all (Q_prime)
		  if (is_proc(Q_prime))
		    uni(Q_prime.cur(), p_p);
		if (to_comp_[p_p] == (ncomps_ + 1)) // new component
		  ++ncomps_;
		else
		  {
		    comp_value_.pop_back();
		    parent_.pop_back();
		    data_.pop_back();
		  }
	      }

	    // Resolving phase
	    std::map<comp_type, comp_type>		cmps;
	    comp_type				nc = 0;
	    ncomps_ = 0;
	    //	unsigned i = 0;
	    for (int p = I.size() - 1; p >= 0; --p)
	      {
		point_type p_p = I[p];
		if (cmps.find(comp_value_[find_root(to_comp_[p_p])]) == cmps.end())
		  {
		    ++ncomps_;
		    //		++i;
		    //		std::cout << "new component\n";
		    cmps[comp_value_[find_root(to_comp_[p_p])]] = nc;
		    comp_value_[find_root(to_comp_[p_p])] = nc++;
		  }
	      }
	    //	std::cout << i << " components\n";
	    image_out_type output(input_.size());
	    for (int p = I.size() - 1; p >= 0; --p)
	      {
		point_type p_p = I[p];
		output[p_p] = comp_value_[find_root(to_comp_[p_p])];
	      }
	    return output;
	  }

	  // access to attributes
	  // i index of component
	  const attr_type &get_attr(const comp_type &i) const
	  {
	    return data_[find_root(i)];
	  }

	  comp_type ncomps() const
	  {
	    return ncomps_;
	  }

	protected:

	  unsigned					capacity_chunk;
	  unsigned					capacity;
	  const image_type				&input_;
	  mutable std::vector<comp_type>			parent_;
	  typename oln::mute<input_type, comp_type>::ret		to_comp_;// comp number from a point
	  comp_type					ncomps_;
	  std::vector<oln_value_type(image_out_type)>	comp_value_;
	  std::vector<attr_type>			data_;
	  env_type					env_;
	};
      } // !abstract



      template<class T, class DestType, class A, class Exact = mlc::final>
      struct cc:
	public abstract::tarjan_with_attr<typename mlc::exact_vt<cc<T, DestType, A, Exact>,
								 Exact>::ret>
      {

	typedef oln_point_type(T)			point_type;
	typedef oln_value_type(T)			data_type;
	//    typedef abstract::image<T>		image_type;
	typedef oln_concrete_type(T) image_type;

	typedef unsigned			comp_type;

	typedef cc<T, DestType, A, Exact>			self_type; /*< Self type of the class.*/
	typedef mlc_exact_vt_type(self_type, Exact)		exact_type;
	typedef abstract::tarjan_with_attr<exact_type>		super_type;

	cc(const image_type &ima,
	   const attr_env_type(A) &env): super_type(ima, env)
	{
	}

	std::vector<point_type> get_processing_order_impl()
	{
	  std::vector<point_type>	res;
	  oln_iter_type(image_type)	it(input_);

	  res.reserve(input_.npoints());
	  for_all(it)
	    res.push_back(it);
	  return res;
	}

	void
	mark_set_impl(const point_type &x)
	{
	  if (parent_.size() == parent_.capacity())
	    {
	      capacity = parent_.capacity() + capacity_chunk;
	      parent_.reserve(capacity);
	      comp_value_.reserve(capacity);
	    }
	  to_comp_[x] = ncomps_ + 1;
	  data_.push_back(A(input_, x, env_));
	  parent_.push_back(ncomps_ + 1);
	  //comp_value_.push_back(input_[x]);
	  comp_value_.push_back(ncomps_ + 1);
	}

	void
	uni_impl(const point_type& n, const point_type& p)
	{
	  comp_type		r = find_root(to_comp_[n]);
	  precondition(to_comp_[n] <= ncomps_);
	  precondition(to_comp_[p] <= (ncomps_ + 1));
	  if (r != to_comp_[p])
	    if (input_[n] == input_[p])
	      {
		if (to_comp_[p] == (ncomps_ + 1)) // first merge of p component
		  {
		    precondition(r < comp_value_.capacity());
		    //comp_value_[r] = input_[p];
		    //		comp_value_[to_comp_[p]] = comp_value_[r];
		    data_[r] += data_[to_comp_[p]];
		    precondition(r <= ncomps_);
		    to_comp_[p] = r;
		  }
		else
		  {
		    precondition(r < parent_.capacity());
		    data_[parent_[to_comp_[p]]] += data_[parent_[r]];
		    // 		comp_value_[parent_[to_comp_[p]]] = ntg::min(comp_value_[parent_[r]],
		    // 							     comp_value_[parent_[to_comp_[p]]]);
		    parent_[r] = parent_[to_comp_[p]];

		  }
	      }
	  //      precondition(comp_value_[parent_[r]] <= 255);
	}
      };

      // traits specialization
      template <typename T, typename DestType, typename A, typename Exact>
      struct tarjan_traits<cc<T, DestType, A, Exact> >
      {
	typedef T					input_type;
	typedef typename  mute<T, DestType>::ret	output_type;
	typedef A					attr_type;
      };


    } // end of namespace tarjan

  } // end of namespace topo

} // end of namespace oln

#endif // ! OLENA_TOPO_TARJAN_TARJAN_WITH_ATTR_HH
