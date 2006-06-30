// -*- C++ -*-

%include swilena_config.i

%include swilena_describe.i
%include swilena_core.i

%define decl_arith(Dim)

%include swilena_basics ## Dim ## d.i

%{
#include <oln/arith/ops.hh>
%}

%inline %{

  template<class R, class I1, class I2>
  static R my_plus(const I1& i1, const I2& i2)
  { return ::oln::arith::plus_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_minus(const I1& i1, const I2& i2)
  { return ::oln::arith::minus_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_times(const I1& i1, const I2& i2)
  { return ::oln::arith::times_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_div(const I1& i1, const I2& i2)
  { return ::oln::arith::div_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_min(const I1& i1, const I2& i2)
  { return ::oln::arith::min_with_ret<R>(i1, i2); }

  template<class R, class I1, class I2>
  static R my_max(const I1& i1, const I2& i2)
  { return ::oln::arith::max_with_ret<R>(i1, i2); }

%}

%enddef
