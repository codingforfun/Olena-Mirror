// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \file
///

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_TREE_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_TREE_HH

#include <vector>
#include <climits>
#include <cmath>

# include <scribo/layout/internal/docstrum/kd_node.hh>
# include <scribo/layout/internal/docstrum/kd_hyperrect.hh>
# include <scribo/layout/internal/docstrum/bbox2d.hh>
# include <scribo/layout/internal/docstrum/nearest_neighbors.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;


	class kd_tree
	{
	public:
	  static const int nb_dims = 2;

	  kd_tree(const int nb_nodes);

	  int find_parent(const bbox2d* bbox,
			  int* const dim) const;

	  void insert(const bbox2d* bbox);

	  void build_kd_tree(const std::vector<const bbox2d*>& points);

	  int euclidean_distance(const bbox2d* bbox1,
				 const bbox2d* bbox2);

	  float point_to_line_distance(const float slope,
				       const float y_intercept,
				       const bbox2d* bbox);

	  void nn(const bbox2d* bbox,
		  const int node_index,
		  nearestn& n);

	  void nn_within_range(const bbox2d* bbox,
			       const int node_index,
			       const float max_distance,
			       std::vector<unsigned>& neighbors_id,
			       const float slope,
			       const float y_intercept);

	  void find_nnearest(const bbox2d* bbox,
			     nearestn& n);

	  void find_nnearest_within_range(std::vector<unsigned>& neighbors_id,
					  const float max_distance,
					  const bbox2d* bbox,
					  const float slope,
					  const float y_intercept);

	  bool empty();

	  int size();

	private:
	  bool root_inserted_;
	  int nodes_inserted_;
	  kd_hyperrect hyperrect_;
	  std::vector<kd_node> nodes_;
	};


# ifndef MLN_INCLUDE_ONLY

	//---------------------------
	// KD tree
	//---------------------------

	inline
	kd_tree::kd_tree(const int nb_nodes)
	  : root_inserted_(false), nodes_inserted_(0)
	{
	  nodes_.reserve(nb_nodes);
	}

	inline
	int
	kd_tree::find_parent(const bbox2d* bbox,
			     int* const dim) const
	{
	  int node_index = 0;
	  int father_index = 0;
	  const point2d& center = bbox->pcenter();
	  int coord_value = 0;

	  while (node_index != -1 && not nodes_[node_index].is_leaf())
	  {
	    (not *dim) ? coord_value = center.col() : coord_value = center.row();

	    const kd_node& current_node = nodes_[node_index];
	    father_index = node_index;

	    int node_coord_value = current_node.coord(*dim);

	    if (coord_value <= node_coord_value)
	      node_index = current_node.left_son();
	    else
	      node_index = current_node.right_son();

	    *dim = (*dim + 1) % 2;
	  }

	  if (node_index == -1)
	  {
	    node_index = father_index;
	    *dim = (*dim + 1) % 2;
	  }

	  return node_index;
	}

	inline
	void
	kd_tree::insert(const bbox2d* bbox)
	{
	  if (not root_inserted_)
	  {
	    kd_node node(bbox, 0, 0);
	    root_inserted_ = true;
	    hyperrect_.init(bbox);
	    nodes_.push_back(node);
	  }
	  else
	  {
	    int dim = 0;
	    const int node_index = find_parent(bbox, &dim);
	    kd_node node(bbox, node_index, (dim + 1) % 2);

	    const point2d& center = bbox->pcenter();
	    int coord_value = 0;

	    (not dim) ? coord_value = center.col() : coord_value = center.row();

	    if (coord_value <= nodes_[node_index].coord(dim))
	      nodes_[node_index].set_left_son(nodes_.size());
	    else
	      nodes_[node_index].set_right_son(nodes_.size());

	    hyperrect_.extend(bbox);
	    nodes_.push_back(node);
	  }

	  ++nodes_inserted_;
	}

	inline
	void
	kd_tree::build_kd_tree(const std::vector<const bbox2d*>& points)
	{
	  const int nb_points = points.size();

	  for (int i = 0; i < nb_points; ++i)
	    insert(points[i]);
	}

	inline
	int
	kd_tree::euclidean_distance(const bbox2d* bbox1,
				    const bbox2d* bbox2)
	{
	  const point2d& center1 = bbox1->pcenter();
	  const point2d& center2 = bbox2->pcenter();
	  const int dx = center1.col() - center2.col();
	  const int dy = center1.row() - center2.row();

	  return (dx * dx + dy * dy);
	}

	inline
	float
	kd_tree::point_to_line_distance(const float slope,
					const float y_intercept,
					const bbox2d* bbox)
	{
	  const point2d& center = bbox->pcenter();

	  return (std::fabs(-slope * center.col() +
			    center.row() - y_intercept) /
		  sqrt(slope * slope + 1));
	}

	inline
	void
	kd_tree::nn(const bbox2d* bbox,
		    const int node_index,
		    nearestn& n)
	{
	  if (node_index == -1)
	    return;

	  const kd_node& node = nodes_[node_index];
	  const bbox2d* b = node.bbox();

	  if (node.is_leaf())
	  {
	    const int d = euclidean_distance(bbox, b);

	    if (d < n.get_highest_distance() && bbox != b)
	      n.add_point(nnpoint(b, d));

	    return;
	  }

	  int nearer_sub_tree = -1;
	  int further_sub_tree = -1;
	  bool nearer_h = false; // hyperrect.min
	  bool further_h = false; //
	  const int dim = node.dim();
	  int coord_value = 0;

	  (not dim) ? coord_value = bbox->pcenter().col() :
	    coord_value = bbox->pcenter().row();

	  if (coord_value <= node.coord(dim))
	  {
	    nearer_sub_tree = node.left_son();
	    further_sub_tree = node.right_son();
	    nearer_h = true; // hyperrect.max
	  }
	  else
	  {
	    nearer_sub_tree = node.right_son();
	    further_sub_tree = node.left_son();
	    further_h = true; // hyperrect.max
	  }

	  if (nearer_sub_tree > 0)
	  {
	    if (nearer_h)
	    {
	      const int tmp = hyperrect_.max_coord(dim);
	      hyperrect_.set_max_coord(dim, node.coord(dim));
	      nn(bbox, nearer_sub_tree, n);
	      hyperrect_.set_max_coord(dim, tmp);
	    }
	    else
	    {
	      const int tmp = hyperrect_.min_coord(dim);
	      hyperrect_.set_min_coord(dim, node.coord(dim));
	      nn(bbox, nearer_sub_tree, n);
	      hyperrect_.set_min_coord(dim, tmp);
	    }
	  }

	  const int d = euclidean_distance(bbox, b);

	  if (d < n.get_highest_distance() && bbox != b)
	    n.add_point(nnpoint(b, d));

	  if (further_sub_tree > 0)
	  {
	    if (further_h)
	    {
	      const int tmp = hyperrect_.max_coord(dim);
	      hyperrect_.set_max_coord(dim, node.coord(dim));

	      if (hyperrect_.distance(bbox) < n.get_highest_distance())
		nn(bbox, further_sub_tree, n);

	      hyperrect_.set_max_coord(dim, tmp);
	    }
	    else
	    {
	      const int tmp = hyperrect_.min_coord(dim);
	      hyperrect_.set_min_coord(dim, node.coord(dim));

	      if (hyperrect_.distance(bbox) < n.get_highest_distance())
		nn(bbox, further_sub_tree, n);

	      hyperrect_.set_min_coord(dim, tmp);
	    }
	  }
	}

	inline
	void
	kd_tree::nn_within_range(const bbox2d* bbox,
				 const int node_index,
				 const float max_distance,
				 std::vector<unsigned>& neighbors_id,
				 const float slope,
				 const float y_intercept)
	{
	  if (node_index == -1)
	    return;

	  const kd_node& node = nodes_[node_index];
	  const bbox2d* b = node.bbox();

	  if (node.is_leaf())
	  {
	    const float d = point_to_line_distance(slope, y_intercept, b);

	    if (d < max_distance && bbox != b)
	      neighbors_id.push_back(b->bbox_number());

	    return;
	  }

	  int nearer_sub_tree = -1;
	  int further_sub_tree = -1;
	  bool nearer_h = false; // hyperrect.min
	  bool further_h = false; //
	  const int dim = node.dim();
	  int coord_value = 0;

	  (not dim) ? coord_value = bbox->pcenter().col() :
	    coord_value = bbox->pcenter().row();

	  if (coord_value <= node.coord(dim))
	  {
	    nearer_sub_tree = node.left_son();
	    further_sub_tree = node.right_son();
	    nearer_h = true; // hyperrect.max
	  }
	  else
	  {
	    nearer_sub_tree = node.right_son();
	    further_sub_tree = node.left_son();
	    further_h = true; // hyperrect.max
	  }

	  if (nearer_sub_tree > 0)
	  {
	    if (nearer_h)
	    {
	      const int tmp = hyperrect_.max_coord(dim);
	      hyperrect_.set_max_coord(dim, node.coord(dim));
	      nn_within_range(bbox, nearer_sub_tree, max_distance,
			      neighbors_id, slope, y_intercept);
	      hyperrect_.set_max_coord(dim, tmp);
	    }
	    else
	    {
	      const int tmp = hyperrect_.min_coord(dim);
	      hyperrect_.set_min_coord(dim, node.coord(dim));
	      nn_within_range(bbox, nearer_sub_tree, max_distance,
			      neighbors_id, slope, y_intercept);
	      hyperrect_.set_min_coord(dim, tmp);
	    }
	  }

	  const float d = point_to_line_distance(slope, y_intercept, b);

	  if (d < max_distance && bbox != b)
	    neighbors_id.push_back(b->bbox_number());

	  if (further_sub_tree > 0)
	  {
	    if (further_h)
	    {
	      const int tmp = hyperrect_.max_coord(dim);
	      hyperrect_.set_max_coord(dim, node.coord(dim));

	      if (hyperrect_.vertical_distance(bbox) < max_distance * max_distance)
		nn_within_range(bbox, further_sub_tree, max_distance,
				neighbors_id, slope, y_intercept);

	      hyperrect_.set_max_coord(dim, tmp);
	    }
	    else
	    {
	      const int tmp = hyperrect_.min_coord(dim);
	      hyperrect_.set_min_coord(dim, node.coord(dim));

	      if (hyperrect_.vertical_distance(bbox) < max_distance * max_distance)
		nn_within_range(bbox, further_sub_tree, max_distance,
				neighbors_id, slope, y_intercept);

	      hyperrect_.set_min_coord(dim, tmp);
	    }
	  }
	}

	inline
	void
	kd_tree::find_nnearest(const bbox2d* bbox,
			       nearestn& n)
	{
	  if (not root_inserted_)
	    return;

	  nn(bbox, 0, n);
	}

	inline
	void
	kd_tree::find_nnearest_within_range(std::vector<unsigned>& neighbors_id,
					    const float max_distance,
					    const bbox2d* bbox,
					    const float slope,
					    const float y_intercept)
	{
	  if (not root_inserted_)
	    return;

	  nn_within_range(bbox, 0, max_distance, neighbors_id, slope, y_intercept);
	}

	inline
	bool
	kd_tree::empty()
	{
	  return nodes_inserted_ == 0;
	}

	inline
	int
	kd_tree::size()
	{
	  return nodes_inserted_;
	}


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_KD_TREE_HH
