%{
#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <sstream>
%}

%include std_string.i

%define WindowMethods(Dim, Type)
     Type();
     Type(unsigned);
     bool has(const dpoint ## Dim ## d&);
     dpoint ## Dim ## d dp(unsigned) const;
     unsigned card() const;
     int delta() const;
     Type operator-() const;
     %extend {
       Type neg() const
	 { return -(*self); }

       std::string describe() const
	 {
	   std::ostringstream o;
	   o << *self;
	   return o.str();
	 }
     };
%enddef

%define SimpleWindowMethods(Dim, Type)
     WindowMethods(Dim, Type);

     Type& add(const dpoint ## Dim ## d&);

     %extend {
  Type inter(const Type& other) const
    { return oln::inter(*self, other); }
  Type uni(const Type& other) const
    { return oln::inter(*self, other); }
     };
%enddef

%define WWindowMethods(Dim, T)
     WindowMethods(Dim, w_window ## Dim ## d);

     %extend {
  static oln::w_window ## Dim ## d<T> of_win(T w, const oln::window ## Dim ## d& win)
    { return oln::mk_w_win_from_win(w, win); }
     };

     w_window ## Dim ## d& add(const dpoint ## Dim ## d&, T);
     T w(unsigned) const;
%enddef
