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

#ifndef OLENA_TOPO_INTER_PIXEL_NODE_HH
# define OLENA_TOPO_INTER_PIXEL_NODE_HH

# include <oln/topo/inter-pixel/internal/dir.hh>

namespace oln {

  namespace topo {
    /// oln::topo::inter_pixel::interpixel implementation.
    namespace inter_pixel {
      /*! Inter pixel node.
      **
      ** A node is a junction of edge; the edge are
      ** represented by the directions.
      **
      ** \param I image.
      */
      template<class I>
      class node
      {
      public:
	enum { dim = I::dim };

	typedef oln_dir_type(I) dir_type;

      public:
	node() : rank_(0)
	{
	  for (unsigned i = 0; i < 2 * dim; ++i)
	    data_[i] = false;
	}

	/// Add an adge (a direction).
	void
	set(dir_type i)
	{
	  if (data_[i] == false)
	    ++rank_;

	  data_[i] = true;
	}

	/// Return true if the direction \a i joins the node.
	bool
	get(dir_type i) const
	{
	  return data_[i];
	}
	/// Degree of the node.
	unsigned
	rank() const
	{
	  return rank_;
	}

      private:
	unsigned rank_;
	bool data_[dim * 2];
      };
/// Type of node of a given image type.
# define oln_node_type(ImgType)			\
oln::topo::inter_pixel::node< ImgType >

/// A pair of a point and a direction.
# define oln_head_type(ImgType)						\
typename std::pair<oln_point_type(ImgType), oln_dir_type(ImgType) >

    } // end of namespace inter_pixel

  } // end of namespace topo

} // end of namespace oln

#endif // ! OLENA_TOPO_INTER_PIXEL_NODE_HH
