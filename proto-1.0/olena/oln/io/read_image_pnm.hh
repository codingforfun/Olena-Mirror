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

#ifndef OLN_IO_READ_IMAGE_PNM_HH
# define OLN_IO_READ_IMAGE_PNM_HH

# include <iostream>
# include <string>

# include <oln/io/read_image_pnm_decl.hh>
# include <oln/io/read_image_pnm_2d.hh>
# include <oln/core/abstract/image.hh>
# include <oln/io/utils.hh>

namespace oln {


  namespace io {


    namespace impl {

      template <typename V, typename I>
      void read_pnm_(const ntg::real_value<V>&,
		     abstract::image<I>& ima,
		     std::istream& istr,
		     const internal::pnm_header &hdr)
      {
	typedef oln_type_of(I, size) size_type;

	if (hdr.type == "P5")
	  {
	    read_pnm_real<I, size_type> tmp(istr, hdr);

	    tmp.run();
	    tmp.get_output(ima.exact());
	  }
	else
	  std::cerr << "io error: pnm type mismatch data type" << std::endl;
      }

      template <typename V, typename I>
      void read_pnm_(const ntg::enum_value<V>&,
		     abstract::image<I>& ima,
		     std::istream& istr,
		     const internal::pnm_header &hdr)
      {
	typedef oln_type_of(I, size) size_type;

	if (hdr.type == "P4")
	  {
	    read_pnm_bin<I, size_type> tmp(istr, hdr);

	    tmp.run();
	    tmp.get_output(ima.exact());
	  }
	else
	  std::cerr << "io error: pnm type mismatch data type" << std::endl;
      }

      template <typename V, typename I>
      void read_pnm_(const ntg::vect_value<V>&,
		     abstract::image<I>& ima,
		     std::istream& istr,
		     const internal::pnm_header &hdr)
      {
	typedef oln_type_of(I, size) size_type;

	if (hdr.type == "P6")
	  {
	    read_pnm_vect<I, size_type> tmp(istr, hdr);

	    tmp.run();
	    tmp.get_output(ima.exact());
	  }
	else
	  std::cerr << "io error: pnm type mismatch data type" << std::endl;
      }

      template <typename V, typename I>
      void read_pnm(const ntg::value<V>& v,
		    abstract::image<I>& ima,
		    std::istream& istr,
		    const internal::pnm_header &hdr)
      {
	read_pnm_(v.exact(), ima.exact(), istr, hdr);
      }

    }


    void check_hdr_ext(const internal::pnm_header& hdr,
		       const std::string& ext)
    {
      if ((ext == "pbm" && hdr.type != "P4") ||
	  (ext == "pgm" && hdr.type != "P5") ||
	  (ext == "ppm" && hdr.type != "P6"))
	std::cerr << "io warning: extension mismatch pnm type." << std::cerr;
    }



    template <typename I>
    void read_pnm(abstract::image<I>& ima,
		  std::istream& istr,
		  const std::string& ext)
    {
      typedef oln_type_of(I, value) value_type;
      typedef ntg_type(value_type) ntg_value_type;
      internal::pnm_header hdr;

      if (!internal::read_pnm_header(istr, hdr))
	{
	  std::cerr << "io error: invalid pnm header." << std::cerr;
	  return;
	}

      if (hdr.type == "P1" || hdr.type == "P2" || hdr.type == "P3")
	std::cerr << "io error: ascii pnm format not supported." << std::endl;

      check_hdr_ext(hdr, ext);

      impl::read_pnm(ntg_value_type(), ima, istr, hdr);

    }


  }


}

#endif // ! OLN_IO_READ_IMAGE_PNM_HH
