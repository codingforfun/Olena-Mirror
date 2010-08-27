# Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
# (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.

SUBDIRS = demo tests src

.PHONY: bin

bin:
	make -C src

# Regen files recursively.
include $(top_srcdir)/build-aux/regen-recursive.mk
REGEN_SUBDIRS += tests

regen-am:
	cd $(srcdir)							\
	  && $(abs_top_srcdir)/build-aux/generate_dist_headers.sh	\
	       scribo headers.mk nodist-headers

# FIXME: Change generate_dist_headers.sh so that the action looks like this:
#
#	$< $@.tmp && mv -f $@.tmp $@ && chmod -w $@
#
$(srcdir)/headers.mk: $(top_srcdir)/build-aux/generate_dist_headers.sh
	cd $(srcdir)							\
	  && $(abs_top_srcdir)/build-aux/generate_dist_headers.sh	\
	       scribo headers.mk nodist-headers

include $(srcdir)/headers.mk




# Packaging
#----------

tmp_inst_dir = scribo_inst.tmp
tmp_inst_path = $(abs_builddir)/$(tmp_inst_dir)

regen-inst:
	rm -rf $(tmp_inst_path) 					\
	&& mkdir $(tmp_inst_path)					\
	&& $(MAKE) DESTDIR=$(tmp_inst_path) install			\
	&& mkepmlist --prefix $(bindir) -g sys -u root 			\
		$(builddir)/$(tmp_inst_dir)/$(bindir) 			\
			> $(top_srcdir)/pkg-aux/scribo_bin_list.inc 	\
	&& mkepmlist --prefix $(libexecdir) -g sys -u root 		\
		$(builddir)/$(tmp_inst_dir)/$(libexecdir) 		\
			>> $(top_srcdir)/pkg-aux/scribo_bin_list.inc	\
	&& mkepmlist --prefix $(includedir) -g sys -u root 		\
		$(builddir)/$(tmp_inst_dir)/$(includedir) 		\
			> $(top_srcdir)/pkg-aux/scribo_include_list.inc \
	&& rm -rf $(tmp_inst_path)


deb:
	rm -rf $(tmp_inst_path) 					\
	&& mkdir $(tmp_inst_path)					\
	&& $(MAKE) DESTDIR=$(tmp_inst_path) install			\
	&& epm -v -v -f deb -n --output-dir $(top_builddir) scribo 	\
		$(top_builddir)/pkg-aux/scribo.list 			\
	&& rm -rf $(tmp_inst_path)


rpm:
	rm -rf $(tmp_inst_path) 					\
	&& mkdir $(tmp_inst_path)					\
	&& $(MAKE) DESTDIR=$(tmp_inst_path) install			\
	&& epm -v -v -f rpm -n --output-dir $(top_builddir) scribo 	\
		$(top_builddir)/pkg-aux/scribo.list 			\
	&& rm -rf $(tmp_inst_path)

pkg:
	rm -rf $(tmp_inst_path) 					\
	&& mkdir $(tmp_inst_path)					\
	&& $(MAKE) DESTDIR=$(tmp_inst_path) install			\
	&& epm -v -v -f rpm -n --output-dir $(top_builddir) scribo 	\
		$(top_builddir)/pkg-aux/scribo.list 			\
	&& epm -v -v -f deb -n --output-dir $(top_builddir) scribo 	\
		$(top_builddir)/pkg-aux/scribo.list 			\
	&& rm -rf $(tmp_inst_path)

