#!/usr/bin/env ruby

# Set swilena_path to the location where swilena ruby modules are
$: << ENV["SWILENA_PATH"] if ENV.has_key? "SWILENA_PATH"

require "swilena_image2d"
require "swilena_ntg"
require "swilena_display"

include Swilena_ntg
include Swilena_image2d

display = SwilenaDisplay.new

display.tmpdir = "/tmp/swilena"
display.display_command = "display %{image_file}"

ima = Image2d_u8.new(5,5)

for i in 0..5 do
  for j in 0..5 do
    ima[i, j] = i + j
  end
end

print ima
display.display_image (ima, "image1")

display.wait_all_displays()
