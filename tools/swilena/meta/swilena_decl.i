// -*- C++ -*-

// Copyright (C) 2003  EPITA Research and Development Laboratory
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
