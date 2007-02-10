// Copyright (C) 2001, 2002, 2003, 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_IO_PNM_READ_DATA_HH
# define OLENA_IO_PNM_READ_DATA_HH

# include <oln/core/image.hh>
# include <oln/io/image_base.hh>
# include <oln/io/pnm_common.hh>

# include <iostream>

namespace oln {

  namespace io {

    namespace internal {

      
      /*! \class pnm_read_data
      **
      ** Default version.
      */
      
      template<pnm_type V, reader_id R>
      struct pnm_read_data
      {
	template <class I>
	static bool
	read(std::istream&, I&, const pnm2d_info&)
	{ 
	  return false; 
	}
      }; 

      /*---------------------.
      | pnm read data binary |
      `---------------------*/

      /*! \class pnm_read_data<PnmBinary, ReadPnmPlain>
      **
      ** Specialized version for extracting pnm binary 2d image
      ** in plain file.
      **
      ** \todo FIXME: implement an iterator over data
      */

      template <>
      struct pnm_read_data<PnmBinary, ReadPnmPlain>
      {
	template <class I>
	static bool
	read(std::istream& in, I& output, const pnm2d_info&)
	{
	  // FIXME: implement an iterator over data
	  oln_iter_type(I) it (output);
	  
	  for (it = begin; it != end; ++it)
	    {
	      unsigned char c;
	      do
		{
		  c = in.get();
		}
	      while ((c != '0') && (c != '1'));
	      output[it] = (c == '0');
	    }
	  return true;
	}
      };
      
      /*! \class pnm_read_data<PnmBinary, ReadPnmRaw>
      **
      ** Specialized version for extracting pnm binary 2d image
      ** in raw file.
      */
      
      template <>
      struct pnm_read_data<PnmBinary, ReadPnmRaw>
      {
	template <class I>
	static bool
	read(std::istream& in, I& output, const pnm2d_info& info)
	{
	  oln_iter_type(I) it(output);
	  
	  coord cols = 0;
	  unsigned bits = 0;
	  unsigned char c = 0;
	  
	  for (it = begin; it != end; ++it)
	    {
	      if (bits == 0)
		{
		  c = in.get();
		  bits = 8;
		}
	      bool b = (c & (1 << --bits)) ? false : true;
	      output[it] = b;
	      if (++cols >= info.cols)
		c = cols = bits = 0;
	    }
	  return true;
	}
      };

      /*----------------------.
      | pnm read data integer |
      `----------------------*/
        
      /*! \class pnm_read_data<PnmInteger, ReadPnmPlain>
      **
      ** Specialized version for extracting pnm integer 2d image
      ** in plain file.
      **
      ** \todo FIXME: implement an iterator over data
      */
      
      
      template <>
      struct pnm_read_data<PnmInteger, ReadPnmPlain>
      {
	template <class I>
	static bool
	read(std::istream& in, I& output, const pnm2d_info&)
	{
	  // FIXME: implement an iterator over data
	  oln_iter_type(I) it (output);
	  
	  int c;
	  for (it = begin; it != end; ++it)
	    {
	      in >> c;
	      output[it] = c;
	    }
	  return true;
	}
      };
      
          
      /*! \class pnm_read_data<PnmInteger, ReadPnmRaw>
      **
      ** Specialized version for extracting pnm integer 2d image
      ** in raw file.
      **
      ** \todo FIXME: implement an iterator over data
      */
      
      template <>
      struct pnm_read_data<PnmInteger, ReadPnmRaw>
      {
	template <class I>
	static bool
	read(std::istream& in, I& output, const pnm2d_info& info)
	{
	  // FIXME: implement an iterator over data
	  oln_iter_type(I) it (output);
	  
	  for (it = begin; it != end; ++it)
	    {
	      if (info.max_val <= 255)
		{
		  unsigned char c;
		  in.read((char*) &c, 1);
		  output[it] = c;
		}
	      else
		{
		  unsigned short i;
		  in.read((char*) &i, 2);
		  output[it] = i;
		}
	    }
	  return true;
	}
      };

      /*------------------------.
      | pnm read data vectorial |
      `------------------------*/

          
      /*! \class pnm_read_data<PnmVectorial, ReadPnmPlain>
      **
      ** Specialized version for extracting pnm vectorial 2d image
      ** in plain file.
      **
      ** \todo FIXME: implement an iterator over data
      */
      
      template <>
      struct pnm_read_data<PnmVectorial, ReadPnmPlain>
      {
	template <class I>
	static bool
	read(std::istream& in, I& output, const pnm2d_info&)
	{
	  // FIXME: implement an iterator over data
	  oln_iter_type(I) it (output);
	  
	  for (it = begin; it != end; ++it)
	    {
	      int tmp;
	      in >> tmp; output[it][0] = tmp;
	      in >> tmp; output[it][1] = tmp;
	      in >> tmp; output[it][2] = tmp;
	    }	  
	  return true;
	}
      };

          
      /*! \class pnm_read_data<PnmInteger, ReadPnmRaw>
      **
      ** Specialized version for extracting pnm integer 2d image
      ** in raw file.
      **
      ** \todo FIXME: implement an iterator over data
      */
      
      template <>
      struct pnm_read_data<PnmVectorial, ReadPnmRaw>
      {
	template <class I>
	static bool
	read(std::istream& in, I& output, const pnm2d_info& info)
	{
	  // FIXME: implement an iterator over data
	  oln_iter_type(I) it (output);
	  
	  for (it = begin; it != end; ++it)
	    {
	      if (info.max_val <= 255)
		{
		  unsigned char c[3];
		  in.read((char*) c, 3);
		  output[it][0] = c[0];
		  output[it][1] = c[1];
		  output[it][2] = c[2];
		}
	      else
		{
		  precondition(sizeof(unsigned short) == 2);
		  unsigned short i[3];
		  in.read((char*) i, 3 * sizeof(unsigned short));
		  output[it][0] = i[0];
		  output[it][1] = i[1];
		  output[it][2] = i[2];
		}
	    }
	  return true;
	}
      };      

    } // end of namespace internal
    
  } // end of namespace io
  
} // end of namespace oln

#endif // ! OLENA_IO_PNM_READ_DATA_HH
