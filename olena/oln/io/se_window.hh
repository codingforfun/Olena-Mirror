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

#ifndef OLENA_IO_SE_WINDOW_HH
# define OLENA_IO_SE_WINDOW_HH

# include <mlc/type.hh>

# include <oln/core/image1d.hh>
# include <oln/core/image2d.hh>
# include <oln/core/image3d.hh>
# include <oln/core/window1d.hh>
# include <oln/core/window2d.hh>
# include <oln/core/window3d.hh>
# include <oln/core/macros.hh>

# include <oln/io/image.hh>

namespace oln {

  namespace io {

    namespace internal {

      /// \todo FIXME: this code should be factorized.

      /*!
      ** \brief Read a window (1 dimension) from a file.
      ** \arg output The new window.
      ** \arg name The name of the file.
      ** \return True if successful.
      */
      inline bool
      read(window1d& output, const std::string& name)
      {
	image1d<ntg::bin> im;
	if (!read(im, name))
	  return false;
	if (!(im.ncols() % 2))
	  return false;
	image1d<ntg::bin>::iter_type it(im);
	window1d w;
	for_all (it)
	  if (!im[it])
	    w.add(dpoint1d(it) - dpoint1d(im.ncols()/2));
	output = w;
	return true;
      }

      /*!
      ** \brief Read a window (2 dimensions) from a file.
      ** \arg output The new window.
      ** \arg name The name of the file.
      ** \return True if successful.
      */
      inline bool
      read(window2d& output, const std::string& name)
      {
	image2d<ntg::bin> im;
	if (!read(im, name))
	  return false;
	if (!(im.ncols() % 2) || !(im.nrows() % 2))
	  {
	    // std::clog << "[both image dimensions have to be odd for a"
	    //	      << "window2d]"
	    //	      << std::flush;
	    return false;
	  }
	image2d<ntg::bin>::fwd_iter_type it(im);
	window2d w;
	for_all (it)
	  if (!im[it])
	    w.add(dpoint2d(it) - dpoint2d(im.nrows()/2, im.ncols()/2));
	output = w;
	return true;
      }

      /*!
      ** \brief Read a window (3 dimensions) from a file.
      ** \arg output The new window.
      ** \arg name The name of the file.
      ** \return True if successful.
      */
      inline bool
      read(window3d& output, const std::string& name)
      {
	image3d<ntg::bin> im;
	if (!read(im, name))
	  return false;
	if (!(im.ncols() % 2) || !(im.nrows() % 2) || !(im.nslices() % 2))
	  return false;
	image3d<ntg::bin>::iter_type it(im);
	window3d w;
	for_all (it)
	  if (!im[it])
	    w.add(dpoint3d(it) - dpoint3d(im.nrows()/2,
					  im.ncols()/2,
					  im.nslices()/2));
	output = w;
	return true;
      }

      /*!
      ** \brief Write a window (1 dimension) to a file.
      ** \arg input The window to write.
      ** \arg name The name of the file.
      ** \return True if successful.
      */
      inline bool
      write(const window1d& input, const std::string& name)
      {
	image1d<ntg::bin> im(input.delta()*2+1);
	image1d<ntg::bin>::iter_type it(im);
	for_all (it) im[it] = true;
	for (unsigned i = 0; i < input.card(); ++i)
	  im[point1d(input.delta()) + input.dp(i)] = false;
	if (!write(im, name))
	  return false;
	return true;
      }

      /*!
      ** \brief Write a window (2 dimensions) to a file.
      ** \arg input The window to write.
      ** \arg name The name of the file.
      ** \return True if successful.
      */
      inline bool
      write(const window2d& input, const std::string& name)
      {
	image2d<ntg::bin> im(input.delta()*2+1, input.delta()*2+1);
	image2d<ntg::bin>::fwd_iter_type it(im);
	for_all (it) im[it] = true;
	for (unsigned i = 0; i < input.card(); ++i)
	  im[point2d(input.delta(), input.delta()) + input.dp(i)] = false;
	if (!write(im, name))
	  return false;
	return true;
      }

      /*!
      ** \brief Write a window (3 dimensions) to a file.
      ** \arg input The window to write.
      ** \arg name The name of the file.
      ** \return True if successful.
      */
      inline bool
      write(const window3d& input, const std::string& name)
      {
	image3d<ntg::bin> im(input.delta()*2+1,
			     input.delta()*2+1,
			     input.delta()*2+1);
	image3d<ntg::bin>::iter_type it(im);
	for_all (it) im[it] = true;
	for (unsigned i = 0; i < input.card(); ++i)
	  im[point3d(input.delta(), input.delta(), input.delta())
	     + input.dp(i)] = false;
	if (!write(im, name))
	  return false;
	return true;
      }

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_SE_WINDOW_HH
