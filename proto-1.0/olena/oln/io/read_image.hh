#ifndef OLN_IO_READ_IMAGE_HH
# define OLN_IO_READ_IMAGE_HH

# include <iostream>
# include <string>

# include <mlc/box.hh>

# include <oln/core/macros.hh>

# include <oln/io/read_image_2d_pnm.hh>
# include <oln/io/utils.hh>

namespace oln {


  namespace io {

    struct filename
    {
      explicit filename(const std::string& str) :
	str_(str)
      {}
      const std::string& get() const
      {
	return str_;
      }
    private:
      std::string str_;
    };

    filename read(const std::string& str)
    {
      filename tmp(str);
      return tmp;
    }

    template <typename I>
    void do_read(abstract::image<I>& ima, const filename& name)
    {
      std::string ext;
      const oln_value_type(I) t;

      ext = internal::utils::extension(name.get());

      if (ext == "pgm" ||
	  ext == "pbm" ||
	  ext == "ppm")
	impl::read(ima.exact(), t, name.get(), ext);
      else
	{
	  std::cout << "input method for '"
		    << name.get()
		    << "' not implemented yet"
		    << std::endl;
	}

    }

  }

}

#endif // ! OLN_IO_READ_IMAGE_HH
