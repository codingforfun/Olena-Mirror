

template <class T>
struct f_get_point_type
{
  typedef typename T::point_type ret;
};

#define get_point_type(T) typename f_get_point_type<T>::ret

struct undefined {};

struct abstract_image
{
  typedef undefined point_type;
};

template <class T>
struct image2d : public abstract_image
{
  typedef image2d<T> self_type;
  typedef get_point_type(self_type) point_type;
};

int main()
{
  image2d<int>::point_type p;
}
