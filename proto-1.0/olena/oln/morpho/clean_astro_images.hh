// Copyright (C) 2001-2005  EPITA Research and Development Laboratory
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

#ifndef OLENA_MORPHO_CLEAN_ASTRO_IMAGES_HH
# define OLENA_MORPHO_CLEAN_ASTRO_IMAGES_HH

# include <mlc/any.hh>

# include <oln/core/box.hh>
# include <oln/utils/record.hh>
# include <oln/core/abstract/image_entry.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/level/fill.hh>

# include <oln/morpho/maxtree.hh>
# include <oln/basics.hh>

# include <vector>

// just for test
#include <ntg/real/int_u.hh>


namespace oln {

  namespace morpho {

    namespace misc {

      template<typename I>
      bool have_the_center_point(const abstract::image_with_nbh<I>& children,
				 const oln_type_of(I, point)& p,
				 int row,
				 int col)
      {
	typedef oln_type_of(I, point) point_type;
	if (((p.row() == (row / 2)) or (p.row() == (row - 1) / 2) or (p.row() == (row + 1) / 2))
	    and ((p.col() == (col / 2)) or (p.col() == (col - 1) / 2) or (p.col() == (col + 1) / 2)))
	  return true;
	std::vector<point_type> f = children[p].value();
	typename std::vector<point_type>::const_iterator pt;
	for (pt = f.begin(); pt != f.end(); pt++)
	  {
	    return have_the_center_point(children, *pt, row, col);
	  }
	return false;
      }


      template <typename I, typename II>
      const oln_type_of(I, value) min_nbh(const abstract::image_with_nbh<I>& input,
					  const abstract::image_with_nbh<II>& children,
					  const oln_type_of(II, point)& p)
      {
	typedef oln_type_of(I, point) point_type;
	std::vector<point_type> f = children[p].value();
	oln_type_of(I, niter) n(input.exact());
	oln_type_of(I, value) min;
	typename std::vector<point_type>::const_iterator pt;
	for (pt = f.begin(); pt != f.end(); pt++)
	  {
	    min = input[*pt].value();
	    for_all_n_of_p (n, *pt)
	      {
		min = ntg::min(min, input[n].value());
	      }

	    return ntg::min(min, min_nbh(input, children, *pt));
	  }
	return input[p];
      }

      template <typename I>
      struct find_box
      {
	typedef oln_type_of(I, point) point_type;

	find_box(const point_type& p) { row_min_ = p.row(); row_max_ = p.row(); col_min_ = p.col(); col_max_ = p.col(); }
	int row_min_, row_max_, col_min_, col_max_;


	void box(const abstract::image_with_nbh<I>& children,
		 const oln_type_of(I, point)& p)
	{
	  std::vector<point_type> f = children[p].value();
	  typename std::vector<point_type>::const_iterator pt;
	  for (pt = f.begin(); pt != f.end(); pt++)
	    {
	      if (row_min_ > pt->row())
		row_min_ = pt->row();
	      if (row_max_ < pt->row())
		row_max_ = pt->row();
	      if (col_min_ > pt->col())
		col_min_ = pt->col();
	      if (col_max_ < pt->col())
		col_max_ = pt->col();
	      box(children, *pt);
	    }
	}
      };


      template <typename I>
      bool is_a_circle(const abstract::image_with_nbh<I>& children,
		       const oln_type_of(I, point)& p,
		       float c_a)
      {
	find_box<I> fb(p);
	fb.box(children,p);
	int w = fb.col_max_ - fb.col_min_ + 1;
	int h = fb.row_max_ - fb.row_min_ + 1;
	float theoric_area = 3.14f * ((h + w) / 4.0) * ((h + w) / 4.0);

#ifdef DEBUG
	std::cout << "POINT : " << p << std::endl;
	std::cout << "col_min : " << fb.col_min_ << std::endl;
	std::cout << "col_max : " << fb.col_max_ << std::endl;
	std::cout << "row_min : " << fb.row_min_ << std::endl;
	std::cout << "row_max : " << fb.row_max_ << std::endl;
	std::cout << "theoric area : " << theoric_area << std::endl;
	std::cout << "computed area : " << c_a << std::endl;
	std::cout << "t - sqrt(c) : " << theoric_area - sqrt(c_a) << std::endl;
	std::cout << "t + sqrt(c) : " << theoric_area + sqrt(c_a) << std::endl;
	std::cout << "w : " << w << std::endl << "h : " << h << std::endl;
#endif // ! DEBUG
	if (((theoric_area - sqrt(c_a) < c_a) &&
	     (theoric_area + sqrt(c_a) > c_a)) &&
	    (c_a > (0.9 * 0.75 * w * h)) &&
	    (c_a < (1.1 * 0.75 * w * h)))
	  return true;
	return false;
      }

    } // end of oln::morpho::misc


    template <typename I>
    struct clean_astro : public max_tree<I, clean_astro<I> >
    {
      typedef max_tree<I, clean_astro<I> > super_type;
      typedef oln_type_of(I, point) point_type;

      // Attributes.
      oln_ch_concrete_type(I, unsigned) area;
      oln_type_of(I, value) level_;
      unsigned area_;


      // specific methods

      void set_area(unsigned nb)
      {
	area_ = nb;
      }

      void set_level(unsigned nb)
      {
	level_ = nb;
      }

      void remove_star(const point_type& p)
      {
	this->is_deleted[p] = true;
	misc::set_children(this->marked, this->children, this->is_deleted, p, true);
	this->new_value[p] = this->input[this->father[p]].value();
	misc::set_children(this->marked,
			   this->children, this->new_value, p, this->input[this->father[p]].value());
      }

      void remove_little_surface(const point_type& p)
      {
	this->is_deleted[p] = true;
	misc::set_children(this->marked, this->children, this->is_deleted, p, true);
	this->new_value[p] = this->input[this->father[p]].value();
	misc::set_children(this->marked,
			   this->children, this->new_value, p, this->input[this->father[p]].value());
      }

      bool is_a_star(const point_type& p)
      {
	if (not (this->marked[p]))
	  {
	    if (this->area[p] >= area_)
	      if (not (misc::have_the_center_point(this->children, p, this->input.size().nrows(),
						   this->input.size().ncols())))
		if (misc::is_a_circle(this->children, p, this->area[p]))
		  return true;
	  }
	return false;
      }

      bool is_a_little_surface(const point_type& p)
      {
	if (this->area[p] <= 20 && this->input[p] < 150)
	  return true;
	return false;
      }

      // fill the holes

      void impl_init_aux_data(const point_type& p)
      {
	this->area[p] = 1;
      }

      void impl_merge_aux_data(const point_type& r , const point_type& p)
      {
	this->area[p] = this->area[r].value() + this->area[p].value();
      }

      void impl_eligible_component(const point_type& p)
      {
	if (this->input[p].value() > this->level_)
	  {
	    // 	  if (is_a_little_surface(p))
	    // 	    remove_little_surface(p);
	    if (is_a_star(p))
	      remove_star(p);
	  }
      }

      void impl_init_aux_traitement()
      {
      }

      // Ctor.
      clean_astro(const abstract::image_with_nbh<I>& input) :
	super_type(input)
      {
	oln_ch_concrete_type(I, unsigned) tmp(input.size());
	this->area = tmp;

	this->area_ = 0;
	this->level_ = 0;
      }

    };

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLENA_MORPHO_CLEAN_ASTRO_IMAGES_HH
