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

#ifndef OLENA_IO_PNM_READ_3D_HH_
# define OLENA_IO_PNM_READ_3D_HH_

# include <ntg/bin.hh>
# include <oln/core/image2d.hh>
# include <oln/io/pnm_read_2d.hh>

# include <list>
# include <iostream>
# include <cstdio>

namespace oln {

  namespace io {

    namespace internal {      

      /*-------------------.
      | pnm_reader3d (Any) |
      `-------------------*/

      // Only PnmRaw images can store more than one 2d image. The
      // format is simple: each slice is stored as a bi-dimensional
      // image.

      template <pnm_type P, class I>
      struct pnm_reader<ReadPnmRaw, 3, P, I>
      {
	typedef pnm_reader<ReadPnmRaw, 2, P, image2d<Value(I)> > reader_2d;
	typedef image2d<Value(I)> image2d_type;

	static std::string name()
	{ 
	  return reader_2d::name();
	}

	static bool knows_ext(const std::string& ext)
	{ 
	  return reader_2d::knows_ext(ext);
	}

	static bool read(std::istream& in, I& im)
	{
	  typedef typename std::list<image2d_type*>::iterator iterator_type;
	  std::list<image2d_type*> image2d_list;
	  bool fail = false;	  
	  coord ncols = -1, nrows = -1;
	  while (in.peek() != EOF)
	    {
	      image2d_type* new_ima = new image2d_type();
	      image2d_list.push_back(new_ima);
	      if (!reader_2d::read(in, *new_ima))
		{
		  fail = true;
		  break;
		}
	      if (ncols < 0)
		{
		  ncols = new_ima->ncols();
		  nrows = new_ima->nrows();
		}
	      else 
		if (new_ima->ncols() != ncols
		    || new_ima->nrows() != nrows)
		  {
		    fail = true;
		    break;
		  }
	    }

	  if (image2d_list.size() == 0)
	    fail = true;

	  if (!fail)
	    to_image3d_(image2d_list, im);

	  for (iterator_type it = image2d_list.begin(); 
	       it != image2d_list.end();
	       ++it)
	    delete (*it);
	  return !fail;
	}

      private:
	static void 
	to_image3d_(std::list<image2d_type*>& image2d_list, I& output)
	{
	  typedef typename std::list<image2d_type*>::iterator iterator_type;
	  unsigned slice = 0;
	  output = I(image2d_list.size(),
		     image2d_list.front()->nrows(),
		     image2d_list.front()->ncols());
	  for (iterator_type it = image2d_list.begin();
	       it != image2d_list.end();
	       ++it, ++slice)
	    {
	      Iter(image2d_type) p(*(*it));
	      for_all(p)
		output(slice, p.row(), p.col()) = (*(*it))[p];
	    }
	}
      };

    } // end of internal

  } // end of io

} // end of oln


#endif // ! OLENA_IO_PNM_READ_3D_HH_
