#include <assert.h>
#include <iostream>


typedef int value_type;
typedef int point_type;


template <typename I>
struct value_box
{
  I& ima;
  point_type p;

  value_box(I& ima, point_type p) :
    ima(ima), p(p)
  {}
  value_box& operator=(value_type v)
  {
    ima.set(p, v);
    return *this;
  }
  operator value_type() const
  {
    return ima.get(p);
  }
};

template <typename I>
struct value_box <const I>
{
  const I& ima;
  point_type p;

  value_box(const I& ima, point_type p) :
    ima(ima), p(p)
  {}
  operator value_type() const
  {
    return ima.get(p);
  }
};

template <typename I>
std::ostream& operator<<(std::ostream& ostr, const value_box<I>& v)
{
  return ostr << value_type(v);
}


template <typename E>
struct image
{
  E& exact() { return *((E*)(void*)this); }
  const E& exact() const { return *((const E*)(const void*)this); }

  value_box<E> operator[](point_type p)
  {
    return value_box<E>(this->exact(), p);
  }
  value_box<const E> operator[](point_type p) const
  {
    return value_box<const E>(this->exact(), p);
  }
};

template <typename E>
struct readonly_image : public image<E>
{
  friend class value_box<E>;
  friend class value_box<const E>;
private:
  const value_type get(point_type p) const
  {
    return this->exact().impl_get(p);
  }
};

template <typename E>
struct readwrite_image : public image<E>
{
  friend class value_box<E>;
  friend class value_box<const E>;
private:
  const value_type get(point_type p) const
  {
    return this->exact().impl_get(p);
  }
  void set(point_type p, value_type v)
  {
    this->exact().impl_set(p, v);
  }
};


struct mutable_dummy_image : public readwrite_image< mutable_dummy_image >
{
  value_type data;

  mutable_dummy_image()
  {
    data = 3;
  }
  value_type impl_get(point_type p) const
  {
    std::cout << "get" << std::endl;
    return data;
  }
  void impl_set(point_type p, value_type v)
  {
    std::cout << "set" << std::endl;
    data = v;
  }
};


struct constant_dummy_image : public readonly_image< constant_dummy_image >
{
  value_type data;

  constant_dummy_image()
  {
    data = 3;
  }
  value_type impl_get(point_type p) const
  {
    std::cout << "get" << std::endl;
    return data;
  }
};


int main()
{
  point_type p;
  {
    mutable_dummy_image ima;
    ima[p] = 3 * ima[p] * ima[p];
    std::cout << ima[p] << std::endl;
  }
  {
    constant_dummy_image ima;
    //ima[p] = 7;
    std::cout << ima[p] << std::endl;    
  }
}
