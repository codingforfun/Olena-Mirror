#ifndef PROTO_OLN_CORE_SIZE1D_HH
# define PROTO_OLN_CORE_SIZE1D_HH


namespace oln {


  struct image1d_size;

  template <>
  struct props <image1d_size> : public default_props<dim1d>
  {
  };

  struct image1d_size : public abstract::image_size< image1d_size >
  {
    image1d_size() {}

    image1d_size(coord nindices_) :
      nindices_(nindices_)
    {}

    image1d_size(const image1d_size& rhs) :
      nindices_(rhs.nindices_)
    {}

    void operator=(const image1d_size&);

    bool op_eq_impl(const image1d_size& rhs) const {
      return nindices_ == rhs.nindices_;
    }

    const coord nindices() const { return nindices_; }
    coord& nindices() { return nindices_; }

  protected:
    coord nindices_;
  };

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_SIZE1D_HH
