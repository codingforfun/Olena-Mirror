// -*- C++ -*-

%import swilena.i

// Headers must be re-imported at this point
%{
#include <ntg/all.hh>
%}

/*-------.
| Points |
`-------*/

%include swilena_dpoint.i
%include swilena_point.i

#if DIMENSION == 1
MakeDPoint(1);
MakePoint(1);
#elif DIMENSION == 2
MakeDPoint(2);
MakePoint(2);
#elif DIMENSION == 3
MakeDPoint(3);
MakePoint(3);
#endif

/*--------------------.
| Structural Elements |
`--------------------*/

%include swilena_window.i
%include swilena_neighborhood.i

%include swilena_w_window.i

#if DIMENSION == 1
MakeWindow(1);
MakeNeigh(1);
MakeWWindow(1, int);
MakeWWindow(1, float);
#elif DIMENSION == 2
MakeWindow(2);
MakeNeigh(2);
MakeWWindow(2, int);
MakeWWindow(2, float);
#elif DIMENSION == 3
MakeWindow(3);
MakeNeigh(3);
MakeWWindow(3, int);
MakeWWindow(3, float);
#endif

/*-------.
| Images |
`-------*/

%include swilena_image.i

%define MakeDimScalarImages(Dim)
     MakeImage(image ## Dim ## d_bin, Dim, bin);
     MakeImage(image ## Dim ## d_u8, Dim, int_u8);
     MakeImage(image ## Dim ## d_u16, Dim, int_u16);
     MakeImage(image ## Dim ## d_u32, Dim, int_u32);
     MakeImage(image ## Dim ## d_s8, Dim, int_s8);
     MakeImage(image ## Dim ## d_s16, Dim, int_s16);
     MakeImage(image ## Dim ## d_s32, Dim, int_s32);
     MakeImage(image ## Dim ## d_u8s, Dim, int_u8s);
     MakeImage(image ## Dim ## d_u16s, Dim, int_u16s);
     MakeImage(image ## Dim ## d_u32s, Dim, int_u32s);
     MakeImage(image ## Dim ## d_s8s, Dim, int_s8s);
     MakeImage(image ## Dim ## d_s16s, Dim, int_s16s);
     MakeImage(image ## Dim ## d_s32s, Dim, int_s32s);
     MakeImage(image ## Dim ## d_float_s, Dim, float_s);
     MakeImage(image ## Dim ## d_float_d, Dim, float_d);
%enddef
%define MakeDimImages(Dim)
     MakeDimScalarImages(Dim);
     MakeImage(image ## Dim ## d_rgb_8, Dim, rgb_8);
     MakeImage(image ## Dim ## d_rgb_16, Dim, rgb_16);
     MakeImage(image ## Dim ## d_rgb_32, Dim, rgb_32);
%enddef

#if DIMENSION == 1
MakeDimImages(1);
#elif DIMENSION == 2
MakeDimImages(2);
#elif DIMENSION == 3
MakeDimImages(3);
#endif

