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

#ifndef OLENA_IO_IMAGE_READ_HH
# define OLENA_IO_IMAGE_READ_HH

# include <mlc/bool.hh>

# include <oln/config/system.hh>
# include <oln/core/abstract/image_with_dim.hh>
# include <oln/core/image2d.hh>

# include <oln/io/image_base.hh>
# include <oln/io/pnm_read.hh>
# include <oln/io/pnm_read_2d.hh>
# include <oln/io/pnm_read_3d.hh>
# include <oln/io/stream_wrapper.hh>
# include <oln/io/utils.hh>

# include <list>
# include <iostream>

namespace oln {

  namespace io {

    namespace internal {

      /*!
      ** \brief Image's reader.
      ** \param R Type of reader.
      */
      template< reader_id R, typename T >
      struct try_readers
      {
	/*!
	** \brief Read an object from a stream.
	** Try do deduce the image format from the extension.
	** \arg output The new object to write.
	** \arg in The stream.
	** \arg ext The extension's filename.
	*/
	static bool
	by_extension(T& output, std::istream& in, const std::string& ext)
	{
	  if (image_reader<R,T>::knows_ext(ext))
	    if (image_reader<R,T>::read(in, output))
	      return true;
	  in.seekg(0, std::ios::beg);
	  return try_readers<reader_id(unsigned(R)-1), T>
	           ::by_extension(output, in, ext);
	}

	/*!
	** \brief Read an object from a stream.
	** Try to match the file format referring to the data only.
	** \arg output The new object to write.
	** \arg in The stream.
	*/
	static bool
	by_data(T& output, std::istream& in)
	{
	  if (image_reader<R,T>::read(in, output))
	    return true;
	  in.seekg(0, std::ios::beg);
	  return try_readers<reader_id(unsigned(R)-1), T>::by_data(output, in);
	}
      };

      /*!
      ** \brief Reader for image of type None.
      **
      ** In fact, do nothing because of the type of reader.
      ** Used when an errors occurs on others types of reader.
      */
      template< typename T >
      struct try_readers<ReadNone, T>
      {
	/*!
	** \brief Read an object from a stream.
	** Try do deduce the image format from the extension.
	*/
	static bool
	by_extension(T&, std::istream&, const std::string&)
	{
	  return false;
	}

	/*!
	** \brief Read an object from a stream.
	** Try to match the file format referring to the data only.
	*/
	static bool
	by_data(T&, std::istream&)
	{
	  return false;
	}
      };

      /*!
      ** \brief Readers trier functor, helper for stream_wrappers.
      */
      struct readers_trier
      {
	/*!
	** \brief Readers trier functor, helper for stream_wrappers.
	** \arg output The new object.
	** \arg in The stream to read from.
	** \arg ext The extension.
	**
	** First try to read by extension, then by data.
	*/
	template <class T>
	static bool
	doit(T& output, std::istream& in, const std::string ext)
	{
	  bool result = try_readers<ReadAny,T>::by_extension(output, in, ext);
	  if (!result)
	    result = try_readers<ReadAny,T>::by_data(output, in);
	  return result;
	}
      };

      /*!
      ** \brief Read an image (2 dimensions) from a file.
      ** \arg output Where to write the new image.
      ** \arg name The filename.
      ** \return True if successful.
      **
      ** Try to read first by extension and then by data.
      ** If you want to know more about, see
      ** oln::io::internal::try_stream_wrappers_in
      */
      template <unsigned N, class E>
      inline bool
      read(abstract::image_with_dim<N,E>& output, const std::string& name)
      {
	mlc::is_true<N == 2 || N == 3>::ensure();

	///< \todo FIXME: E or abstract::image_with_dim<N,E> ? E is convenient.
	typedef try_stream_wrappers_in<StreamAny, E, readers_trier>
	  stream_trier;

	std::string ext = internal::utils::extension(name);

	if (stream_trier::by_extension(output.exact(), name, ext))
	  return true;
	if (stream_trier::by_data(output.exact(), name))
	  return true;

	// No valid file found using name directly. Let's try other
	// possible filenames based on name.

	std::list<std::string> names;
	stream_wrappers_find_files<StreamAny>::doit(names, name);

	for (std::list<std::string>::iterator it = names.begin();
	     it !=  names.end();
	     ++it)
	  {
	    std::string ext = utils::extension(*it);
	    if (stream_trier::by_extension(output.exact(), *it, ext))
	      {
		// std::clog << "[" << name << " found as " << *it << "] "
		//           << std::flush;
		return true;
	      }
	  }
	for (std::list<std::string>::iterator it = names.begin();
	     it !=  names.end();
	     ++it)
	  if (stream_trier::by_data(output.exact(), *it))
	    {
	      // std::clog << "['" << name << "' found as '" << *it << "'] "
	      //	<< std::flush;
	      return true;
	    }
	// std::clog << "[unable to read '" << name << "'] " << std::endl;
	return false;
      }

      /*!
      ** \brief Read an image (1 dimension) from a file.
      ** \arg output Where to write the new image.
      ** \arg name The filename.
      ** \return True if successful.
      **
      ** In fact, real work is not done here : this function calls 'read' for
      ** images of 2 dimensions:
      ** Mono-dimensional images is a special case of bi-dimensional
      ** images.
      */
      template <class E>
      inline bool
      read(abstract::image_with_dim<1, E>& output, const std::string& name)
      {
	image2d<oln_value_type(E)> tmp;
	if (!read(tmp, name))
	  return false;
	if (tmp.nrows() != 1)
	  return false;
	typename image2d<oln_value_type(E)>::iter_type it(tmp);
	output.exact() = E(tmp.ncols());
	for_all(it)
	  output(it.col()) = tmp[it];
	return true;
      }

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_IMAGE_READ_HH
