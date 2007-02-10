// -*- C++ -*-

// Copyright (C) 2003, 2004, 2006  EPITA Research and Development Laboratory
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

/*----------------------------------------------.
| oln::window, and oln::neighborhood structures |
`----------------------------------------------*/

// These structures share nearly the same interface;
// the following macro factorizes the declaration.

%define structelt_methods(T, Dim)
     // constructors
     T();
     T(unsigned size);

     // characteristics
     coord delta() const;
     unsigned card() const;
     bool is_centered() const;

     bool has(const oln::dpoint ## Dim ## d& ) const;
     oln::dpoint ## Dim ## d dp(unsigned i) const;

     // accessors & operations

    EXTEND_DESCRIBE(T)

%enddef


%define structelt_methods_now(T, Dim)

     structelt_methods(T, Dim)

    // FIXME: SWIG is unable to wrap this ctor because of a strange
    // invalid pointer conversion w.r.t. its second argument.  Have a
    // look at the SWIG manual for more details on how arrays are
    // handled.
#if 0
     // utility constructor
     T(unsigned n, const coord crd[]);
#endif

     // set operations
     %extend
     {
        T uni(const T& other) const
	  { return oln::uni(*self, other); }
	T inter(const T& other) const
          { return oln::inter(*self, other); }
     }

     // structure manipulators
     T& add(const oln::dpoint ## Dim ## d&);


// Specialized manipulators
#if Dim == 1
T& add(coord);
#elif Dim == 2
T& add(coord, coord);
#elif Dim == 3
T& add(coord, coord, coord);
#endif

#if Dim == 2
     // I/O
     %extend
     {
       bool load(const char *name)
	 { return oln::load(*self, name); }

       bool save(const char *name) const
	 { return oln::save(*self, name); }
     }
#endif

%enddef


// The generic declarator for a given dimension.
%define decl_structelt(Dim)

%include swilena_describe.i
%include swilena_core.i

%include swilena_basics ## Dim ##d.i

%import swilena_point ## Dim ## d.i

namespace oln
{
  struct window ## Dim ## d
  {
    structelt_methods_now(window ## Dim ## d, Dim)
  };

#if Dim == 1
  const window1d& win_c2_only();
  const window1d& win_c2p();
  window1d mk_win_segment(unsigned);
#elif Dim == 2
  const window2d& win_c4_only();
  const window2d& win_c4p();
  const window2d& win_c8_only();
  const window2d& win_c8p();
  window2d mk_win_rectangle(unsigned, unsigned);
  window2d mk_win_ellipse(float, float);
  window2d mk_win_square(unsigned);
  window2d mk_win_disc(float);
#elif Dim == 3
  const window3d& win_c6_only();
  const window3d& win_c6p();
  const window3d& win_c18_only();
  const window3d& win_c18p();
  const window3d& win_c26_only();
  const window3d& win_c26p();
  window3d mk_win_block(unsigned, unsigned, unsigned);
  window3d mk_win_ellipsoid(float, float, float);
  window3d mk_win_cube(unsigned);
  window3d mk_win_ball(float);
#endif

  struct neighborhood ## Dim ## d
  {
    structelt_methods_now(neighborhood ## Dim ## d, Dim)
  };

#if Dim == 1
  const neighborhood1d& neighb_c2();
  neighborhood1d mk_neighb_segment(unsigned);
  window1d mk_win_from_neighb(const neighborhood1d&);
#elif Dim == 2
  const neighborhood2d& neighb_c4();
  const neighborhood2d& neighb_c8();
  neighborhood2d mk_neighb_square(unsigned);
  neighborhood2d mk_neighb_rectangle(unsigned, unsigned);
  window2d mk_win_from_neighb(const neighborhood2d&);
#elif Dim == 3
  const neighborhood3d& neighb_c6();
  const neighborhood3d& neighb_c18();
  const neighborhood3d& neighb_c26();
  neighborhood3d mk_neighb_block(unsigned, unsigned, unsigned);
  neighborhood3d mk_neighb_cube(unsigned);
  window3d mk_win_from_neighb(const neighborhood3d&);
#endif
}

%enddef
