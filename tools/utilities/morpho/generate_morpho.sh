#!/bin/sh
# generate_toolbox.sh
#
# $Id: generate_morpho.sh 1.3.1.3 Sun, 05 Jan 2003 02:31:13 +0100 raph $

if [ "x$1" = "x" ]; then
   echo "usage: $0 <source directory>" >&2
   exit 1
fi


# Global variables
DATA_TYPE_LIST="int_u8"
DIM_TYPE_LIST="2d"
FCT_LIST="erosion       dilation       opening       closing \
          fast::erosion fast::dilation fast::opening fast::closing \
          fast::beucher_gradient  \
          fast::internal_gradient \
          fast::external_gradient \
          white_top_hat       black_top_hat       self_complementary_top_hat \
          fast::white_top_hat fast::black_top_hat fast::self_complementary_top_hat"

FCT_LIST_TWOWIN="hit_or_miss fast::hit_or_miss \
                 thinning    fast::thinning \
                 thickening  fast::thickening"

FCT_LIST_WATERSHED="watershed_seg watershed_con"

FCT_LIST_GAUSSIAN="fast::gaussian \
                   fast::gaussian_derivative \
                   fast::gaussian_second_derivative"

FCT_LIST_NEIGHB="sure::regional_minima \
                 sequential::regional_minima \
                 hybrid::regional_minima"

FCT_LIST_EXTREMA="sure::minima_imposition \
                  sequential::minima_imposition \
                  hybrid::minima_imposition"

FILE="$1/morpho-rules.make"

lf() {
   echo "\\"
   echo -n "  $1 "
}

echo -n "bin_PROGRAMS = " >"$FILE"
for dim in $DIM_TYPE_LIST; do
  for data in $DATA_TYPE_LIST; do

    for fct in $FCT_LIST; do
      lf "${fct/::/_}"_"$dim"_"$data" 
    done

    lf "laplacian"_"$dim"_"$data" 
    lf "fast_laplacian"_"$dim"_"$data" 
    lf "top_hat_contrast_op"_"$dim"_"$data" 
    lf "fast_top_hat_contrast_op"_"$dim"_"$data" 

    for fct in $FCT_LIST_TWOWIN; do
      lf "${fct/::/_}"_"$dim"_"$data"
    done

    for fct in $FCT_LIST_WATERSHED; do
      lf "${fct/::/_}"_"$dim"_"$data" 
    done

    for fct in $FCT_LIST_GAUSSIAN; do
      lf "${fct/::/_}"_"$dim"_"$data" 
    done

    for fct in $FCT_LIST_NEIGHB; do
      lf "${fct/::/_}"_"$dim"_"$data" 
    done

    for fct in $FCT_LIST_EXTREMA; do
      lf "${fct/::/_}"_"$dim"_"$data" 
    done
 
  done
done >> "$FILE"
 
echo >> $FILE;
echo >> $FILE;

for dim in $DIM_TYPE_LIST; do
  for data in $DATA_TYPE_LIST; do
    for fct in $FCT_LIST; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template.cc" 
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='$fct'" 
      echo 
    done

    echo "laplacian"_"$dim"_"$data"_"SOURCES = morpho_template_with_functor.cc" 
    echo "laplacian"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='laplacian' -DFUNCTOR='convert::force<$data>()'" 
    echo 
    echo "fast_laplacian"_"$dim"_"$data"_"SOURCES = morpho_template_with_functor.cc" 
    echo "fast_laplacian"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='fast::laplacian' -DFUNCTOR='convert::force<$data>()'" 
    echo 

    echo "top_hat_contrast_op"_"$dim"_"$data"_"SOURCES = morpho_template_with_functor.cc" 
    echo "top_hat_contrast_op"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='top_hat_contrast_op' -DFUNCTOR='convert::bound<$data>()'" 
    echo 
    echo "fast_top_hat_contrast_op"_"$dim"_"$data"_"SOURCES = morpho_template_with_functor.cc" 
    echo "fast_top_hat_contrast_op"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='fast::top_hat_contrast_op' -DFUNCTOR='convert::bound<$data>()'" 
    echo 

    for fct in $FCT_LIST_TWOWIN; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_with_twowin.cc" 
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DWINDOW_TYPE=window$dim -DFUNC='$fct'" 
      echo 
    done

    for fct in $FCT_LIST_WATERSHED; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_watershed.cc"
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DNEIGHBORHOOD_TYPE=neighborhood$dim -DFUNC='$fct'"
      echo 
    done

    for fct in $FCT_LIST_GAUSSIAN; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_gaussian.cc" 
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DFUNC='$fct'" 
      echo 
    done

    for fct in $FCT_LIST_NEIGHB; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_with_neighb.cc" 
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DNEIGHBORHOOD_TYPE=neighborhood$dim -DFUNC='$fct'" 
      echo 
    done

    for fct in $FCT_LIST_EXTREMA; do
      echo "${fct/::/_}"_"$dim"_"$data"_"SOURCES = morpho_template_extrema.cc"
      echo "${fct/::/_}"_"$dim"_"$data"_"CXXFLAGS = \$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DNEIGHBORHOOD_TYPE=neighborhood$dim -DFUNC='$fct'" 
      echo 
    done

  done
done >> "$FILE"
