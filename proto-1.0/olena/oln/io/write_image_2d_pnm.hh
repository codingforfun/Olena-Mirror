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


#ifndef OLN_IO_WRITE_IMAGE_2D_PNM_HH
# define OLN_IO_WRITE_IMAGE_2D_PNM_HH

# include <iostream>
# include <string>

# include <mlc/box.hh>

# include <ntg/core/macros.hh>
# include <ntg/real/int_u8.hh>
# include <ntg/real/integer.hh>
# include <ntg/enum/enum.hh>
# include <ntg/color/color.hh>

# include <oln/core/2d/image2d.hh>
# include <oln/core/macros.hh>
# include <oln/core/abstract/op.hh>

# include <oln/io/utils.hh>

namespace oln {

  namespace io {

    namespace impl {

      template <typename I>
      struct write_image_2d_raw
	: public oln::abstract::void_op<write_image_2d_raw<I> >
      {
	typedef oln_value_type(I) value_type;
	typedef ntg_io_type(value_type) io_type;

	const I& to_write_;
	std::ofstream& ostr_;

	write_image_2d_raw(const I& to_write, std::ofstream& ostr):
	  to_write_(to_write),
	  ostr_(ostr)
	{}

	void impl_run()
	{
	  point2d p;
	  value_type c;
	  bool b;

	  for (p.row() = 0; p.row() < to_write_.size().nrows(); ++p.row())
	    for (p.col() = 0; p.col() < to_write_.size().ncols(); ++p.col())
	      {
		c = to_write_[p];
		b = write_value_type(c);
	      }
	  while (b == false)
	    b = write_value_type(c);
	  ostr_.close();
	}

	bool write_value_type(const ntg::integer<value_type> &c)
	{
	  ostr_ << (io_type)(c.exact());
	  return true;
	}

	bool write_value_type(const ntg::color<value_type> &c)
	{
	  assert ((ntg_nb_comp(value_type) == 3));

	  for (unsigned i = 0; i < 3; i++)
	    ostr_ << (io_type)(c[i]);
	  return true;
	}

	bool write_value_type(const ntg::enum_value<value_type> &c)
	{
	  assert ((ntg_max_val(value_type) == 1));
	  static int offset = 7;
	  static unsigned char v = 0;
	  bool ret = false;

	  if (offset == -1)
	    {
	      ostr_ << v;
	      offset = 7;
	      v = 0;
	      ret = true;
	    }
	  if (c == 1)
	    v |= 1 << offset;
	  offset--;
	  return ret;
	}
      };


      template <typename I, typename T>
      void write(const abstract::image2d<I>& to_write,
		 const ntg::integer<T>&,
		 const std::string& name,
		 const std::string& ext)
      {
	std::ofstream	ostr;
	point2d p;
	size2d s = to_write.size();

	if (ext == "pgm")
	  if (internal::write_pnm_header(ostr, name, "P5",
					 s.ncols(), s.nrows(),
					 ntg_max_val(T)))
	    {
	      write_image_2d_raw<I> tmp(to_write.exact(), ostr);
	      tmp.run();
	    }
	  else
	    std::cerr << "error: unable to write header" << std::endl;
	else
	  std::cerr << "error: image data type (`integer') does not match"
		    << " file extension (`" << ext << "')" << std::endl;
      }

      template <typename I, typename T>
      void write(const abstract::image2d<I>& to_write,
		 const ntg::color<T>&,
		 const std::string& name,
		 const std::string& ext)
      {
	std::ofstream	ostr;
	point2d p;
	size2d s = to_write.size();

	if (ext == "ppm")
	  if (internal::write_pnm_header(ostr, name, "P6",
					 s.ncols(), s.nrows(),
					 ntg_max_val(T)))
	    {
	      write_image_2d_raw<I> tmp(to_write.exact(), ostr);
	      tmp.run();
	    }
	  else
	    std::cerr << "error: unable to write header" << std::endl;
	else
	  std::cerr << "error: image data type (`color') does not match"
		    << " file extension (`" << ext << "')" << std::endl;
      }

      template <typename I, typename T>
      void write(const abstract::image2d<I>& to_write,
		 const ntg::enum_value<T>&,
		 const std::string& name,
		 const std::string& ext)
      {
	std::ofstream	ostr;
	point2d p;
	size2d s = to_write.size();

	if (ext == "pbm")
	  if (internal::write_pnm_header(ostr, name, "P4",
					 s.ncols(), s.nrows(),
					 ntg_max_val(T)))
	    {
	      write_image_2d_raw<I> tmp(to_write.exact(), ostr);
	      tmp.run();
	    }
	  else
	    std::cerr << "error: unable to write header" << std::endl;
	else
	  std::cerr << "error: image data type (`enum_value') does not match"
		    << " file extension (`" << ext << "')" << std::endl;
      }

    }

  }

}



#endif // ! OLN_IO_WRITE_IMAGE_2D_PNM_HH
