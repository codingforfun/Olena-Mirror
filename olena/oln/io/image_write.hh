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

#ifndef OLENA_IO_IMAGE_WRITE_HH
# define OLENA_IO_IMAGE_WRITE_HH

# include <mlc/bool.hh>

# include <oln/config/system.hh>
# include <oln/core/abstract/image_with_dim.hh>
# include <oln/core/image2d.hh>
# include <oln/core/image1d.hh>

# include <oln/io/image_base.hh>
# include <oln/io/pnm_write_2d.hh>
# include <oln/io/pnm_write_3d.hh>
# include <oln/io/stream_wrapper.hh>
# include <oln/io/utils.hh>

# include <list>
# include <iostream>

namespace oln {

  namespace io {

    namespace internal {

      /*!
      ** \brief Image's writer.
      ** \param W Type of writer.
      */
      template< writer_id W, typename T >
      struct try_writers
      {
	/*!
	** \brief Write object on the stream.
	** Try do deduce the image format from the extension.
	** \arg input The object to read.
	** \arg out The stream.
	** \arg ext The extension's filename.
	*/
	static bool
	by_extension(const T& input, std::ostream& out, const std::string& ext)
	{
	  if (image_writer<W,T>::knows_ext(ext))
	    if (image_writer<W,T>::write(out, input))
	      return true;
	  return try_writers<writer_id(unsigned(W)-1), T>
                   ::by_extension(input, out, ext);
	}

	/*!
	** \brief Try to match the file format referring to the data only.
	** \arg input The  object to read.
	** \arg out The stream.
	*/
	static bool
	by_data(const T& input, std::ostream& out, const std::string& ext)
	{
	  if (image_writer<W,T>::write(out, input))
	    {
	      // std::clog << "[ambiguous extension '" << ext
	      //	<< "' wrote as " << image_writer<W,T>::name() << "] "
	      //	<< std::flush;
	      return true;
	    }
	  return try_writers<writer_id(unsigned(W)-1), T>
	           ::by_data(input, out, ext);
	}
      };


      /*!
      ** \brief Reader for image of type None.
      **
      ** In fact, do nothing because of the type of writer.
      ** Used when an errors occurs on others types of writer.
      */
      template< typename T >
      struct try_writers<WriteNone, T>
      {
	/*!
	** \brief Write object on the stream.
	** Try do deduce the image format from the extension.
	*/
	static bool
	by_extension(const T&, std::ostream&, const std::string&)
	{
	  return false;
	}

	/*!
	** \brief Write object on the stream.
	** Try to match the file format referring to the data only.
	*/
	static bool
	by_data(const T&, std::ostream&, const std::string&)
	{
	  return false;
	}
      };

      /*!
      ** \brief Writers trier functor, helper for stream_wrappers.
      */
      struct writers_trier
      {
	/*!
	** \brief Writers trier functor, helper for stream_wrappers.
	** \arg output The new object.
	** \arg in The stream to read from.
	** \arg ext The extension.
	**
	** First try to write by extension, then by data.
	*/
	template <class T>
	static bool
	doit(const T& input, std::ostream& out, const std::string ext)
	{
	  bool result = try_writers<WriteAny,T>::by_extension(input, out, ext);
	  if (!result)
	    result = try_writers<WriteAny,T>::by_data(input, out, ext);
	  return result;
	}
      };


      /*!
      ** \brief Write an image (2 dimensions) to a file.
      ** \arg input The image to write to file.
      ** \arg name The filename.
      ** \return True if successful.
      */
      template <class E, unsigned N>
      inline bool
      write(const abstract::image_with_dim<N, E>& input,
	    const std::string& name)
      {
	mlc::is_true<N == 2 || N == 3>::ensure();

	typedef try_stream_wrappers_out<StreamAny, E, writers_trier>
	  stream_trier;

	std::string ext = utils::extension(name);

	if (stream_trier::by_extension(input.exact(), name, ext))
	  return true;
	// std::clog << "[unable to write '" << name << "'] " << std::endl;
	return false;
      }

      /*!
      ** \brief Write an image (1 dimension) to a file.
      ** \arg input The image to write to file.
      ** \arg name The filename.
      ** \return True if successful.
      **
      ** In fact, real work is not done here : this function calls 'write' for
      ** images of 2 dimensions:
      ** Mono-dimensional images is a special case of bi-dimensional
      ** images.
      */
      template <class E>
      inline bool
      write(const abstract::image_with_dim<1, E>& input,
	    const std::string& name)
      {
	image2d<oln_value_type(E)> tmp(1, input.ncols());
	oln_iter_type(image1d<oln_value_type(E) > ) it(input);
	for_all(it)
	  tmp(0, it.col()) = input[it];
	if (!write(tmp, name))
	  return false;
	return true;
      }

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_IMAGE_WRITE_HH
