#ifndef PROTO_OLN_CORE_SIZE2D_HH
# define PROTO_OLN_CORE_SIZE2D_HH


namespace oln {


  struct image2d_size;

  template <>
  struct props <image2d_size> : public default_props<dim2d>
  {
  };

  struct image2d_size : public abstract::image_size< image2d_size >
  {
    image2d_size() {}

    image2d_size(coord nrows_, coord ncols_) :
      nrows_(nrows_),
      ncols_(ncols_)
    {}

    image2d_size(const image2d_size& rhs) :
      nrows_(rhs.nrows_),
      ncols_(rhs.ncols_)
    {}

    void operator=(const image2d_size& rhs)
    {
      this->nrows_ = rhs.nrows_;
      this->ncols_ = rhs.ncols_;
    }

    bool op_eq_impl(const image2d_size& rhs) const {
      return nrows_ == rhs.nrows_ && ncols_ == rhs.ncols_;
    }

    const coord nrows() const { return nrows_; }
    const coord ncols() const { return ncols_; }

    coord& nrows() { return nrows_; }
    coord& ncols() { return ncols_; }

  protected:
    coord nrows_, ncols_;
  };

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_SIZE2D_HH
