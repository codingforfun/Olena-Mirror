# Copyright (C) 2006 EPITA Research and Development Laboratory.
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
# the Free Software Foundation, 51 Franklin Street, Fifth Floor,
# Boston, Boston, MA 02110-1301, USA.

# Extension of the Kernel.require method understanding Libtool libraries.
# Inspired by ltihooks.py.

# Extract the real filename of a library from a Libtool library.
def extract_dlname(libtool_library)
  dlname = ""
  installed = true
  File.open(libtool_library) do |file|
    file.each_line do |line|
      case line.chomp
      when /^dlname='(.*)'/
        dlname = $1
      when /^installed=(.*)$/
        installed = $1 == "yes"
      end
    end
  end
  if dlname.empty?
    raise "No dlname found in " + libtool_library
  end
  
  if $LTREQUIRE_NO_PATH_PREFIX then
    # Simply return the name of the dynamic module.
    dlname
  else
    if installed
      File.join(File.dirname(libtool_library), dlname)
    else
      File.join(File.dirname(libtool_library), '.libs', dlname)
    end
  end
end

# Add a hook to Kernel.require to handle Libtool libraries.
module Kernel
  alias_method :old_require, :require
  def require(library_name)
    real_library_name = ""
    # If no extension was provided, check for a Libtool library first.
    ltlib_name = if library_name =~ /\.la$/
      then library_name
      else library_name + ".la"
      end
    $:.each do |dir|
      full_name = File.join(dir, ltlib_name)
      if File::exists?(full_name)
        real_library_name = extract_dlname(full_name)
        break
      end
    end
    # Otherwise, juste use the original name.
    if real_library_name.empty?
      real_library_name = library_name
    end

    # Call the actual require method.
    old_require(real_library_name)
  end
end
