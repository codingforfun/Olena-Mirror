// -*- C++ -*-

%include swilena_describe.i
%include swilena_core.i

%define decl_arith(Dim)

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
#endif

%{
#include <oln/arith/ops.hh>
%}

%inline %{

  template<class R, class I1, class I2>
  static R my_plus(const I1& i1, const I2& i2)
  { return ::oln::arith::plus<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_minus(const I1& i1, const I2& i2)
  { return ::oln::arith::minus<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_times(const I1& i1, const I2& i2)
  { return ::oln::arith::times<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_div(const I1& i1, const I2& i2)
  { return ::oln::arith::div<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_min(const I1& i1, const I2& i2)
  { return ::oln::arith::min<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_max(const I1& i1, const I2& i2)
  { return ::oln::arith::max<R>(i1, i2); }

%}

%enddef
