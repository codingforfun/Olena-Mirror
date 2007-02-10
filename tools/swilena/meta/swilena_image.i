// -*- C++ -*-

// Copyright (C) 2003, 2006  EPITA Research and Development Laboratory
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


%include swilena_config.i

%define decl_image(Dim)
%include swilena_describe.i
%include swilena_exception.i
%include swilena_core.i

%include swilena_basics ## Dim ## d.i

%import swilena_imagesize ## Dim ## d.i
%import swilena_point ## Dim ## d.i

%nodefaultctor oln::io::internal::anything;
namespace oln
{
  template<typename T>
  struct image ## Dim ## d;

  namespace io {
    namespace internal {
      struct anything {
	anything();
	anything(const anything& rhs);
      };
    }
    ::oln::io::internal::anything load(const char*);
  }
}
%enddef

%define make_image(Name, Dim, T, ValueType)
#define I image ## Dim ## d < T >

namespace oln
{

  template <>
  struct image ## Dim ## d < T >
  {
    // default constructor
    I();
    I(const image ## Dim ## d_size&);
    I(const oln::io::internal::anything& any);

    // operators
    I clone() const;

    void clear();

    image ## Dim ## d_size size() const;
    size_t npoints() const;
    coord border() const;

     bool hold(const point ## Dim ## d&) const;

     %extend {

#if defined SWIGPYTHON || defined SWIGRUBY
       ValueType __getitem__(const point ## Dim ## d& p)
	 { return (*self)[p]; }
       I& __setitem__(const point ## Dim ## d& p, ValueType v)
	 { (*self)[p] = v; return *self; }
#else
       ValueType at(const point ## Dim ## d& p)
	 { return (*self)[p]; }
       I& set(const point ## Dim ## d& p, ValueType v) 
	 { (*self)[p] = v; return *self; }
       T& ref(const point ## Dim ## d& p)
	 { return (*self)[p]; }
#endif
     }
     
     EXTEND_DESCRIBE(I)

#if Dim == 1
     I(coord, coord border = 2);

     T& operator()(coord);

     %extend {
#if defined SWIGRUBY
       ValueType __getitem__(coord x)
	 { return (*self)(x); }
       I __setitem__(coord x, ValueType v)
	 { (*self)(x) = v; return *self; }
#else
       ValueType at(coord x) { return (*self)(x); }
       I& set(coord x, ValueType v) { (*self)(x) = v; return *self; }
       T& ref(coord x) { return (*self)(x); }
#endif
     }

#elif Dim == 2
     I(coord, coord, coord border = 2);

     T& operator()(coord, coord);

     %extend {
#if defined SWIGRUBY
       ValueType __getitem__(coord x, coord y)
       { return (*self)(x, y); }
       I __setitem__(coord x, coord y, ValueType v)
       { (*self)(x, y) = v; return *self; }
#else
       ValueType at(coord x, coord y) { return (*self)(x, y); }
       I& set(coord x, coord y, ValueType v)  { (*self)(x, y) = v; return *self; }
       T& ref(coord x, coord y)  { return (*self)(x, y); }
#endif
      }

#elif Dim == 3
     I(coord, coord, coord, coord border = 3);

     T& operator()(coord, coord, coord);

     %extend {
#if defined SWIGRUBY
       ValueType __getitem__(coord x, coord y, coord z)
	 { return (*self)(x, y, z); }
       I __setitem__(coord x, coord y, coord z, ValueType v)
	 { (*self)(x, y, z) = v; return *self; }
#else
       ValueType at(coord x, coord y, coord z)
       { return (*self)(x, y, z); }
       I& set(coord x, coord y, coord z, ValueType v) 
       { (*self)(x, y, z) = v; return *self; }
       T& ref(coord x, coord y, coord z) 
       { return (*self)(x, y, z); }
#endif
     }
#endif

#if Dim >= 1
     coord ncols() const;
#endif
#if Dim >= 2
     coord nrows() const;
#endif 
#if Dim >= 3
     coord nslices() const;
#endif

     // image I/O

#if Dim == 2
     %extend
     {
       bool load(const char *name)
       { return oln::load(*self, name); }
       bool save(const char *name) const
       { return oln::save(*self, name); }
     }
#endif

  };
}

%template(Name) oln::image ## Dim ## d< T >;

#undef I
%enddef
