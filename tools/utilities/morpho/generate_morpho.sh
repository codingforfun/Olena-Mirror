#!/bin/sh
# generate_morpho.sh
#
# Copyright (C) 2001, 2002, 2003 EPITA Research and Development Laboratory

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

flatten() {
    name=`echo "$@" | sed -e 's,::,_,g'`
    echo "${name}_$suffix"
}

echo -n "bin_PROGRAMS = " >"$FILE"
for dim in $DIM_TYPE_LIST; do
  for data in $DATA_TYPE_LIST; do

    suffix=${dim}_$data

    for fct in $FCT_LIST; do
      lf `flatten "$fct"`
    done

    lf laplacian_$suffix
    lf fast_laplacian_$suffix
    lf top_hat_contrast_op_$suffix 
    lf fast_top_hat_contrast_op_$suffix 

    for fct in $FCT_LIST_TWOWIN; do lf `flatten "$fct"`; done
    for fct in $FCT_LIST_WATERSHED; do lf `flatten "$fct"`; done
    for fct in $FCT_LIST_GAUSSIAN; do lf `flatten "$fct"`; done
    for fct in $FCT_LIST_NEIGHB; do  lf `flatten "$fct"`; done
    for fct in $FCT_LIST_EXTREMA; do  lf `flatten "$fct"`; done
  done
done >> "$FILE"
echo >> "$FILE"

echo -n "dist_man_MANS = " >>"$FILE"
for dim in $DIM_TYPE_LIST; do
  for data in $DATA_TYPE_LIST; do

    suffix=${dim}_$data.1

    for fct in $FCT_LIST; do
      lf `flatten "$fct"`
    done

    lf laplacian_$suffix
    lf fast_laplacian_$suffix
    lf top_hat_contrast_op_$suffix 
    lf fast_top_hat_contrast_op_$suffix 

    for fct in $FCT_LIST_TWOWIN; do lf `flatten "$fct"`; done
    for fct in $FCT_LIST_WATERSHED; do lf `flatten "$fct"`; done
    for fct in $FCT_LIST_GAUSSIAN; do lf `flatten "$fct"`; done
    for fct in $FCT_LIST_NEIGHB; do  lf `flatten "$fct"`; done
    for fct in $FCT_LIST_EXTREMA; do  lf `flatten "$fct"`; done
  done
done >> "$FILE"
 
echo >> $FILE;
echo >> $FILE;

print_man()
{
  echo "if HAVE_HELP2MAN"
  echo "${prog}.1: ${prog}\$(EXEEXT)"
  printf '\t'; echo "PATH=.:\$\$PATH \$(HELP2MAN) -N --output=\$@ ${prog}\$(EXEEXT)"
  echo "endif"
}
get_flags()
{
  common_flags="\$(AM_CXXFLAGS) -DDATA_TYPE=$data -DIMAGE_TYPE=image$dim -DFUNC='$1'"
}
common()
{
  prog=`flatten "$1"`
  get_flags "$1"
  echo "${prog}_SOURCES = $2"
  echo "${prog}_CXXFLAGS = $common_flags $3"
  print_man 
  echo
}

for dim in $DIM_TYPE_LIST; do
  for data in $DATA_TYPE_LIST; do
    suffix=${dim}_$data

    for fct in $FCT_LIST; do
      common "$fct" morpho_template.cc "-DWINDOW_TYPE=window$dim"
    done

    for fields in \
        "laplacian/convert::force<$data>" \
        "fast::laplacian/convert::force<$data>" \
        "top_hat_contrast_op/convert::bound<$data>" \
	"fast::top_hat_contrast_op/convert::bound<$data>"; do
	func=`echo "$fields" | cut -d/ -f1`
	functor=`echo "$fields" | cut -d/ -f2`
        common "$func" morpho_template_with_functor.cc "-DWINDOW_TYPE=window$dim -DFUNCTOR='$functor()'"
    done
 
    for fct in $FCT_LIST_TWOWIN; do
      common "$fct" morpho_template_with_twowin.cc "-DWINDOW_TYPE=window$dim" 
    done

    for fct in $FCT_LIST_WATERSHED; do
      common "$fct" morpho_template_watershed.cc "-DNEIGHBORHOOD_TYPE=neighborhood$dim"
    done

    for fct in $FCT_LIST_GAUSSIAN; do
      common "$fct" morpho_template_gaussian.cc 
    done

    for fct in $FCT_LIST_NEIGHB; do
      common "$fct" morpho_template_with_neighb.cc "-DNEIGHBORHOOD_TYPE=neighborhood$dim" 
    done

    for fct in $FCT_LIST_EXTREMA; do
      common "$fct" morpho_template_extrema.cc "-DNEIGHBORHOOD_TYPE=neighborhood$dim" 
    done

  done
done >> "$FILE"
