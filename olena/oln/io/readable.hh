
#ifndef MILENA_IO_READABLE_HH_
# define MILENA_IO_READABLE_HH_

# include <string>

namespace mln {

  namespace io {

    namespace internal {

      template<typename T>
      bool read_any(T& output, const std::string& name);
      
      class anything
      {
      public:
	anything(const std::string& str) : _str(str) {}
	
	template< typename T >
	T& assign(T& output) const
	{
	  read_any(output, _str);
	  return output;
	}
	
      private:
	std::string _str;
      };

    } // end of internal

  } // end of io

} // end of mln


#endif // MILENA_IO_READABLE_HH_


