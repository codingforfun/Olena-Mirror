// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_IO_PNM_WRITE_DATA_HH_
# define OLENA_IO_PNM_WRITE_DATA_HH_

# include <oln/core/image.hh>
# include <oln/io/image_base.hh>
# include <oln/io/pnm_common.hh>

# include <iostream>

namespace oln {

  namespace io {

    namespace internal {

      template<pnm_type V, writer_id R>
      struct pnm_write_data
      {
	template <class I>
	static bool
	write(std::ostream&, const I&, const pnm2d_info&)
	{ return false; }
      };

      /*----------------------.
      | pnm write data binary |
      `----------------------*/

      template <>
      struct pnm_write_data<PnmBinary, WritePnmPlain>
      {
	template <class I>
	static bool
	write(std::ostream& out, const I& input, const pnm2d_info&)
	{
	  // FIXME: implement an iterator over data
	  Iter(I) it(input);
	  
	  unsigned stride = 0;
	  for (it = begin; it != end; ++it)
	    {
	      bool b = input[it];
	      
	      out.put(b == true ? '0' : '1');
	      if (++stride >= 70)
		{
		  out << std::endl;
		  stride = 0;
		}
	    }
	  out << std::endl;
	  return true;
	}
      };

      template <>
      struct pnm_write_data<PnmBinary, WritePnmRaw>
      {
	template <class I>
	static bool
	write(std::ostream& out, const I& input, const pnm2d_info& info)
	{
	  // FIXME: implement an iterator over data
	  Iter(I) it(input);

	  coord cols = 0;
	  unsigned char c = 0;
	  coord stride = 0;
	  for (it = begin; it != end; ++it)
	    {
	      c <<= 1;
	      c += (input[it] == true ? 0 : 1);
	      if (++cols >= info.cols)
		{
		  c <<= (8 - stride - 1);
		  out << c;
		  c = cols = stride = 0;
		}
	      else
		if (++stride >= 8)
		  {
		    out << c;
		    c = stride = 0;
		  }
	    }
	  postcondition(stride == 0);
	  return true;
	}
      };

      /*-----------------------.
      | pnm write data integer |
      `-----------------------*/

      template <>
      struct pnm_write_data<PnmInteger, WritePnmPlain>
      {
	template <class I>
	static bool
	write(std::ostream& out, const I& input, const pnm2d_info& info)
	{
	  // FIXME: implement an iterator over data
	  Iter(I) it(input);
	  
	  int stride = 0;
	  for (it = begin; it != end; ++it)
	    {
	      int i = input[it];
	      out << i;
	      if (++stride >= info.cols)
		{
		  out << std::endl;
		  stride = 0;
		}
	      else
		out << ' ';
	    }
	  return true;
	}
      };

      template <>
      struct pnm_write_data<PnmInteger, WritePnmRaw>
      {
	template <class I>
	static bool
	write(std::ostream& out, const I& input, const pnm2d_info& info)
	{
	  // FIXME: implement an iterator over data
	  Iter(I) it (input);
	  
	  for (it = begin; it != end; ++it)
	    {
	      if (info.max_val <= 255)
		{
		  unsigned char c = input[it];
		  out.write((char*) &c, 1);
		}
	      else
		{
		  unsigned short i = input[it];
		  out.write((char*) &i, 2);
		}
	    }
	  return true;
	}
      };

      /*-------------------------.
      | pnm write data vectorial |
      `-------------------------*/

      template <>
      struct pnm_write_data<PnmVectorial, WritePnmPlain>
      {
	template <class I>
	static bool
	write(std::ostream& out, const I& input, const pnm2d_info& info)
	{
	  // FIXME: implement an iterator over data
	  Iter(I) it(input);
	  
	  int stride = 0;
	  for (it = begin; it != end; ++it)
	    {
	      int tmp;
	      tmp = input[it][0]; out << tmp << ' ';
	      tmp = input[it][1]; out << tmp << ' ';
	      tmp = input[it][2]; out << tmp << ' ';
	      
	      if (++stride >= info.cols)
		{
		  out << std::endl;
		  stride = 0;
		}
	      else
		out << ' ';
	    }
	  return true;
	}
      };	

      template <>
      struct pnm_write_data<PnmVectorial, WritePnmRaw>
      {
	template <class I>
	static bool
	write(std::ostream& out, const I& input, const pnm2d_info& info)
	{
	  // FIXME: implement an iterator over data
	  Iter(I) it (input);
	  
	  for (it = begin; it != end; ++it)
	    {
	      if (info.max_val <= 255)
		{
		  unsigned char c[3];
		  c[0] = input[it][0];
		  c[1] = input[it][1];
		  c[2] = input[it][2];
		  out.write((char*) c, 3);
		}
	      else
		{
		  precondition(sizeof(unsigned short) == 2);
		  unsigned short i[3];
		  i[0] = input[it][0];
		  i[1] = input[it][1];
		  i[2] = input[it][2];
		  out.write((char*) i, 3 * sizeof(unsigned short));
		}
	    }
	  return true;
	}
      }; 
      
    } // end of internal
    
  } // end of io
  
} // end of oln

#endif // ! OLENA_IO_PNM_WRITE_DATA_HH_
