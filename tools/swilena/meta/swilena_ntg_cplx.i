%module swilena_ntg_cplx
%include swilena_exception.i

%import swilena_ntg.i

%{
#include <ntg/all.hh>
%}

%template(cplx_rf) ntg::cplx<ntg::rect, ntg::sfloat>;
%template(cplx_rd) ntg::cplx<ntg::rect, ntg::dfloat>;
%template(cplx_pf) ntg::cplx<ntg::polar, ntg::sfloat>;
%template(cplx_pd) ntg::cplx<ntg::polar, ntg::dfloat>;
