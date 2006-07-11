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
  if installed
    filename = File.join(File.dirname(libtool_library), dlname)
  else
    filename = File.join(File.dirname(libtool_library), '.libs', dlname)
  end
  filename
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
