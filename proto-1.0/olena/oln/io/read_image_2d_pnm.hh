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
      struct read_image_2d_raw;

    }

  }

  template <typename I>
  struct category_type< io::impl::read_image_2d_raw<I> >
  {
    typedef cat::image ret;
  };

  template <typename I>
  struct props <cat::image, io::impl::read_image_2d_raw<I> >
    : public props<cat::image, I>
  {
    typedef I delegated_type;
  };



  namespace io {

    namespace impl {

      template <typename I>
      struct read_image_2d_raw:
	oln::abstract::op<I, read_image_2d_raw<I> >
      {

	typedef oln::abstract::op<I, read_image_2d_raw<I> > super_type;
	typedef oln_value_type(I) value_type;
	typedef ntg_io_type(value_type) io_type;

	mlc::box<I> image_;
	std::ifstream& istr_;
	internal::pnm_info& info_;

	read_image_2d_raw(I& image,
			  std::ifstream &istr,
			  internal::pnm_info &info) :
	  super_type(image),
	  image_(image),
	  istr_(istr),
	  info_(info)
	{}


	read_image_2d_raw<I>& output(I& output)
	{
	  output = *image_;
	  return *this;
	}

	void impl_run()
	{
	  value_type c;
	  point2d p;
	  oln::image2d<value_type>  tmp(info_.rows, info_.cols);

	  if (info_.max_val > ntg_max_val(value_type))
	    {
	      std::cerr << "Can't load image, data type is not large enough"
			<< std::endl;
	      return;
	    }

	  for (p.row() = 0; p.row() < info_.rows && !istr_.eof(); ++p.row())
	    for (p.col() = 0; p.col() < info_.cols && !istr_.eof(); ++p.col())
	      {
		read_value_type(c);
		tmp[p] = c;
	      }
	  istr_.close();
	  *image_ = tmp;
	}

	void read_value_type(ntg::integer<value_type> &c)
	{
	  io_type v;
	  istr_.read(&v, sizeof (io_type));
	  c = v;
	}

	void read_value_type(ntg::color<value_type> &c)
	{
	  assert((ntg_nb_comp(value_type) == 3));
	  io_type v;

	  for (unsigned i = 0; i < 3; i++)
	    {
	      istr_.read(&v, sizeof (v));
	      c[i] = v;
	    }
	}

	void read_value_type(ntg::enum_value<value_type> &c)
	{
	  static io_type v;
	  static int offset = -1;

	  if (offset == -1)
	    {
	      istr_.read((char *)(&v), sizeof (v));
	      offset = 7;
	    }
	  if ((int)(v & (1<<offset--)) == 0)
	    c = 0;
	  else
	    c = 1;
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
		read_image_2d_raw<I> tmp(ima.exact(), istr, info);
		tmp.run();
		tmp.output(ima.exact());
	      }
	    else if (info.type == "P2")
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
      }



      template <typename I, typename T>
      void read(abstract::image2d<I>& ima,
		const ntg::color<T>&,
		const std::string& filename,
		const std::string& ext)
      {
	std::ifstream istr;
	internal::pnm_info info;

	if (internal::read_pnm_header(istr, info, filename))
	  if (ext == "ppm")
	    if (info.type == "P6")
	      {
		read_image_2d_raw<I> tmp(ima.exact(), istr, info);
		tmp.run();
		tmp.output(ima.exact());
	      }
	    else if (info.type == "P3")
	      std::cerr << "error: read_image_2d_ppm_ascii not implemented"
			<< std::endl;
	    else
	      std::cerr << "error: file header (`" << info.type
			<< "') does not match file extension (`"
			<< ext << "')" << std::endl;
	  else
	    std::cerr << "error: image data type (`color') does not match"
		      << " file extension (`" << ext << "')" << std::endl;
	else
	  std::cerr << "error: unable to get a valid header" << std::endl;
      }

      template <typename I, typename T>
      void read(abstract::image2d<I>& ima,
		const ntg::enum_value<T>&,
		const std::string& filename,
		const std::string& ext)
      {
	std::ifstream istr;
	internal::pnm_info info;

	if (internal::read_pnm_header(istr, info, filename))
	  if (ext == "pbm")
	    if (info.type == "P4")
	      {
		read_image_2d_raw<I> tmp(ima.exact(), istr, info);
		tmp.run();
		tmp.output(ima.exact());
	      }
	    else if (info.type == "P1")
	      std::cerr << "error: read_image_2d_ppm_ascii not implemented"
			<< std::endl;
	    else
	      std::cerr << "error: file header (`" << info.type
			<< "') does not match file extension (`"
			<< ext << "')" << std::endl;
	  else
	    std::cerr << "error: image data type (`enum_value') does not match"
		      << " file extension (`" << ext << "')" << std::endl;
	else
	  std::cerr << "error: unable to get a valid header" << std::endl;
      }

    }

  }

}


#endif // ! OLN_IO_READ_IMAGE_2D_PNM_HH
