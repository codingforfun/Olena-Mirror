#ifndef OLENA_IO_IMAGE_BASE_HH_
# define OLENA_IO_IMAGE_BASE_HH_

# include <oln/config/system.hh>

# include <iostream>
# include <string>

namespace oln {

  namespace io {

    namespace internal {

      /*---------------------.
      | image default reader |
      `---------------------*/
      
      enum reader_id { ReadNone     = 0,
		       ReadPnmPlain = 1,
		       ReadPnmRaw   = 2,
		       ReadAny      = 2 };

      
      // Default reader
      
      template<reader_id F, class I>
      struct reader
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }

	static bool knows_ext(const std::string&)
	{ return false; }

	static bool read(std::istream&, I&)
	{ return false; }
      };

      /*---------------------.
      | image default writer |
      `---------------------*/

      enum writer_id { WriteNone     = 0,
		       WritePnmPlain = 1,
		       WritePnmRaw   = 2,
		       WriteAny      = 2 };
      
      
      // Default writer
      
      template<writer_id F, class I>
      struct writer
      {
	static const std::string& name()
	{ static const std::string _name("-"); return _name; }

	static bool knows_ext(const std::string&)
	{ return false; }

	static bool write(std::ostream&, const I&)
	{ return false; }
      };      

    } // end of internal
    
  } // end of io
  
} // end of oln

#endif // ! OLENA_IO_IMAGE_BASE_HH_
