
#include <iostream>


template <typename T>
struct props;


namespace abstract {

  template <typename E>
  struct image
  {};

  // data constness

  template <typename E>
  struct readonly_image : public image<E>
  {};

  template <typename E>
  struct readwrite_image : public image<E>
  {};

  // dimension

  template <typename E>
  struct image1d : public image<E>
  {};

  template <typename E>
  struct image2d : public image<E>
  {};

  // ...

} // end of namespace abstract



template <typename TAG, typename E> struct image_case;

#define decl_case(TAG, MCLASS)                   \
template <typename E>                            \
struct image_case < TAG, E > : public MCLASS <E> \
{ protected: image_case() {} }



struct constness_tag {};
struct readonly_tag : public constness_tag {};
struct readwrite_tag : public constness_tag {};


decl_case ( readonly_tag, abstract::readonly_image );
decl_case ( readwrite_tag, abstract::readwrite_image );



template <typename TAG, typename E> struct tag_retrieve;

template <typename E>
struct tag_retrieve < constness_tag, E >
{
  typedef typename props<E>::constness_tag ret;
  // test if ret derives from constness_tag
};



template <typename TAG, typename E>
struct image_switch : public image_case< typename tag_retrieve<TAG,E>::ret, E>
{
};



namespace abstract {

  template <typename E>
  struct image_entry : public image_switch < constness_tag, E >
                       // ...
  {
  };

}



// image2d


struct image2d; // fwd decl

template <>
struct props < image2d >
{
  typedef readwrite_tag constness_tag;
};


struct image2d : public abstract::image_entry< image2d >
{};



// foo


template <typename I>
void foo(abstract::readonly_image<I>&)
{
  std::cout << "foo(abstract::readonly_image)" << std::endl;
}


template <typename I>
void foo(abstract::readwrite_image<I>&)
{
  std::cout << "foo(abstract::readwrite_image)" << std::endl;
}


int main()
{
  image2d ima;
  foo(ima);
}
