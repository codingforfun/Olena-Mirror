// Copyright (C) 2006 EPITA Research and Development Laboratory
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
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
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

#ifndef OLENA_IO_SE_FWD_HH
# define OLENA_IO_SE_FWD_HH

#include <string>

/// \file oln/io/se_fwd.hh
/// \brief Forward declarations for oln/io/se_window.hh and
/// oln/io/se_neighborhood.hh

namespace oln {

      /// Windows.
      /// \{
      class window1d;
      class window2d;
      class window3d;
      /// \}

      /// Neighborhoods.
      /// \{
      class neighborhood1d;
      class neighborhood2d;
      class neighborhood3d;
      /// \}

  namespace io {

    namespace internal {

      /// Functions from oln/io/se_window.hh.
      /// \{
      inline bool read(window1d& output, const std::string& name);
      inline bool read(window2d& output, const std::string& name);
      inline bool read(window3d& output, const std::string& name);
      inline bool write(const window1d& input, const std::string& name);
      inline bool write(const window2d& input, const std::string& name);
      inline bool write(const window3d& input, const std::string& name);
      /// \}

      /// Functions from oln/io/se_neighborhood.hh.
      /// \{
      inline bool read(neighborhood1d& output, const std::string& name);
      inline bool read(neighborhood2d& output, const std::string& name);
      inline bool read(neighborhood3d& output, const std::string& name);
      inline bool write(const neighborhood1d& input, const std::string& name);
      inline bool write(const neighborhood2d& input, const std::string& name);
      inline bool write(const neighborhood3d& input, const std::string& name);
      /// \}

    } // end of namespace internal

  } // end of namespace io

} // end of namespace oln

#endif // OLENA_IO_SE_FWD_HH
