// -*- C++ -*-

/*------------------------.
| oln::w_window structure |
`------------------------*/

%define structelt_methods_w(T, W, Dim)
     structelt_methods(T, Dim)

     W w(unsigned) const;
     
     T& add(const dpoint ## Dim ## d&, const W& );

%enddef

%define decl_w_win(Dim)

%include swilena_describe.i
%include swilena_core.i

%import swilena_structelt ## Dim ## d.i

#if Dim == 1
%{
#include <oln/core/w_window1d.hh>
%}
#elif Dim == 2
%{
#include <oln/core/w_window2d.hh>
%}
#elif Dim == 3
%{
#include <oln/core/w_window3d.hh>
%}
#else
#error Unsupported dimension.
#endif

namespace oln
{
  template<typename T>
  struct w_window ## Dim ## d
  {
    structelt_methods_w(w_window ## Dim ## d<T>, T, Dim);

    %extend
    {
      static w_window ## Dim ## d<T> of_window(T w, const window ## Dim ## d& win)
	{ return oln::mk_w_win_from_win(w, win); }
    }
  };
}
%enddef

%define make_w_win(name, Dim, T)
%template(name) oln::w_window ## Dim ## d<T >;
%enddef
