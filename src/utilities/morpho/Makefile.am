
AM_CPPFLAGS = -I$(OLN_INCLUDEDIR)
AM_CXXFLAGS = -Wall -W -ftemplate-depth-50 

bin_PROGRAMS = \
  erosion_2d_int_u8  dilation_2d_int_u8  opening_2d_int_u8  closing_2d_int_u8  fast_erosion_2d_int_u8  fast_dilation_2d_int_u8  fast_opening_2d_int_u8  fast_closing_2d_int_u8  fast_beucher_gradient_2d_int_u8  fast_internal_gradient_2d_int_u8  fast_external_gradient_2d_int_u8  white_top_hat_2d_int_u8  black_top_hat_2d_int_u8  self_complementary_top_hat_2d_int_u8  fast_white_top_hat_2d_int_u8  fast_black_top_hat_2d_int_u8  fast_self_complementary_top_hat_2d_int_u8  laplacian_2d_int_u8  fast_laplacian_2d_int_u8  top_hat_contrast_op_2d_int_u8  fast_top_hat_contrast_op_2d_int_u8  hit_or_miss_2d_int_u8  fast_hit_or_miss_2d_int_u8  thinning_2d_int_u8  fast_thinning_2d_int_u8  thickening_2d_int_u8  fast_thickening_2d_int_u8  watershed_seg_2d_int_u8  watershed_con_2d_int_u8  fast_gaussian_2d_int_u8  fast_gaussian_derivative_2d_int_u8  fast_gaussian_second_derivative_2d_int_u8  sure_regional_minima_2d_int_u8  sequential_regional_minima_2d_int_u8  hybrid_regional_minima_2d_int_u8  sure_minima_imposition_2d_int_u8  sequential_minima_imposition_2d_int_u8  hybrid_minima_imposition_2d_int_u8

erosion_2d_int_u8_SOURCES = morpho_template.cc
erosion_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='erosion'

dilation_2d_int_u8_SOURCES = morpho_template.cc
dilation_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='dilation'

opening_2d_int_u8_SOURCES = morpho_template.cc
opening_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='opening'

closing_2d_int_u8_SOURCES = morpho_template.cc
closing_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='closing'

fast_erosion_2d_int_u8_SOURCES = morpho_template.cc
fast_erosion_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::erosion'

fast_dilation_2d_int_u8_SOURCES = morpho_template.cc
fast_dilation_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::dilation'

fast_opening_2d_int_u8_SOURCES = morpho_template.cc
fast_opening_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::opening'

fast_closing_2d_int_u8_SOURCES = morpho_template.cc
fast_closing_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::closing'

fast_beucher_gradient_2d_int_u8_SOURCES = morpho_template.cc
fast_beucher_gradient_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::beucher_gradient'

fast_internal_gradient_2d_int_u8_SOURCES = morpho_template.cc
fast_internal_gradient_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::internal_gradient'

fast_external_gradient_2d_int_u8_SOURCES = morpho_template.cc
fast_external_gradient_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::external_gradient'

white_top_hat_2d_int_u8_SOURCES = morpho_template.cc
white_top_hat_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='white_top_hat'

black_top_hat_2d_int_u8_SOURCES = morpho_template.cc
black_top_hat_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='black_top_hat'

self_complementary_top_hat_2d_int_u8_SOURCES = morpho_template.cc
self_complementary_top_hat_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='self_complementary_top_hat'

fast_white_top_hat_2d_int_u8_SOURCES = morpho_template.cc
fast_white_top_hat_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::white_top_hat'

fast_black_top_hat_2d_int_u8_SOURCES = morpho_template.cc
fast_black_top_hat_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::black_top_hat'

fast_self_complementary_top_hat_2d_int_u8_SOURCES = morpho_template.cc
fast_self_complementary_top_hat_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::self_complementary_top_hat'

laplacian_2d_int_u8_SOURCES = morpho_template_with_functor.cc
laplacian_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='laplacian' -DFUNCTOR='convert::force<int_u8>()'

fast_laplacian_2d_int_u8_SOURCES = morpho_template_with_functor.cc
fast_laplacian_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::laplacian' -DFUNCTOR='convert::force<int_u8>()'

top_hat_contrast_op_2d_int_u8_SOURCES = morpho_template_with_functor.cc
top_hat_contrast_op_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='top_hat_contrast_op' -DFUNCTOR='convert::bound<int_u8>()'

fast_top_hat_contrast_op_2d_int_u8_SOURCES = morpho_template_with_functor.cc
fast_top_hat_contrast_op_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::top_hat_contrast_op' -DFUNCTOR='convert::bound<int_u8>()'

hit_or_miss_2d_int_u8_SOURCES = morpho_template_with_twowin.cc
hit_or_miss_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='hit_or_miss'

fast_hit_or_miss_2d_int_u8_SOURCES = morpho_template_with_twowin.cc
fast_hit_or_miss_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::hit_or_miss'

thinning_2d_int_u8_SOURCES = morpho_template_with_twowin.cc
thinning_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='thinning'

fast_thinning_2d_int_u8_SOURCES = morpho_template_with_twowin.cc
fast_thinning_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::thinning'

thickening_2d_int_u8_SOURCES = morpho_template_with_twowin.cc
thickening_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='thickening'

fast_thickening_2d_int_u8_SOURCES = morpho_template_with_twowin.cc
fast_thickening_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DWINDOW_TYPE=window2d -DFUNC='fast::thickening'

watershed_seg_2d_int_u8_SOURCES = morpho_template_watershed.cc
watershed_seg_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DNEIGHBORHOOD_TYPE=neighborhood2d -DFUNC='watershed_seg'

watershed_con_2d_int_u8_SOURCES = morpho_template_watershed.cc
watershed_con_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DNEIGHBORHOOD_TYPE=neighborhood2d -DFUNC='watershed_con'

fast_gaussian_2d_int_u8_SOURCES = morpho_template_gaussian.cc
fast_gaussian_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DFUNC='fast::gaussian'

fast_gaussian_derivative_2d_int_u8_SOURCES = morpho_template_gaussian.cc
fast_gaussian_derivative_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DFUNC='fast::gaussian_derivative'

fast_gaussian_second_derivative_2d_int_u8_SOURCES = morpho_template_gaussian.cc
fast_gaussian_second_derivative_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DFUNC='fast::gaussian_second_derivative'

sure_regional_minima_2d_int_u8_SOURCES = morpho_template_with_neighb.cc
sure_regional_minima_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DNEIGHBORHOOD_TYPE=neighborhood2d -DFUNC='sure::regional_minima'

sequential_regional_minima_2d_int_u8_SOURCES = morpho_template_with_neighb.cc
sequential_regional_minima_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DNEIGHBORHOOD_TYPE=neighborhood2d -DFUNC='sequential::regional_minima'

hybrid_regional_minima_2d_int_u8_SOURCES = morpho_template_with_neighb.cc
hybrid_regional_minima_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DNEIGHBORHOOD_TYPE=neighborhood2d -DFUNC='hybrid::regional_minima'

sure_minima_imposition_2d_int_u8_SOURCES = morpho_template_extrema.cc
sure_minima_imposition_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DNEIGHBORHOOD_TYPE=neighborhood2d -DFUNC='sure::minima_imposition'

sequential_minima_imposition_2d_int_u8_SOURCES = morpho_template_extrema.cc
sequential_minima_imposition_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DNEIGHBORHOOD_TYPE=neighborhood2d -DFUNC='sequential::minima_imposition'

hybrid_minima_imposition_2d_int_u8_SOURCES = morpho_template_extrema.cc
hybrid_minima_imposition_2d_int_u8_CXXFLAGS = $(AM_CXXFLAGS) -DDATA_TYPE=int_u8 -DIMAGE_TYPE=image2d -DNEIGHBORHOOD_TYPE=neighborhood2d -DFUNC='hybrid::minima_imposition'

