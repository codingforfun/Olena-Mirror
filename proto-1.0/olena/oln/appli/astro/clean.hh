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

#ifndef OLENA_APPLI_ASTRO_CLEAN_HH
# define OLENA_APPLI_ASTRO_CLEAN_HH

# include <mlc/any.hh>

# include <oln/core/box.hh>
# include <oln/utils/record.hh>
# include <oln/core/abstract/image_entry.hh>
# include <oln/core/ch_value_type.hh>
# include <oln/level/fill.hh>

# include <oln/canvas/maxtree.hh>
# include <oln/basics.hh>

# include <vector>



#define get_aux_data(IT1ER, AT1T1R) this->aux_data_[IT1ER].call(&T1::AT1T1R##_get)
#define set_aux_data(IT1ER, AT1T1R, VAL) this->aux_data_[IT1ER].call(&T1::AT1T1R##_set, VAL)



namespace oln {

  namespace appli {

    namespace astro {

      namespace misc {

	template<typename I>
	bool have_the_center_point(const abstract::image_with_nbh<I>& children,
				   const oln_type_of(I, point)& p,
				   int row,
				   int col)
	{
	  typedef oln_type_of(I, point) point_type;
	  if (((p.row() == (row / 2)) or
	       (p.row() == (row - 1) / 2) or
	       (p.row() == (row + 1) / 2))
	      and ((p.col() == (col / 2)) or
		   (p.col() == (col - 1) / 2) or
		   (p.col() == (col + 1) / 2)))
	    return true;
	  std::vector<point_type> f = children[p].value();
	  typename std::vector<point_type>::const_iterator pt;

	  bool ret = false;
	  for (pt = f.begin(); pt != f.end(); pt++)
	    ret |= have_the_center_point(children, *pt, row, col);
	  return ret;
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
	struct is_a_circle_recu
	{
	  typedef oln_type_of(I, point) point_type;

	  is_a_circle_recu(const point_type& p) { row_min_ = p.row(); row_max_ = p.row(); col_min_ = p.col(); col_max_ = p.col(); }
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

	// by checking the geometry of the image
	template <typename I>
	bool is_a_circle(const abstract::image_with_nbh<I>& children,
			 const oln_type_of(I, point)& p,
			 float c_a)
	{
	  is_a_circle_recu<I> fb(p);
	  fb.box(children,p);
	  int w = fb.col_max_ - fb.col_min_ + 1;
	  int h = fb.row_max_ - fb.row_min_ + 1;
	  float theoretic_area = 3.14f * ((h + w) / 4.0) * ((h + w) / 4.0);

#ifdef DEBUG
	  std::cout << "POINT1 : " << p << std::endl;
	  std::cout << "col_min : " << fb.col_min_ << std::endl;
	  std::cout << "col_max : " << fb.col_max_ << std::endl;
	  std::cout << "row_min : " << fb.row_min_ << std::endl;
	  std::cout << "row_max : " << fb.row_max_ << std::endl;
	  std::cout << "theoretic area : " << theoretic_area << std::endl;
	  std::cout << "computed area : " << c_a << std::endl;
	  std::cout << "t - sqrt(c) : " << theoretic_area - sqrt(c_a) << std::endl;
	  std::cout << "t + sqrt(c) : " << theoretic_area + sqrt(c_a) << std::endl;
	  std::cout << "w : " << w << std::endl << "h : " << h << std::endl;
#endif // ! DEBUG
	  // 0.75
	  if (((theoretic_area - sqrt(c_a) < c_a) &&
	       (theoretic_area + sqrt(c_a) > c_a)) &&
	      (c_a > (0.9 * 0.9 * w * h)) &&
	      (c_a < (1.1 * 0.9 * w * h)))
	    return true;
	  return false;
	}

	// by checking the geometry of the component.
// 	void is_a_rectangle(const abstract::image_with_nbh<I>& children,
// 			    const oln_type_of(I, point)& p,
// 			    float min, float max)
// 	{



// 	}


      } // end of oln::appli::astro::misc

      struct my_type
      {
	my_type() { area_ = 0; height_ = 0; area2_= 0; int_volume_ = 0; ext_volume_ = 0;}

	unsigned area_get() const { return area_; }
	void area_set(unsigned a) { area_ = a; }
	unsigned area_;

	unsigned area2_get() const { return area2_; }
	void area2_set(unsigned a) { area2_ = a; }
	unsigned area2_;

	unsigned height_get() const { return height_; }
	void height_set(unsigned a) { height_ = a; }
	unsigned height_;

	unsigned ext_volume_get() const { return ext_volume_; }
	void ext_volume_set(unsigned a) { ext_volume_ = a; }
	unsigned ext_volume_;

	unsigned int_volume_get() const { return int_volume_; }
	void int_volume_set(unsigned a) { int_volume_ = a; }
	unsigned int_volume_;
      };

      struct my_type2
      {
	my_type2() {}
      };


      template <typename I, typename T1 = my_type, typename T2 = my_type2>
      struct clean : public canvas::max_tree<I, T1, T2, clean<I, T1, T2> >
      {
	typedef canvas::max_tree<I, T1, T2, clean<I, T1, T2> > super_type;
	typedef oln_type_of(I, point) point_type;

	// specific methods
	void set_area(unsigned nb)
	{
	  area_ = nb;
	}

	void set_level(unsigned nb)
	{
	  level_ = nb;
	}

	void set_height(unsigned nb)
	{
	  height_ = nb;
	}

	void remove_star(const point_type& p)
	{
	  this->is_deleted[p] = true;
	  canvas::misc::set_children(this->marked, this->children, this->is_deleted, p, true);
	  this->new_value[p] = this->input[this->parent[p]].value();
	  canvas::misc::set_children(this->marked,
				     this->children, this->new_value, p, this->input[this->parent[p]].value());
	}

	// attributes to compute the output

	bool level_attr(const point_type& p)
	{
	  if (level_tag_)
	    if (not (this->input[p].value() > this->level_))
	      return false;
	  return true;
	}

	bool area_attr(const point_type& p)
	{
	  if (area_tag_)
	    if (not (this->aux_data_[p].call(&T1::area_get) >= area_))
	      return false;
	  return true;
	}

	bool circle_attr(const point_type& p)
	{
	  if (circle_tag_)
	    if (not (misc::is_a_circle(this->children,
				       p,
				       this->aux_data_[p].call(&T1::area_get))))
	      return false;
	  return true;
	}

	bool tour_attr(const point_type& p)
	{
	  if (tour_tag_)
	    if (not (get_aux_data(p, int_volume) <=
		     get_aux_data(p, ext_volume) / 2))
	      return false;
	  return true;
	}

	bool center_p_attr(const point_type& p)
	{
	  if (center_p_tag_)
	    if (misc::have_the_center_point(this->children,
					    p,
					    this->input.size().nrows(),
					    this->input.size().ncols()))
	      return false;
	  return true;
	}

	bool height_attr(const point_type& p)
	{
	  if (height_tag_)
	    if (not (get_aux_data(p, height) > height_))
	      return false;
	  return true;
	}

	// fill the holes

	void impl_init_aux_data(const point_type& p)
	{
	  this->aux_data_[p].call(&T1::area2_set, 1);
	}

	void impl_merge_aux_data(const point_type& r , const point_type& p)
	{
	  this->aux_data_[p].call(&T1::area2_set,
				  this->aux_data_[r].call(&T1::area2_get) +
				  this->aux_data_[p].call(&T1::area2_get));
	}

	void impl_eligible_component(const point_type& p)
	{
	  if (not (this->marked[p]))
	    if (level_attr(p))
	      if (area_attr(p))
		if (circle_attr(p))
		  if (tour_attr(p))
		    if (center_p_attr(p))
		      remove_star(p);
	}


	void impl_init_compute_attributes()
	{
	  oln_type_of(I, fwd_piter) p(this->input.size());
	  for_all_p(p)
	    {
	      this->aux_data_[p].call(&T1::area_set, 1);
	      this->aux_data_[p].call(&T1::height_set, 0);
	      this->aux_data_[p].call(&T1::int_volume_set, 0);
	      this->aux_data_[p].call(&T1::ext_volume_set, 0);
	    }
	}


	// FIXME S: structure a la olena, avec iterateurs d olena
	void impl_compute_attributes()
	{
	  typename std::vector<std::vector<point_type> >::reverse_iterator s;
	  typename std::vector<point_type>::const_iterator p;
	  typename std::vector<point_type>::const_iterator c;
	  for (s = this->S.rbegin(); s != this->S.rend(); s++)
	    {
	      for (p = s->begin(); p != s->end(); p++)
		{
		  std::vector<point_type> v = this->children[*p].value();
		  for (c = v.begin(); c != v.end(); c++)
		    {

		      set_aux_data(*p, area, get_aux_data(*p, area) + get_aux_data(*c, area));

		      unsigned tmp = get_aux_data(*c, height) +
			this->input[*c].value() - this->input[*p].value();

		      set_aux_data(*p, height, ntg::max(tmp, get_aux_data(*p, height)));

		      set_aux_data(*p, int_volume, get_aux_data(*p, int_volume) +
				   get_aux_data(*c, int_volume));
		    }

		  unsigned tmp = this->input[*p].value() - this->input[this->parent[local_root(*p)].value()].value();
		  if (not tmp)
		    tmp = 1;

		  set_aux_data(*p, int_volume, get_aux_data(*p, int_volume) + (get_aux_data(*p, area) * tmp));
		  set_aux_data(*p, ext_volume, get_aux_data(*p, int_volume) +
			       (get_aux_data(*p, area) * get_aux_data(*p, height)));
		  if (not get_aux_data(*p, ext_volume))
		    set_aux_data(*p, ext_volume, 1);
		}
	    }
	}

	void impl_init_aux_processing()
	{
	}

	// Ctor.
	clean(const abstract::image_with_nbh<I>& input) :
	  super_type(input)
	{
	  area_ = 0;
	  level_ = 0;
	  height_ = 0;

	  level_tag_ = false;
	  area_tag_ = false;
	  tour_tag_ = false;
	  circle_tag_ = false;
	  height_tag_ = false;
	  center_p_tag_ = false;
	}

	// Attributes.
	// tags
	bool level_tag_;
	bool area_tag_;
	bool tour_tag_;
	bool circle_tag_;
	bool height_tag_;
	bool center_p_tag_;

      protected:

	// Attributes.
	oln_type_of(I, value) level_;
	oln_type_of(I, value) height_;
	unsigned area_;
      };

  } // end of namespace oln::appli::astro

  } // end of namespace oln::appli

} // end of namespace oln


#endif // ! OLENA_APPLI_ASTRO_CLEAN_HH
