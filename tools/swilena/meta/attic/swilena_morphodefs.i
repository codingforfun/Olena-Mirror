%include swilena_image.i

%{
#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <oln/utils/copy.hh>
#include <oln/morpho/erosion.hh>
#include <oln/morpho/dilation.hh>
#include <oln/morpho/opening.hh>
#include <oln/morpho/closing.hh>
/* #include <oln/morpho/laplacian.hh> */
/* #include <oln/morpho/hit_or_miss.hh> */
#include <oln/morpho/gradient.hh>
#include <oln/morpho/top_hat.hh>
#include <oln/morpho/watershed.hh>
#include <oln/morpho/thinning.hh>
#include <oln/morpho/thickening.hh>
#include <oln/morpho/reconstruction.hh>
#include <oln/morpho/geodesic_dilation.hh>
#include <oln/morpho/geodesic_erosion.hh>
%}

/*---------------.
| General Macros |
`---------------*/

%define MorphoFunc1(Name, Impl, R, T1, T2)
%{
  R Name(T1 x, T2 y)
    { return oln::morpho:: Impl(x, y); }
%}
R Name(T1, T2);
%enddef
%define MorphoFunc2(Name, Impl, R, T1, T2, T3)
%{
R Name(T1 x, T2 y, T3 z)
    { return oln::morpho:: Impl(x, y, z); }
%}
R Name(T1, T2, T3);
%enddef
%define MorphoFunc3(Name, Impl, R, T1, T2, T3, T4)
%{
R Name(T1 x, T2 y, T3 z, T4 t)
    { return oln::morpho:: Impl(x, y, z, t); }
%}
%enddef

%define MorphoF1(Name, R, T1, T2)
     MorphoFunc1(Name, Name, R, T1, T2);
%enddef
%define MorphoF2(Name, R, T1, T2, T3)
     MorphoFunc2(Name, Name, R, T1, T2, T3);
%enddef
%define MorphoF3(Name, R, T1, T2, T3, T4)
     MorphoFunc3(Name, Name, R, T1, T2, T3, T4);
%enddef

%define FastMorphoF1(Name, R, T1, T2)
     MorphoFunc1(fast_ ## Name, fast:: Name, R, T1, T2);
%enddef
%define FastMorphoF2(Name, R, T1, T2, T3)
     MorphoFunc2(fast_ ## Name, fast:: Name, R, T1, T2, T3);
%enddef
%define FastMorphoF3(Name, R, T1, T2, T3, T4)
     MorphoFunc3(fast_ ## Name, fast:: Name, R, T1, T2, T3, T4);
%enddef

/*---------------------.
| Classical Algorithms |
`---------------------*/
     
%define MorphoClosing(Img, W)
     MorphoF1(closing, Img, const Img&, const W&);
%enddef
%define FastMorphoClosing(Img, W)
     MorphoClosing(Img, W);
     FastMorphoF1(closing, Img, const Img&, const W&);
%enddef


%define MorphoOpening(Img, W)
     MorphoF1(opening, Img, const Img&, const W&);
%enddef
%define FastMorphoOpening(Img, W)
     MorphoOpening(Img, W);
     FastMorphoF1(opening, Img, const Img&, const W&);
%enddef

%define MorphoDilation(Img, W)
     MorphoF1(dilation, Img, const Img&, const W&);
%enddef
%define FastMorphoDilation(Img, W)
     MorphoDilation(Img, W);
     FastMorphoF1(dilation, Img, const Img&, const W&);
%enddef

%define MorphoErosion(Img, W)
     MorphoF1(erosion, Img, const Img&, const W&);
%enddef
%define FastMorphoErosion(Img, W)
     MorphoErosion(Img, W);
     FastMorphoF1(erosion, Img, const Img&, const W&);
%enddef


/*----------.
| Gradients |
`----------*/

%define MorphoBeucherGradient(Img, W)
     MorphoF1(beucher_gradient, Img, const Img&, const W&);
%enddef
%define FastMorphoBeucherGradient(Img, W)
     MorphoBeucherGradient(Img, W);
     FastMorphoF1(beucher_gradient, Img, const Img&, const W&);
%enddef


%define MorphoInternalGradient(Img, W)
     MorphoF1(internal_gradient, Img, const Img&, const W&);
%enddef
%define FastMorphoInternalGradient(Img, W)
     MorphoInternalGradient(Img, W);
     FastMorphoF1(internal_gradient, Img, const Img&, const W&);
%enddef


%define MorphoExternalGradient(Img, W)
     MorphoF1(external_gradient, Img, const Img&, const W&);
%enddef
%define FastMorphoExternalGradient(Img, W)
     MorphoExternalGradient(Img, W);
     FastMorphoF1(external_gradient, Img, const Img&, const W&);
%enddef

/*---------.
| Top Hats |
`---------*/

%define MorphoWhiteTopHat(Img, W)
     MorphoF1(white_top_hat, Img, const Img&, const W&);
%enddef
%define FastMorphoWhiteTopHat(Img, W)
     MorphoWhiteTopHat(Img, W);
     FastMorphoF1(white_top_hat, Img, const Img&, const W&);
%enddef

%define MorphoBlackTopHat(Img, W)
     MorphoF1(black_top_hat, Img, const Img&, const W&);
%enddef
%define FastMorphoBlackTopHat(Img, W)
     MorphoBlackTopHat(Img, W);
     FastMorphoF1(black_top_hat, Img, const Img&, const W&);
%enddef

%define MorphoSelfComplementaryTopHat(Img, W)
     MorphoF1(self_complementary_top_hat, Img, const Img&, const W&);
%enddef
%define FastMorphoSelfComplementaryTopHat(Img, W)
     MorphoSelf_ComplementaryTopHat(Img, W);
     FastMorphoF1(self_complementary_top_hat, Img, const Img&, const W&);
%enddef

/*----------------------.
| Thinning & Thickening |
`----------------------*/

%define MorphoThinning(Img, W1, W2)
     MorphoF2(thinning, Img, const Img&, const W1&, const W2&);
%enddef
%define FastMorphoThinning(Img, W1, W2)
     MorphoThinning(Img, W1, W2);
     FastMorphoF2(thinning, Img, const Img&, const W1&, const W2&);
%enddef

%define MorphoThickening(Img, W1, W2)
     MorphoF2(thickening, Img, const Img&, const W1&, const W2&);
%enddef
%define FastMorphoThickening(Img, W1, W2)
     MorphoThickening(Img, W1, W2);
     FastMorphoF2(thickening, Img, const Img&, const W1&, const W2&);
%enddef


/*----------------------------.
| Geodesic dilation & erosion |
`----------------------------*/

%define MorphoGeodesicDilation(Img1, Img2, N)
     MorphoFunc2(sure_geodesic_dilation, sure::geodesic_dilation, Img1, const Img1&, const Img2&, const N&);
     MorphoF2(geodesic_dilation, Img1, const Img1&, const Img2&, const N&);
%enddef
%define MorphoGeodesicErosion(Img1, Img2, N)
     MorphoFunc2(sure_geodesic_erosion, sure::geodesic_dilation, Img1, const Img1&, const Img2&, const N&);
     MorphoF2(geodesic_erosion, Img1, const Img1&, const Img2&, const N&);
%enddef

/*---------------.
| Reconstruction |
`---------------*/

%define MorphoReconstructionDilation(P, Img1, Img2, N, W)
     MorphoFunc2(sure_geodesic_reconstruction_dilation, sure::geodesic_reconstruction_dilation,
	      Img1, const Img1&, const Img2&, const N&);
     MorphoFunc2(sequential_geodesic_reconstruction_dilation, sequential::geodesic_reconstruction_dilation,
	      Img1, const Img1&, const Img2&, const N&);
//     MorphoF2(vincent_sequential_geodesic_reconstruction_dilation,
//	      Img1, const Img1&, const Img2&, const N&);
     MorphoFunc2(hybrid_geodesic_reconstruction_dilation, hybrid::geodesic_reconstruction_dilation,
	      Img1, const Img1&, const Img2&, const N&);
//     MorphoF3(exist_init_dilation,
//	      bool, const P&, const Img1&, const Img2&, const W&);
%enddef
%define MorphoReconstructionErosion(P, Img1, Img2, N, W)
     MorphoFunc2(sure_geodesic_reconstruction_erosion, sure::geodesic_reconstruction_erosion,
	      Img1, const Img1&, const Img2&, const N&);
     MorphoFunc2(sequential_geodesic_reconstruction_erosion, sequential::geodesic_reconstruction_erosion,
	      Img1, const Img1&, const Img2&, const N&);
     MorphoFunc2(hybrid_geodesic_reconstruction_erosion, hybrid::geodesic_reconstruction_erosion,
	      Img1, const Img1&, const Img2&, const N&)
//     MorphoF3(exist_init_erosion,
//	      bool, const P&, const Img1&, const Img2&, const W&);
%enddef


/*-----------.
| Watersheds |
`-----------*/

%inline %{
namespace swilena
  {
    template<typename LabelImg, typename Img, typename N>
      void watershed_seg(LabelImg& l, const Img& i, const N& n)
      { 
	l = oln::morpho::watershed_seg<Value(LabelImg) >(i, n); 
      }
    template<typename LabelImg, typename Img, typename N>
      void watershed_con(LabelImg& l, const Img& i, const N& n)
      { 
	l = oln::morpho::watershed_con<Value(LabelImg) >(i, n); 
      }

  }
%}

%define MorphoWatershed(Img, LabelImg, N)
     %template (watershed_seg) swilena::watershed_seg<LabelImg, Img, N>;
     %template (watershed_con) swilena::watershed_con<LabelImg, Img, N>;
%{
void watershed_seg_or(const Img& x, LabelImg& y, const N& z)
    { oln::morpho::watershed_seg_or(x, y, z); }
%}
void watershed_seg_or(const Img& x, LabelImg& y, const N& z);
%enddef


// FIXME: do laplacian, hit_or_miss
     
