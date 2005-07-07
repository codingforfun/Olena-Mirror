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

#ifndef OLN_IO_WRITE_IMAGE_PNM_2D_HH
# define OLN_IO_WRITE_IMAGE_PNM_2D_HH

# include <ntg/all.hh>

# include <oln/canvas/io.hh>

# include <oln/core/2d/image2d.hh>
# include <oln/io/write_image_pnm_decl.hh>

namespace oln {


  namespace io {


    namespace impl {


      template <typename I>
      struct write_pnm_vect<I, oln::size2d>
	: public canvas::write_2d<I, write_pnm_vect<I, oln::size2d> >
      {
	typedef canvas::write_2d<I, write_pnm_vect<I, oln::size2d> > super_type;
	typedef oln_type_of(I, value) value_type;
	typedef ntg_comp_type(value_type) comp_type;
	typedef oln_io_type(ntg_nbits(ntg_comp_type(value_type))) v_type;

	void impl_write_point()
	{
	  value_type c = this->input[this->p];
	  for (unsigned i = 0; i < ntg_nb_comp(value_type); ++i)
	    {
	      v_type v;
	      v = c[i];
	      this->ostr.write((char*)&v, sizeof (v_type));
	    }
	}

	write_pnm_vect(const abstract::image<I>& input,
		       std::ostream& ostr) :
	  super_type(input, ostr)
	{}

      };

      template <typename I>
      struct write_pnm_real<I, oln::size2d>
	: public canvas::write_2d<I, write_pnm_real<I, oln::size2d> >
      {
	typedef canvas::write_2d<I, write_pnm_real<I, oln::size2d> > super_type;
	typedef oln_type_of(I, value) value_type;
	typedef ntg_type(value_type) ntg_value_type;
	typedef oln_io_type(ntg_nbits(ntg_value_type)) v_type;


	void impl_write_point()
	{
	  v_type v;
	  v = this->input[this->p];
	  this->ostr.write((char*)&v, sizeof (v_type));
	}

	write_pnm_real(const abstract::image<I>& input,
		       std::ostream& ostr) :
	  super_type(input, ostr)
	{}

      };

      template <typename I>
      struct write_pnm_bin<I, oln::size2d>
	: public canvas::write_2d<I, write_pnm_bin<I, oln::size2d> >
      {
	typedef canvas::write_2d<I, write_pnm_bin<I, oln::size2d> > super_type;
	typedef oln_type_of(I, value) value_type;
	typedef ntg_type(value_type) ntg_value_type;
	typedef oln_io_type(ntg_nbits(ntg_value_type)) v_type;

	int offset;
	char v;

	void impl_extra_work()
	{
	  if (offset != 7)
	    {
	      this->ostr.write(&v, 1);
	      offset = 7;
	      v = 0;
	    }
	}

	void impl_write_point()
	{
	  if (offset == -1)
	    {
	      this->ostr.write(&v, 1);
	      offset = 7;
	      v = 0;
	    }
	  if (this->input[this->p] == value_type(0))
	    v |= 1 << offset;
	  offset--;
	}

	write_pnm_bin(const abstract::image<I>& input,
		      std::ostream& ostr) :
	  super_type(input, ostr),
	  offset(7),
	  v(0)
	{
	}

      };



    }


  }


}

#endif // ! OLN_IO_WRITE_IMAGE_PNM_2D_HH
