#!/usr/bin/env ruby

# Set swilena_path to the location where swilena ruby modules are
$: << ENV["SWILENA_PATH"] if ENV.has_key? "SWILENA_PATH"

require "swilena_image2d"
require "swilena_ntg_int_u"
require "swilena_display"

include Swilena_ntg_int_u
include Swilena_image2d

display = SwilenaDisplay.new

display.tmpdir = "/tmp/swilena"
display.display_command = "display %{image_file}"

ima = Image2d_u8.new(5,5)

0.upto(5) do |i|
  0.upto(5) do |j|
    ima.set(i, j, Int_u8.new(i + j))
  end
end

print ima
display.display_image (ima, "image1")

ima2 = Image2d_u16.new(10, 10)
0.upto(5) do |i|
  0.upto(5) do |j|
    ima2.set(i, j, Int_u16.new(1))
  end
end
display.display_image (ima2, "image2")

display.wait_all_displays()
