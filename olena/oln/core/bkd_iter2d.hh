#ifndef PROTO_OLN_CORE_BKD_ITER2D_HH
# define PROTO_OLN_CORE_BKD_ITER2D_HH


namespace oln {


  struct bkd_iter2d;

  template <>
  struct props <bkd_iter2d> : public default_props<dim2d>
  {
  };

  struct bkd_iter2d : public abstract::iter< bkd_iter2d >
  {
    bkd_iter2d() {}

    bkd_iter2d(const image2d_size& size) :
      size(size)
    {
    }

    // FIXME: for backward compatibility
    template <typename I>
    bkd_iter2d(const abstract::image<I>& ima) :
      size(ima.size())
    {
    }

    const coord row() const
    {
      return p_.row();
    }

    const coord col() const
    {
      return p_.col();
    }

    void begin_impl()
    {
      p_.row() = size.nrows() - 1;
      p_.col() = size.ncols() - 1;
    }

    bool is_at_end_impl() const
    {
      return p_.row() == -1;
    }

    void next_impl()
    {
      --p_.col();
      if (p_.col() == -1)
	{
	  p_.col() = size.ncols() - 1;
	  --p_.row();
	}
    }

  protected:
    image2d_size size;
  };

} // end of namespace oln


#endif // ndef PROTO_OLN_CORE_BKD_ITER2D_HH
