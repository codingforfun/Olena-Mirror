#ifndef PROTO_IMAGE_HH
# define PROTO_IMAGE_HH

# include "dim2d.hh"
# include "size.hh"
# include "point.hh"
# include "mlc.hh"


namespace abstract {

  template <typename E>
  struct image : public virtual mlc::any__best_speed<E>
  {
    Image_size_type(E) size() const {
      return this->exact().size_impl();
    }

    const Value_type(E) operator[](const Point_type(E)& p) const {
      return this->exact().opsqbr_impl(p);
    }
    Value_type(E)& operator[](const Point_type(E)& p) {
      return this->exact().opsqbr_impl(p);
    }

    bool hold(const Point_type(E)& p) const {
      return this->exact().hold_impl(p);
    }

  protected:
    typedef mlc::any__best_speed<E> super;
    //image(E *const this_) : super(this) {}
    image() { std::cout << "hop!" << std::endl; }

    virtual ~image() {}
  };


  // discriminant is dimension

  template <typename E>
  struct image2d : public virtual image<E>
  {
    coord_t nrows() const { return this->size().nrows(); }
    coord_t ncols() const { return this->size().ncols(); }
    const Value_type(E) operator()(coord_t row, coord_t col) const {
      return this->operator[](Point_type(E)(row, col));
    }
    Value_type(E)& operator()(coord_t row, coord_t col) {
      return this->operator[](Point_type(E)(row, col));
    }
  };


  // discriminant is value type tag

  template <typename E>
  struct label_image : public virtual image<E>
  {};

  template <typename E>
  struct binary_image : public label_image<E>
  {};

  template <typename E>
  struct scalar_image : public virtual image<E>
  {};

  template <typename E>
  struct vector_image : public virtual image<E>
  {};


  // a class with TWO properties:

  template <typename E>
  struct scalar_image2d : public virtual image<E>
  {};


  namespace internal {


    // FIXME: switch over data storage (linear or not)


    // switch over dimension

    template <typename Dim, typename E>
    struct dim_switch;

    template <typename E>
    struct dim_switch <dim2d, E> : public image2d<E>
    {};

    // switch over value type tag

    template <unsigned Tag, typename E>
    struct value_tag_switch : public image<E>
    {};

    template <typename E>
    struct value_tag_switch<label_tag, E> : public label_image<E>
    {};

    template <typename E>
    struct value_tag_switch<binary_tag, E> : public binary_image<E>
    {};

    template <typename E>
    struct value_tag_switch<scalar_tag, E> : public scalar_image<E>
    {};

    template <typename E>
    struct value_tag_switch<vector_tag, E> : public vector_image<E>
    {};


  } // end of namespace internal




  // entry point of image hierarchy

  template <typename E>
  struct image_entry : public virtual internal::dim_switch< Dim_type(E), E>,
		       public virtual internal::value_tag_switch< props<Value_type(E)>::tag_value, E >
  {
  protected:
    image_entry() :
      internal::dim_switch< Dim_type(E), E>(),
      internal::value_tag_switch< props<Value_type(E)>::tag_value, E >()
    {
      mlc::set_this((E*)this);
    }
  };



} // end of namespace abstract



// image2d
template <typename T>
struct image2d;

template <typename T>
struct props < image2d<T> > : public default_props<dim2d>
{
  typedef T value_type;
};

template <typename T>
struct image2d : public abstract::image_entry< image2d<T> >
{
  typedef abstract::image_entry< image2d<T> > super;
  image2d() {}

  image2d(coord_t nrows_, coord_t ncols_) :
    size_(nrows_, ncols_)
  {
    array_ = new T*[nrows_];
    buffer_ = new T[nrows_ * ncols_];
    T* buf = buffer_;
    for (int row = 0; row < nrows_; ++row)
      {
	array_[row] = buf;
	buf += ncols_;
      }
  }

  image2d_size size_impl() const {
    return size_;
  }

  const T opsqbr_impl(const point2d& p) const {
    assert(this->hold(p));
    return array_[p.row()][p.col()];
  }
  T& opsqbr_impl(const point2d& p) {
    assert(this->hold(p));
    return array_[p.row()][p.col()];
  }

  bool hold_impl(const point2d& p) const {
    return
      p.row() >= 0 && p.row() < size_.nrows()
      && p.col() >= 0 && p.col() < size_.ncols();
  }

protected:
  image2d_size size_;
  T* buffer_;
  T** array_;
};




#endif // ndef PROTO_IMAGE_HH
