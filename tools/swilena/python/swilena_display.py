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


import string
import commands
from threading import Thread

display_command = "display %{image_file}"
tmpdir = "/tmp"
threads = []

def launch_image_viewer(image):
    command = string.replace(display_command, "%{image_file}", image)
    status, output = commands.getstatusoutput(command)
    if status != 0:
        print "Error while running `%s'!\n" % command

def display_image(oln_image, name):
    image_file = tmpdir + "/" + name
    oln_image.save(image_file)
    new_thread = Thread(target=launch_image_viewer,
                        args = (image_file,))
    new_thread.start()
    threads.append(new_thread)
    
def wait_all_displays():
    [thread.join() for thread in threads]
