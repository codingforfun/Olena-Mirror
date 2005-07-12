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

#ifndef OLN_IO_PNM_HH
# define OLN_IO_PNM_HH

# include <iostream>
# include <string>

# include <oln/core/abstract/image.hh>

# include <oln/io/scalar.hh>
# include <oln/io/vectorial.hh>
# include <oln/io/pnm_header.hh>

# include <oln/io/pnm_real.hh>
# include <oln/io/pnm_bin.hh>
# include <oln/io/pnm_vect.hh>

namespace oln {


  namespace io {


    namespace pnm {


      namespace impl {

	template <typename V, typename I>
	void read(const ntg::real_value<V>&,
		  oln::abstract::image<I>& ima,
		  std::istream& istr,
		  const header &hdr)
	{
	  typedef oln_type_of(I, grid) grid_type;

	  if (hdr.type == "P5")
	    {
	      assert(ntg_max_val(V) - ntg_min_val(V) <= hdr.max_val);

	      if (hdr.max_val < 256)
		{
		  typedef read_real<char, grid_type> reader_type;
		  reader_type pnm_reader(istr, hdr);
		  read_scalar<I, reader_type> reader(pnm_reader);

		  reader.run();
		  reader.assign(ima);
		}
	      else
		{
		  typedef read_real<short, grid_type> reader_type;
		  reader_type pnm_reader(istr, hdr);
		  read_scalar<I, reader_type> reader(pnm_reader);

		  reader.run();
		  reader.assign(ima);
		}
	    }
	  else
	    std::cerr << "io error: pnm type mismatch data type" << std::endl;
	}

	template <typename V, typename I>
	void read(const ntg::enum_value<V>&,
		  abstract::image<I>& ima,
		  std::istream& istr,
		  const header &hdr)
	{
	  typedef oln_type_of(I, grid) grid_type;

	  if (hdr.type == "P4")
	    {
	      assert(ntg_max_val(V) == 1 && ntg_min_val(V) == 0);

	      typedef read_bin<grid_type> reader_type;
	      reader_type pnm_reader(istr, hdr);
	      read_scalar<I, reader_type> reader(pnm_reader);

	      reader.run();
	      reader.assign(ima);
	    }
	  else
	    std::cerr << "io error: pnm type mismatch data type" << std::endl;
	}

	template <typename V, typename I>
	void read(const ntg::vect_value<V>&,
		  abstract::image<I>& ima,
		  std::istream& istr,
		  const header &hdr)
	{
	  typedef oln_type_of(I, grid) grid_type;

	  assert(ntg_nb_comp(V) == 3);
	  if (hdr.type == "P6")
	    {
	      assert(ntg_max_val(ntg_comp_type(V)) -
		     ntg_min_val(ntg_comp_type(V)) <= hdr.max_val);

	      if (hdr.max_val < 256)
		{
		  typedef read_vect<char, grid_type> reader_type;
		  reader_type pnm_reader(istr, hdr);
		  read_vectorial<I, reader_type> reader(pnm_reader);

		  reader.run();
		  reader.assign(ima);
		}
	      else
		{
		  typedef read_vect<short, grid_type> reader_type;
		  reader_type pnm_reader(istr, hdr);
		  read_vectorial<I, reader_type> reader(pnm_reader);

		  reader.run();
		  reader.assign(ima);
		}
	    }
	  else
	    std::cerr << "io error: pnm type mismatch data type" << std::endl;
	}


      }


      void check_hdr_ext(const header& hdr,
			 const std::string& ext)
      {
	if ((ext == "pbm" && hdr.type != "P4") ||
	    (ext == "pgm" && hdr.type != "P5") ||
	    (ext == "ppm" && hdr.type != "P6"))
	  std::cerr << "io warning: extension mismatch pnm type." << std::cerr;
      }



      template <typename I>
      void read(abstract::image<I>& ima,
		std::istream& istr,
		const std::string& ext)
      {
	typedef oln_type_of(I, value) value_type;
	typedef ntg_type(value_type) ntg_value_type;
	header hdr;

	if (!read_header(istr, hdr))
	  {
	    std::cerr << "io error: invalid pnm header." << std::cerr;
	    return;
	  }

	if (hdr.type == "P1" || hdr.type == "P2" || hdr.type == "P3")
	  std::cerr << "io error: ascii pnm format not supported."
		    << std::endl;

	check_hdr_ext(hdr, ext);

	impl::read(ntg_value_type(), ima.exact(), istr, hdr);
      }


    }


  }


}

#endif // ! OLN_IO_PNM_HH
