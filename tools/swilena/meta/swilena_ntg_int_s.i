%module swilena_ntg_int_s
%include swilena_exception.i

%import swilena_ntg.i

%{
#include <ntg/all.hh>
%}

%template(int_s8) ntg::int_s<8, ntg::strict>;
%template(int_s8u) ntg::int_s<8, ntg::unsafe>;
%template(int_s8s) ntg::int_s<8, ntg::saturate>;
%template(int_s16) ntg::int_s<16, ntg::strict>;
%template(int_s16u) ntg::int_s<16, ntg::unsafe>;
%template(int_s16s) ntg::int_s<16, ntg::saturate>;
%template(int_s32) ntg::int_s<32, ntg::strict>;
%template(int_s32u) ntg::int_s<32, ntg::unsafe>;
%template(int_s32s) ntg::int_s<32, ntg::saturate>;


