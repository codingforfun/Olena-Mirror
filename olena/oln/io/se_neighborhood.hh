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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_IO_SE_NEIGHBORHOOD_HH
# define OLENA_IO_SE_NEIGHBORHOOD_HH

# include <oln/core/image2d.hh>
# include <oln/core/neighborhood2d.hh>
# include <oln/core/macros.hh>
# include <oln/io/image.hh>

namespace oln {

  namespace io {

    namespace internal {

      /*!
      ** \brief Read a neighborhood from a file.
      ** \arg output The new neighborhood.
      ** \arg name The name of the file.
      ** \return True if successful.
      */
      inline bool
      read(neighborhood2d& output, const std::string& name)
      {
	image2d<ntg::bin> im;
	if (!read(im, name))
	  return false;

	if (!(im.ncols() % 2) || !(im.nrows() % 2))
	  {
	    // std::clog << "[both image dimensions have to be odd for "
	    //      << "a neighborhood2d]" << std::flush;
	    return false;
	  }
	image2d<ntg::bin>::fwd_iter_type it(im);
	neighborhood2d w;
	dpoint2d dcenter = dpoint2d(im.nrows()/2, im.ncols()/2);
	for_all (it)
	  {
	    if (it.row() == im.nrows()/2 && it.col() == im.ncols()/2+1)
	      break;
	    if (im[it])
	      {
		dpoint2d dp = dpoint2d(it) - dcenter;
		if (dp.is_centered()) return false;
		w.add(dp);
	      }
	  }
	for_all_remaining (it)
	  {
	    point2d p = it - dcenter;
	    point2d inv_p = -p + dcenter;
	    if (im[it] != im[inv_p])
	      {
		// std::clog << "[a neighborhood2d must be symmetric]"
		//  << std::flush;
		return false;
	      }
	  }
	output = w;
	return true;
      }

      /*!
      ** \brief Write a neighborhood to a file.
      ** \arg input The neighborhood to write.
      ** \arg name The name of the file.
      ** \return True if successful.
      */
      inline bool
      write(const neighborhood2d& input, const std::string& name)
      {
	image2d<ntg::bin> im(input.delta()*2+1, input.delta()*2+1);
	image2d<ntg::bin>::fwd_iter_type it(im);
	for_all (it) im[it] = false;
	for (unsigned i = 0; i < input.card(); ++i)
	  im[point2d(input.delta(),input.delta()) + input.dp(i)] = true;
	if (!write(im, name))
	  return false;
	return true;
      }

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // OLENA_IO_SE_NEIGHBORHOOD_HH
