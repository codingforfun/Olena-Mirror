// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_ATTRIBUTE_UNION_FIND_HH
# define OLENA_ATTRIBUTE_UNION_FIND_HH

#include <oln/level/fill.hh>

#include <ntg/bin.hh>

#include <vector>
#include <oln/utils/histogram.hh>
#include <oln/morpho/attributes.hh>

namespace oln {
  namespace morpho {
    namespace tarjan {

      template< class T>
      struct T_attribute
      {
	T_attribute():value(1){}

	T_attribute( const T v):value(v){}


	T_attribute(const T_attribute& rhs){ value = rhs.value;}

	T_attribute
	operator+(const T_attribute & rhs) const
	{
	  return T_attribute(value + rhs.value);
	}

	T_attribute&
	operator=(const T_attribute & rhs)
	{
	  value = rhs.value;
	  return *this;
	}

	const bool
	operator<(const T_attribute & rhs) const
	{
	  return value < rhs.value;
	}

	T value;

      };

      template<class T, class ATTRIBUTE, class Env = NullEnv>
      struct tarjan_set
      {
	typedef oln_point_type(T) point_type;
	typedef oln_value_type(T) data_type;
	typedef oln_concrete_type(T) image_type;
	typedef typename ATTRIBUTE::lambda_type	lambda_type;
	typedef Env	env_type;

	// ACTIVE and INACTIVE are defined with a hook to be static
	// and initialized ionly once.
	static const point_type&
	ACTIVE()
	{
	  static struct foo_def
	  {
	    point_type elt;
	    foo_def()
	    {
	      const unsigned dim = point_type::dim;
	      for (unsigned i = 0; i < dim; ++i )
		elt.nth(i) = -1;
	    }
	  } tmp;

	  return tmp.elt;
	}

	static const point_type&
	INACTIVE()
	{
	  static struct foo_def
	  {
	    point_type elt;
	    foo_def() {
	      const unsigned dim = point_type::dim;
	      for (unsigned i = 0; i < dim; ++i )
		elt.nth(i) = -2;
	    }
	  } tmp;

	  return tmp.elt;
	}

	tarjan_set(const image_type& ima, const env_type &env) : input_(ima),
								 parent_(ima.size()),
								 aux_data_(ima.size()),
								 env_(env)
	{
	  level::fill(parent_, INACTIVE());
	}

	void
	make_set(const point_type& x)
	{
	  precondition(parent_[x] == INACTIVE());
	  parent_[x] = ACTIVE();
	  aux_data_[x] = ATTRIBUTE(input_, x, env_);
	}

	point_type
	find_root(const point_type& x)
	{
	  if ((parent_[x] != ACTIVE()) && (parent_[x] != INACTIVE()))
	    {
	      parent_[x] = find_root(parent_[x]);
	      return parent_[x];
	    }
	  else
	    return x;
	}

	bool
	criterion(const point_type& x, const point_type& y)
	{
	  precondition((parent_[x] == ACTIVE()) || (parent_[x] == INACTIVE()));
	  precondition((parent_[y] == ACTIVE()) || (parent_[y] == INACTIVE()));
	  return ( (input_[x] == input_[y]) || (aux_data_[x] < *lambda_));
	}

	void
	uni(const point_type& n, const point_type& p)
	{
	  point_type r = find_root(n);
	  if (r != p)
	    if (criterion(r,p))
	      {
		aux_data_[p] += aux_data_[r];
		parent_[r] = p;
	      }
	    else
	      {
		aux_data_[p] = *lambda_;
	      }
	}

	// bool closing = true -> a closing is performed,
	// an opening otherwise.
	template<bool closing, class N>
	image_type
	get_comptute(const lambda_type & lambda,
		     const abstract::neighborhood<N>& Ng)
	{
	  lambda_ = &lambda;

	  std::vector<point_type>	I(input_.npoints());

	  oln::utils::select_distrib_sort<closing>()(input_, I);

	  level::fill(aux_data_, ntg_sup_val(lambda_type));
	  aux_data_.border_adapt_assign(Ng.delta(), ntg_sup_val(lambda_type));

	  // We are ready to perform stuff
	  for (unsigned int p = 0; p < I.size(); ++p)
	    {
	      point_type p_p = I[p];
	      make_set(p_p);
	      oln_neighb_type(N) Q_prime(Ng, p_p);
	      for_all (Q_prime)
		if (is_proc(Q_prime))
		  uni(Q_prime.cur(), p_p);
	    }

	  // Resolving phase
	  image_type output(input_.size());
	  for (int p = I.size() - 1; p >= 0; --p)
	    {
	      point_type p_p = I[p];
	      if ((parent_[p_p] == ACTIVE()) ||  (parent_[p_p] == INACTIVE()))
		output[p_p] = input_[p_p];
	      else
		output[p_p] = output[parent_[p_p]];
	      // this code is equivalent to
	      // 	output[I[p].first] = input_[find_root(I[p].first)];

	    }
	  return output;
	}

      protected:
	// tells if a point has been proceded
	bool is_proc(const point_type &p) const
	{
	  //FIXME: odd way to call !=,  but it is to help the compiler
	  //to find the good one  when ATTRIBUTE is templeted by types
	  //in other namepaces.

	  // return aux_data_[p] != ntg_max_val(lambda_type);
	  return aux_data_[p].operator!=(ntg_sup_val(lambda_type));
	};


	const image_type & input_;
	typename mute<T, point_type>::ret parent_;
	typename mute<T, ATTRIBUTE>::ret aux_data_;
	const lambda_type *lambda_;
	const env_type	env_;
      };
    }
  }
}
#endif
