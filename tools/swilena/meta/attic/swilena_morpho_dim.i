// -*- C++ -*-
%include swilena_morphodefs.i

%define DoMakeWS(Dim, Type, LabelType)
  MorphoWatershed(oln::image##Dim##d<oln:: Type >, \
		  oln::image##Dim##d<oln:: LabelType >, \
		  oln::neighborhood##Dim##d );
%enddef
%define MakeWS(Dim, Type)  
  DoMakeWS(Dim, Type, int_u8);
  DoMakeWS(Dim, Type, int_u16);
  DoMakeWS(Dim, Type, int_u32);
%enddef

%define DoMakeMorphoW(Img, W)
  FastMorphoClosing(Img, W);
FastMorphoOpening(Img, W);
FastMorphoDilation(Img, W);
FastMorphoErosion(Img, W);

FastMorphoBeucherGradient(Img, W);
FastMorphoInternalGradient(Img, W);
FastMorphoExternalGradient(Img, W);

FastMorphoWhiteTopHat(Img, W);
FastMorphoBlackTopHat(Img, W);
FastMorphoSelfComplementaryTopHat(Img, W);

// FIXME: don't we need different structural elements there ?
FastMorphoThinning(Img, W, W);
FastMorphoThickening(Img, W, W);

%enddef

%define Float_DoMakeMorphoW(Img, W)
  
  // No fast versions when working with floats
  
  MorphoClosing(Img, W);
MorphoOpening(Img, W);
MorphoDilation(Img, W);
MorphoErosion(Img, W);

// FIXME: don't we need different structural elements there ?
MorphoThinning(Img, W, W);
MorphoThickening(Img, W, W);

// No gradients nor tophats for floats (yet)
//      MorphoBeucherGradient(Img, W); 
//      MorphoInternalGradient(Img, W);
//      MorphoExternalGradient(Img, W);

//      MorphoWhiteTopHat(Img, W);
//      MorphoBlackTopHat(Img, W);
//      MorphoSelfComplementaryTopHat(Img, W);
     
%enddef

%define DoMakeMorphoN(P, Img, N, W)
  // FIXME: don't we need different window types here ?
  MorphoGeodesicDilation(Img, Img, N);
MorphoGeodesicErosion(Img, Img, N);
MorphoReconstructionDilation(P, Img, Img, N, W);
MorphoReconstructionErosion(P, Img, Img, N, W);
%enddef

%define MakeMorpho(Dim, Type)
 DoMakeMorphoW(oln::image##Dim##d<oln::Type >, oln::window##Dim##d);
 DoMakeMorphoN(oln::point##Dim##d, oln::image##Dim##d<oln:: Type >, oln::neighborhood##Dim##d, oln::window##Dim##d);
MakeWS(Dim, Type);
%enddef

%define Float_MakeMorpho(Dim, Type)
  Float_DoMakeMorphoW(oln::image##Dim##d<oln:: Type>, oln::window##Dim##d);
MakeWS(Dim, Type);

// Geodesic stuff does not work with floats
//  DoMakeMorphoN(oln::point##Dim##d, oln::image##Dim##d<oln:: Type >, \
// 	       oln::neighborhood##Dim##d, oln::window##Dim##d);
%enddef

%define MakeMorphoDim(Dim)
MakeMorpho(Dim, int_u8);
MakeMorpho(Dim, int_u16);
MakeMorpho(Dim, int_u32);
//MakeMorpho(Dim, int_u8s);
//MakeMorpho(Dim, int_u16s);
//MakeMorpho(Dim, int_u32s);
MakeMorpho(Dim, int_s8);
MakeMorpho(Dim, int_s16);
MakeMorpho(Dim, int_s32);
//MakeMorpho(Dim, int_s8s);
//MakeMorpho(Dim, int_s16s);
//MakeMorpho(Dim, int_s32s);
%enddef

%define Float_MakeMorphoDim(Dim)
  Float_MakeMorpho(Dim, float_s);
  Float_MakeMorpho(Dim, float_d);
%enddef


#if DIMENSION == 1
MakeMorphoDim(1);
Float_MakeMorphoDim(1);
#elif DIMENSION == 2
MakeMorphoDim(2);
Float_MakeMorphoDim(2);
#elif DIMENSION == 3
MakeMorphoDim(3);
Float_MakeMorphoDim(3);
#endif
