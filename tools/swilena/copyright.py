# Copyright (C) 2007  EPITA Research and Development Laboratory
#
# This file is part of the Olena Library.  This library is free
# software; you can redistribute it and/or modify it under the terms
# of the GNU General Public License version 2 as published by the
# Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
# Boston, MA 02110-1301, USA.
#
# As a special exception, you may use this file as part of a free
# software library without restriction.  Specifically, if other files
# instantiate templates or use macros or inline functions from this
# file, or you compile this file and link it with other files to
# produce an executable, this file does not by itself cause the
# resulting executable to be covered by the GNU General Public
# License.  This exception does not however invalidate any other
# reasons why the executable file might be covered by the GNU General
# Public License.

import re

def notice(template_file, years):
  years_re = re.compile ('<YEARS>')
  bol_re = re.compile ('^')
  
  template = open (template_file)
  line = template.readline ()
  notice = []
  while line:
    prefixed_line = bol_re.sub ('// ', line)
    notice.append (years_re.sub (years, prefixed_line))   
    line = template.readline ()
  template.close ()
  return notice
