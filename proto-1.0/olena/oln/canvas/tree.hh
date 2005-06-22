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

#ifndef OLENA_CANVAS_TREE_HH
# define OLENA_CANVAS_TREE_HH

# include <mlc/any.hh>

# include <oln/core/box.hh>
# include <oln/utils/record.hh>
# include <oln/core/abstract/image_entry.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/level/fill.hh>
# include <oln/basics.hh>

# include <vector>


namespace oln {

  namespace canvas {

    template <typename I, typename T1, typename T2, typename E>
    struct tree : public mlc::any<E>
    {
      typedef oln_type_of(I, point) point_type;

      // interface
      void compute_tree()
      {
	first_pass();
	cpt_tree();
      }

      void compute_attributes()
      {
	init_attributes();
	cpt_attributes();
      }

      void process_filter()
      {
	init_filter();
	pcs_filter();
      }

      const point_type parent_get(const point_type& p) const
      {
	return this->exact().impl_parent_get(p);
      }

      const std::vector<point_type> children_get(const point_type& p) const
      {
	return this->exact().impl_children_get(p);
      }

      const point_type local_root(const point_type& p) const
      {
	return this->exact().impl_local_root(p);
      }

      point_type find_root(const point_type& p)
      {
	return this->exact().impl_find_root(p);
      }

      oln_ch_concrete_type(I, T1)& aux_data_get()
      {
	return aux_data_;
      }

      bool is_local_root(const point_type& p) const
      {
	return this->exact().impl_is_local_root(p);
      }

      // Concrete method.
      void first_pass()
      {
	// init
	compute_S();
	set_is_processed_to_false();
	set_default_output();

	oln_type_of(I, niter) n(input);
	typename std::vector<std::vector<point_type> >::reverse_iterator i;
	typename std::vector<point_type>::const_iterator j;
	for (i = S.rbegin(); i != S.rend(); i++)
	  {
	    for (j = i->begin(); j != i->end(); j++)
	      {
		make_set(*j);
		for_all_n_of_p (n, *j)
		  if (input.hold(n) and is_processed(n) and merge_condition(n, *j))
		    do_union(n, *j);
		set_is_processed(*j);
	      }
	  }
      }

      bool merge_condition(const point_type& n, const point_type& p)
      {
	// FIXME
	return true;
      }

      void do_union(const point_type& n, const point_type& p)
      {
	point_type r = find_root(n);
	if (r != p)
	  parent_set(r, p);
      }

      // Abstract methods.
      // ---------------------------

      // init

      void init_processing()
      {
	this->exact().impl_init_processing();
      }

      void compute_S()
      {
	this->exact().impl_compute_S();
      }
      void set_default_output()
      {
	this->exact().impl_set_default_output();
      }

      // about is_processed
      void set_is_processed_to_false()
      {
	this->exact().impl_set_is_processed_to_false();
      }
      void set_is_processed(const point_type& p)
      {
	this->exact().impl_set_is_processed(p);
      }
      bool is_processed(const point_type& n) const
      {
	return this->exact().impl_is_processed(n);
      }

      // core

      void cpt_tree()
      {
	this->exact().impl_cpt_tree();
      }

      void init_filter()
      {
	this->exact().impl_init_filter();
      }

      void pcs_filter()
      {
	this->exact().impl_pcs_filter();
      }

      void init_attributes()
      {
	this->exact().impl_init_attributes();
      }

      void cpt_attributes()
      {
	this->exact().impl_cpt_attributes();
      }

      void make_set(const point_type& p)
      {
	this->exact().impl_make_set(p);
      }

      void parent_set(const point_type& r, const point_type& p)
      {
	this->exact().impl_parent_set(r, p);
      }

      // Attributes.
      box<const I> input;
      oln_type_of(I, concrete) output;

    protected:
      std::vector<std::vector<point_type> > S;
      oln_ch_concrete_type(I, T1) aux_data_;
      std::vector<std::vector<T2> > aux_level_data_;


      // Ctor.
      tree(const abstract::image_with_nbh<I>& input) :
	input(input),
	S()
      {
	oln_ch_concrete_type(I, T1) tmp(this->input.size());
	aux_data_ = tmp;
      }

      // Check for impls..
      ~tree()
      {
	// FIXME... mlc_check_method_impl(E, void, init,          , );
      }

    };


  } // end of namespace oln::canvas

} // end of namespace oln


#endif // ! OLENA_CANVAS_TREE_HH
