#ifndef PROTO_OLN_CORE_IMAGE2D_HH
# define PROTO_OLN_CORE_IMAGE2D_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/dim2d.hh>
# include <oln/core/macros.hh>
# include <oln/core/image2d_size.hh>
# include <oln/core/constants.hh>
# include <oln/core/point2d.hh>

namespace oln {


  // fwd decls

  template <typename T> struct image2d;

  namespace io {
    struct data;
    template <typename T> void load(image2d<T>& target, const io::data& data);
  }


  // FIXME: for backward compatibility
  namespace border {
//     namespace impl {
//       template <typename T>
//       void set_width(image2d<T>& target, coord_t width);
//     }
//     template <typename I>
//     void set_width(const abstract::image<I>& target, coord_t width);
//     template <typename I>
//     void assign(const abstract::image<I>& target, oln_value_type(I) value);
  }


  // image2d

  template <typename T>
  struct props < image2d<T> > : public default_props <dim2d>
  {
    typedef T value_type;
    enum { linbuf_value = true };
  };

  template <typename T>
  struct image2d : public abstract::image_entry< image2d<T> >
  {
    typedef abstract::image_entry< image2d<T> > super;

    image2d() :
      super(this),
      size_(0, 0),
      border_(0),
      buffer_(0),
      array_(0)
    {
      this->exact_ptr = this;
    }

    image2d(const io::data& data) :
      super(this),
      size_(0, 0),
      border_(0),
      buffer_(0),
      array_(0)
    {
      this->exact_ptr = this;
      io::load(*this, this->data.filename);
    }

    image2d(coord nrows_, coord ncols_,
	    coord border_ = constant::default_border) :
      super(this),
      size_(nrows_, ncols_),
      border_(border_),
      buffer_(0),
      array_(0)
    {
      assert(nrows_ > 0 &&
	     ncols_ > 0 &&
	     border_ >= 0);
      this->exact_ptr = this;
      allocate();
    }

    image2d(const image2d_size& size_,
	    coord border_ = constant::default_border) :
      super(this),
      size_(size_),
      border_(border_),
      buffer_(0),
      array_(0)
    {
      assert(border_ >= 0);
      this->exact_ptr = this;
      allocate();
    }


    // FIXME: hum...
    image2d(const image2d& rhs) :
      super(this),
      size_(rhs.size_),
      border_(rhs.border_),
      buffer_(rhs.buffer_),
      array_(rhs.array_)
    {
      this->exact_ptr = this;
    }

    // FIXME: hum...
    void operator=(const image2d& rhs)
    {
      this->size_ = rhs.size_;
      this->border_ = rhs.border_;
      this->buffer_ = rhs.buffer_;
      this->array_ = rhs.array_;
    }

    // FIXME: for backward compatibility
    void border_adapt_assign(coord width, T value) const
    {
//       border::set_width(*this, width);
//       border::assign(*this, value);
    }

    void resize(coord nrows_, coord ncols_,
		coord border_ = constant::default_border)
    {
      size_.nrows() = nrows_;
      size_.ncols() = ncols_;

      //FIXME: bug with g++-2.95 when abstract::image inherits from any__best_memory
//       std::cout << "size_.nrows()=" << size_.nrows() << std::endl;
//       std::cout << "size_impl().nrows()=" << this->size_impl().nrows() << std::endl;
//       std::cout << "size().nrows()=" << this->size().nrows() << std::endl;

      this->border_ = border_;
      allocate();
    }

    const image2d_size& size_impl() const {
      return size_;
    }

    const T op_square_brackets_impl(const point2d& p) const {
      assert(this->hold_large(p));
      assert(array_ != 0);
      return array_[p.row()][p.col()];
    }

    T& op_square_brackets_impl(const point2d& p) {
      assert(this->hold_large(p));
      assert(array_ != 0);
      return array_[p.row()][p.col()];
    }

    bool hold_impl(const point2d& p) const {
      return
	p.row() >= 0 && p.row() < size_.nrows()
	&& p.col() >= 0 && p.col() < size_.ncols();
    }

    bool hold_large_impl(const point2d& p) const {
      return
	p.row() >= - border_ && p.row() < size_.nrows() + border_
	&& p.col() >= - border_ && p.col() < size_.ncols() + border_;
    }

    size_t npoints_impl() const {
      return size_t(size_.nrows()) * size_t(size_.ncols());
    }

    size_t buffer_size_impl() const {
      return size_t(size_.nrows() + 2 * border_)
	* size_t(size_.ncols() + 2 * border_);
    }

    // not _impl methods

    coord border() const {
      return border_;
    }

    const image2d<T> clone_impl() const
    {
      assert(array_ != 0);
      image2d<T> output(this->nrows(), this->ncols(), border());
      memcpy(output.buffer(), this->buffer(),
	     this->buffer_size() * sizeof(T));
      return output;
    }


    // hooks (handle carefully!)

    T* buffer_impl() { return buffer_; }
    const T* buffer_impl() const { return buffer_; }

    T** array() { return array_; }
    T** const array() const { return array_; }

  protected:

    image2d_size size_;
    coord border_;
    T* buffer_;
    T** array_;

  private:

    void allocate(T*& buffer__, T**& array__,
		  coord nrows__, coord ncols__,
		  coord border__) {
      coord nrows_eff = nrows__ + 2 * border__;
      coord ncols_eff = ncols__ + 2 * border__;
      array__ = new T*[nrows_eff];
      buffer__ = new T[size_t(nrows_eff) * size_t(ncols_eff)];
      T* buf = buffer__ + border__;
      for (coord row = 0; row < nrows_eff; ++row)
	{
	  array__[row] = buf;
	  buf += ncols_eff;
	}
      array__ += border__;
    }

    void allocate() {
      allocate(this->buffer_, this->array_,
	       this->size_.nrows(), this->size_.ncols(),
	       this->border_);
//       coord_t nrows_eff = size_.nrows() + 2 * border_;
//       coord_t ncols_eff = size_.ncols() + 2 * border_;
//       array_ = new T*[nrows_eff];
//       buffer_ = new T[size_t(nrows_eff) * size_t(ncols_eff)];
//       T* buf = buffer_ + border_;
//       for (coord_t row = 0; row < nrows_eff; ++row)
// 	{
// 	  array_[row] = buf;
// 	  buf += ncols_eff;
// 	}
//       array_ += border_;
    }

    void desallocate() {
      if (buffer_ == 0 || array_ == 0)
	std::cerr << "pb" << std::endl;
      // FIXME:
      assert(buffer_ != 0 && array_ != 0);
      delete[] buffer_;
      buffer_ = 0; // safety
      array_ -= border_;
      delete[] array_;
      array_ = 0;  // safety
    }

//     template <typename U>
//     friend void border::impl::set_width(image2d<U>& target, coord width);
  };

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_IMAGE2D_HH
