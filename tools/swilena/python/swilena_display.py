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
