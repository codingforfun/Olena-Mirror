// Copyright (C) 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_IO_BASE_HH_
# define OLENA_IO_BASE_HH_

# include <oln/io/readable.hh>

# include <list>
# include <iostream>

namespace oln {

  namespace io {

    namespace internal {

      template<typename T>
      bool read_any(T& output, const std::string& name);

      template<typename T>
      bool write_any(const T& input, const std::string& name);

    } // end of internal


    // aliases

    inline
    internal::anything load(const std::string& name)
    {	return internal::anything(name); }


    template<typename T>
    bool load(T& output, std::string name)
    {	return internal::read_any(output, name); }


    template< typename T >
    bool save(const T& input, const std::string& name)
    {	return internal::write_any(input, name); }

    namespace internal {

      // stream wrappers

      enum wrapper_id { WrapNone = 0,
			WrapFile = 1,
			WrapGz   = 2,
			WrapAny  = 2 };

      template< wrapper_id W >
      struct wrapper
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }

	static bool knows_ext(const std::string&) { return false; }

	static std::istream* wrap_in(std::string&) { return 0; }
	static std::ostream* wrap_out(std::string&) { return 0; }

  	static void find(std::list<std::string>&, const std::string&) {}
      };


      // stream readers

      enum reader_id { ReadNone     = 0,
		       ReadPnmPlain = 1,
		       ReadPnmRaw   = 2,
		       ReadAny      = 2 };

      template< reader_id F, typename T >
      struct reader
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }

	static bool knows_ext(const std::string&)
	{ return false; }

	static bool read(std::istream&, T&)
	{ return false; }
      };


      // stream writers

      enum writer_id { WriteNone     = 0,
		       WritePnmPlain = 1,
		       WritePnmRaw   = 2,
		       WriteAny      = 2 };

      template< writer_id F, typename T >
      struct writer
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }

	static bool knows_ext(const std::string&)
	{ return false; }

  	static bool write(std::ostream&, const T&)
  	{ return false; }
      };


    } // end of internal

  } // end of io

  // Export load & save into namespace oln:: for convenience.
  using io::load;
  using io::save;

} // end of oln

# include <oln/io/base.hxx>

#endif // OLENA_IO_BASE_HH_
