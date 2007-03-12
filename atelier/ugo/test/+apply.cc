#include <oln/core/2d/image2d_b.hh>
#include <oln/core/2d/window2d.hh>
#include <oln/debug/iota.hh>
#include <oln/level/apply.hh>


namespace my
{

  float sqr(float i)
  {
    return i * i;
  }


  struct sqr_t : oln::Function<sqr_t>
  {
    typedef float argument;
    typedef float result;
    float operator()(float i) const
    {
      return i * i;
    }
  };



  struct meta_sqr_t : oln::Function<meta_sqr_t>
  {
    template <typename /* argument is */ T>
    struct result {
      typedef T ret;
    };

    template <typename T>
    T operator()(T i) const
    {
      return i * i;
    }
  } meta_sqr;



  template <typename T>
  struct min_ : oln::Function< min_<T> >
  {
    typedef T    argument;
    typedef void result;

    mutable T val_;
    mutable bool ok_;

    min_()
    {
      this->init();
    }

    void init()
    {
      ok_ = false;
    }

    T value() const
    {
      precondition(ok_);
      return val_;
    }

    template <typename U>
    void operator()(U i) const
    {
      if (not ok_)
	{
	  ok_ = true;
	  val_ = static_cast<T>(i);
	  return;
	}
      if (i < val_)
	val_ = static_cast<T>(i);
    }

  };


}




namespace oln
{

  template <typename I>
  void println(const Image<I>& input)
  {
    typename I::fwd_piter p(input.points());
    for_all(p)
      std::cout << input(p) << ' ';
    std::cout << std::endl;
  }



  template <typename F, typename I, typename W>
  typename F::result
  apply_on_window(const Function<F>&  fun,
		  const Image<I>&     input,
		  const oln_point(I)& p,
		  const Window<W>&    win)
  {
    fun.init();
    oln_qiter(W) q(p, win);
    for_all(q)
      fun(input(q));
    return fun.value();
  }



  template <typename F, typename I, typename W>
  typename F::result
  apply_on_nbh(const Function<F>&       fun,
	       const Image_with_Nbh<I>& input,
	       const oln_point(I)&      p)
  {
    fun.init();
    oln_niter(I) n(p, input.points());
    for_all(n)
      fun(input(n));
    return fun.value();
  }




  template <typename F, typename I, typename W>
  I apply(const Function<F>& fun, const Image<I>& input, const Window<W>& win)
  {
    I output(input.points());
    oln_piter(I) p(input.points());
    for_all(p)
      output(p) = apply_on_window(fun, input, p, win);
    return output;
  }



//   template <typename I, typename W>
//   I erosion(const Image<I>& input, const Window<W>& win)
//   {
//     my::min_<oln_value(I)> min;
//     I output(input.points());
//     oln_piter(I) p(input.points());
//     oln_qiter(W) q(p, win);
//     for_all(p)
//       {
// 	min.init();
// 	for_all(q)
// 	  min(input(q));
// 	output(p) = min.value();
//       }
//     return output;
//   }


//   template <typename I, typename W>
//   I erosion_better(const Image<I>& input, const Window<W>& win)
//   {
//     I output(input.points());
//     oln_piter(I) p(input.points());
//     for_all(p)
//       {
// 	my::min_<oln_value(I)> min;
// 	output(p) = apply_local(min, input, p, win);
//       }
//     return output;
//   }





  // Fwd decl.

  template <typename I, typename W>
  I erosion(const Image<I>& input, const Window<W>& win);

  template <typename I>
  I elementary_erosion(const Image_with_Nbh<I>& input);



  namespace impl
  {

    template <typename I, typename W>
    I erosion(const Image<I>& input, const Window<W>& win)
    {
      my::min_<oln_value(I)> min;
      return apply(min, input, win);
    }

//     template <typename I, typename W>
//     I erosion(const Binary_Image<I>& input, const Window<W>& win)
//     {
//       my::and_<oln_value(I)> f;
//       return apply(f, input, win);
//     }

    // ...

  }


  // Facade.

  template <typename I, typename W>
  I erosion(const Image<I>& input, const Window<W>& win)
  {
    // I doit être Point_Wise_Accessible_Image...
    return impl::erosion(exact(input), exact(win));
  }



}




int main()
{
  using namespace oln;

  point2d p(0,0), q(6,6);
  box2d b(p, q);
  image2d_b<int> ima(b);

  debug::iota(ima);
  println(ima);

//   image2d_b<float> imaf = level::apply(my::sqr_t(), ima);
//   println(imaf);

//   ima = erosion(ima, win3x3);
//   println(ima);

  std::cout << my::meta_sqr(3.14) << std::endl;

  level::apply(my::meta_sqr, ima);
}
