#ifndef OLENA_CORE_1D_ARRAY1D_HH
# define OLENA_CORE_1D_ARRAY1D_HH


# include <oln/core/abstract/data_storage.hh>
# include <oln/core/1d/size1d.hh>
# include <oln/core/1d/point1d.hh>

namespace oln {


  template <typename T> struct array1d;
  template <typename T>
  struct category_type< array1d<T> > { typedef cat::data_storage ret; };


  template <typename T>
  struct props < cat::data_storage, array1d<T> > // FIXME: add inheritance
  {
    typedef size1d  size_type;
    typedef point1d point_type;
    typedef T       data_type;
  };


  template <typename T>
  class array1d : public abstract::data_storage< array1d<T> >
  {

  public:

    array1d() :
      buffer_(0),
      size_()
    {
      this->exact_ptr = this;
      invariant_();
    }

    // w/o impl
    array1d(const array1d&);
    void operator=(const array1d&);


    array1d(const size1d& s) :
      buffer_(0),
      size_()
    {
      this->exact_ptr = this;
      this->resize(s);
    }

    ~array1d()
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
	  // size
	  size_ = size1d();
	}
      invariant_();
    }

    const size1d& size() const
    {
      return size_;
    }

    void impl_resize(const size1d& s)
    {
      precondition(s.nindices() > 0 and
		   s.border() >= 0);
      invariant_();
      this->clear_data();
      size_ = s;
      size_t nelts_eff = size_.nindices() + 2 * size_.border();
      buffer_ = new T[nelts_eff];
      invariant_();
    }

    unsigned long impl_npoints() const
    {
      return size_.npoints();
    }

    bool impl_hold(const point1d& p) const
    {
      return
	p.index() >= 0 and
	p.index() < size_.nindices();
    }

    bool impl_hold_large(const point1d& p) const
    {
      return
	p.index() >= - size_.border() and
	p.index() < size_.nindices() + size_.border();
    }

    const T impl_get(const point1d& p) const
    {
      invariant_();
      return buffer_[p.index()];
    }

    void impl_set(const point1d& p, const T& v)
    {
      invariant_();
      buffer_[p.index()] = v;
    }

    void impl_set_data(const T& v)
    {
      invariant_();
      const size_t len = size_.nindices() + 2 * size_.border();
      T* p = buffer_;
      for (size_t i = 0; i < len; ++i)
	*p++ = v;
    }

  private:

    T* buffer_;
    size1d size_;

    void invariant_() const
    {
      invariant((buffer_ != 0 and
		 size_.nindices() > 0 and
		 size_.border() >= 0)
		or
		(buffer_ == 0 and
		 size_.nindices() == 0 and
		 size_.border() == 0));
    }

  };

} // end of namespace oln


#endif // ! OLENA_CORE_1D_ARRAY1D_HH
