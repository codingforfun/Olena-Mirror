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

      struct my_type
      {
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

	unsigned row_max_get() const { return row_max_;}
	void row_max_set(unsigned a) { row_max_ = a; }
	unsigned row_max_;

	unsigned col_max_get() const { return col_max_;}
	void col_max_set(unsigned a) { col_max_ = a; }
	unsigned col_max_;

	unsigned row_min_get() const { return row_min_;}
 	void row_min_set(unsigned a) { row_min_ = a; }
	unsigned row_min_;

	unsigned col_min_get() const { return col_min_;}
	void col_min_set(unsigned a) { col_min_ = a; }
	unsigned col_min_;

	bool have_center_get() const { return have_center_;}
	void have_center_set(bool a) { have_center_ = a; }
	bool have_center_;
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
	void set_area_min(unsigned nb)
	{
	  area_min_ = nb;
	}

	void set_tower(float nb)
	{
	  tour_ = nb;
	}

	void set_circle(float nb)
	{
	  circle_ = nb;
	}

	void set_area_max(unsigned nb)
	{
	  area_max_ = nb;
	}

	void set_level(unsigned nb)
	{
	  level_ = nb;
	}

	void set_height(unsigned nb)
	{
	  height_ = nb;
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
	    if (not ((get_aux_data(p, area) >= area_min_) &&
		     (get_aux_data(p, area) <= area_max_)))
	      return false;
	  return true;
	}

	bool circle_attr(const point_type& p)
	{
	  if (circle_tag_)
	    {
	      int w = get_aux_data(p, col_max) - get_aux_data(p, col_min) + 1;
	      int h = get_aux_data(p, row_max) - get_aux_data(p, row_min) + 1;
	      float theoretic_area = 3.14f * ((h + w) / 4.0) * ((h + w) / 4.0);
	      if (not (((theoretic_area - sqrt(get_aux_data(p, area)) < get_aux_data(p, area)) &&
			(theoretic_area + sqrt(get_aux_data(p, area)) > get_aux_data(p, area))) &&
		       (get_aux_data(p, area) > (0.9 * circle_ * w * h)) &&
		       (get_aux_data(p, area) < (1.1 * circle_ * w * h))))
		return false;
	    }
	  return true;
	}

	bool tour_attr(const point_type& p)
	{
	  if (tour_tag_)
	    {
	      float tmp = (float)(get_aux_data(p, int_volume)) /
		(float)(get_aux_data(p, ext_volume));
	      if (not (tmp >= tour_))
		// 	    if (not (get_aux_data(p, int_volume) <=
		// 		     get_aux_data(p, ext_volume) / 2))
		return false;
	    }
	  return true;
	}

	bool center_p_attr(const point_type& p)
	{
	  if (center_p_tag_)
	    if (get_aux_data(p, have_center))
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
	  set_aux_data(p, area2, 1);
	}

	void impl_merge_aux_data(const point_type& r , const point_type& p)
	{
	  set_aux_data(p, area2, get_aux_data(r, area2) +
		       get_aux_data(p, area2));
	}

	bool impl_is_an_eligible_component(const point_type& p)
	{
	  if (level_attr(p))
	    if (area_attr(p))
	      if (circle_attr(p))
		if (tour_attr(p))
		  if (center_p_attr(p))
		    return true;
	  return false;
	}


	void impl_init_attributes()
	{
	  oln_type_of(I, fwd_piter) p(this->input.size());
	  for_all_p(p)
	    {
	      if (area_tag_ || tour_tag_ || circle_tag_)
		set_aux_data(p, area, 1);
	      if (height_tag_ || tour_tag_)
		set_aux_data(p, height, 0);
	      if (tour_tag_)
		{
		  set_aux_data(p, int_volume, 0);
		  set_aux_data(p, ext_volume, 0);
		}
	      if (circle_tag_)
		{
		  set_aux_data(p, row_max, ((point_type)(p)).row());
		  set_aux_data(p, row_min, ((point_type)(p)).row());
		  set_aux_data(p, col_max, ((point_type)(p)).col());
		  set_aux_data(p, col_min, ((point_type)(p)).col());
		}
	      if (center_p_tag_)
		set_aux_data(p, have_center, (((((point_type)(p)).row() == (this->input.size().nrows() / 2)) or
					       (((point_type)(p)).row() == (this->input.size().nrows() - 1) / 2) or
					       (((point_type)(p)).row() == (this->input.size().nrows() + 1) / 2))
					      and ((((point_type)(p)).col() == (this->input.size().ncols() / 2)) or
						   (((point_type)(p)).col() == (this->input.size().ncols() - 1) / 2) or
						   (((point_type)(p)).col() == (this->input.size().ncols() + 1) / 2))));
	    }
	}


	// FIXME S: structure a la olena, avec iterateurs d olena
	void impl_cpt_attributes()
	{
	  // bottom-up pass
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
		      if (area_tag_ || tour_tag_ || circle_tag_)
			set_aux_data(*p, area, get_aux_data(*p, area) + get_aux_data(*c, area));
		      if (height_tag_ || tour_tag_)
			{
			  unsigned tmp = get_aux_data(*c, height) +
			    this->input[*c].value() - this->input[*p].value();
			  set_aux_data(*p, height, ntg::max(tmp, get_aux_data(*p, height)));
			}
		      if (tour_tag_)
			set_aux_data(*p, int_volume, get_aux_data(*p, int_volume) +
				     get_aux_data(*c, int_volume));
		      if (circle_tag_)
			{
			  set_aux_data(*p, row_min, ntg::min(get_aux_data(*p, row_min),
							 get_aux_data(*c, row_min)));
			  set_aux_data(*p, row_max, ntg::max(get_aux_data(*p, row_max),
							 get_aux_data(*c, row_max)));
			  set_aux_data(*p, col_min, ntg::min(get_aux_data(*p, col_min),
							 get_aux_data(*c, col_min)));
			  set_aux_data(*p, col_max, ntg::max(get_aux_data(*p, col_max),
							     get_aux_data(*c, col_max)));
			}
		      if (center_p_tag_)
			set_aux_data(*p, have_center, get_aux_data(*c, have_center));
		    }
		  if (tour_tag_)
		    {
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
	}

	void impl_init_aux_processing()
	{
	}

	// Ctor.
	clean(const abstract::image_with_nbh<I>& input) :
	  super_type(input)
	{
	  area_max_ = 0;
	  area_min_ = 0;
	  circle_ = 0.0;
	  tour_ = 0.0;
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

	// 0 a 1 : 0.9 au debut
	float circle_;
	unsigned area_min_;
	unsigned area_max_;
	// 0 a 1 : 0.5 au debut
	float tour_;
      };

  } // end of namespace oln::appli::astro

  } // end of namespace oln::appli

} // end of namespace oln


#endif // ! OLENA_APPLI_ASTRO_CLEAN_HH
