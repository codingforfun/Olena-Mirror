#ifndef OLENA_IO_IMAGE_READ_HH_
# define OLENA_IO_IMAGE_READ_HH_

# include <oln/config/system.hh>
# include <oln/core/abstract/image.hh>

# include <oln/io/image_base.hh>
# include <oln/io/pnm_read.hh>
# include <oln/io/stream_wrapper.hh>
# include <oln/io/utils.hh>

# include <list>
# include <iostream>

namespace oln {

  namespace io {

    namespace internal {      

      /*------------.
      | try_readers |
      `------------*/

      // Try readers recursively

      template< reader_id R, typename T >
      struct try_readers
      {
	// Try to deduce the file format from the extension
	static bool by_extension(T& output, std::istream& in, 
				 const std::string& ext)
	{
	  if (reader<R,T>::knows_ext(ext))
	    if (reader<R,T>::read(in, output))
	      return true;
	  in.seekg(0, std::ios::beg);
	  return try_readers<reader_id(unsigned(R)-1), T>
	           ::by_extension(output, in, ext);
	}

	// Try to match the file format referring to the data only
	static bool by_data(T& output, std::istream& in)
	{
	  if (reader<R,T>::read(in, output))
	    return true;
	  in.seekg(0, std::ios::beg);
	  return try_readers<reader_id(unsigned(R)-1), T>::by_data(output, in);
	}
      };

      // End of try_readers loop

      template< typename T >
      struct try_readers<ReadNone, T>
      {
	static bool by_extension(T&, std::istream&, const std::string&) 
	{ return false; }
	static bool by_data(T&, std::istream&) { return false; }
      };

      /*----------------------.
      | read(image, filename) |
      `----------------------*/

      // Readers trier functor, helper for stream_wrappers

      struct readers_trier
      {
	template <class T>
	static bool doit(T& output, std::istream& in, const std::string ext)
	{	  
	  bool result = try_readers<ReadAny,T>::by_extension(output, in, ext);
	  if (!result)
	    result = try_readers<ReadAny,T>::by_data(output, in);
	  return result;
	}
      };

      // Read images

      template <class E>
      inline bool
      read(abstract::image<E>& output, const std::string& name)
      {
	// FIXME: E or abstract::image<E> ?
	// E is convenient.
	typedef try_stream_wrappers_in<StreamAny, E, readers_trier> 
	  stream_trier;

	std::string ext = internal::utils::extension(name);
	
	if (stream_trier::by_extension(output.exact(), name, ext))
	  return true;
	if (stream_trier::by_data(output.exact(), name))
	  return true;

	// No valid file found using name directly. Let's try other
	// possible filenames based on name.

	std::list<std::string> names;
	stream_wrappers_find_files<StreamAny>::doit(names, name);

	for (std::list<std::string>::iterator it = names.begin(); 
	     it !=  names.end(); 
	     ++it)
	  {
	    std::string ext = utils::extension(*it);
	    if (stream_trier::by_extension(output.exact(), *it, ext))
	      {
		std::clog << "[" << name << " found as " << *it << "] " 
			  << std::flush;
		return true;
	      }
	  }
	for (std::list<std::string>::iterator it = names.begin(); 
	     it !=  names.end(); 
	     ++it)
	  if (stream_trier::by_data(output.exact(), *it))
	    {
	      std::clog << "['" << name << "' found as '" << *it << "'] " 
			<< std::flush;
	      return true;
	    }
	std::clog << "[unable to read '" << name << "'] " << std::endl;
	return false;
      }

    } // end of internal
    
  } // end of io
  
} // end of oln

#endif // ! OLENA_IO_IMAGE_READ_HH_
