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

#ifndef OLN_IO_WRITE_IMAGE_PNM_HH
# define OLN_IO_WRITE_IMAGE_PNM_HH

# include <oln/io/write_image_pnm_decl.hh>
# include <oln/io/write_image_pnm_2d.hh>

namespace oln {


  namespace io {


    namespace impl {


      template <typename V, typename I>
      void write_pnm_(const ntg::real_value<V>&,
		      const abstract::image<I>& ima,
		      std::ostream& istr,
		      const std::string& ext)
      {
	typedef oln_type_of(I, size) size_type;

	if (ext == "pgm")
	  {
	    write_pnm_real<I, size_type> tmp(ima, istr);

	    tmp.run();
	  }
	else
	  std::cerr << "io error: extension mismatch data type" << std::endl;
      }

      template <typename V, typename I>
      void write_pnm_(const ntg::enum_value<V>&,
		      const abstract::image<I>& ima,
		      std::ostream& ostr,
		      const std::string& ext)
      {
	typedef oln_type_of(I, size) size_type;

	if (ext == "pbm")
	  {
	    write_pnm_bin<I, size_type> tmp(ima, ostr);

	    tmp.run();
	  }
	else
	  std::cerr << "io error: extension mismatch data type" << std::endl;
      }

      template <typename V, typename I>
      void write_pnm_(const ntg::vect_value<V>&,
		      const abstract::image<I>& ima,
		      std::ostream& ostr,
		      const std::string& ext)
      {
	typedef oln_type_of(I, size) size_type;
	precondition(ntg_nb_comp(V) == 3);

	if (ext == "ppm")
	  {
	    write_pnm_vect<I, size_type> tmp(ima, ostr);

	    tmp.run();
	  }
	else
	  std::cerr << "io error: extension mismatch data type" << std::endl;
      }

      template <typename V, typename I>
      void write_pnm(const ntg::value<V>& v,
		     const abstract::image<I>& ima,
		     std::ostream& ostr,
		     const std::string& ext)
      {
	write_pnm_(v.exact(), ima.exact(), ostr, ext);
      }

    }


    template <typename V>
    std::string get_pnm_type(const ntg::enum_value<V>&)
    {
      return "P4";
    }

    template <typename V>
    std::string get_pnm_type(const ntg::real_value<V>&)
    {
      return "P5";
    }

    template <typename V>
    std::string get_pnm_type(const ntg::vect_value<V>&)
    {
      return "P6";
    }

    template <typename V>
    int get_max_val(const ntg::value<V>&)
    {
      return ntg_max_val(V) - ntg_min_val(V);
    }

    template <typename V>
    int get_max_val(const ntg::vect_value<V>&)
    {
      return ntg_max_val(ntg_comp_type(V)) - ntg_min_val(ntg_comp_type(V));
    }


    template <typename I>
    void write_pnm(const abstract::image<I>& ima,
		   std::ostream& ostr,
		   const std::string& ext)
    {
      typedef oln_type_of(I, value) value_type;
      typedef ntg_type(value_type) ntg_value_type;
      int max_val = get_max_val(ntg_value_type());

      if (max_val <= 65535)
	{
	  internal::write_pnm_header(ostr, get_pnm_type(ntg_value_type()),
				     ima.exact().size().ncols(),
				     ima.exact().size().nrows(),
				     max_val);

	  impl::write_pnm(ntg_value_type(), ima.exact(), ostr, ext);
	}
      else
	std::cerr << "io error: data type too large" << std::endl;


    }


  }


}



# endif // ! OLN_IO_WRITE_IMAGE_PNM_HH
