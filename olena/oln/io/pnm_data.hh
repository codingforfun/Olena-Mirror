#ifndef OLENA_IO_PNM_DATA_HH_
# define OLENA_IO_PNM_DATA_HH_

# include <oln/core/image.hh>

# include <iostream>

namespace oln {

  namespace io {

    namespace internal {

      /*------------------------.
      | pnm read data functions |
      `------------------------*/

      template <class I>
      inline void      
      pnm_read_data_plain_binary(std::istream& in, 
				 I& output)
      {
	// FIXME: implement an iterator over data
	Iter(I) it (output);

	for (it = begin; it != end; ++it)
	  {
	    unsigned char c;
	    do
	      {
		c = in.get();
	      }
	    while ((c != '0') && (c != '1'));
	    output[it] = (c == '1');
	  }
      }

      template <class I>
      inline void      
      pnm_read_data_plain_integer(std::istream& in, 
				  I& output)
      {
	// FIXME: implement an iterator over data
	Iter(I) it (output);

	int c;
	for (it = begin; it != end; ++it)
	  {
	    in >> c;
	    output[it] = c;
	  }
      }

      /*-------------------------.
      | pnm write data functions |
      `-------------------------*/

      template <class I>
      inline void      
      pnm_write_data_plain_binary(std::ostream& out, 
				  const I& input)
      {
	Iter(I) it(input);

	unsigned stride = 0;
	for (it = begin; it != end; ++it)
	  {
	    bool b = input[it];

	    out.put(b == true ? '1' : '0');
	    if (++stride >= 70)
	      {
		out << std::endl;
		stride = 0;
	      }
	  }
	out << std::endl;
      }

      template <class I>
      inline void      
      pnm_write_data_plain_integer(std::ostream& out, 
				   const I& input)
      {
	Iter(I) it(input);

	int stride = 0;
	for (it = begin; it != end; ++it)
	  {
	    int i = input[it];
	    out << i;
	    // FIXME: we should not assume that input is an 
	    // image_with_dim<2> !
	    if (++stride >= input.ncols())
	      {
		out << std::endl;
		stride = 0;
	      }
	    else
	      out << ' ';
	  }
      }      

    } // end of pnm
    
  } // end of io
  
} // end of oln

#endif // ! OLENA_IO_PNM_DATA_HH_
