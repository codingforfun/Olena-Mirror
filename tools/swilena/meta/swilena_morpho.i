// -*- C++ -*-

/********** Morpho algorithms taking two arguments ***********/

%define decl_morpho_2(Incl, Func, HAS_FAST)
%{
#include Incl
%}

%inline %{
template<typename R, typename A1, typename A2 >
static R my_ ## Func( A1& a1,  A2& a2)
{ return oln::morpho::Func(a1, a2); }
%}
#if HAS_FAST
%inline %{
template<typename R, typename A1, typename A2 >
static R my_fast_ ## Func( A1& a1,  A2& a2)
{ return oln::morpho::fast::Func(a1, a2); }
%}
#endif

%enddef
%define make_morpho_2(Func, R, A1, A2, HAS_FAST)
%template(Func) my_ ## Func<R, A1, A2 >;
#if HAS_FAST
%template(fast_ ## Func) my_fast_ ## Func<R, A1, A2 >;
#endif
%enddef

/************ Morpho algorithms taking three arguments *******************/

%define decl_morpho_3(Incl, Func, HAS_FAST)
%{
#include Incl
%}

%inline %{
template<typename R, typename A1, typename A2, typename A3>
static R my_ ## Func( A1& a1,  A2& a2,  A3& a3)
{ return oln::morpho::Func(a1, a2, a3); }
%}
#if HAS_FAST
%inline %{
template<typename R, typename A1, typename A2, typename A3>
static R my_fast_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::fast::Func(a1, a2, a3); }
%}
#endif

%enddef
%define make_morpho_3(Func, R, A1, A2, A3, HAS_FAST)
%template(Func) my_ ## Func<R, A1, A2, A3 >;
#if HAS_FAST
%template(fast_ ## Func) my_fast_ ## Func<R, A1, A2, A3 >;
#endif
%enddef

/************ Morpho algorithms taking four arguments *******************/

%define decl_morpho_4(Incl, Func, HAS_FAST)
%{
#include Incl
%}

%inline %{
template<typename R, typename A1, typename A2, typename A3, typename A4>
static R my_ ## Func( A1& a1,  A2& a2,  A3& a3,  A4& a4)
{ return oln::morpho::Func(a1, a2, a3, a4); }
%}
#if HAS_FAST
%inline %{
template<typename R, typename A1, typename A2, typename A3, typename A4>
static R my_fast_ ## Func( A1& a1,  A2& a2,  A3& a3,  A4& a4)
  { return oln::morpho::fast::Func(a1, a2, a3, a4); }
%}
#endif

%enddef
%define make_morpho_4(Func, R, A1, A2, A3, A4, HAS_FAST)
%template(Func) my_ ## Func<R, A1, A2, A3, A4 >;
#if HAS_FAST
%template(fast_ ## Func) my_fast_ ## Func<R, A1, A2, A3, A4 >;
#endif
%enddef

/************ Morpho algorithms taking three arguments and located in three namespaces  ****************/

%define decl_morpho_3ssh(Incl, Func)
%{
#include Incl
%}

%inline %{
template<typename R, typename A1, typename A2, typename A3>
static R my_sure_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::sure::Func(a1, a2, a3); }
template<typename R, typename A1, typename A2, typename A3>
static R my_sequential_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::sequential::Func(a1, a2, a3); }
template<typename R, typename A1, typename A2, typename A3>
static R my_hybrid_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::hybrid::Func(a1, a2, a3); }
%}
%enddef
%define make_morpho_3ssh(Func, R, A1, A2, A3)
%template(sure_ ## Func) my_sure_ ## Func<R, A1, A2, A3 >;
%template(sequential_ ## Func) my_sequential_ ## Func<R, A1, A2, A3 >;
%template(hybrid_ ## Func) my_hybrid_ ## Func<R, A1, A2, A3 >;
%enddef

/************ Morpho algorithms taking two arguments and located in three namespaces  ****************/

%define decl_morpho_2ssh(Incl, Func)
%{
#include Incl
%}

%inline %{
template<typename R, typename A1, typename A2>
static R my_sure_ ## Func( A1& a1,  A2& a2)
  { return oln::morpho::sure::Func(a1, a2); }
template<typename R, typename A1, typename A2>
static R my_sequential_ ## Func( A1& a1,  A2& a2)
  { return oln::morpho::sequential::Func(a1, a2); }
template<typename R, typename A1, typename A2>
static R my_hybrid_ ## Func( A1& a1,  A2& a2)
  { return oln::morpho::hybrid::Func(a1, a2); }
%}
%enddef
%define make_morpho_2ssh(Func, R, A1, A2)
%template(sure_ ## Func) my_sure_ ## Func<R, A1, A2>;
%template(sequential_ ## Func) my_sequential_ ## Func<R, A1, A2>;
%template(hybrid_ ## Func) my_hybrid_ ## Func<R, A1, A2>;
%enddef

/************ Morpho algorithms taking three arguments and located in two namespaces  ****************/

%define decl_morpho_3ss(Incl, Func)
%{
#include Incl
%}
%inline %{
template<typename R, typename A1, typename A2, typename A3>
static R my_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::Func(a1, a2, a3); }
template<typename R, typename A1, typename A2, typename A3>
static R my_sure_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::sure::Func(a1, a2, a3); }
%}
%enddef
%define make_morpho_3ss(Func, R, A1, A2, A3)
%template(Func) my_ ## Func<R, A1, A2, A3 >;
%template(sure_ ## Func) my_sure_ ## Func<R, A1, A2, A3 >;
%enddef

/*************** Watershed *****************/

%define decl_morpho_watershed(Func)
%{
#include <oln/morpho/watershed.hh>
%}
%inline %{
template<typename Ret, typename Dt, typename I, typename N>
static Ret my_ ## Func( I& img,  N& neigh)
{ return oln::morpho::Func<Dt >(img, neigh); }
%}
%enddef
%define make_morpho_watershed(Func, R, Dt, Img, Neigh)
%template(Func ## _ ## Dt) my_ ## Func<R, Dt, Img, Neigh >;
%enddef

/***************** Function families **********************/
%define decl_classical_family()
     decl_morpho_2(<oln/morpho/erosion.hh>, erosion, 1)
     decl_morpho_2(<oln/morpho/dilation.hh>, dilation, 1)
     decl_morpho_2(<oln/morpho/opening.hh>, opening, 1)
     decl_morpho_2(<oln/morpho/closing.hh>, closing, 1)

     decl_morpho_3(<oln/morpho/thinning.hh>, thinning, 1)
     decl_morpho_3(<oln/morpho/thickening.hh>, thickening, 1)

     decl_morpho_3ss(<oln/morpho/geodesic_erosion.hh>, geodesic_erosion)
     decl_morpho_3ss(<oln/morpho/geodesic_dilation.hh>, geodesic_dilation)

     decl_morpho_3ssh(<oln/morpho/reconstruction.hh>, geodesic_reconstruction_dilation)
     decl_morpho_3ssh(<oln/morpho/reconstruction.hh>, geodesic_reconstruction_erosion)

     decl_morpho_3ssh(<oln/morpho/extrema.hh>, minima_imposition)
     decl_morpho_2ssh(<oln/morpho/extrema.hh>, regional_minima)

     /* FIXME: functors are unsupported */
/*      decl_morpho_3(<oln/morpho/gradient.hh>, beucher_gradient, 1) */
/*      decl_morpho_3(<oln/morpho/gradient.hh>, internal_gradient, 1) */
/*      decl_morpho_3(<oln/morpho/gradient.hh>, external_gradient, 1) */
     decl_morpho_2(<oln/morpho/gradient.hh>, beucher_gradient, 1)
     decl_morpho_2(<oln/morpho/gradient.hh>, internal_gradient, 1)
     decl_morpho_2(<oln/morpho/gradient.hh>, external_gradient, 1)

     /* FIXME: functors are unsupported */
/*      decl_morpho_3(<oln/morpho/laplacian.hh>, laplacian, 1) */
     decl_morpho_2(<oln/morpho/laplacian.hh>, laplacian, 1)

     /* FIXME: functors are unsupported */
/*      decl_morpho_3(<oln/morpho/top_hat.hh>, white_top_hat, 1) */
/*      decl_morpho_3(<oln/morpho/top_hat.hh>, black_top_hat, 1) */
/*      decl_morpho_3(<oln/morpho/top_hat.hh>, self_complementary_top_hat, 1) */
/*      decl_morpho_3(<oln/morpho/top_hat.hh>, top_hat_contrast_op, 1) */
     decl_morpho_2(<oln/morpho/top_hat.hh>, white_top_hat, 1)
     decl_morpho_2(<oln/morpho/top_hat.hh>, black_top_hat, 1)
     decl_morpho_2(<oln/morpho/top_hat.hh>, self_complementary_top_hat, 1)
     decl_morpho_2(<oln/morpho/top_hat.hh>, top_hat_contrast_op, 1)

     decl_morpho_3(<oln/morpho/hit_or_miss.hh>, hit_or_miss, 1)

     /* FIXME: functors are unsupported */
/*      decl_morpho_4(<oln/morpho/hit_or_miss.hh>, hit_or_miss, 1) */

     decl_morpho_3(<oln/morpho/hit_or_miss.hh>, hit_or_miss_opening, 1)
     decl_morpho_3(<oln/morpho/hit_or_miss.hh>, hit_or_miss_opening_bg, 1)
     decl_morpho_3(<oln/morpho/hit_or_miss.hh>, hit_or_miss_closing, 1)
     decl_morpho_3(<oln/morpho/hit_or_miss.hh>, hit_or_miss_closing_bg, 1)   
%enddef

%define make_classical_family(Img, M, W, N)
     make_morpho_2(erosion, Img, Img, W, 1)
     make_morpho_2(dilation, Img, Img, W, 1)
     
     make_morpho_3(thickening, Img, Img, W, W, 1)
     make_morpho_3(thinning, Img, Img, W, W, 1)

     make_morpho_3ss(geodesic_erosion, Img, Img, Img, N)
     make_morpho_3ss(geodesic_dilation, Img, Img, Img, N)
     make_morpho_3ssh(geodesic_reconstruction_dilation, Img, Img, Img, N)
     make_morpho_3ssh(geodesic_reconstruction_erosion, Img, Img, Img, N)

     make_morpho_3ssh(minima_imposition, Img, Img, M, N)
     make_morpho_2ssh(regional_minima, M, Img, N)


     make_morpho_2(beucher_gradient, Img, Img, W, 1)
     make_morpho_2(internal_gradient, Img, Img, W, 1)
     make_morpho_2(external_gradient, Img, Img, W, 1)

     // FIXME: this is broken for the moment (why ?)
/*      make_morpho_2(laplacian, Img, Img, W, 1) */

     make_morpho_2(white_top_hat, Img, Img, W, 1)
     make_morpho_2(black_top_hat, Img, Img, W, 1)
     make_morpho_2(self_complementary_top_hat, Img, Img, W, 1)
     make_morpho_2(top_hat_contrast_op, Img, Img, W, 1)

     make_morpho_3(hit_or_miss, Img, Img, W, W, 1)
     make_morpho_3(hit_or_miss_opening, Img, Img, W, W, 1)
     make_morpho_3(hit_or_miss_opening_bg, Img, Img, W, W, 1)
     make_morpho_3(hit_or_miss_closing, Img, Img, W, W, 1)
     make_morpho_3(hit_or_miss_closing_bg, Img, Img, W, W, 1)   
     
%enddef

%define decl_extrema_killer_family()
     decl_morpho_3(<oln/morpho/extrema_killer.hh>, sure_maxima_killer, 0)
     decl_morpho_3(<oln/morpho/extrema_killer.hh>, fast_maxima_killer, 0)
     decl_morpho_3(<oln/morpho/extrema_killer.hh>, sure_minima_killer, 0)
     decl_morpho_3(<oln/morpho/extrema_killer.hh>, fast_minima_killer, 0)
%enddef
%define make_extrema_killer_family(Img, N)
     make_morpho_3(sure_maxima_killer, Img, Img, unsigned, N, 0)
     make_morpho_3(fast_maxima_killer, Img, Img, unsigned, N, 0)
     make_morpho_3(sure_minima_killer, Img, Img, unsigned, N, 0)
     make_morpho_3(fast_minima_killer, Img, Img, unsigned, N, 0)
%enddef

%define decl_watershed_family()
     decl_morpho_watershed(watershed_seg)
     decl_morpho_watershed(watershed_con)
     decl_morpho_3(<oln/morpho/watershed.hh>, watershed_seg_or, 0)
%enddef
%define make_watershed_family(Dt, Img1, Img2, N)
     make_morpho_watershed(watershed_seg, Img2, Dt, Img1, N)
     make_morpho_watershed(watershed_con, Img2, Dt, Img1, N)
     make_morpho_3(watershed_seg_or, Img2, Img1, Img2, N, 0)
%enddef
