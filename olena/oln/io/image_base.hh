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

#ifndef OLENA_IO_IMAGE_BASE_HH
# define OLENA_IO_IMAGE_BASE_HH

# include <oln/config/system.hh>

# include <iostream>
# include <string>

namespace oln {

  namespace io {

    namespace internal {

      /*!
      ** \brief Reader identifiers.
      **
      ** They are used recursively: each time a ReadId fails, then
      ** try with ReadId - 1.
      */
      enum reader_id { ReadNone     = 0,
		       ReadPnmPlain = 1,
		       ReadPnmRaw   = 2,
		       ReadAny      = 2 };


      /*!
      ** \brief Read an image of type reader_id.
      ** \param F The reader identifier.
      **
      ** In fact, do nothing because of the type of reader.
      ** Used when an errors occurs on others types of reader.
      ** \see reader_id
      */
      template<reader_id F, class I>
      struct image_reader
      {
	///< Do nothing because of the type of reader.
	static const std::string&
	name()
	{
	  static const std::string name_("-");
	  return name_;
	}

	///< Do nothing because of the type of reader.
	static bool
	knows_ext(const std::string&)
	{
	  return false;
	}

	///< Do nothing because of the type of reader.
	static bool
	read(std::istream&, I&)
	{
	  return false;
	}
      };


      /*!
      ** \brief Writer identifiers.
      **
      ** They are used recursively: each time a WriteId fails, then
      ** try with WriteId - 1.
      */
      enum writer_id { WriteNone     = 0,
		       WritePnmPlain = 1,
		       WritePnmRaw   = 2,
		       WriteAny      = 2 };



      /*!
      ** \brief Write an image of type writer_id.
      ** \param F The writer identifier.
      **
      ** In fact, do nothing because of the type of writer.
      ** Used when an errors occurs on others types of writer.
      ** \see writer_id
      */
      template<writer_id F, class I>
      struct image_writer
      {
	///< Do nothing because of the type of writer.
	static const std::string&
	name()
	{
	  static const std::string name_("-");
	  return name_;
	}

	///< Do nothing because of the type of writer.
	static bool
	knows_ext(const std::string&)
	{
	  return false;
	}

	///< Do nothing because of the type of writer.
	static bool
	write(std::ostream&, const I&)
	{
	  return false;
	}
      };

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // ! OLENA_IO_IMAGE_BASE_HH
