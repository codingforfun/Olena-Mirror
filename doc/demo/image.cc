#include <oln/basics1d.hh>
#include <oln/basics2d.hh>
#include <oln/basics3d.hh>
#include <mlc/type.hh>
#include <mlc/cmp.hh>
#include <oln/core/abstract/image_with_type.hh>
#include <oln/arith/ops.hh>
#include <oln/arith/logic.hh>
#include <oln/io/basics.hh>
#include <oln/morpho/watershed.hh>

using namespace oln;
using namespace ntg;

using std::cout;
using std::endl;

struct a_2dpred {
  template<class T>
  bin operator()(point2d p, T) const {
    return (p.row() + p.col()) % 2;
  }
};


template<class I>
typename mute<I>::ret
foo(const abstract::image<I>& input)
{
  std::cout << I::name() << std::endl;
  typename mute<I>::ret output(input.size());
  typename I::iter_type p(input);
  for_all(p) output[p] = input[p] + 51;
  return output;
}


template<class I, class P>
void bar(abstract::image<I>& input, const abstract::point<P>& p)
{
  mlc::eq<I::dim, P::dim>::ensure();
  input[p] = 69;
}

template<class I, class P>
void base(abstract::image<I>& input, const abstract::binary_image<P>& pred)
{
  mlc::eq<I::dim, P::dim>::ensure();
  typename I::iter_type p(input);
  for_all (p) {
    if (pred[p])
      input[p] = 0;
  }
}

template <class T>
mlc_hierarchy::any<T>& id(mlc_hierarchy::any<T>& v)
{
  return v;
}


int main()
{
  image2d<int_u8> ima(3,3);
  image3d<bin> bi1(2,2,2);
  image3d<bin> bi2(2,2,2);
  bi1(1,1,0) = true;
  bi2(0,1,0) = true;
  cout << bi1 << endl;
  cout << bi2 << endl;
  cout << arith::logic_or(bi1, bi2) << endl;

  bi1.border_set_width(2);
  cout << bi1 << endl;
  bi1.border_adapt_copy(3);
  cout << bi1 << endl;
  bi1.border_set_width(1);
  cout << bi1 << endl;
  cout << bi1.border() << endl;

  ima[point2d(1, 1)] = 2;
  cout << ima << endl;

  ima = foo(ima);
  cout << ima << endl;

  bar(ima, point2d());
  cout << ima << endl;

  cout << ntg::type_traits<convert::force<int_u8>::output<int>::ret>::name () << endl;
  //cout << ntg::type_traits<convoutput<convert::force<int_u8>,abstractint>::ret>::name () << endl;

  image2d<int_u8> i2 = arith::plus(convert::force<int_u8>(), ima, ima);

  cout << i2 << endl;
  cout << arith::min_cst(ima, int_u8(52)) << endl;

  window2d win = mk_win_square (11);
  cout << win << endl;

  window2d::iter_type it(win);
  for_all(it) cout << it << ":";
  cout << endl;

  window3d win2 = mk_win_cube (3);
  cout << win2 << endl;

  cout << mk_w_win_from_win (1.2f, win2) << endl;

  w_window2d<int> win3 = (mlc::ints_2d =
			  1, 2, 3, mlc::lbrk,
			  4, 5, mlc::x(),
			  7, 8, 9, mlc::end);
  cout << win3 << endl;

  /* Try the watershed on 1D images. */
  image1d<int_u8> im1d(10);
  im1d(0) = 4;
  im1d(1) = 7;			/* Top. */
  im1d(2) = 5;
  im1d(3) = 3;
  im1d(4) = 6;
  im1d(5) = 8;
  im1d(6) = 9;			/* Top. */
  im1d(7) = 6;
  im1d(8) = 4;
  im1d(9) = 1;
  neighborhood1d n1 = mk_neighb_segment(3);
  image1d<int_u8> im1dw = morpho::watershed_seg<int_u8>(im1d, n1);
  cout << im1dw << endl;
}
