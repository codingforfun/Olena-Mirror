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

#ifndef OLENA_TOPO_INTER_PIXEL_INTER_PIXEL_HH
# define OLENA_TOPO_INTER_PIXEL_INTER_PIXEL_HH

# include <oln/basics.hh>
# include <oln/topo/inter-pixel/node.hh>
# include <oln/topo/inter-pixel/fwd-dir-iter.hh>
# include <oln/topo/inter-pixel/bkd-dir-iter.hh>

# include <ntg/basics.hh>

namespace oln {

  namespace topo {

    namespace inter_pixel {
 /// Return the inter pixel type of an image type.
# define oln_interpixel_type(ImgType)		\
typename oln::mute< ImgType, oln_node_type(ImgType) >::ret

      /*! Inter pixel class.
      **
      ** This example give the node of the black connected component (bottom left).
      ** \code
      ** #include <ntg/int.hh>
      ** #include <oln/basics2d.hh>
      ** #include <oln/topo/inter-pixel/inter-pixel.hh>
      ** #include <iostream>
      ** using namespace oln::topo::inter_pixel;
      **
      ** int main()
      ** {
      **   typedef oln::image2d<ntg::int_u8> img_type;
      **   img_type in = oln::load(IMG_IN "test-cmap.pgm");
      **   interpixel<oln::image2d<ntg::int_u8> > ip(in);
      **   std::cout << ip << std::endl;
      **   // Print:
      **   //  (5,0): east north south
      **   //  (5,5): north west south
      **   //  (7,5): east north west
      **   //  (10,9): east north west
      **   //  (10,11): north west south
      **   //  (11,14): north west south
      ** }
      ** \endcode
      ** \todo FIXME: Test the output values in the tests.
      */
      template <class I>
      class interpixel
      {
      public:
	typedef oln_dpoint_type(I)		dpoint_type;
	typedef oln_point_type(I)		point_type;
	typedef oln_node_type(I)		node_type;
	typedef oln_fwd_dir_iter_type(I)	fwd_dir_iter_type;
	typedef oln_bkd_dir_iter_type(I)	bkd_dir_iter_type;
	typedef oln_dir_type(I)			dir_type;
	typedef oln_dir_traits_type(I)		dir_traits_type;
	typedef oln_head_type(I)		head_type;
	typedef oln_interpixel_type(I)		inter_pixel_type;

      public:
	/// Construct an inter pixel map of the image \a img.
	interpixel(const I & img) :
	  data_(img.nrows() + 1, img.ncols() + 1)
	{
	  // FIXME: assume there is no boundary pixel with a max value.
	  // pearhaps we should clone img with a higher type.
	  img.border_adapt_assign(1, ntg_max_val(oln_value_type(I)));

	  oln_iter_type(inter_pixel_type) p(data_);
	  fwd_dir_iter_type it;

	  for_all(p)
	    {
	      for_all(it)
		if (img[p + neighb_[it]] != img[p + neighb_[it.next()]])
		  data_[p].set(it);
	    }
	}

	/// FIXME: add doc.
	const node_type&
	operator[](const point_type & p) const
	{
	  return data_[p];
	}

	/*! FIXME: add doc.
	**
	** \pre precondition(data_[in.first].get(in.second) == true)
	*/
	head_type
	folw(const head_type& in) const
	{
	  precondition(data_[in.first].get(in.second) == true);

	  head_type out = in;

	  do
	    {
	      out.first += inter_neighb_[out.second];

	      if (out.first == in.first || data_[out.first].rank() > 2)
		{
		  out.second = dir_traits_type::opposite(out.second);
		  break;
		}

	      dir_type next = dir_traits_type::next(out.second);
	      dir_type prev = dir_traits_type::prev(out.second);

	      out.second = data_[out.first].get(next) ? next :
		data_[out.first].get(prev) ? prev : out.second;
	    }
	  while (out.first != in.first);

	  return out;
	}

	/// Print the inter pixel map.
	std::ostream&
	print(std::ostream & ostr) const
	{
	  oln_iter_type(inter_pixel_type) p(data_);

	  for_all(p)
	    if (data_[p].rank() > 2)
	      {
		ostr << p << ":";

		fwd_dir_iter_type it;
		for_all(it)
		  if (data_[p].get(it))
		    ostr << " " << it;

		ostr << std::endl;
	      }

	  return ostr;
	}

      private:
	inter_pixel_type	data_;

      public:
	static const dpoint_type neighb_[4];
	static const dpoint_type inter_neighb_[4];
      };

    } // end of namespace inter_pixel

  } // end of namespace topo

} // end of namespace oln

template <class I>
inline std::ostream & operator<<(std::ostream & ostr,
				 const oln::topo::inter_pixel::interpixel<I> & ip)
{
  return ip.print(ostr);
}

template <class I>
const oln_dpoint_type(I)
  oln::topo::inter_pixel::interpixel<I>::neighb_[4] = {oln_dpoint_type(I)(0,0),
						       oln_dpoint_type(I)(-1,0),
						       oln_dpoint_type(I)(-1,-1),
						       oln_dpoint_type(I)(0,-1)};
template <class I>
const oln_dpoint_type(I)
  oln::topo::inter_pixel::interpixel<I>::inter_neighb_[4] = {oln_dpoint_type(I)(0,1),
							     oln_dpoint_type(I)(-1,0),
							     oln_dpoint_type(I)(0,-1),
							     oln_dpoint_type(I)(1,0)};

#endif // ! OLENA_TOPO_INTER_PIXEL_INTER_PIXEL_HH
