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

# include <oln/io/read_image_2d_pnm.hh>
# include <oln/io/utils.hh>

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
    void do_read(abstract::image<I>& ima, const filename& name)
    {
      std::string ext;
      const oln_type_of(I, value) t;

      ext = internal::utils::extension(name.get());

      if (ext == "pgm" ||
	  ext == "pbm" ||
	  ext == "ppm")
	impl::read(ima.exact(), t, name.get(), ext);
      else
	{
	  std::cout << "input method for '"
		    << name.get()
		    << "' not implemented"
		    << std::endl;
	}

    }

  }

}

#endif // ! OLN_IO_READ_IMAGE_HH
