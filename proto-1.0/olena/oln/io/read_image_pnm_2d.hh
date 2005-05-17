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

#ifndef OLN_IO_READ_IMAGE_PNM_2D_HH
# define OLN_IO_READ_IMAGE_PNM_2D_HH

# include <ntg/all.hh>

# include <oln/canvas/io.hh>

# include <oln/core/2d/image2d.hh>
# include <oln/io/read_image_pnm_decl.hh>

namespace oln {


  namespace io {


    namespace impl {


      template <typename I>
      struct read_pnm_vect<I, oln::size2d>
	: public canvas::read_2d<I, read_pnm_vect<I, oln::size2d> >
      {
	typedef canvas::read_2d<I, read_pnm_vect<I, oln::size2d> > super_type;
	typedef oln_type_of(I, value) value_type;
	typedef ntg_comp_type(value_type) comp_type;
	typedef oln_io_type(ntg_nbits(ntg_comp_type(value_type))) v_type;

	void impl_read_point()
	{
	  value_type c;
	  for (unsigned i = 0; i < ntg_nb_comp(value_type); ++i)
	    {
	      v_type v;
	      this->istr.read((char*)&v, sizeof (v_type));
	      c[i] = (ntg_storage_type(ntg_comp_type(value_type)))v;
	    }
	  this->output[this->p] = c;
	}

	void impl_preconditions()
	{
	  precondition(ntg_max_val(comp_type) - ntg_min_val(comp_type)
		       <= this->hdr.max_val);
	  precondition(ntg_nb_comp(value_type) == 3);
	}

	read_pnm_vect(std::istream& istr,
		      const internal::pnm_header& hdr) :
	  super_type(istr, hdr)
	{}

      };

      template <typename I>
      struct read_pnm_real<I, oln::size2d>
	: public canvas::read_2d<I, read_pnm_real<I, oln::size2d> >
      {
	typedef canvas::read_2d<I, read_pnm_real<I, oln::size2d> > super_type;
	typedef oln_type_of(I, value) value_type;
	typedef ntg_type(value_type) ntg_value_type;
	typedef oln_io_type(ntg_nbits(ntg_value_type)) v_type;


	void impl_read_point()
	{
	  v_type v;
	  this->istr.read((char*)&v, sizeof (v_type));
	  this->output[this->p] = (ntg_storage_type(value_type))v;
	}

	void impl_preconditions()
	{
	  precondition(ntg_max_val(value_type) - ntg_min_val(value_type)
		       <= this->hdr.max_val);
	}

	read_pnm_real(std::istream& istr,
		      const internal::pnm_header& hdr) :
	  super_type(istr, hdr)
	{}

      };

      template <typename I>
      struct read_pnm_bin<I, oln::size2d>
	: public canvas::read_2d<I, read_pnm_bin<I, oln::size2d> >
      {
	typedef canvas::read_2d<I, read_pnm_bin<I, oln::size2d> > super_type;
	typedef oln_type_of(I, value) value_type;
	typedef ntg_type(value_type) ntg_value_type;
	typedef oln_io_type(ntg_nbits(ntg_value_type)) v_type;

	char v;
	int offset;

	void impl_extra_work()
	{
	  offset = -1;
	}

	void impl_read_point()
	{
	  if (offset == -1)
	    {
	      this->istr.read(&v, 1);
	      offset = 7;
	    }
	  if ((int)(v & (1<<offset--)) == 0)
	    this->output[this->p] = 0;
	  else
	    this->output[this->p] = 1;
	}

	void impl_preconditions()
	{
	  precondition(ntg_max_val(value_type) - ntg_min_val(value_type)
		       <= this->hdr.max_val);
	}

	read_pnm_bin(std::istream& istr,
		     const internal::pnm_header& hdr) :
	  super_type(istr, hdr),
	  offset(-1)
	{}

      };



    }


  }


}

#endif // ! OLN_IO_READ_IMAGE_PNM_2D_HH
