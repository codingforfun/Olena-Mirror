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
