%module swilena_ntg_int_s
%include swilena_exception.i

%import swilena_ntg.i

%{
#include <ntg/all.hh>
%}

%define declare_int_s(Name, nbits, behavior)

%template(Name) ntg::int_s<nbits, behavior>;

// See comments in swilena_ntg_int_u.i
#define ntg_ ## Name ntg::int_s<nbits, behavior>

%enddef

declare_int_s(int_s8, 8, ntg::strict);
declare_int_s(int_s8u, 8, ntg::unsafe);
declare_int_s(int_s8s, 8, ntg::saturate);
declare_int_s(int_s16, 16, ntg::strict);
declare_int_s(int_s16u, 16, ntg::unsafe);
declare_int_s(int_s16s, 16, ntg::saturate);
declare_int_s(int_s32, 32, ntg::strict);
declare_int_s(int_s32u, 32, ntg::unsafe);
declare_int_s(int_s32s, 32, ntg::saturate);
