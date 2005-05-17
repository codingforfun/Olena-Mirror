// Copyright (C) 2005  EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef OLN_IO_WRITE_IMAGE_HH
# define OLN_IO_WRITE_IMAGE_HH

# include <string>

# include <oln/io/gz_stream.hh>
# include <oln/io/utils.hh>
# include <oln/io/write_image_pnm.hh>

namespace oln {

  namespace io {

    template <typename I>
    void write_dispatch_ext(const abstract::image<I>& ima,
			    std::ostream& ostr,
			    const std::string& ext)
    {
      if (ext == "pgm" || ext == "pbm" || ext == "ppm" ||
	  ext == "pnm")
	write_pnm(ima, ostr, ext);
      else
	std::cout << "io error: format not supported."
		  << std::endl;
    }

#if defined HAVE_ZLIB && HAVE_ZLIB == 1
    template <typename I>
    void write_gz(const abstract::image<I>& ima, const std::string& name)
    {
      gz::zofstream zostr(name.c_str(), std::ios::out);

      if (zostr.is_open() == false)
	std::cerr << "error: couldn't open " << name << std::endl;
      else
	{
	  std::string ext;

	  ext = internal::extension(name.substr(0, name.size() - 3));
	  write_dispatch_ext(ima, zostr, ext);
	}
      zostr.close();
    }

#endif // ! HAVE_ZLIB

    template <typename I>
    void write_non_gz(const abstract::image<I>& ima,
		      const std::string& name,
		      const std::string& ext)
    {
      std::ofstream ostr;

      ostr.open(name.c_str(), std::ifstream::out);

      if (ostr.is_open() == false)
	std::cerr << "error: couldn't open " << name << std::endl;
      else
	write_dispatch_ext(ima, ostr, ext);
      ostr.close();
    }

    template <typename I>
    void write(const abstract::image<I>& ima, const std::string& name)
    {
      std::string ext;

      ext = internal::extension(name);

#if defined HAVE_ZLIB && HAVE_ZLIB == 1
      if (ext == "gz")
	write_gz(ima, name);
      else
	write_non_gz(ima, name, ext);
# else
      write_non_gz(ima, name, ext);
#endif // ! HAVE_ZLIB
    }

  }

}

#endif // ! OLN_IO_WRITE_IMAGE_HH
