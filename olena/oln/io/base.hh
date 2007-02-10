// Copyright (C) 2001, 2002, 2003, 2004, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLENA_IO_BASE_HH
# define OLENA_IO_BASE_HH

# include <oln/io/image_read.hh>
# include <oln/io/image_write.hh>
/// Forward declarations.
# include <oln/io/se_fwd.hh>

namespace oln {

  /*! \namespace io
  ** \brief io namespace.
  */
  namespace io {

    /*! \namespace internal
    ** \brief abstract internal.
    */
    namespace internal {

      /*!
      ** \brief Read from a file.
      **
      ** The specialized read function is called by template specialization.
      */
      template<typename T>
      bool
      read_any(T& output, const std::string& name)
      {
	return read(output, name);
      }

      /*!
      ** \brief Write anything to a file.
      **
      ** The specialized write function is called by template specialization.
      */
      template<typename T>
      bool
      write_any(const T& input, const std::string& name)
      {
	return write(input, name);
      }

    } // end of internal

    // aliases

    /*
    ** \brief Load object from a file. This could be an image but also
    ** a window or a neighborhood.
    ** \arg name The name of the file.
    ** \return The new object.
    ** \sa oln::abstract::iter
    **
    ** Call abstract::image::has_impl to test that your image has been
    ** successfully loaded.
    ** See oln::io::internal::reader_id to know which file are supported.
    ** \sa abstract::image::has_impl
    ** \sa
    */
    inline
    internal::anything
    load(const std::string& name)
    {
      return internal::anything(name);
    }

    /*
    ** \brief Load an image or a structuring element from a file.
    ** \arg output The object to write to a file.
    ** \arg name The name of the file.
    ** \return True if successful.
    **
    ** Depending of T, work is dispatched.
    */
    template<class T>
    bool
    load(T& output, std::string name)
    {
      return internal::read_any(output, name);
    }

    /*
    ** \brief Write an image or a structuring element to a file.
    ** a window or a neighborhood.
    ** \arg input The object to read.
    ** \arg name The name of the file.
    ** \return True if successful.
    **
    ** Depending of T, work is dispatched.
    ** \sa oln::abstract::iter
    */
    template< typename T >
    bool
    save(const T& input, const std::string& name)
    {
      return internal::write_any(input, name);
    }

  } // end of namespace io

  // Export load & save into namespace oln:: for convenience.
  using io::load;
  using io::save;

} // end of namespace oln

// Implementations.
# include <oln/io/se_window.hh>
# include <oln/io/se_neighborhood.hh>

#endif // OLENA_IO_BASE_HH
