// -*- C++ -*-
// Generic declaration forms.

// Explicit template prefixes
#define swpref_0p()
#define swprefa_0p()
#define swpref_1p()  , typename P1
#define swprefa_1p() <P1 >
#define swpref_2p()  , typename P1, typename P2
#define swprefa_2p() <P1, P2 >
#define swpref_3p()  , typename P1, typename P2, typename P3
#define swprefa_3p() <P1, P2, P3 >

// Function parameters and arguments
#define swprm_0()
#define swarg_0() ()
#define swcl_0() ()

#define swprm_1() , typename A1
#define swarg_1() (A1& a1)
#define swcl_1() (a1)

#define swprm_2() , typename A1, typename A2
#define swarg_2() (A1& a1, A2& a2)
#define swcl_2() (a1, a2)

#define swprm_3() , typename A1, typename A2, typename A3
#define swarg_3() (A1& a1, A2& a2, A3& a3)
#define swcl_3() (a1, a2, a3)

#define swprm_4() , typename A1, typename A2, typename A3, typename A4
#define swarg_4() (A1& a1, A2& a2, A3& a3, A4& a4)
#define swcl_4() (a1, a2, a3, a4)

// Generic declaration form
%define swdecl(Name, Func, Pref, Args)
%inline %{
template<typename Ret swpref_ ## Pref () swprm_ ## Args () >
static Ret my_ ## Name swarg_ ## Args ()
{ return Func swprefa_ ## Pref() swcl_ ## Args() ; }
%}
%enddef

%define swdecli(Incl, Name, Func, Pref, Args)
%{
#include Incl
%}
swdecl(Name, Func, Pref, Args)
%enddef

// Instanciation forms
%define swmake0(Name, R)
%template(Name) my_ ## Name<R >;
%enddef

%define swmake1(Name, R, A1)
%template(Name) my_ ## Name<R, A1 >;
%enddef

%define swmake2(Name, R, A1, A2)
%template(Name) my_ ## Name<R, A1, A2 >;
%enddef

%define swmake3(Name, R, A1, A2, A3)
%template(Name) my_ ## Name<R, A1, A2, A3 >;
%enddef

%define swmake4(Name, R, A1, A2, A3, A4)
%template(Name) my_ ## Name<R, A1, A2, A3, A4 >;
%enddef

%define swmake5(Name, R, A1, A2, A3, A4, A5)
%template(Name) my_ ## Name<R, A1, A2, A3, A4, A5 >;
%enddef


