#ifndef OLENA_CORE_2D_ARRAY2D_HH
# define OLENA_CORE_2D_ARRAY2D_HH


# include <oln/core/abstract/data_storage.hh>
# include <oln/core/2d/size2d.hh>
# include <oln/core/2d/point2d.hh>

namespace oln {


  template <typename T> struct array2d;
  template <typename T> 
  struct category_type< array2d<T> > { typedef cat::data_storage ret; };


  template <typename T>
  struct props < cat::data_storage, array2d<T> > // FIXME: add inheritance
  {
    typedef size2d  size_type;
    typedef point2d point_type;
    typedef T       data_type;
  };


  template <typename T>
  class array2d : public abstract::data_storage< array2d<T> >
  {

  public:

    array2d() :
      buffer_(0),
      array_(0),
      size_()
    {
      this->exact_ptr = this;
      invariant_();
    }

    // w/o impl
    array2d(const array2d&);
    void operator=(const array2d&);


    array2d(const size2d& s) :
      buffer_(0),
      array_(0),
      size_()
    {
      this->exact_ptr = this;
      this->resize(s);
    }

    ~array2d()
    {
      this->impl_clear_data();
    }

    bool impl_has_data() const
    {
      invariant_();
      return buffer_ != 0;
    }

    void impl_clear_data()
    {
      invariant_();
      if (this->has_data())
	{
	  // buffer
	  delete[] buffer_;
	  buffer_ = 0;
	  // array
	  array_ -= size_.border();
	  delete[] array_;
	  array_ = 0;
	  // size
	  size_ = size2d();
	}
      invariant_();
    }

    const size2d& size() const
    {
      return size_;
    }

    void impl_resize(const size2d& s)
    {
      precondition(s.nrows() > 0 and
		   s.ncols() > 0 and
		   s.border() >= 0);
      invariant_();
      this->clear_data();
      size_ = s;

      size_t nrows_eff = size_.nrows() + 2 * size_.border();
      size_t ncols_eff = size_.ncols() + 2 * size_.border();
      size_t nelts_eff = nrows_eff * ncols_eff;

      buffer_ = new T[nelts_eff];
      array_ = new T*[nrows_eff];

      T* buf = buffer_ + size_.border();
      for (size_t row = 0; row < nrows_eff; ++row)
	{
	  array_[row] = buf;
	  buf += ncols_eff;
	}
      array_ += size_.border();
      invariant_();
    }

    unsigned long impl_npoints() const
    {
      return size_.npoints();
    }

    bool impl_hold(const point2d& p) const
    {
      return
	p.row() >= 0 and
	p.row() < size_.nrows() and
	p.col() >= 0 and
	p.col() < size_.ncols();
    }

    bool impl_hold_large(const point2d& p) const
    {
      return
	p.row() >= - size_.border() and
	p.row() < size_.nrows() + size_.border() and
	p.col() >= - size_.border() and
	p.col() < size_.ncols() + size_.border();
    }

    const T impl_get(const point2d& p) const
    {
      invariant_();
      return array_[p.row()][p.col()];
    }

    void impl_set(const point2d& p, const T& v)
    {
      invariant_();
      array_[p.row()][p.col()] = v;
    }

    void impl_set_data(const T& v)
    {
      invariant_();
      const size_t nrows_eff = size_.nrows() + 2 * size_.border();
      const size_t ncols_eff = size_.ncols() + 2 * size_.border();
      const size_t len = nrows_eff * ncols_eff;
      T* p = buffer_;
      for (size_t i = 0; i < len; ++i)
	*p++ = v;
    }

  private:

    T* buffer_;
    T** array_;
    size2d size_;

    void invariant_() const
    {
      invariant((buffer_ != 0 and
		 array_ != 0 and
		 size_.nrows() > 0 and
		 size_.ncols() > 0 and
		 size_.border() >= 0)
		or
		(buffer_ == 0 and
		 array_ == 0 and
		 size_.nrows().is_undefined() and
		 size_.ncols().is_undefined() and
		 size_.border().is_undefined()));
    }

  };

} // end of namespace oln


#endif // ! OLENA_CORE_2D_ARRAY2D_HH
