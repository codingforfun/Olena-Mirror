// Copyright (C) 2004  EPITA Research and Development Laboratory
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


namespace oln
{
  namespace morpho
  {
    template <class I, class D, class Env>
    template <class N>
    f_tarjan_map<I, D, Env>::f_tarjan_map(bool is_closing,
					  const typename f_tarjan_map<I, D, Env>::input_type& input,
					  const abstract::neighborhood<N>& ng,
					  const typename f_tarjan_map<I, D, Env>::lambda_type& lambda,
					  const Env & env) :
      is_closing(is_closing),
      input(input),
      lambda(lambda),
      parent(input.size()),
      is_proc(input.size()),
      output(input.size()),
      env(env)
    {
      // init S and parent
      typedef typename std::vector<point_type> vector_type;
      vector_type S(input.npoints());
      if (is_closing)
	utils::distrib_sort(input, S);
      else
	utils::distrib_sort_inv(input, S);

      level::fill(parent, inactive());
      level::fill(is_proc, false);

      // array S contains sorted pixel list
      make_set(S[0]);
      is_proc[S[0]] = true;

      for (int ip = 1; ip < int(S.size()); ++ip)
	{
	  point_type p = S[ip];

	  // p_ is the pixel just processed before p
	  point_type p_ = S[ip - 1];
	  if (input[p] != input[p_])
	    {
	      // for all the previous layer
	      for (int iq = ip - 1; iq >= 0; --iq)
		{
		  point_type q = S[iq];
		  if (input[q] != input[p_])
		    break;
		  // if the attribute is 'reached', we do not need
		  // it anymore
		  if (parent[q] == active() && !(auxdata[q] < lambda))
		    {
		      parent[q] = inactive();
		      auxdata.erase(q);
		    }
		}
	    }
	  make_set(p);

	  oln_neighb_type(N) n(ng, p);
	  for_all(n)
	    if (input.hold(n) && is_proc[n])
	      do_union(n, p);

	  is_proc[p] = true;
	}

      // resolving phase in reverse sort order

      for (int ip = int(S.size()) - 1; ip >= 0; --ip)
	{
	  point_type p = S[ip];
	  if (parent[p] == active() || parent[p] == inactive())
	    output[p] = input[p];
	  else
	    output[p] = output[parent[p]];
	}
    }

    template <class I, class D, class Env>
    const typename f_tarjan_map<I, D, Env>::point_type
    f_tarjan_map<I, D, Env>::inactive()
    {
      static const point_type it = utils::internal::mk_point_looking_like<point_type>(-2);
      return it;
    }

    template <class I, class D, class Env>
    const typename f_tarjan_map<I, D, Env>::point_type
    f_tarjan_map<I, D, Env>::active()
    {
      static const point_type it = utils::internal::mk_point_looking_like<point_type>(-1);
      return it;
    }

    template <class I, class D, class Env>
    void
    f_tarjan_map<I, D, Env>::make_set(const typename f_tarjan_map<I, D, Env>::point_type& x)
    {
      precondition(parent[x] == inactive());
      parent[x] = active();
      auxdata[x] = D(input, x, env);
    }

    template <class I, class D, class Env>
    void
    f_tarjan_map<I, D, Env>::link(const typename f_tarjan_map<I, D, Env>::point_type& x,
				  const typename f_tarjan_map<I, D, Env>::point_type& y)
    {
      if (parent[x] == active() && parent[y] == active())
	{
	  auxdata[y] += auxdata[x];
	  auxdata.erase(x);
	}
      else
	if (parent[x] == active())
	  auxdata.erase(x);
	else
	  {
	    parent[y] = inactive();
	    auxdata.erase(y);
	  }
      parent[x] = y;
    }

    template <class I, class D, class Env>
    typename f_tarjan_map<I, D, Env>::point_type
    f_tarjan_map<I, D, Env>::find_root(const typename f_tarjan_map<I, D, Env>::point_type& x)
    {
      if (parent[x] != active() && parent[x] != inactive())
	{
	  parent[x] = find_root(parent[x]);
	  return parent[x];
	}
      return x;
    }

    template <class I, class D, class Env>
    bool
    f_tarjan_map<I, D, Env>::equiv(const typename f_tarjan_map<I, D, Env>::point_type& x,
				   const typename f_tarjan_map<I, D, Env>::point_type& y) const
    {
      return input[x] == input[y] || parent[x] == active();
    }

    template <class I, class D, class Env>
    void
    f_tarjan_map<I, D, Env>::do_union(const typename f_tarjan_map<I, D, Env>::point_type& n,
				      const typename f_tarjan_map<I, D, Env>::point_type& p)
    {
      point_type r = find_root(n);
      if (r != p)
	{
	  if (equiv(r, p))
	    link(r, p);
	  else
	    if (parent[p] == active())
	      {
		parent[p] = inactive();
		auxdata.erase(p);
	      }
	}
    }
  } // end of namespace morpho
} // end of namespace oln

