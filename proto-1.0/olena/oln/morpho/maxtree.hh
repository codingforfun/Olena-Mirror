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

#ifndef OLENA_MORPHO_MAX_TREE_HH
# define OLENA_MORPHO_MAX_TREE_HH

# include <mlc/any.hh>

# include <oln/core/box.hh>
# include <oln/utils/record.hh>
# include <oln/core/abstract/image_entry.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/level/fill.hh>

# include <oln/morpho/tree.hh>
# include <oln/basics.hh>

# include <cassert>
# include <vector>
# include <functional>
# include <set>


namespace oln {

  namespace morpho {

    namespace misc {

      struct pair_cmp
      {
	template<typename L, typename P>
	bool operator()(const std::pair<L,P>& lhs,
			const std::pair<L,P>& rhs) const
	{
	  return lhs.first < rhs.first
	    or (lhs.first == rhs.first
		and lhs.second.fwd_less(rhs.second));
	}
      };

      template<typename I, typename II, typename O, typename V>
      void set_children(abstract::image_with_nbh<I>& marked,
			const abstract::image_with_nbh<II>& children,
			abstract::image_with_nbh<O>& out,
			const oln_type_of(I, point)& p,
			const V v)
      {
	typedef oln_type_of(I, point) point_type;
	std::vector<point_type> f = children[p].value();
	typename std::vector<point_type>::const_iterator pt;
	for (pt = f.begin(); pt != f.end(); pt++)
	  {
	    out[*pt] = v;
	    marked[*pt] = true;
	    set_children(marked, children, out, *pt, v);
	  }
      }


      template <class I>
      std::vector<std::vector<oln_type_of(I, point)> >
      sort(const abstract::image_with_nbh<I>& input)
      {
	typedef oln_type_of(I, point) point_type;
	std::vector<std::vector<point_type> > S(256);
	oln_type_of(I, fwd_piter) p(input.size());

#ifdef DEBUG
	typedef std::pair<oln_type_of(I, value), point_type> pair_t;
	std::set<pair_t,pair_cmp> sc;
#endif // ! DEBUG

	for_all_p (p)
	  {
	    S[input[p]].push_back(p);
#ifdef DEBUG
	    sc.insert(pair_t(input[p], p));
#endif // ! DEBUG
	  }

#ifdef DEBUG
	{
	  unsigned l;

	  typename std::set<pair_t,pair_cmp>::const_iterator i = sc.begin();
	  typename std::vector<std::vector<point_type> >::const_iterator v;

	  for (l = 0, v = S.begin();
	       v != S.end(); v++, ++l)
	    {
	      for (typename std::vector<point_type>::const_iterator p = v->begin();
		   p != v->end(); p++, ++i)
		{
		  assert(input[*p] == l);
		  assert(i->first == l);
		  assert(i->second == *p);
		}
	    }
	}
#endif // ! DEBUG

	return S;
      }

    } // end of oln::morpho::misc


    template <typename I, typename E>
    struct max_tree : public canvas::tree<I, E>
    {
      typedef canvas::tree<I, E> super_type;
      typedef oln_type_of(I, point) point_type;

      // Attributes.
      oln_ch_concrete_type(I, bool)       is_proc;
      oln_ch_concrete_type(I, bool)       marked;
      oln_ch_concrete_type(I, bool)       is_deleted;
      oln_ch_concrete_type(I, point_type) father;
      oln_ch_concrete_type(I, std::vector<point_type>) children;
      std::vector<std::vector<point_type> > v_root_point;
      oln_type_of(I, concrete)            new_value;

      void impl_compute_S()
      {
	this->S = misc::sort(this->input);
      }

      bool condition_add_root_point(const point_type& f, const point_type& p)
      {
	if (this->input[f] == this->input[p])
	  return false;
	return true;
      }

      void left_hand_walk(const point_type& p)
      {
	if (condition_add_root_point(p, father[p]))
	  v_root_point[this->input[p]].push_back(p);

	std::vector<point_type> f = children[p].value();
	typename std::vector<point_type>::const_iterator pt;
	for (pt = f.begin(); pt != f.end(); pt++)
	  {
	    left_hand_walk(*pt);
	  }
      }

      void impl_compute_tree()
      {
	v_root_point[this->input[this->root_point]].push_back(this->root_point);
	left_hand_walk(this->root_point);
      }

      void impl_set_default_output()
      {
      }

      void impl_set_is_processed_to_false()
      {
	oln_type_of(I, fwd_piter) p(is_proc.size());
	for_all_p (p)
	  is_proc[p] = false;
      }

      void impl_set_is_processed(const point_type& p)
      {
	is_proc[p] = true;
      }

      bool impl_is_processed(const point_type& n) const
      {
	return is_proc[n];
      }

      void init_aux_data(const point_type& p)
      {
	this->exact().impl_init_aux_data(p);
      }

      const point_type impl_get_father(const point_type& p) const
      {
	return father[p].value();
      }

      void merge_aux_data(const point_type& r , const point_type& p)
      {
	children[p].call(&std::vector<point_type>::push_back, r);
	this->exact().impl_merge_aux_data(r,p);
      }

      void impl_make_set(const point_type& p)
      {
	father[p] = p;
	init_aux_data(p);
      }

      point_type impl_find_root(const point_type& x)
      {
	if (is_root(x))
	  return x;
	return find_root(father[x]);
      }

      bool is_root(const point_type& x) const
      {
	return father[x] == x;
      }

      void impl_set_father(const point_type& r, const point_type& p)
      {
	father[r] = p;
	merge_aux_data(r,p);
      }

      void eligible_component(const point_type& p)
      {
	this->exact().impl_eligible_component(p);
      }

      void compute_image_is_deleted()
      {
	for (int i = 0; i <= 255 ;i++)
	  for (typename std::vector<point_type>::const_iterator p = v_root_point[i].begin();
	       p != v_root_point[i].end(); p++)
	    eligible_component(*p);
      }

      void impl_init_processing()
      {
	level::fill(this->marked, false);
	level::fill(this->is_deleted, false);
	this->exact().impl_init_aux_processing();
      }

      void impl_processing()
      {
	compute_image_is_deleted();

	oln_type_of(I, fwd_piter) p(this->input.size());
	for_all_p (p)
	  {
	    if (is_deleted[p])
	      this->output[p] = this->new_value[p];
	    else
	      this->output[p] = this->input[p];
	  }
      }

    protected:

      // Ctor.
      // FIXME : will be changed
      max_tree(const abstract::image_with_nbh<I>& input) :
	super_type(input)
      {
	oln_ch_concrete_type(I, bool) tmp1(input.size());
	is_proc = tmp1;

	oln_ch_concrete_type(I, point_type) tmp2(input.size());
	father = tmp2;

	oln_ch_concrete_type(I, std::vector<point_type>) tmp3(input.size());
	children = tmp3;

	oln_type_of(I, concrete) tmp4(input.size());
	this->output = tmp4;

	std::vector<std::vector<point_type> > tmp5(256);
	v_root_point = tmp5;

	oln_ch_concrete_type(I, bool) tmp6(input.size());
	marked = tmp6;

	oln_ch_concrete_type(I, bool) tmp7(input.size());
	is_deleted = tmp7;

	oln_type_of(I, concrete) tmp8(input.size());
	new_value = tmp8;
      }

    };

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_MAX_TREE_HH
