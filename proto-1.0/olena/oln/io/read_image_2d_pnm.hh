#ifndef OLN_IO_READ_IMAGE_2D_PNM_HH
# define OLN_IO_READ_IMAGE_2D_PNM_HH

# include <iostream>
# include <string>

# include <mlc/box.hh>

# include <ntg/core/macros.hh>
# include <ntg/real/int_u8.hh>
# include <ntg/real/integer.hh>
# include <ntg/enum/enum.hh>
# include <ntg/color/color.hh>

# include <oln/core/2d/image2d.hh>
# include <oln/core/macros.hh>
# include <oln/core/abstract/op.hh>

# include <oln/io/utils.hh>


namespace oln {

  template <typename I>
  struct image2d;

  namespace io {

    namespace impl {


      template <typename I>
      struct read_image_2d_pgm_raw:
	oln::abstract::void_op<read_image_2d_pgm_raw<I> >
      {
	mlc::box<I> image_;
	std::ifstream& istr_;
	internal::pnm_info& info_;

	read_image_2d_pgm_raw(I& image,
			      std::ifstream &istr,
			      internal::pnm_info &info) :
	  image_(image),
	  istr_(istr),
	  info_(info)
	{}

	typedef oln_value_type(I) value_type;
	typedef typename mlc::traits<value_type>::encoding_type encoding_type;

	read_image_2d_pgm_raw<I>& output(I& output)
	{
	  output = *image_;
	  return *this;
	}

	void impl_run()
	{
	  encoding_type c;

	  if (info_.max_val > ntg_max_val(value_type))
	    {
	      std::cerr << "Can't load image, data type is not large enough"
			<< std::endl;
	      return;
	    }
	  point2d p;
	  oln::image2d<value_type>  tmp(info_.rows, info_.cols);

	  std::cout << info_.cols << "," << info_.rows << ","
		    << info_.max_val << std::endl;


	  for (p.row() = 0; p.row() < info_.rows && !istr_.eof(); ++p.row())
	    for (p.col() = 0; p.col() < info_.cols && !istr_.eof(); ++p.col())
	      {
		istr_.read(&c, sizeof (encoding_type));
		tmp[p] = c;
	      }
	  istr_.close();
	  *image_ = tmp;
	  std::cout << "debug : size = " << image_->size() << std::endl;
	}
      };


      template <typename I, typename T>
      void read(abstract::image2d<I>& ima,
		const ntg::integer<T>&,
		const std::string& filename,
		const std::string& ext)
      {
	std::ifstream istr;
	internal::pnm_info info;

	if (internal::read_pnm_header(istr, info, filename))
	  if (ext == "pgm")
	    if (info.type == "P5")
	      {
		read_image_2d_pgm_raw<I> tmp(ima.exact(), istr, info);
		tmp.run();
		tmp.output(ima.exact());
	      }
	    else
	      if (info.type == "P2")
		std::cerr << "error: read_image_2d_pgm_ascii not implemented"
			  << std::endl;
	      else
		std::cerr << "error: file header (`" << info.type
			  << "') does not match file extension (`"
			  << ext << "')" << std::endl;
	  else
	    std::cerr << "error: image data type (`integer') does not match"
		      << " file extension (`" << ext << "')" << std::endl;
	else
	  std::cerr << "error: unable to get a valid header" << std::endl;
	std::cout << "debug : size = " << ima.size() << std::endl;


      }

      template <typename I, typename T>
      void read(abstract::image2d<I>& ima,
		const ntg::color<T>&,
		const std::string& filename,
		const std::string& ext)
      {
	std::cout << "read for image2d<color>" << std::endl;
      }

      template <typename I, typename T>
      void read(abstract::image2d<I>& ima,
		const ntg::enum_value<T>&,
		const std::string& filename,
		const std::string& ext)
      {
	std::cout << "read for image2d<enum>" << std::endl;
      }


    }

  }

}


#endif // ! OLN_IO_READ_IMAGE_2D_PNM_HH
