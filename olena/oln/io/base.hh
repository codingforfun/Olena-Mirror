// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

# include <oln/io/image_read.hh>
# include <oln/io/image_write.hh>

# include <oln/io/se_window.hxx>
# include <oln/io/se_neighborhood.hxx>

namespace oln {

  namespace io {

    namespace internal {

      template<typename T>
      bool read_any(T& output, const std::string& name)
      {
	return read(output, name);
      }

      template<typename T>
      bool write_any(const T& input, const std::string& name)
      {
	return write(input, name);
      }

    } // end of internal

    // aliases
    
    inline
    internal::anything load(const std::string& name)
    { return internal::anything(name); }

    template<class T>
    bool load(T& output, std::string name)
    { return internal::read_any(output, name); }

    template< typename T >
    bool save(const T& input, const std::string& name)
    { 
      return internal::write_any(input, name);
    }

  } // end of io

  // Export load & save into namespace oln:: for convenience.
  using io::load;
  using io::save;

} // end of oln

#endif // OLENA_IO_BASE_HH_
