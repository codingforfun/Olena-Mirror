#ifndef PROTO_DIM2D_HH
# define PROTO_DIM2D_HH

# include "dim.hh"


// fwd decls:
struct dim2d;
struct point2d;
struct iter2d;
struct image2d_size;
// FIXME: to be continued



template <>
struct props <dim2d> {
  enum { dim_value = 2 };
  typedef dim2d dim_type;

  typedef point2d point_type;
  typedef image2d_size image_size_type;
  typedef iter2d iter_type;
  // FIXME: to be continued
};

struct dim2d : public abstract::dim< dim2d >
{
};


#endif // ndef PROTO_DIM2D_HH
