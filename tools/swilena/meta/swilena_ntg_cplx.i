%module swilena_ntg_cplx
%include swilena_exception.i

%import swilena_ntg.i

%{
#include <ntg/all.hh>
%}

%define declare_cplx(Name, repr, T)

%template(Name) ntg::cplx<repr, T>;

// See comments in swilena_ntg_int_u.i
#define ntg_ ## Name ntg::cplx<repr, T>

%enddef


declare_cplx(cplx_rf, ntg::rect, ntg_float_s);
declare_cplx(cplx_rd, ntg::rect, ntg_float_d);
declare_cplx(cplx_pf, ntg::polar, ntg_float_s);
declare_cplx(cplx_pd, ntg::polar, ntg_float_d);
