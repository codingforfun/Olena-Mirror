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

namespace oln {
  namespace morpho {
    namespace tarjan {
      namespace internal_tarjan_ {

	template <class P, class V>
	bool vec_sort_max(const std::pair<P, V>& l,
			  const std::pair<P, V>& r)
	{
	  if (r.second != l.second)
	    return r.second < l.second;

	  const unsigned dim = 2;
	  for (unsigned i = 0; i< dim; ++i)
	    if (l.first.nth(i) != r.first.nth(i))
	      return l.first.nth(i) < r.first.nth(i);
	  // it means l == r
	  return true;
	}


	template <class P, class V>
	bool vec_sort_min(const std::pair<P, V>& l,
			  const std::pair<P, V>& r)
	{
	  if (r.second != l.second)
	    return r.second > l.second;

	  const unsigned dim = P::dim;
	  for (unsigned i = 0; i< dim; ++i)
	    if (l.first.nth(i) != r.first.nth(i))
	      return l.first.nth(i) < r.first.nth(i);
	  // it means l == r
	  return true;
	}

      }
    }
  }
}

namespace oln {
  namespace morpho {
    namespace tarjan {

      template< class T>
      struct T_attribute
      {
	T_attribute():value(1){}

	T_attribute( const T v):value(v){}


	T_attribute(const T_attribute& rhs){ value = rhs.value;}

	T_attribute operator+(const T_attribute & rhs) const
	{
	  return T_attribute(value + rhs.value);
	}

	T_attribute & operator=(const T_attribute & rhs)
	{
	  value = rhs.value;
	  return *this;
	}

	const bool operator<(const T_attribute & rhs) const
	{
	  return value < rhs.value;
	}

	T value;
      };


      template<class T, class ATTRIBUTE>
      struct tarjan_set
      {
	typedef oln_point_type(T) point_t;
	typedef oln_value_type(T) data_t;
	typedef oln_concrete_type(T) image_t;


	// ACTIVE and INACTIVE are defined with a hook to be static
	// and initialized ionly once.
	static const point_t & ACTIVE()
	{
	  static struct foo_def{
	    point_t elt;
	    foo_def()
	    {
	      const unsigned dim = point_t::dim;
	      for (unsigned i = 0; i < dim; ++i )
		elt.nth(i) = -1;
	    }
	  } tmp;

	  return tmp.elt;
	}

	static const point_t &INACTIVE()
	{
	  static struct foo_def{
	    point_t elt;
	    foo_def() {
	      const unsigned dim = point_t::dim;
	      for (unsigned i = 0; i < dim; ++i )
		elt.nth(i) = -2;
	    }
	  } tmp;

	  return tmp.elt;
	}




	tarjan_set(const image_t& ima) : input_(ima),
					 parent_(ima.size()),
					 aux_data_(ima.size())
	{
	  level::fill(parent_, INACTIVE());
	}

	void make_set(const point_t& x)
	{
	  precondition(parent_[x] == INACTIVE());
	  parent_[x] = ACTIVE();
	  aux_data_[x] = 1;
	}



	point_t find_root(const point_t& x)
	{
	  if ((parent_[x] != ACTIVE()) && (parent_[x] != INACTIVE()))
	    {
	      parent_[x] = find_root(parent_[x]);
	      return parent_[x];
	    }
	  else
	    return x;
	}

	bool criterion(const point_t& x, const point_t& y)
	{
	  precondition((parent_[x] == ACTIVE()) || (parent_[x] == INACTIVE()));
	  precondition((parent_[y] == ACTIVE()) || (parent_[y] == INACTIVE()));
	  return ( (input_[x] == input_[y]) || (aux_data_[x] < lambda_));
	}

	void uni(const point_t& n, const point_t& p)
	{
	  point_t r = find_root(n);
	  if (r != p)
	    if (criterion(r,p))
	      {
		aux_data_[p] = aux_data_[p] + aux_data_[r];
		parent_[r] = p;
	      }
	    else
	      {
		aux_data_[p] = lambda_;
	      }
	}


	// bool closing = true -> a closing is performed,
	// an opening otherwise.
	template<class N>
	image_t get_comptute(const ATTRIBUTE & lambda,
			     const abstract::neighborhood<N>& Ng,
			     const bool closing)
	{
	  typedef std::pair<point_t, data_t> pixel_t;
	  lambda_ = lambda;

	  std::vector<pixel_t> I;
	  I.reserve(input_.npoints());

	  {
	    // sort pixels with respect to their level and scanning order
	   oln_iter_type(T) p(input_);
	    for_all(p)
	      I.push_back(pixel_t(p.cur(), input_[p]));
	    if (closing)
	      sort(I.begin(), I.end(), internal_tarjan_::vec_sort_min<point_t, data_t> );
	    else
	      sort(I.begin(), I.end(), internal_tarjan_::vec_sort_max<point_t, data_t> );
	  }

	  // Image to know which pixels have been processed
	  typename mute<T, ntg::bin>::ret is_proc(input_.size());
	  level::fill(is_proc, false);

	  // We are ready to perform stuff
	  for (unsigned int p = 0; p < I.size(); ++p)
	    {
	      point_t p_p = I[p].first;
	      make_set(p_p);
	      is_proc[p_p] = true;
	      Neighb(N) Q_prime(Ng, p_p);
	      for_all (Q_prime) if (input_.hold(Q_prime)) if (is_proc[Q_prime] == true)
		uni(Q_prime.cur(), p_p);
	    }

	  // Resolving phase
	  image_t output(input_.size());
	  for (int p = I.size() - 1; p >= 0; --p)
	    {
	      point_t p_p = I[p].first;
	      if ((parent_[p_p] == ACTIVE()) ||  (parent_[p_p] == INACTIVE()))
		output[p_p] = I[p].second;
	      else
		output[p_p] = output[parent_[p_p]];
	      // this code is equivalent to
	      // 	output[I[p].first] = input_[find_root(I[p].first)];

	    }
	  return output;
	}

	const image_t & input_;
	typename mute<T, point_t>::ret parent_;
	typename mute<T, ATTRIBUTE>::ret aux_data_;
	ATTRIBUTE lambda_;

      };
    }
  }
}
#endif
