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

     bool has(const dpoint ## Dim ## d& ) const;
     dpoint ## Dim ## d dp(unsigned i) const;

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
     T& add(const dpoint ## Dim ## d&);


// Specialized manipulators
#if Dim == 1
T& add(coord);
#elif Dim == 2
T& add(coord, coord);
#elif Dim == 3
T& add(coord, coord, coord);
#endif

     // I/O
     %extend
     {
       bool load(const char *name) const
	 { return oln::load(*self, name); }
       bool save(const char *name) const
	 { return oln::save(*self, name); }
     }
%enddef


// The generic declarator for a given dimension.
%define decl_structelt(Dim)

%include swilena_describe.i
%include swilena_core.i

%import swilena_point ## Dim ## d.i

#if Dim == 1
%{
#include <oln/core/neighborhood1d.hh>
#include <oln/core/window1d.hh>
%}
#elif Dim == 2
%{
#include <oln/core/neighborhood2d.hh>
#include <oln/core/window2d.hh>
%}
#elif Dim == 3
%{
#include <oln/core/neighborhood3d.hh>
#include <oln/core/window3d.hh>
%}
#else
#error Unsupported dimension.
#endif

%{
#include <oln/io/basics.hh>
#include <oln/io/pnm.hh>
#include <oln/io/se.hh>
%}

namespace oln
{
  struct window ## Dim ## d
  {
    structelt_methods_now(window ## Dim ## d, Dim)
  };

  struct neighborhood ## Dim ## d
  {
    structelt_methods_now(neighborhood ## Dim ## d, Dim)
  };

}

%enddef
