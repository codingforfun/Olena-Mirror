%include swilena_meta_win.i

namespace oln
{
  template<typename T>
  struct w_window1d
  {
    WWindowMethods(1, T);

    w_window1d& add(unsigned, T);

  };

  template<typename T>
  struct w_window2d
  {
    WWindowMethods(2, T);
    
    w_window2d& add(unsigned, unsigned, T);
  };

  template<typename T>
  struct w_window3d
  {
    WWindowMethods(3, T);

    w_window3d& add(unsigned, unsigned, unsigned, T);
  };

}

%define MakeWWindow(Dim, T)
%{
  typedef oln::w_window ## Dim ## d<T > w_win ## Dim ## d_ ## T;
%}
  typedef oln::w_window ## Dim ## d<T > w_win ## Dim ## d_ ## T;

%template (w_win ## Dim ## d_ ## T) oln::w_window ## Dim ## d<T >;

%enddef



