# Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE).
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

include $(top_srcdir)/scribo/scribo.mk

all-local:
	test -d output || mkdir output

EXTRA_DIST = README TODO originals/competition-entry-region-model.xsd

if HAVE_POPPLER
if HAVE_TESSERACT

AM_CPPFLAGS += $(POPPLER_CPPFLAGS) $(TESSERACT_CPPFLAGS)
AM_CXXFLAGS += -Wall -Werror -O3 -DNDEBUG
AM_LDFLAGS = $(POPPLER_LDFLAGS) $(TESSERACT_LDFLAGS) 

noinst_PROGRAMS = table old

table_SOURCES = src/disjoint_set.hh src/xml.hh src/xml.cc src/new.cc
table_CXXFLAGS = $(AM_CXXFLAGS) -DMLN_WO_GLOBAL_VARS
old_SOURCES = src/main.cc

endif HAVE_TESSERACT
endif HAVE_POPPLER

CLEANFILES = log final.xml

clean-local:
	-rm -rf output
