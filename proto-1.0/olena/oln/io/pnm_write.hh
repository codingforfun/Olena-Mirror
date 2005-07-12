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

#ifndef OLN_IO_PNM_WRITE_HH
# define OLN_IO_PNM_WRITE_HH

# include <iostream>

# include <ntg/core/macros.hh>

# include <oln/core/abstract/image.hh>

# include <oln/io/scalar.hh>
# include <oln/io/vectorial.hh>

# include <oln/io/pnm_real.hh>
# include <oln/io/pnm_bin.hh>
# include <oln/io/pnm_vect.hh>

namespace oln {


  namespace io {


    namespace pnm {


      namespace impl {

	template <typename V, typename I>
	void write(const ntg::real_value<V>&,
		   const abstract::image<I>& ima,
		   std::ostream& ostr,
		   const std::string& ext)
	{
	  typedef oln_type_of(I, grid) grid_type;

	  if (ext == "pgm")
	    {
	      if (ntg_max_val(V) - ntg_min_val(V) < 256)
		{
		  typedef write_real<char, grid_type> writer_type;
		  writer_type pnm_writer(ostr);
		  write_scalar<I, writer_type> writer(ima, pnm_writer);

		  writer.run();
		}
	      else
		{
		  typedef write_real<short, grid_type> writer_type;
		  writer_type pnm_writer(ostr);
		  write_scalar<I, writer_type> writer(ima, pnm_writer);

		  writer.run();
		}
	    }
	  else
	    std::cerr << "io error: extension mismatch data type" << std::endl;
	}

	template <typename V, typename I>
	void write(const ntg::enum_value<V>&,
		   const abstract::image<I>& ima,
		   std::ostream& ostr,
		   const std::string& ext)
	{
	  typedef oln_type_of(I, grid) grid_type;

	  if (ext == "pbm")
	    {
	      assert(ntg_max_val(V) == 1 && ntg_min_val(V) == 0);

	      typedef write_bin<grid_type> writer_type;
	      writer_type pnm_writer(ostr);
	      write_scalar<I, writer_type> writer(ima, pnm_writer);

	      writer.run();
	    }
	  else
	    std::cerr << "io error: extension mismatch data type" << std::endl;
	}

	template <typename V, typename I>
	void write(const ntg::vect_value<V>&,
		   const abstract::image<I>& ima,
		   std::ostream& ostr,
		   const std::string& ext)
	{
	  typedef oln_type_of(I, grid) grid_type;

	  if (ext == "ppm")
	    {
	      assert(ntg_nb_comp(V) == 3);

	      if (ntg_max_val(ntg_comp_type(V)) -
		  ntg_min_val(ntg_comp_type(V)) < 256)
		{
		  typedef write_vect<char, grid_type> writer_type;
		  writer_type pnm_writer(ostr);
		  write_vectorial<I, writer_type> writer(ima, pnm_writer);

		  writer.run();
		}
	      else
		{
		  typedef write_vect<short, grid_type> writer_type;
		  writer_type pnm_writer(ostr);
		  write_vectorial<I, writer_type> writer(ima, pnm_writer);

		  writer.run();
		}
	    }
	  else
	    std::cerr << "io error: extension mismatch data type" << std::endl;
	}


      }

      template <typename I>
      void write(const abstract::image<I>& ima,
		 std::ostream& ostr,
		 const std::string& ext)
      {
	typedef oln_type_of(I, value) value_type;
	typedef ntg_type(value_type) ntg_value_type;

	impl::write(ntg_value_type(), ima.exact(), ostr, ext);
      }


    }


  }


}

# endif // ! OLN_IO_PNM_WRITE_HH
