#ifndef PROTO_SIZE_HH
# define PROTO_SIZE_HH

# include "any.hh"
# include "props.hh"
# include "dim2d.hh"


typedef int coord_t;

namespace abstract {

  template <typename E>
  struct image_size : public mlc::any__best_memory<E>
  {
    bool operator==(const image_size& rhs) const {
      return this->exact().opeq_impl(rhs.exact());
    }
  protected:
    image_size() {}
  };

} // end of namespace abstract



// image2d_size


struct image2d_size;

template <>
struct props <image2d_size> : public default_props<dim2d>
{
};

struct image2d_size : public abstract::image_size< image2d_size >
{
  image2d_size() {}

  image2d_size(coord_t nrows_, coord_t ncols_) :
    nrows_(nrows_),
    ncols_(ncols_)
  {}
  bool opeq_impl(const image2d_size& rhs) const {
    return nrows_ == rhs.nrows_ && ncols_ == rhs.ncols_;
  }
  const coord_t nrows() const { return nrows_; }
  const coord_t ncols() const { return ncols_; }
  coord_t& nrows() { return nrows_; }
  coord_t& ncols() { return ncols_; }
protected:
  coord_t nrows_, ncols_;
};


#endif // ndef PROTO_SIZE_HH
