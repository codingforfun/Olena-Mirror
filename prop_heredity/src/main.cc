#include <iostream>

#include "mlc.hh"
#include "image.hh"
#include "iter.hh"
#include "morpher.hh"



template <typename I>
std::ostream& operator<<(std::ostream& ostr,
			 const abstract::image<I>& input)
{
  Iter_type(I) p(input.size());
  for_all(p)
    ostr << input[p] << ' ';
  return ostr;
}


// IDEA: split overloaded versions and a single facade that forces args downcast
// rationale: the client should *not* have to downcast args


template <typename I>
void foo_overloaded(abstract::binary_image<I>& input)
{
  Iter_type(I) p(input.size());
  for_all(p)
    std::cout << input[p] << '*';
  std::cout << std::endl;
}


template <typename I>
void foo_overloaded(abstract::image2d<I>& input)
{
  Value_type(I) i = 0;

  for (int row = 0; row < input.nrows(); ++row)
    for (int col = 0; col < input.ncols(); ++col)
      input(row, col) = i++;
}


template <typename I>
void foo(abstract::image<I>& input)
{
  foo_overloaded(input.exact());
}



int main()
{
  image2d<unsigned> ima(4, 4);
  foo(ima);
  std::cout << ima << std::endl;
  
  image_piece< image2d<unsigned> > pima = piece(ima, point2d(1,1), image2d_size(2,2));
  foo(pima);
  std::cout << ima << std::endl;
}
