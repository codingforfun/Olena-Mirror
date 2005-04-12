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

# include <ntg/all.hh>

# include <oln/core/2d/image2d.hh>

// commented cause 'void_op' was removed
//# include <oln/core/abstract/op.hh>

# include <oln/io/utils.hh>

namespace oln {

  namespace io {

    namespace impl {

      template <typename I>
      struct write_image_2d_raw
      // commented below cause 'void_op' was removed
// 	: public oln::abstract::void_op<write_image_2d_raw<I> >
      {
	typedef oln_type_of(I, value) value_type;

	const I& to_write_;
	std::ostream& ostr_;
	int bin_offset;
	char bin_v;


	write_image_2d_raw(const I& to_write, std::ostream& ostr):
	  to_write_(to_write),
	  ostr_(ostr)
	{
	  bin_offset = 7;
	  bin_v = 0;
	}

	void impl_run()
	{
	  point2d p;
	  value_type c;
	  bool b = false;

	  for (p.row() = 0; p.row() < to_write_.size().nrows(); ++p.row())
	    for (p.col() = 0; p.col() < to_write_.size().ncols(); ++p.col())
		// FIXME: SHOULD NOT BE .value() !!!
		b = write_value_type(to_write_[p].value());
	  while (b == false)
	    b = write_value_type(c);
	  bin_offset = 7;
	  bin_v = 0;
	}

	//FIXME: Should work with builtin types.

	template <typename E>
	bool write_value_type(const ntg::real_value<E> &c)
	{
	  typedef oln_io_type(ntg_nbits(E)) v_type;

	  v_type v;
	  v = ntg::cast::bound<E, v_type>(c.exact());
	  ostr_.write((char*)&v, sizeof (v_type));
	  return true;
	}

	template <typename E>
	bool write_value_type(const ntg::vect_value<E> &c)
	{
	  for (unsigned i = 0; i < 3; i++)
	    {
	      typedef  oln_io_type(ntg_nbits(ntg_comp_type(value_type))) v_type;

	      v_type v;
	      v = c[i];
	      ostr_.write((char*)&v, sizeof (v_type));
	    }
	  return true;
	}

	bool write_value_type(const ntg::bin &c)
	{
	  bool ret = false;

	  if (bin_offset == -1)
	    {
	      bin_v = ~bin_v;
	      ostr_.write(&bin_v, 1);
	      bin_offset = 7;
	      bin_v = 0;
	      ret = true;
	    }
	  if (c == value_type(0))
	    bin_v |= 1 << bin_offset;
	  bin_offset--;
	  return ret;
	}
      };


      template <typename I, typename T>
      void write(const abstract::image2d<I>& to_write,
		 const ntg::real_value<T>&,
		 std::ostream&	ostr,
		 const std::string& ext)
      {

	point2d p;
	size2d s = to_write.size();

	precondition(ntg_nbits(T) <= 16);
	if (ext == "pgm")
	  if (internal::write_pnm_header(ostr, "P5",
					 s.ncols(), s.nrows(),
					 ntg_max_val(T)))
	    {
	      write_image_2d_raw<I> tmp(to_write.exact(), ostr);
	      tmp.run();
	    }
	  else
	    std::cerr << "error: unable to write file header" << std::endl;
	else
	  std::cerr << "error: image data type (`integer') does not match"
		    << " file extension (`" << ext << "')" << std::endl;
      }

      template <typename I, typename T>
      void write(const abstract::image2d<I>& to_write,
		 const ntg::color_value<T>&,
		 std::ostream& ostr,
		 const std::string& ext)
      {

	point2d p;
	size2d s = to_write.size();

	precondition(ntg_nbits(ntg_comp_type(T)) <= 16);
	precondition(ntg_nb_comp(T) == 3);
	if (ext == "ppm")
	  if (internal::write_pnm_header(ostr, "P6",
					 s.ncols(), s.nrows(),
					 ntg_max_val(ntg_comp_type(T))))
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

      template <typename I>
      void write(const abstract::image2d<I>& to_write,
		 const ntg::bin&,
		 std::ostream& ostr,
		 const std::string& ext)
      {
	point2d p;
	size2d s = to_write.size();

	if (ext == "pbm")
	  if (internal::write_pnm_header(ostr, "P4",
					 s.ncols(), s.nrows(),
					 1))
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
