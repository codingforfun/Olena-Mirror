#ifndef OLENA_IO_STREAM_WRAPPER_HH_
# define OLENA_IO_STREAM_WRAPPER_HH_

# include <oln/config/system.hh>

# include <oln/io/utils.hh>

# include <list>
# include <string>

namespace oln {

  namespace io {

    namespace internal {

      // stream wrappers
    
      enum stream_id { StreamNone = 0,
		       StreamFile = 1,
		       StreamGz   = 2,
		       StreamAny  = 2 };
    
      template< stream_id W >
      struct stream_wrapper
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }
      
	static bool knows_ext(const std::string&) { return false; }
      
	static std::istream* wrap_in(std::string&) { return 0; }
	static std::ostream* wrap_out(std::string&) { return 0; }
      
	static void find(std::list<std::string>&, const std::string&) {}
      };

      /*---------------------------.
      | stream_wrappers_find_files |
      `---------------------------*/
    
      // Find files compatible with the given root, extension free filename

      template <stream_id W>
      struct stream_wrappers_find_files
      {
	static void doit(std::list<std::string>& names, const std::string& name)
	{
	  stream_wrapper<W>::find(names, name);
	  return stream_wrappers_find_files<stream_id(unsigned(W)-1)>
                   ::doit(names, name);
	}
      };

      // End of the loop

      template <>
      struct stream_wrappers_find_files<StreamNone>
      {
	static void doit(std::list<std::string>& names, 
			 const std::string& name) {}
      };

      /*-----------------------.
      | try_stream_wrappers_in |
      `-----------------------*/

      // Try stream wrappers recursively   
      // On each potential stream format, try to read using Reader

      template<stream_id W, typename T, class Reader>
      struct try_stream_wrappers_in
      {
	static bool by_extension(T& output, 
				 const std::string& name, 
				 const std::string& ext)
	{
	  if (stream_wrapper<W>::knows_ext(ext))
	    {
	      std::string wrapped_name = name;
	      if (std::istream* in = stream_wrapper<W>::wrap_in(wrapped_name))
		{
		  std::string wrapped_ext = utils::extension(wrapped_name);
		  bool result = Reader::doit(output, *in, wrapped_ext);
		  delete in;
		  if (result)
		    return true;
		}
	    }
	  return try_stream_wrappers_in<stream_id(unsigned(W)-1),T,Reader>
                   ::by_extension(output, name, ext);
	}

	// FIXME: it sounds strange to read wrapped file without
	// matching its extension.
	static bool by_data(T& output, const std::string& name)
	{
	  std::string wrapped_name = name;
	  if (std::istream* in = stream_wrapper<W>::wrap_in(wrapped_name))
	    {
	      std::string wrapped_ext = utils::extension(wrapped_name);
	      bool result = Reader::doit(output, *in, wrapped_ext);
	      delete in;
	      if (result)
		return true;
	    }
	  return try_stream_wrappers_in<stream_id(unsigned(W)-1), T, Reader>
	           ::by_data(output, name);
	}
      };

      // End of loop
    
      template< typename T, class Reader >
      struct try_stream_wrappers_in<StreamNone, T, Reader>
      {
	static bool by_extension(T&, const std::string&, const std::string&)
	{ return false; }

	static bool by_data(T&, const std::string&) { return false; }
      };

      /*------------------------.
      | try_stream_wrappers_out |
      `------------------------*/

      // Try stream wrappers recursively
      // On each potential stream format, try to write using the given Writer

      template<stream_id W, typename T, class Writer>
      struct try_stream_wrappers_out
      {
	static bool by_extension(const T& input, 
				 const std::string& name, 
				 const std::string& ext)
	{
	  if (stream_wrapper<W>::knows_ext(ext))
	    {
	      std::string wrapped_name = name;
	      std::ostream* out = stream_wrapper<W>::wrap_out(wrapped_name);
	      if (out)
		{
		  std::string wrapped_ext = utils::extension(wrapped_name);
		  bool result = Writer::doit(input, *out, wrapped_ext);
		  delete out;
		  if (result)
		    return true;
		}
	    }
	  return try_stream_wrappers_out<stream_id(unsigned(W)-1),T,Writer>
                   ::by_extension(input, name, ext);
	}
      };

      // End of loop
    
      template< typename T, class Reader >
      struct try_stream_wrappers_out<StreamNone, T, Reader>
      {
	static bool by_extension(const T&, 
				 const std::string&, 
				 const std::string&) 
	{ return false; }
      };

    } // end of internal

  } // end of io

} // end of oln

#endif // ! OLENA_IO_STREAM_WRAPPER_HH_
