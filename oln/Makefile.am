## Process this file with Automake to produce Makefile.in -*- Makefile -*-

# OLN_SRC_SUBDIRS is set when "configure" is run.
SUBDIRS = img $(OLN_SRC_SUBDIRS)

###
### Olena headers
###

nobase_include_HEADERS =			\
  oln/arith/internal/opdecls.hh			\
  oln/arith/logic.hh				\
  oln/arith/ops.hh				\
  oln/basics.hh					\
  oln/basics1d.hh				\
  oln/basics2d.hh				\
  oln/basics3d.hh				\
  oln/config/math.hh				\
  oln/config/system.hh				\
  oln/config/pconf.hh				\
  oln/convert/basics.hh				\
  oln/convert/bound.hh				\
  oln/convert/colorconv.hh			\
  oln/convert/conversion.hh			\
  oln/convert/conversion_ng_se.hh		\
  oln/convert/force.hh				\
  oln/convert/nrgbxyz.hh			\
  oln/convert/nrgbyiq.hh			\
  oln/convert/nrgbyuv.hh			\
  oln/convert/nrgbhsi.hh			\
  oln/convert/nrgbhsl.hh			\
  oln/convert/nrgbhsv.hh			\
  oln/convert/rgbnrgb.hh			\
  oln/convert/rgbxyz.hh				\
  oln/convert/stretch.hh			\
  oln/convol/convolution.hh			\
  oln/convol/fast_gaussian.hh			\
  oln/convol/fast_gaussian.hxx			\
  oln/convol/fast_gaussian_coefficient.hh	\
  oln/core/accum.hh				\
  oln/core/apply.hh				\
  oln/core/box.hh				\
  oln/core/box.hxx				\
  oln/core/bkd_iter1d.hh			\
  oln/core/bkd_iter2d.hh			\
  oln/core/bkd_iter3d.hh			\
  oln/core/border.hh				\
  oln/core/border1d.hh				\
  oln/core/border2d.hh				\
  oln/core/border3d.hh				\
  oln/core/compose.hh				\
  oln/core/contract.hh				\
  oln/core/coord.hh				\
  oln/core/dpoint.hh				\
  oln/core/dpoint1d.hh				\
  oln/core/dpoint1d.hxx				\
  oln/core/dpoint2d.hh				\
  oln/core/dpoint2d.hxx				\
  oln/core/dpoint3d.hh				\
  oln/core/dpoint3d.hxx				\
  oln/core/dpointnd.hh				\
  oln/core/fold.hh				\
  oln/core/fwd_iter1d.hh			\
  oln/core/fwd_iter2d.hh			\
  oln/core/fwd_iter3d.hh			\
  oln/core/generate.hh				\
  oln/core/image.hh				\
  oln/core/image1d.hh				\
  oln/core/image1d_size.hh			\
  oln/core/image2d.hh				\
  oln/core/image2d_size.hh			\
  oln/core/image3d.hh				\
  oln/core/image3d_size.hh			\
  oln/core/image_size.hh			\
  oln/core/imagend_size.hh			\
  oln/core/internal/fake.hh			\
  oln/core/internal/image1d.hh			\
  oln/core/internal/image1d_data.hh		\
  oln/core/internal/image2d.hh			\
  oln/core/internal/image2d_data.hh		\
  oln/core/internal/image3d.hh			\
  oln/core/internal/image3d_data.hh		\
  oln/core/internal/iter1d.hh			\
  oln/core/internal/iter2d.hh			\
  oln/core/internal/iter3d.hh			\
  oln/core/internal/neighborhood.hh		\
  oln/core/internal/objs.hxx			\
  oln/core/internal/real_image1d.hh		\
  oln/core/internal/real_image2d.hh		\
  oln/core/internal/real_image3d.hh		\
  oln/core/internal/rec_operable.hh		\
  oln/core/internal/w_window.hh			\
  oln/core/internal/window.hh			\
  oln/core/iter.hh				\
  oln/core/macros.hh				\
  oln/core/neighborhood.hh			\
  oln/core/neighborhood1d.hh			\
  oln/core/neighborhood2d.hh			\
  oln/core/neighborhood3d.hh			\
  oln/core/objs.hh				\
  oln/core/point.hh				\
  oln/core/point1d.hh				\
  oln/core/point1d.hxx				\
  oln/core/point2d.hh				\
  oln/core/point2d.hxx				\
  oln/core/point3d.hh				\
  oln/core/point3d.hxx				\
  oln/core/pointnd.hh				\
  oln/core/pred_image.hh			\
  oln/core/regular_image.hh			\
  oln/core/structelt.hh				\
  oln/core/traverse.hh				\
  oln/core/type.hh				\
  oln/core/typeadj.hh				\
  oln/core/w_window.hh				\
  oln/core/w_window1d.hh			\
  oln/core/w_window2d.hh			\
  oln/core/w_window3d.hh			\
  oln/core/window.hh				\
  oln/core/window1d.hh				\
  oln/core/window2d.hh				\
  oln/core/window3d.hh				\
  oln/core/winiter.hh				\
  oln/core/winneighb.hh				\
  oln/io/base.hh				\
  oln/io/base.hxx				\
  oln/io/basics.hh				\
  oln/io/file.hh				\
  oln/io/gz.hh					\
  oln/io/gz_stream.hh				\
  oln/io/pnm.hh					\
  oln/io/pnm3d.hh				\
  oln/io/pnm_read.hxx				\
  oln/io/pnm_read3d.hxx				\
  oln/io/pnm_read_common.hxx			\
  oln/io/pnm_write.hxx				\
  oln/io/pnm_write3d.hxx			\
  oln/io/pnm_write_common.hxx			\
  oln/io/readable.hh				\
  oln/io/se.hh					\
  oln/io/se_neighborhood.hxx			\
  oln/io/se_window.hxx				\
  oln/level/cc.hh				\
  oln/level/compare.hh				\
  oln/level/connected.hh			\
  oln/level/fill.hh				\
  oln/level/invert.hh				\
  oln/level/lut.hh				\
  oln/level/set_level.hh			\
  oln/level/threshold.hh			\
  oln/math/macros.hh				\
  oln/morpho/closing.hh				\
  oln/morpho/closing.inc			\
  oln/morpho/dilation.hh			\
  oln/morpho/erosion.hh				\
  oln/morpho/extrema.hh				\
  oln/morpho/extrema.hxx			\
  oln/morpho/extrema_killer.hh			\
  oln/morpho/fast_morpho.hh			\
  oln/morpho/fast_morpho.hxx			\
  oln/morpho/geodesic_dilation.hh		\
  oln/morpho/geodesic_erosion.hh		\
  oln/morpho/gradient.hh			\
  oln/morpho/gradient.inc			\
  oln/morpho/hit_or_miss.hh			\
  oln/morpho/hit_or_miss.inc			\
  oln/morpho/laplacian.hh			\
  oln/morpho/laplacian.inc			\
  oln/morpho/minmax.hh				\
  oln/morpho/opening.hh				\
  oln/morpho/opening.inc			\
  oln/morpho/reconstruction.hh			\
  oln/morpho/splitse.hh				\
  oln/morpho/stat.hh				\
  oln/morpho/thinning.hh			\
  oln/morpho/thinning.inc			\
  oln/morpho/thickening.hh			\
  oln/morpho/thickening.inc			\
  oln/morpho/top_hat.hh				\
  oln/morpho/top_hat.inc			\
  oln/morpho/watershed.hh			\
  oln/morpho/watershed.hxx			\
  oln/topo/dmap.hh				\
  oln/topo/dmap.hxx				\
  oln/transforms/fft.hh				\
  oln/transforms/dwt.hh				\
  oln/transforms/wavelet_coeffs.hh		\
  oln/utils/copy.hh				\
  oln/utils/histogram.hh			\
  oln/utils/stat.hh				\
  oln/utils/timer.hh				\
  oln/types/all.hh				\
  oln/types/behaviour.hh			\
  oln/types/bin.hh				\
  oln/types/builtins_properties.hh		\
  oln/types/cast.hh				\
  oln/types/color.hh				\
  oln/types/cplx.hh				\
  oln/types/cplx_representation.hh		\
  oln/types/cycle.hh				\
  oln/types/debug.hh				\
  oln/types/global_ops.hh			\
  oln/types/global_ops_defs.hh			\
  oln/types/global_ops_traits.hh		\
  oln/types/hsi.hh				\
  oln/types/hsl.hh				\
  oln/types/hsv.hh				\
  oln/types/int_s.hh				\
  oln/types/int_u.hh				\
  oln/types/interval.hh				\
  oln/types/nrgb.hh				\
  oln/types/optraits.hh				\
  oln/types/optraits_bin.hh			\
  oln/types/optraits_builtins.hh		\
  oln/types/optraits_builtins_bool.hh		\
  oln/types/optraits_builtins_float.hh		\
  oln/types/optraits_builtins_int.hh		\
  oln/types/optraits_builtins_int_defs.hh	\
  oln/types/optraits_cplx.hh			\
  oln/types/optraits_cycle.hh			\
  oln/types/optraits_int_s.hh			\
  oln/types/optraits_int_u.hh			\
  oln/types/optraits_range.hh			\
  oln/types/optraits_scalar.hh			\
  oln/types/optraits_scalar_defs.hh		\
  oln/types/optraits_vec.hh			\
  oln/types/predecls.hh				\
  oln/types/range.hh				\
  oln/types/rec_value.hh			\
  oln/types/rgb.hh				\
  oln/types/to_oln.hh				\
  oln/types/type.hh				\
  oln/types/typetraits.hh			\
  oln/types/typetraits_builtins.hh		\
  oln/types/typetraits_builtins_bool.hh		\
  oln/types/typetraits_builtins_float.hh	\
  oln/types/typetraits_builtins_int.hh		\
  oln/types/vec.hh				\
  oln/types/xyz.hh				\
  oln/types/yiq.hh				\
  oln/types/yuv.hh


###
### Autoconf macro repository
###

# oln.m4 is for users, install it with other aclocal data.
m4datadir = $(datadir)/aclocal
dist_m4data_DATA = config/oln.m4

###
### Development files
###

EXTRA_DIST = config/oln-local.m4 oln/config/pconf-hh.in


###
### Stuff pertaining to compiler-specific auto-configuration
###

CLEANFILES = conf/config.log* conf/configure.log* conf/*.log conf/config.status conf/oln-config*.sh conf/*.1
MAINTAINERCLEANFILES = conf/aclocal.m4 conf/autom4te.cache
EXTRA_DIST += conf/configure.ac \
	conf/compilers.def \
	conf/gen-scripts.sh 

dist_pkgdata_DATA = conf/configure conf/oln-config.shin

if OLN_CONFIG_SH
# FIXME: when the two following rules are enabled, they are
# always run, resulting in errors if aclocal and/or autoconf
# are not available.

# $(srcdir)/conf/aclocal.m4: $(srcdir)/conf/configure.ac $(srcdir)/config/oln.m4 
#	cd $(srcdir)/conf && $(ACLOCAL) -I .. -I ../..

# $(srcdir)/conf/configure: $(srcdir)/conf/configure.ac $(srcdir)/conf/aclocal.m4
#	cd $(srcdir)/conf && $(AUTOCONF)

install-data-hook: $(srcdir)/conf/configure $(srcdir)/conf/gen-scripts.sh $(srcdir)/conf/compilers.def
	mkdir -p conf && $(SHELL) $(srcdir)/conf/gen-scripts.sh $(DESTDIR)$(includedir) $(srcdir)/conf ./conf
	$(mkinstalldirs) $(DESTDIR)$(bindir)
	@ for p in conf/oln-config-*.sh; do \
	  if test -f $$p; then \
	    f=`echo "$$p" | sed 's,^.*/,,;$(transform)'`; \
	    echo " $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f"; \
	    $(INSTALL_PROGRAM_ENV) $(INSTALL_PROGRAM) $$p $(DESTDIR)$(bindir)/$$f || exit 1; \
	  else :; fi; \
	done

uninstall-hook:
	@ for p in conf/oln-config-*.sh; do \
	  f=`echo "$$p" | sed 's,^.*/,,;$(transform)'`; \
	  echo " rm -f $(DESTDIR)$(bindir)/$$f"; \
	  rm -f $(DESTDIR)$(bindir)/$$f; \
	done

endif

