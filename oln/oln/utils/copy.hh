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

#ifndef OLENA_MISC_COPY_HH
# define OLENA_MISC_COPY_HH

// This file contains implementations of image's copy constructors
//
//   / \    NEVER include this file unless you understand what
//  / ! \    it does and why you need it.
// `-----'
//
// FIXME: should be replaced soon by a correct const image handling.
// a detailed problem description can be found on:
// http://www.lrde.epita.fr/cgi-bin/twiki/view/Projects/OlenaConstImages


namespace oln {

# define OLENA_UTILS_IMAGE_DEEP_COPY		\
  if (rhs._has_data())				\
    *this = rhs.clone()  
  

# ifdef OLENA_CORE_IMAGE1D_HH

template<class T, class Exact>
image1d<T,Exact>::image1d(const self& rhs)
{
  OLENA_UTILS_IMAGE_DEEP_COPY;
}

template<class Exact>
image1d<bin,Exact>::image1d(const self& rhs)
{
  OLENA_UTILS_IMAGE_DEEP_COPY;
}

# endif


# ifdef OLENA_CORE_IMAGE2D_HH

template<class T, class Exact>
image2d<T,Exact>::image2d(const self& rhs)
{
  OLENA_UTILS_IMAGE_DEEP_COPY;
}

template<class Exact>
image2d<bin,Exact>::image2d(const self& rhs)
{
  OLENA_UTILS_IMAGE_DEEP_COPY;
}

# endif


# ifdef OLENA_CORE_IMAGE3D_HH

template<class T, class Exact>
image3d<T,Exact>::image3d(const self& rhs)
{
  OLENA_UTILS_IMAGE_DEEP_COPY;
}

template<class Exact>
image3d<bin,Exact>::image3d(const self& rhs)
{
  OLENA_UTILS_IMAGE_DEEP_COPY;
}

# endif


} // oln

#endif // OLENA_MISC_COPY_HH
