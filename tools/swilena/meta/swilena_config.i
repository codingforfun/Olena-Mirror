								// -*- C++ -*-

// Copyright (C) 2006  EPITA Research and Development Laboratory
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


// Workaround for ruby.hh defining some macros already defined in the
// top-level config.h (PACKAGE_BUGREPORT, PACKAGE_NAME,
// PACKAGE_STRING, PACKAGE_TARNAME, PACKAGE_VERSION).  Simply undef
// them to avoid redefinitions.

%{
#if defined(SWIGRUBY)

# ifdef PACKAGE_BUGREPORT
#  undef PACKAGE_BUGREPORT
# endif

# ifdef PACKAGE_NAME
#  undef PACKAGE_NAME
# endif

# ifdef PACKAGE_STRING
#  undef PACKAGE_STRING
# endif

# ifdef PACKAGE_TARNAME
#  undef PACKAGE_TARNAME
# endif

# ifdef PACKAGE_VERSION
#  undef PACKAGE_VERSION
# endif

#endif // SWIGRUBY
%}
