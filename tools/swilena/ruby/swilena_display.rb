## Copyright (C) 2003  EPITA Research and Development Laboratory
##
## This file is part of the Olena Library.  This library is free
## software; you can redistribute it and/or modify it under the terms
## of the GNU General Public License version 2 as published by the
## Free Software Foundation.
##
## This library is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this library; see the file COPYING.  If not, write to
## the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
## Boston, MA 02110-1301, USA.
##
## As a special exception, you may use this file as part of a free
## software library without restriction.  Specifically, if other files
## instantiate templates or use macros or inline functions from this
## file, or you compile this file and link it with other files to
## produce an executable, this file does not by itself cause the
## resulting executable to be covered by the GNU General Public
## License.  This exception does not however invalidate any other
## reasons why the executable file might be covered by the GNU General
## Public License.


class SwilenaDisplay

  attr_writer :display_command, :tmpdir
  attr_reader :display_command, :tmpdir

  def initialize
    @threads = []
    @display_command = "display %{image_file}"
    @tmpdir = "/tmp"
  end

  def launch_image_viewer(image)
    command = @display_command.gsub("%{image_file}", image)
    `#{command}`
    print "Error while running `#{command}'\n" if $? != 0
  end

  def display_image(oln_image, name)
    image_file = @tmpdir + "/" + name
    oln_image.save(image_file)
    @threads << Thread.new { launch_image_viewer(image_file) }
  end

  def wait_all_displays
    @threads.each { |thread| thread.join }
  end

end
