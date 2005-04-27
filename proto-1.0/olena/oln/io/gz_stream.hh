// Copyright (C) 2001, 2002, 2003, 2004, 2005  EPITA Research and Development Laboratory
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

#ifndef OLN_IO_GZ_STREAM_HH
# define OLN_IO_GZ_STREAM_HH

// #  include <oln/config/system.hh>

// #if defined HAVE_ZLIB && HAVE_ZLIB == 1
// #  include <cassert>
// #  include <fstream>
// #  include <string>
// #  include <zlib.h>

// namespace oln {

//   namespace io {

//     /// Functions for gz files
//     namespace gz {


//       ///Performs operation on compressed files.
//       class zfilebuf : public std::streambuf
//       {
//       public:


// 	zfilebuf() : file(0),  mode(0),  own_file_descriptor(0)
// 	{
// 	  inbuf = new char[lenbuf];
// 	  outbuf = new char[lenbuf];
// 	  setg(0, 0, 0);
// 	  setp(outbuf, outbuf + lenbuf);
// 	}

// 	virtual ~zfilebuf()
// 	{
// 	  sync();
// 	  delete[] inbuf;
// 	  delete[] outbuf;
// 	  if (own_file_descriptor)
// 	    close();
// 	}


// 	/*! \brief Return a stream on the file \a name regarding
// 	** the opening mode: \a io_mode.
// 	*/
// 	zfilebuf*
// 	open(const char *name, int io_mode)
// 	{
// 	  if (is_open())
// 	    return 0;

// 	  char char_mode[10];
// 	  char *p;
// 	  memset(char_mode,'\0',10);
// 	  p = char_mode;

// 	  if (io_mode & std::ios::in)
// 	    {
// 	      mode = std::ios::in;
// 	      *p++ = 'r';
// 	    }
// 	  else
// 	    if (io_mode & std::ios::app)
// 	      {
// 		mode = std::ios::app;
// 		*p++ = 'a';
// 	      }
// 	    else
// 	      {
// 		mode = std::ios::out;
// 		*p++ = 'w';
// 	      }
// 	  if (io_mode & std::ios::binary)
// 	    {
// 	      mode |= std::ios::binary;
// 	      *p++ = 'b';
// 	    }

// 	  // Hard code the compression level
// 	  if (io_mode & (std::ios::out|std::ios::app))
// 	    *p++ = '9';

// 	  if ((file = gzopen(name, char_mode)) == 0)
// 	    return 0;

// 	  name_ = name;
// 	  own_file_descriptor = 1;
// 	  return this;
// 	}

// 	/*! \brief Attach a stream on \a file_descriptor regarding
// 	** the opening mode: \a io_mode.
// 	*/
// 	zfilebuf*
// 	attach(int file_descriptor, int io_mode)
// 	{
// 	  if (is_open())
// 	    return 0;

// 	  char char_mode[10];
// 	  char *p;
// 	  memset(char_mode,'\0',10);
// 	  p = char_mode;

// 	  if (io_mode & std::ios::in)
// 	    {
// 	      mode = std::ios::in;
// 	      *p++ = 'r';
// 	    }
// 	  else
// 	    if (io_mode & std::ios::app)
// 	      {
// 		mode = std::ios::app;
// 		*p++ = 'a';
// 	      }
// 	    else
// 	      {
// 		mode = std::ios::out;
// 		*p++ = 'w';
// 	      }

// 	  if (io_mode & std::ios::binary)
// 	    {
// 	      mode |= std::ios::binary;
// 	      *p++ = 'b';
// 	    }

// 	  // Hard code the compression level
// 	  if (io_mode & (std::ios::out|std::ios::app))
// 	    *p++ = '9';

// 	  if ((file = gzdopen(file_descriptor, char_mode)) == 0)
// 	    return 0;

// 	  own_file_descriptor = 0;
// 	  return this;
// 	}

// 	/// Close the stream.
// 	zfilebuf*
// 	close()
// 	{
// 	  if (is_open())
// 	    {
// 	      sync();
// 	      gzclose(file);
// 	      file = 0;
// 	    }
// 	  return this;
// 	}


// 	int
// 	setcompressionlevel(short comp_level)
// 	{
// 	  return gzsetparams(file, comp_level, -2);
// 	}

// 	int
// 	setcompressionstrategy(short comp_strategy)
// 	{
// 	  return gzsetparams(file, -2, comp_strategy);
// 	}

// 	/// Return true if the stream is open, false otherwise.
// 	inline int
// 	is_open() const
// 	{ return (file != 0); }

// 	virtual std::streampos
// 	seekoff(std::streamoff off, std::ios::seekdir dir, int) // which)
// 	{
// 	  return std::streampos(gzseek(file, off, dir));
// 	}

// 	/// Flush the buffer associated to the stream.
// 	virtual int
// 	sync()
// 	{
// 	  if (!is_open())
// 	    return EOF;
// 	  return flushbuf();
// 	}

//       protected:

// 	/*! \brief Return the next character in the stream.
// 	** On failure, \a EOF is returned.
// 	*/
// 	virtual int
// 	underflow()
// 	{
// 	  // If the file hasn't been opened for reading, error.
// 	  if (!is_open() || !(mode & std::ios::in))
// 	    return EOF;

// 	  if (in_avail())
// 	    return (unsigned char) *gptr();

// 	  if (flushbuf() == EOF)
// 	    return EOF;

// 	  // Attempt to fill the buffer.
// 	  if (fillbuf() == EOF)
// 	    return EOF;

// 	  assert (eback());

// 	  return (unsigned char) *gptr();
// 	}

// 	/*! \brief Flush the output buffer associated to the stream
// 	** then write \a c. On failure, \a EOF is returned.
// 	*/

// 	virtual int
// 	overflow(int c = EOF)
// 	{
// 	  if (!is_open() || !(mode & std::ios::out))
// 	    return EOF;

// 	  assert (pbase());

// 	  if (flushbuf() == EOF)
// 	    return EOF;

// 	  if (c != EOF)
// 	    {
// 	      *pptr() = c;
// 	      pbump(1);
// 	    }
// 	  return 0;
// 	}

//       private:

// 	gzFile file;
// 	short mode;
// 	short own_file_descriptor;
// 	std::string name_;
// 	char *inbuf;
// 	char *outbuf;
// 	static const int lenbuf = 16 * 1024;

// 	/// Flush the output buffer
// 	int
// 	flushbuf()
// 	{
// 	  int n = pptr() - outbuf;

// 	  if (n == 0)
// 	    return 0;

// 	  if (gzwrite(file, outbuf, n) < n)
// 	    return EOF;

// 	  setp(outbuf, outbuf + lenbuf);
// 	  return 0;
// 	}
// 	/// Fill the input buffer.
// 	int
// 	fillbuf()
// 	{
// 	  int t = gzread(file, inbuf, lenbuf);
// 	  if (t <= 0) return EOF;
// 	  setg(inbuf, inbuf, inbuf + t);
// 	  return t;
// 	}

//       };

//       ///Define an interface for compressed file stream manipulation.
//       class zfilestream_common : virtual public std::ios
//       {
// 	friend class zifstream;
// 	friend class zofstream;
// 	friend zofstream &setcompressionlevel(zofstream &, int);
// 	friend zofstream &setcompressionstrategy(zofstream &, int);

//       public:
// 	virtual ~zfilestream_common() {}

// 	/*! \brief Open the stream on the file descriptor:
// 	** \a fd regarding the opening mode.
// 	*/
// 	void
// 	attach(int fd, int io_mode)
// 	{
// 	  if (!buffer.attach(fd, io_mode))
// 	    clear(std::ios::failbit | std::ios::badbit);
// 	  else
// 	    clear();
// 	}

// 	/*! \brief Open the stream on the file named \a name
// 	** regarding the opening mode.
// 	*/
// 	void
// 	open(const char *name, int io_mode)
// 	{
// 	  if (!buffer.open(name, io_mode))
// 	    clear(std::ios::failbit | std::ios::badbit);
// 	  else
// 	    clear();
// 	}

// 	/// Close the current stream.
// 	void
// 	close()
// 	{
// 	  if (!buffer.close())
// 	    clear(std::ios::failbit | std::ios::badbit);
// 	}

// 	bool
// 	is_open()
// 	{
// 	  return buffer.is_open();
// 	}


//       protected:
// 	/// Prevent instantiation.
// 	zfilestream_common() :  std::ios(zfilestream_common::rdbuf())
// 	{ }


//       private:
// 	zfilebuf*
// 	rdbuf()
// 	{
// 	  return &buffer;
// 	}

// 	zfilebuf buffer;
//       };


//       /// Read only zstream.
//       class zifstream : public zfilestream_common, public std::istream
//       {
//       public:

// 	zifstream() : std::istream(zfilestream_common::rdbuf())
// 	{
// 	  clear(std::ios::badbit);
// 	}

// 	/// Open a read only stream on the file named \a name.
// 	zifstream(const char *name, int io_mode = std::ios::in) :
// 	  std::istream(zfilestream_common::rdbuf())
// 	{
// 	  zfilestream_common::open(name, io_mode);
// 	}

// 	/// Open a read only stream on the file descriptor \a fd.
// 	zifstream(int fd, int io_mode = std::ios::in) :
// 	  std::istream(zfilestream_common::rdbuf())
// 	{
// 	  zfilestream_common::attach(fd, io_mode);
// 	}

// 	virtual ~zifstream() {}
//       };

//       class zofstream : public zfilestream_common, public std::ostream {

//       public:

// 	zofstream() : std::ostream(zfilestream_common::rdbuf())
// 	{
// 	  clear(std::ios::badbit);
// 	}

// 	/// Open a write only stream on the file named \a name.
// 	zofstream(const char *name, int io_mode = std::ios::out) :
// 	  std::ostream(zfilestream_common::rdbuf())
// 	{
// 	  zfilestream_common::open(name, io_mode);
// 	}

// 	/// Open a write only stream on the file descriptor \a fd.
// 	zofstream(int fd, int io_mode = std::ios::out) :
// 	  std::ostream(zfilestream_common::rdbuf())
// 	{
// 	  zfilestream_common::attach(fd, io_mode);
// 	}

// 	virtual ~zofstream() {}

//       };

//       // Forward declaration.
//       template <class T>
//       class zomanip;

//       /// Apply a function on \a s via the operator <<.
//       template <class T>
//       zofstream&
//       operator<<(zofstream &s, const zomanip<T> &m) {
// 	return (*m.func)(s, m.val);
//       }


//       /// Define a pair func / val to perform manipulation on zofstream.
//       template<class T> class zomanip
//       {
// 	friend zofstream &operator<< <T>(zofstream &, const zomanip<T> &);
//       public:
// 	zomanip(zofstream &(*f)(zofstream &, T), T v) : func(f), val(v) { }
//       private:
// 	zofstream &(*func)(zofstream &, T);
// 	T val;
//       };


//       /// Set the compression level of \a s to \a l.
//       inline zofstream&
//       setcompressionlevel(zofstream &s, int l) {
// 	(s.rdbuf())->setcompressionlevel(l);
// 	return s;
//       }

//       /// Set the compression strategy of \a s to \a l.
//       inline zofstream&
//       setcompressionstrategy(zofstream &s, int l)
//       {
// 	(s.rdbuf())->setcompressionstrategy(l);
// 	return s;
//       }

//       /// Specialized version for zomanip<int>
//       inline zomanip<int>
//       setcompressionlevel(int l)
//       {
// 	return zomanip<int>(&setcompressionlevel,l);
//       }

//       /// Specialized version for zomanip<int>
//       inline zomanip<int>
//       setcompressionstrategy(int l)
//       {
// 	return zomanip<int>(&setcompressionstrategy,l);
//       }

//     } // end of namespace gz

//   } // end of namespace io

// } // end of namespace oln

// # endif // ! HAVE_ZLIB

#endif // ! OLN_IO_GZ_STREAM_HH
