#ifndef OLENA_IO_UTILS_HH_
# define OLENA_IO_UTILS_HH_

# include <oln/config/system.hh>

# include <string>

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

    } // end of internal

  } // end of io

} // end of oln

#endif // ! OLENA_IO_UTILS_HH_
