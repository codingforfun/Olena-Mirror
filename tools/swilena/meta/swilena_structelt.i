// -*- C++ -*-

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

     T operator-() const; // senseless for neighborhoods
     %extend
     {
	T neg() const // senseless for neighborhoods
	  { return -(*self); }
     }

    EXTEND_DESCRIBE(T)

%enddef


%define structelt_methods_now(T, Dim)

     structelt_methods(T, Dim)

     // utility constructor
     T(unsigned n, const coord crd[]);

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

%import swilena_point ## Dim ## d.i

#if Dim == 1
%{
#include <oln/basics1d.hh>
%}
#elif Dim == 2
%{
#include <oln/basics2d.hh>
%}
#elif Dim == 3
%{
#include <oln/basics3d.hh>
%}
#else
#error Unsupported dimension.
#endif

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
