
#include <iostream>


class data_holder
{
public:
  void m() { std::cout << "ok" << std::endl; }
protected:
  const data_holder& data_() const { return *this; }
  data_holder& data_() { return *this; }
  int i;
};


struct image2d : private data_holder
{
  image2d() { i = 51; }
  const data_holder& data() const { return data_(); }
  data_holder& data() { return data_(); }
};


int main()
{
  image2d ima;
  ima.data().m();
}
