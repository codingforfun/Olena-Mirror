#ifndef PROTO_MORPHER_HH
# define PROTO_MORPHER_HH

# include "image.hh"


template <typename I> struct image_piece;


template <typename I>
struct props < image_piece<I> > : public default_props__Image<I>
{
};


template <typename I>
struct image_piece : public abstract::image_entry< image_piece<I> >
{
  typedef image_piece<I> E;

  image_piece(abstract::image<I>& ref_,
	      Point_type(I) point_,
	      Image_size_type(I) size_) :
    ref_(ref_.exact()),
    point_(point_),
    size_(size_)
  {}

  Image_size_type(E) size_impl() const {
    return size_;
  }

  const Value_type(E) opsqbr_impl(const Point_type(E)& p) const {
//     assert(this->hold(p));
    return ref_[point_ + p];
  }
  Value_type(E)& opsqbr_impl(const Point_type(E)& p) {
//     assert(this->hold(p));
    return ref_[point_ + p];
  }

  bool hold_impl(const Point_type(E)& p) const {
    return true; // FIXME
  }

  I& ref_;
  Point_type(E) point_;
  Image_size_type(E) size_;
};


template <typename I>
image_piece<I> piece(abstract::image<I>& ref_,
		     Point_type(I) point_,
		     Image_size_type(I) size_)
{
  image_piece<I> tmp(ref_, point_, size_);
  return  tmp;
}



#endif // ndef PROTO_MORPHER_HH
