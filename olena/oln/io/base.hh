
#ifndef MILENA_IO_BASE_HH_
# define MILENA_IO_BASE_HH_

# include <list>
# include <iostream>

# include "io/readable.hh"

namespace mln {

  namespace io {

    namespace internal {

      template<typename T>
      bool read_any(T& output, const std::string& name);

      template<typename T>
      bool write_any(const T& input, const std::string& name);

    } // end of internal


    // aliases

    internal::anything read(const std::string& name)
    {	return internal::anything(name); }

    internal::anything load(const std::string& name)
    {	return internal::anything(name); }


    template<typename T>
    bool read(T& output, std::string name)
    {	return internal::read_any(output, name); }

    template<typename T>
    bool load(T& output, std::string name)
    {	return internal::read_any(output, name); }


    template< typename T >
    bool write(const T& input, const std::string& name)
    {	return internal::write_any(input, name); }

    template< typename T >
    bool save(const T& input, const std::string& name)
    {	return internal::write_any(input, name); }



    namespace internal {

      // stream wrappers

      enum wrapper_id { WrapNone = 0,	     
			WrapFile = 1,
			WrapGz   = 2,
			WrapCmd  = 2,
			WrapAny  = 16 };

      template< wrapper_id W >
      struct wrapper
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }

	static bool knows_ext(const std::string&) { return false; }

	static std::istream* wrap_in(std::string&) { return 0; }
	static std::ostream* wrap_out(std::string&) { return 0; }

  	static void find(std::list<std::string>&, const std::string&) {}
      };


      // stream readers

      enum reader_id { ReadNone = 0,
		       ReadP1   = 1,
		       ReadP2   = 2,
		       ReadP3   = 3,
		       ReadP4   = 4,
		       ReadP5   = 5,
		       ReadP6   = 6,
		       ReadAny  = 16 };

      template< reader_id F, typename T >
      struct reader
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }

	static bool knows_ext(const std::string&)
	{ return false; }
	
	static bool read(std::istream&, T&) 
	{ return false; }
      };


      // stream writers

      enum writer_id { WriteNone = 0,
		       WriteP1   = 1,
		       WriteP2   = 2,
		       WriteP3   = 3,
		       WriteP4   = 4,
		       WriteP5   = 5,
		       WriteP6   = 6,
		       WriteAny  = 16 };

      template< writer_id F, typename T >
      struct writer
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }

	static bool knows_ext(const std::string&)
	{ return false; }
	
  	static bool write(std::ostream&, const T&) 
  	{ return false; }
      };


    } // end of internal
    
  } // end of io
  
} // end of mln

# include "io/base.hxx"

#endif // MILENA_IO_BASE_HH_


