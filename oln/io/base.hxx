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

#ifndef OLENA_IO_BASE_HXX_
# define OLENA_IO_BASE_HXX_

# include <oln/io/base.hh>

namespace oln {

  namespace io {

    namespace internal {

      struct utils
      {	
	static std::string extension(const std::string& name)
	{ 
	  std::string ext;
	  int pos = name.rfind('.');
	  if (pos > 0)
	    {
	      ext.assign(name, pos + 1, name.size() - pos);
	      for (std::string::iterator i = ext.begin(); i != ext.end(); ++i) 
		*i = tolower(*i);
	    }
	  return ext;	
	}
      };

      // try readers

      template< reader_id R, typename T >
      struct try_in_
      {
	static bool _soft(T& output, std::istream& in, const std::string& ext)
	{	
	  if (reader<R,T>::knows_ext(ext))
	    if (reader<R,T>::read(in, output))
	      return true;
	  in.seekg(0, std::ios::beg);
	  return try_in_<reader_id(unsigned(R)-1), T>::_soft(output, in, ext);
	}

	static bool _hard(T& output, std::istream& in)
	{
	  if (reader<R,T>::read(in, output))
	    return true;
	  in.seekg(0, std::ios::beg);
	  return try_in_<reader_id(unsigned(R)-1), T>::_hard(output, in);
	}
      };

      template< typename T >
      struct try_in_<ReadNone, T>
      {
	static bool _soft(T&, std::istream&, const std::string&) { return false; }
	static bool _hard(T&, std::istream&) { return false; }
      };

      // try writers

      template< writer_id W, typename T >
      struct try_out_
      {
	static bool _soft(const T& input, std::ostream& out, const std::string& ext)
	{	
	  if (writer<W,T>::knows_ext(ext))
	    if (writer<W,T>::write(out, input))
	      return true;
	  return try_out_<writer_id(unsigned(W)-1), T>::_soft(input, out, ext);
	}

	static bool _hard(const T& input, std::ostream& out, const std::string& ext)
	{	
	  if (writer<W,T>::write(out, input))
	    {		
	      std::clog << "[ambiguous extension '" << ext 
			<< "' wrote as " << writer<W,T>::name() << "] " << std::flush;
	      return true;
	    }
	  return try_out_<writer_id(unsigned(W)-1), T>::_hard(input, out, ext);
	}
      };

      template< typename T >
      struct try_out_<WriteNone, T>
      {
	static bool _soft(const T&, std::ostream&, const std::string&) { return false; }
	static bool _hard(const T&, std::ostream&, const std::string&) { return false; }
      };


      // try wrappers      

      template< wrapper_id W, typename T >
      struct wrap_
      {
	static bool _in_soft(T& output, const std::string& name, const std::string& ext)
	{
	  if (wrapper<W>::knows_ext(ext))
	    {
	      std::string wrapped_name = name;
	      if (std::istream* in = wrapper<W>::wrap_in(wrapped_name))
		{
		  std::string wrapped_ext = utils::extension(wrapped_name);     
		  bool result = try_in_<ReadAny,T>::_soft(output, *in, wrapped_ext);
		  if (!result)
		    result = try_in_<ReadAny,T>::_hard(output, *in);
		  delete in;
		  if (result)
		    return true;
		}
	    }
	  return wrap_<wrapper_id(unsigned(W)-1),T>::_in_soft(output, name, ext);
	}

	static bool _in_hard(T& output, const std::string& name)
	{
	  std::string wrapped_name = name;
	  if (std::istream* in = wrapper<W>::wrap_in(wrapped_name))
	    {
	      std::string wrapped_ext = utils::extension(wrapped_name);     
	      bool result = try_in_<ReadAny,T>::_soft(output, *in, wrapped_ext);
	      if (!result)
		result = try_in_<ReadAny,T>::_hard(output, *in);
	      delete in;
	      if (result)
		return true;
	    }
	  return wrap_<wrapper_id(unsigned(W)-1),T>::_in_hard(output, name);
	}

	static void _find(std::list<std::string>& names, const std::string& name)
	{
	  wrapper<W>::find(names, name);
	  return wrap_<wrapper_id(unsigned(W)-1),T>::_find(names, name);
	}

	static bool _out_soft(const T& input, const std::string& name, const std::string& ext)
	{
	  if (wrapper<W>::knows_ext(ext))
	    {
	      std::string wrapped_name = name;
	      if (std::ostream* out = wrapper<W>::wrap_out(wrapped_name))
		{
		  std::string wrapped_ext = utils::extension(wrapped_name);     
		  bool result = try_out_<WriteAny,T>::_soft(input, *out, wrapped_ext);
		  delete out;
		  if (result)
		    return true;
		}
	    }
	  return wrap_<wrapper_id(unsigned(W)-1),T>::_out_soft(input, name, ext);
	}

	static bool _out_hard(const T& input, const std::string& name, const std::string& ext)
	{
	  if (wrapper<W>::knows_ext(ext))
	    {
	      std::string wrapped_name = name;
	      if (std::ostream* out = wrapper<W>::wrap_out(wrapped_name))
		{
		  std::string wrapped_ext = utils::extension(wrapped_name);     
		  bool result = try_out_<WriteAny,T>::_hard(input, *out, wrapped_ext);
		  delete out;
		  if (result)
		    return true;
		}
	    }
	  return wrap_<wrapper_id(unsigned(W)-1),T>::_out_hard(input, name, ext);
	}
      };

      template< typename T >
      struct wrap_<WrapNone,T>
      {
	static bool _in_soft(T&, const std::string&, const std::string&) { return false; }
	static bool _in_hard(T&, const std::string&) { return false; }
	static void _find(std::list<std::string>&, const std::string&) { }	  
	static bool _out_soft(const T&, const std::string&, const std::string&) { return false; }
	static bool _out_hard(const T&, const std::string&, const std::string&) { return false; }
      };

      // read anything

      template<typename T>
      bool read_any(T& output, const std::string& name)
      {	
	std::string ext = utils::extension(name);

	if (wrap_<WrapAny,T>::_in_soft(output, name, ext))
	  return true;
	if (wrap_<WrapAny,T>::_in_hard(output, name))
	  return true;
	std::list<std::string> names;
	wrap_<WrapAny,T>::_find(names, name);
	for (std::list<std::string>::iterator it = names.begin(); it !=  names.end(); ++it)
	  {
	    std::string ext = utils::extension(*it);
	    if (wrap_<WrapAny,T>::_in_soft(output, *it, ext))
	      {
		std::clog << "[" << name << " found as " << *it << "] " << std::flush;
		return true;
	      }
	  }
	for (std::list<std::string>::iterator it = names.begin(); it !=  names.end(); ++it)
	  if (wrap_<WrapAny,T>::_in_hard(output, *it))
	    {
	      std::clog << "['" << name << "' found as '" << *it << "'] " << std::flush;
	      return true;
	    }
	std::clog << "[unable to read '" << name << "'] " << std::flush;
	return false;
      }
      
      // write anything

      template< typename T >
      bool write_any(const T& input, const std::string& name)
      {
	std::string ext = utils::extension(name);

	if (wrap_<WrapAny,T>::_out_soft(input, name, ext))
	  return true;
	if (wrap_<WrapAny,T>::_out_hard(input, name, ext))
	  return true;
	std::clog << "[unable to write '" << name << "'] " << std::flush;
	return false;
      }


    } // end of internal

  } // end of io

} // end of oln


#endif // OLENA_IO_BASE_HXX_





