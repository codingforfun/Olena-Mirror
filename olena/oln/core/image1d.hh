#ifndef PROTO_OLN_CORE_IMAGE1D_HH
# define PROTO_OLN_CORE_IMAGE1D_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/dim1d.hh>
# include <oln/core/image1d_size.hh>
# include <oln/core/macros.hh>
# include <oln/core/point1d.hh>

namespace oln {


  // fwd decls

  template <typename T> struct image1d;

//   namespace io {
//     struct data;
//     template <typename T> void load(image1d<T>& target, const io::data& data);
//   }


  // FIXME: for backward compatibility
  namespace border {
    namespace impl {
      template <typename T>
      void set_width(image1d<T>& target, coord width);
    }
    template <typename I>
    void set_width(const abstract::image<I>& target, coord width);
    template <typename I>
    void assign(const abstract::image<I>& target, oln_value_type(I) value);
  }


  // image1d

  template <typename T>
  struct props < image1d<T> > : public default_props <dim1d>
  {
    typedef T value_type;
    enum { linbuf_value = true };
  };

  template <typename T>
  struct image1d : public abstract::image_entry< image1d<T> >
  {
    typedef abstract::image_entry< image1d<T> > super;

    image1d() :
      super(this),
      size_(0),
      border_(0),
      buffer_(0)
    {
      this->exact_ptr = this;
    }

//     image1d(const io::data& data) :
//       super(this),
//       size_(0),
//       border_(0),
//       buffer_(0)
//     {
//       exact_ptr = this;
//       io::load(*this, data.filename);
//     }

    image1d(coord indices_,
	    coord border_ = constant::default_border) :
      super(this),
      size_(indices_),
      border_(border_),
      buffer_(0)
    {
      assert(indices_ > 0 &&
	     border_ >= 0);
      this->exact_ptr = this;
      allocate();
    }

    image1d(const image1d_size& size_,
	    coord border_ = constant::default_border) :
      super(this),
      size_(size_),
      border_(border_),
      buffer_(0)
    {
      assert(border_ >= 0);
      this->exact_ptr = this;
      allocate();
    }


    // FIXME: hum...
    image1d(const image1d& rhs) :
      super(this),
      size_(rhs.size_),
      border_(rhs.border_),
      buffer_(rhs.buffer_)
    {
      this->exact_ptr = this;
    }

    // FIXME: hum...
    void operator=(const image1d& rhs)
    {
      this->size_ = rhs.size_;
      this->border_ = rhs.border_;
      this->buffer_ = rhs.buffer_;
    }

    // FIXME: for backward compatibility
    void border_adapt_assign(coord width, T value) const
    {
      border::set_width(*this, width);
      border::assign(*this, value);
    }

    void resize(coord indices_,
		coord border_ = constant::default_border)
    {
      size_.nindices() = indices_;
      this->border_ = border_;
      allocate();
    }

    const image1d_size& size_impl() const {
      return size_;
    }

    const T op_square_brackets_impl(const point1d& p) const {
      assert(this->hold_large(p));
      assert(buffer_ != 0);
      return buffer_[p.index()];
    }

    T& op_square_brackets_impl(const point1d& p) {
      assert(this->hold_large(p));
      assert(buffer_ != 0);
      return buffer_[p.index()];
    }

    bool hold_impl(const point1d& p) const {
      return p.index() >= 0 && p.index() < size_.nindices();
    }

    bool hold_large_impl(const point1d& p) const {
      return p.index() >= - border_ && p.index() < size_.nindices() + border_;
    }

    size_t npoints_impl() const {
      return size_t(size_.nindices());
    }

    size_t buffer_size_impl() const {
      return size_t(size_.nindices() + 2 * border_);
    }

    // not _impl methods

    coord border() const {
      return border_;
    }

    image1d<T> clone() const
    {
      assert(buffer_ != 0);
      image1d<T> output(this->indices(), border());
      memcpy(output.buffer(), this->buffer(),
	     this->buffer_size() * sizeof(T));
      return output;
    }


    // hooks (handle carefully!)

    T* buffer_impl() { return buffer_; }
    const T* buffer_impl() const { return buffer_; }

  protected:

    image1d_size size_;
    coord border_;
    T* buffer_;

  private:

    void allocate(T*& buffer__,
		  coord indices__,
		  coord border__) {
      coord indices_eff = indices__ + 2 * border__;
      buffer__ = new T[size_t(indices_eff)];
      buffer__ += border__;
    }

    void allocate() {
      allocate(this->buffer_,
	       this->size_.nindices(),
	       this->border_);
    }

    void desallocate() {
      assert(buffer_ != 0);
      buffer_ -= border_;
      delete[] buffer_;
      buffer_ = 0; // safety
    }

    template <typename U>
    friend void border::impl::set_width(image1d<U>& target, coord width);
  };

} // end of namespace oln



#endif // ndef PROTO_OLN_CORE_IMAGE1D_HH
