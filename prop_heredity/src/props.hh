#ifndef PROTO_PROPS_HH
# define PROTO_PROPS_HH


template <typename E>
struct props;


# define Dim_type(E) typename props<E>::dim_type
# define Point_type(E) typename props<E>::point_type
# define Image_size_type(E) typename props<E>::image_size_type
# define Iter_type(E) typename props<E>::iter_type
# define Value_type(E) typename props<E>::value_type


template <class Dim>
struct default_props
{
  typedef Dim dim_type;
  typedef Point_type(Dim) point_type;
  typedef Image_size_type(Dim) image_size_type;
  typedef Iter_type(Dim) iter_type;
};


template <class Image>
struct default_props__Image
{
  typedef Dim_type(Image) dim_type;
  typedef Point_type(Image) point_type;
  typedef Image_size_type(Image) image_size_type;
  typedef Iter_type(Image) iter_type;
  typedef Value_type(Image) value_type;
};


#endif // ndef PROTO_PROPS_HH
