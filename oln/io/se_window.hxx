// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_IO_SE_WINDOW_HXX_
# define OLENA_IO_SE_WINDOW_HXX_

# include <oln/io/pnm.hh>

namespace oln {
  namespace io {
    namespace internal {


      template<>
      struct reader<ReadPnmPlain, window2d >
      {
	static const std::string& name()
	{ static const std::string _name("win2d/P1"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppbm"; }

	static bool read(std::istream& in, window2d& win)
	{
	  image2d<bin> im;
	  if (!reader<ReadPnmPlain, image2d<bin> >::read(in, im))
	    return false;
	  if (!(im.ncols() % 2) || !(im.nrows() % 2))
	    {
	      std::clog << "[both image dimensions have to be odd for a window2d]" << std::flush;
	      return false;
	    }
	  image2d<bin>::iter it(im);
	  window2d w;
	  for_all (it)
	    if (im[it])
	      w.add(dpoint2d(it) - dpoint2d(im.nrows()/2, im.ncols()/2));
	  win = w;
	  return true;
	}
      };

      template<>
      struct reader<ReadPnmRaw, window2d >
      {
	static const std::string& name()
	{ static const std::string _name("win2d/P4"); return _name;	}

	static bool knows_ext(const std::string& ext)
	{ return ext == "pbm"; }

	static bool read(std::istream& in, window2d& win)
	{
	  image2d<bin> im;
	  if (!reader<ReadPnmRaw, image2d<bin> >::read(in, im))
	    return false;
	  if (!(im.ncols() % 2) || !(im.nrows() % 2))
	    {
	      std::clog << "[both image dimensions have to be odd for a window2d]" << std::flush;
	      return false;
	    }
	  image2d<bin>::iter it(im);
	  window2d w;
	  for_all (it)
	    if (im[it])
	      w.add(dpoint2d(it) - dpoint2d(im.nrows()/2, im.ncols()/2));
	  win = w;
	  return true;
	}
      };

      template<>
      struct writer<WritePnmPlain, window2d >
      {
	static const std::string& name()
	{ static const std::string _name("win2d/P1"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "ppbm"; }

	static bool write(std::ostream& out, const window2d& win)
	{
	  image2d<bin> im(win.delta()*2+1,win.delta()*2+1);
	  image2d<bin>::iter it(im);
	  for_all (it) im[it] = false;
	  for (unsigned i = 0; i < win.card(); ++i)
	    im[point2d(win.delta(),win.delta()) + win.dp(i)] = true;
	  if (!writer<WritePnmPlain, image2d<bin> >::write(out, im))
	    return false;
	  return true;
	}
      };

      template<>
      struct writer<WritePnmRaw, window2d >
      {
	static const std::string& name()
	{ static const std::string _name("win2d/P4"); return _name; }

	static bool knows_ext(const std::string& ext)
	{ return ext == "pbm"; }

	static bool write(std::ostream& out, const window2d& win)
	{
	  image2d<bin> im(win.delta()*2+1,win.delta()*2+1);
	  image2d<bin>::iter it(im);
	  for_all (it) im[it] = false;
	  for (unsigned i = 0; i < win.card(); ++i)
	    im[point2d(win.delta(),win.delta()) + win.dp(i)] = true;
	  if (!writer<WritePnmRaw, image2d<bin> >::write(out, im))
	    return false;
	  return true;
	}
      };


    } // internal
  } // io
} // oln



#endif // OLENA_IO_SE_WINDOW_HXX_
