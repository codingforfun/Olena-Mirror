#include "basics1d.hh"
#include "basics2d.hh"
#include "basics3d.hh"
#include "core/type.hh"
#include "meta/cmp.hh"
#include "core/pred_image.hh"
#include "arith/ops.hh"
#include "arith/logic.hh"
#include "io/basics.hh"
#include "morpho/watershed.hh"

using namespace oln;

struct a_2dpred {
  template<class T>
  bin operator()(point2d p, T) const {
    return (p.row() + p.col()) % 2;
  }
};


template<class _I>
typename mute<_I>::ret
foo(const image<_I>& _input)
{
  Exact_cref(I, input);
  std::cout << I::name() << std::endl;
  typename mute<I>::ret output(input.size());
  typename I::iter p(input);
  for_all(p) output[p] = input[p] + 51;
  return output;
}


template<class _I, class _P>
void bar(image<_I>& _input, const point<_P>& _p)
{
  Exact_ref(I, input);
  Exact_cref(P, p);
  meta::eq<I::dim, P::dim>::ensure();
  input[p] = 69;
}


template<class _I, class _P>
void base(image<_I>& _input, const pred_image<_P>& _pred)
{
  Exact_ref(I, input);
  Exact_cref(P, pred);
  meta::eq<I::dim, P::dim>::ensure();
  typename I::iter p(input);
  for_all (p) {
    if (pred[p])
      input[p] = 0;
  }
}

template <class T>
type::any<T>& id(type::any<T>& v)
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

  border::set_width(bi1, 2);
  cout << bi1 << endl;
  border::adapt_copy(bi1, 3);
  cout << bi1 << endl;
  border::set_width(bi1, 1);
  cout << bi1 << endl;
  cout << bi1.border() << endl;

  ima[point2d(1, 1)] = 2;
  cout << ima << endl;

  ima = foo(ima);
  cout << ima << endl;

  bar(ima, point2d());
  cout << ima << endl;

  //  cout << conv::force<int_u8>::output<int>::ret::name () << endl;
  //  cout << convoutput<conv::force<int_u8>,int>::ret::name () << endl;

  image2d<int_u8> i2 = arith::plus(conv::force<int_u8>(), ima, ima);

  cout << i2 << endl;
  cout << arith::min_cst(ima, int_u8(52)) << endl;

  window2d win = mk_win_square (11);
  cout << win << endl;

  window2d::iter it(win);
  for_all(it) cout << it << ":";
  cout << endl;

  window3d win2 = mk_win_cube (3);
  cout << win2 << endl;

  cout << mk_w_win_from_win (1.2f, win2) << endl;

  w_window2d<int> win3 = (meta::ints =
			  1, 2, 3, lbrk,
			  4, 5, x(),
			  7, 8, 9, end);
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
