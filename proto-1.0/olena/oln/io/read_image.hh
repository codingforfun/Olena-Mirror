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

#ifndef OLN_IO_READ_IMAGE_HH
# define OLN_IO_READ_IMAGE_HH

# include <iostream>
# include <string>

# include <mlc/box.hh>

# include <ntg/core/macros.hh>

# include <oln/io/read_image_2d_pnm.hh>
# include <oln/io/utils.hh>
# include <oln/io/gz_stream.hh>

namespace oln {

  namespace io {

    struct filename
    {
      explicit filename(const std::string& str) :
	str_(str)
      {}
      const std::string& get() const
      {
	return str_;
      }
    private:
      std::string str_;
    };

    filename read(const std::string& str)
    {
      filename tmp(str);
      return tmp;
    }


    template <typename I>
    void read_dispatch_ext(abstract::image<I>& ima,
			   std::istream& istr,
			   const std::string& ext)

    {
      if (ext == "pgm" || ext == "pbm" || ext == "ppm" ||
	  ext == "pnm")
	{
	  internal::pnm_info info;

	  if (internal::read_pnm_header(istr, info))
	    if ((ext == "ppm" || ext == "pnm") && info.type == "P6")
	      ima.exact() = impl::read<I>(istr, info);
	    else if ((ext == "ppm" || ext == "pnm") && info.type == "P3")
	      std::cerr << "error: read_image_2d_ppm_ascii not implemented"
			<< std::endl;
	    else if ((ext == "pbm" || ext == "pnm") && info.type == "P4")
	      ima.exact() = impl::read<I>(istr, info);
	    else if ((ext == "pbm" || ext == "pnm") && info.type == "P1")
	      std::cerr << "error: read_image_2d_ppm_ascii not implemented"
			<< std::endl;
	    else if ((ext == "pgm" || ext == "pnm") && info.type == "P5")
	      ima.exact() = impl::read<I>(istr, info);
	    else if ((ext == "pgm" || ext == "pnm") && info.type == "P2")
	      std::cerr << "error: read_image_2d_ppm_ascii not implemented"
			<< std::endl;
	    else
	      std::cerr << "error: file header (`" << info.type
			<< "') does not match file extension (`"
			<< ext << "')" << std::endl;
	  else
	    std::cerr << "error: unable to get a valid header" << std::endl;
	}
      else
	std::cout << "no input method for '"
		  << ext << "' file extension"
		  << std::endl;
    }

#if defined HAVE_ZLIB && HAVE_ZLIB == 1
    template <typename I>
    void do_read_gz(abstract::image<I>& ima, const std::string& name)
    {
      gz::zifstream zistr(name.c_str(), std::ios::in);

      if (zistr.is_open() == false)
	std::cerr << "error: couldn't open " << name << std::endl;
      else
	{
	  std::string ext;

	  ext = internal::utils::extension(name.substr(0, name.size() - 3));
	  read_dispatch_ext(ima, zistr, ext);
	}
      zistr.close();
    }

#endif // ! HAVE_ZLIB

    template <typename I>
    void do_read_non_gz(abstract::image<I>& ima,
			const std::string& name,
			const std::string& ext)
    {
      std::ifstream istr;

      istr.open(name.c_str(), std::ifstream::in);

      if (istr.is_open() == false)
	std::cerr << "error: couldn't open " << name << std::endl;
      else
	read_dispatch_ext(ima, istr, ext);
      istr.close();
    }

    template <typename I>
    void do_read(abstract::image<I>& ima, const filename& name)
    {
      std::string ext;

      ext = internal::utils::extension(name.get());

#if defined HAVE_ZLIB && HAVE_ZLIB == 1

      if (ext == "gz")
	do_read_gz(ima, name.get());
      else
	do_read_non_gz(ima, name.get(), ext);
#else
      do_read_non_gz(ima, name.get(), ext);
#endif // ! HAVE_ZLIB
    }


  }

}

#endif // ! OLN_IO_READ_IMAGE_HH
