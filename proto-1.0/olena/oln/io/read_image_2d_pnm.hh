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

#ifndef OLN_IO_READ_IMAGE_2D_PNM_HH
# define OLN_IO_READ_IMAGE_2D_PNM_HH

# include <iostream>
# include <string>

# include <mlc/box.hh>
# include <mlc/math.hh>

# include <ntg/all.hh>

# include <oln/core/2d/image2d.hh>
# include <oln/core/abstract/op.hh>

# include <oln/io/utils.hh>


namespace oln {

  template <typename I>
  struct image2d;

  // fwd decl
  namespace io {
    namespace impl {
      template <typename I> struct read_image_2d_raw;
    }
  }

  // category
  template <typename I>
  struct set_category < io::impl::read_image_2d_raw<I> >
  {
    typedef category::image ret;
  };

  // super_type
  template <typename I>
  struct set_super_type < io::impl::read_image_2d_raw<I> >
  {
    typedef abstract::op<I, io::impl::read_image_2d_raw<I> > ret;
  };


  namespace io {

    namespace impl {

      template <typename I>
      struct read_image_2d_raw : public oln::abstract::op<I, read_image_2d_raw<I> >
      {

	typedef oln::abstract::op<I, read_image_2d_raw<I> > super_type;
	typedef oln_type_of(I, value) value_type;

	mlc::box<I> image_;
	std::istream& istr_;
	internal::pnm_info& info_;
	char v;
	int offset;

	read_image_2d_raw(I& image,
			  std::istream &istr,
			  internal::pnm_info &info) :
	  super_type(image),
	  image_(image),
	  istr_(istr),
	  info_(info),
          offset(-1)
	{}


	read_image_2d_raw<I>& output(I& output)
	{
	  output = *image_;
	  return *this;
	}

	template <typename E>
	void precond(ntg::real_value<E>& c)
	{
	  precondition(ntg_max_val(value_type) <= info_.max_val);
	  precondition(info_.type == "P5");
	}

	template <typename E>
	void precond(ntg::vect_value<E>& c)
	{
	  precondition(ntg_max_val(ntg_comp_type(value_type)) <= info_.max_val);
	  precondition(info_.type == "P6");
	  precondition(ntg_nb_comp(value_type) == 3);
	}

	void precond(ntg::bin& c)
	{
	  precondition(info_.type == "P4");
	}

	void impl_run()
	{
	  value_type c;
	  point2d p;
	  oln::image2d<value_type>  tmp(info_.rows, info_.cols);

	  precond(c);

	  for (p.row() = 0; p.row() < info_.rows && !istr_.eof(); ++p.row())
	    for (p.col() = 0; p.col() < info_.cols && !istr_.eof(); ++p.col())
	      {
		read_value_type(c);
		tmp[p] = c;
	      }
	  *image_ = tmp;
	}

	//FIXME: Should work with builtin types.

	template <typename E>
	void read_value_type(ntg::real_value<E> &c)
	{
	  typedef oln_io_type(ntg_nbits(E)) v_type;
	  v_type v;
	  istr_.read((char*)&v, sizeof (v_type));
	  c = ntg::cast::force<E, v_type>(v);
	}

	template <typename E>
	void read_value_type(ntg::vect_value<E> &c)
	{
	  for (unsigned i = 0; i < ntg_nb_comp(E); i++)
	    {
	      typedef oln_io_type(ntg_nbits(ntg_comp_type(E))) v_type;
	      v_type v;
	      istr_.read((char*)&v, sizeof (v_type));
	      c[i] = ntg::cast::force<ntg_comp_type(E), v_type>(v);
	    }
	}

	void read_value_type(ntg::bin &c)
	{

	  if (offset == -1)
	    {
	      istr_.read(&v, 1);
	      offset = 7;
	    }
	  if ((int)(v & (1<<offset--)) == 0)
	    c = 0;
	  else
	    c = 1;
	}
      };



      template <typename I>
      void read(abstract::image2d<I>& ima,
		std::istream& istr,
		internal::pnm_info info)
      {
	read_image_2d_raw<I> tmp(ima.exact(), istr, info);
	tmp.run();
	tmp.output(ima.exact());
      }

    }

  }

}


#endif // ! OLN_IO_READ_IMAGE_2D_PNM_HH
