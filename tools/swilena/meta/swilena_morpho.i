// -*- C++ -*-

%module swilena_morpho

/*
  Algorithms are supported by a simple two-level mechanism:

  - Declare a my_algorithm template function accepting all type of
    parameters. This is the goal of the decl_morpho_xxx macros.
    ex:
    template <class Ret, class A1, A2>
    Ret my_algorithm(A1& a1, A2& a2);

  - Instantiate it for every possible type. This is the goal of the
    generate_morpho_instantiations.py script.
*/

/********** Morpho algorithms taking two arguments ***********/

%define decl_morpho_2(Incl, Func, HAS_FAST)

%{
#include Incl
%}

%inline %{
template<typename Ret, typename A1, typename A2>
static Ret my_ ## Func(A1& a1,  A2& a2)
{ return oln::morpho::Func(a1, a2); }
%}

#if HAS_FAST
%inline %{

template<typename Ret, typename A1, typename A2 >
static Ret my_fast_ ## Func(A1& a1,  A2& a2)
{
  return oln::morpho::fast::Func(a1, a2);
}

%}
#endif
%enddef

/************ Morpho algorithms taking three arguments ************/

%define decl_morpho_3(Incl, Func, HAS_FAST)
%{
#include Incl
%}

%inline %{
template<typename Ret, typename A1, typename A2, typename A3>
static Ret my_ ## Func(A1& a1,  A2& a2,  A3& a3)
{ return oln::morpho::Func(a1, a2, a3); }
%}
#if HAS_FAST
%inline %{
template<typename Ret, typename A1, typename A2, typename A3>
static Ret my_fast_ ## Func(A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::fast::Func(a1, a2, a3); }
%}
#endif

%enddef

/************ Morpho algorithms taking four arguments ************/

%define decl_morpho_4(Incl, Func, HAS_FAST)
%{
#include Incl
%}

%inline %{
template<typename Ret, typename A1, typename A2, typename A3, typename A4>
static Ret my_ ## Func( A1& a1,  A2& a2,  A3& a3,  A4& a4)
{ return oln::morpho::Func(a1, a2, a3, a4); }
%}
#if HAS_FAST
%inline %{
template<typename Ret, typename A1, typename A2, typename A3, typename A4>
static Ret my_fast_ ## Func( A1& a1,  A2& a2,  A3& a3,  A4& a4)
  { return oln::morpho::fast::Func(a1, a2, a3, a4); }
%}
#endif

%enddef

/************ Morpho algorithms taking three arguments **************/
/************ and located in sure, sequential and hybrid ************/

%define decl_morpho_3ssh(Incl, Func)
%{
#include Incl
%}

%inline %{
template<typename Ret, typename A1, typename A2, typename A3>
static Ret my_sure_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::sure::Func(a1, a2, a3); }
template<typename Ret, typename A1, typename A2, typename A3>
static Ret my_sequential_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::sequential::Func(a1, a2, a3); }
template<typename Ret, typename A1, typename A2, typename A3>
static Ret my_hybrid_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::hybrid::Func(a1, a2, a3); }
%}
%enddef

/************ Morpho algorithms taking two arguments **************/
/************ and located in sure, sequential and hybrid **********/

%define decl_morpho_2ssh(Incl, Func)
%{
#include Incl
%}

%inline %{
template<typename Ret, typename A1, typename A2>
static Ret my_sure_ ## Func( A1& a1,  A2& a2)
  { return oln::morpho::sure::Func(a1, a2); }
template<typename Ret, typename A1, typename A2>
static Ret my_sequential_ ## Func( A1& a1,  A2& a2)
  { return oln::morpho::sequential::Func(a1, a2); }
template<typename Ret, typename A1, typename A2>
static Ret my_hybrid_ ## Func( A1& a1,  A2& a2)
  { return oln::morpho::hybrid::Func(a1, a2); }
%}
%enddef

/************ Morpho algorithms taking three arguments ***********/
/************ and located in morpho and morpho::sure   ***********/

%define decl_morpho_3ss(Incl, Func)
%{
#include Incl
%}
%inline %{
template<typename Ret, typename A1, typename A2, typename A3>
static Ret my_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::Func(a1, a2, a3); }
template<typename Ret, typename A1, typename A2, typename A3>
static Ret my_sure_ ## Func( A1& a1,  A2& a2,  A3& a3)
  { return oln::morpho::sure::Func(a1, a2, a3); }
%}
%enddef

/*************** Watershed *****************/

%define decl_morpho_watershed(Func)
%{
#include <oln/morpho/watershed.hh>
%}
%inline %{
template<typename Ret, typename I, typename N>
static Ret my_ ## Func( I& img,  N& neigh)
{ return oln::morpho::Func<oln_value_type(Ret)>(img, neigh); }
%}
%enddef

/************ Extrema killers ***********/

%define decl_morpho_extrema_killer(Incl, Func)
%{
#include Incl
%}

%inline %{
template<typename Img, typename Neighb>
static Img my_ ## Func(const Img& a1, unsigned area, const Neighb& a3)
{ return oln::morpho::Func(a1, area, a3); }
%}
%enddef

/***************** Function families **********************/

/*-----------------.
| Classical family |
`-----------------*/

/*
  Declare generic functions for classical algorithms
*/

%define decl_classical_family
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
  /* decl_morpho_3(<oln/morpho/gradient.hh>, beucher_gradient, 1) */
  /* decl_morpho_3(<oln/morpho/gradient.hh>, internal_gradient, 1) */
  /* decl_morpho_3(<oln/morpho/gradient.hh>, external_gradient, 1) */
  decl_morpho_2(<oln/morpho/gradient.hh>, beucher_gradient, 1)
  decl_morpho_2(<oln/morpho/gradient.hh>, internal_gradient, 1)
  decl_morpho_2(<oln/morpho/gradient.hh>, external_gradient, 1)

  /* FIXME: functors are unsupported */
  /*      decl_morpho_3(<oln/morpho/laplacian.hh>, laplacian, 1) */
  decl_morpho_2(<oln/morpho/laplacian.hh>, laplacian, 1)

  /* FIXME: functors are unsupported */
  /* decl_morpho_3(<oln/morpho/top_hat.hh>, white_top_hat, 1) */
  /* decl_morpho_3(<oln/morpho/top_hat.hh>, black_top_hat, 1) */
  /* decl_morpho_3(<oln/morpho/top_hat.hh>, self_complementary_top_hat, 1) */
  /* decl_morpho_3(<oln/morpho/top_hat.hh>, top_hat_contrast_op, 1) */
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

/*----------------------.
| Extrema killer family |
`----------------------*/

%define decl_extrema_killer_family
  decl_morpho_extrema_killer(<oln/morpho/extrema_killer.hh>,
			     sure_maxima_killer)
  decl_morpho_extrema_killer(<oln/morpho/extrema_killer.hh>,
			     fast_maxima_killer)
  decl_morpho_extrema_killer(<oln/morpho/extrema_killer.hh>,
			     sure_minima_killer)
  decl_morpho_extrema_killer(<oln/morpho/extrema_killer.hh>,
			     fast_minima_killer)
%enddef

/*-----------------.
| Watershed family |
`-----------------*/

%define decl_watershed_family
     decl_morpho_watershed(watershed_seg)
     decl_morpho_watershed(watershed_con)
     decl_morpho_3(<oln/morpho/watershed.hh>, watershed_seg_or, 0)
%enddef

decl_classical_family
decl_watershed_family
decl_extrema_killer_family
