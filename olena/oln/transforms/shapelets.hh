#include <ntg/int.hh>
#include <oln/basics2d.hh>


#include <oln/core/image.hh>
#include <oln/core/point1d.hh>
#include <oln/core/point2d.hh>
#include <oln/level/fill.hh>
#include <vector>
#include <bits/stl_function.h>

namespace oln
{
  namespace internal
  {
    template <class T>
    struct hermite : public std::binary_function<T, int, T>
    {
      T operator()(unsigned n, T x)
      {

	if (n == 0)
	  return 1;
	if (n == 1)
	  return 2. * x;
	return 2. * x * (*this)(n - 1, x) - 2. * T(n - 1) * (*this)(n - 2, x);
      }
    };

    template <class T>
    struct fact : public std::unary_function<T, T>
    {



      T operator() (T n)
      {
	precondition(n >= 0);
	T res = 1;

	while (n > 0)
	  {
	    res *= n;
	    n--;
	  }
	return res;
      }
    };

    template <class T>
    struct shapelets_beta : public std::binary_function<T, int, T>
    {
      T operator()(int n, T x)
      {
	assert(n >= 0);
	const double c = sqrt(pow(2.0, n) * sqrt(M_PI) * fact<T>()(n));

	return hermite<T>()(n, x) * exp(x * x / -2.0) / c;
      }
    };

    template <unsigned Dim, class T>
    struct shapelets_basis
    {
    };

    template <class T>
    struct shapelets_basis<1, T>
    {
      T operator() (int n, point1d p, T b)
      {
	assert(n >= 0 && b >= 0);
	return shapelets_beta<T>(n, p.col() / b) / sqrt(b);
      }
    };

    template <class T>
    struct shapelets_basis<2, T>
    {
      T operator() (int m, int n, T x, T y, T b)
      {
	assert(m >= 0 && n >= 0 && b >= 0);
	return shapelets_beta<T>()(m, x / b) * shapelets_beta<T>()(n, y / b) / b;
      }
    };


    //FIXME: ima should be non_vectorial
    template <class I>
    std::vector<double>
    shapelets(const oln::abstract::image_with_dim<2, I>& ima,
			 int m, int n,
			 double b)
    {
      assert(m >= 0 && n >= 0 && b >= 0);
      shapelets_basis<2, double> func;

      // Center of the image
      const int col = ima.ncols() / 2;
      const int row = ima.nrows() / 2;

      std::vector<double> res(m * n);

      // For all vector in the base of shapelets
      for (int k = 0; k < m; k++)
	for (int l = 0; l < n; l++)
	  // res[k * n + l] = scalar product of the image and the function
	  {
	    double s = 0.0;

	    oln_iter_type(I) it(ima);
	    for_all(it)
	      {
		s += (ima[it] - ntg_max_val(oln_value_type(I)) / 2) *
		    func(k, l, double(it.row() - row), double(it.col() - col), b);
	      }
	    res[k * n + l] = s;
	    //	    std::cout << k << " - " << l << " = " << s << std::endl;

	  }
      return res;
    }


    template <class D>
    image2d<D>
    unshapelets(const std::vector<double> &vec,
		int m, int n, double b,
		coord nrows, coord ncols)
    {
      precondition(nrows >= 1 && ncols >= 1  &&
		   m >= 0 && n >= 0 && b >= 0);


      typedef image2d<double> tmp_type;
      tmp_type resf(oln::image2d_size(nrows, ncols, 0));

      oln_iter_type(tmp_type) it(resf);

      oln::level::fill(resf, ntg_max_val(D) / 2);
      // For each vector
      for (int k = 0; k < m; k++)
	for (int l = 0; l < n; l++)
	  // Add the value at the point
	  {
	    if (vec[k * n + l] * vec[k * n + l] > 40000)
	      {
		for_all(it)
		  resf[it] += vec[k * n + l] *
		  shapelets_basis<2, double>()(k, l, double(it.row() - nrows / 2), double(it.col() - ncols / 2), b);
		//std::cout << k << " - " << l << std::endl;
	      }
	  }

      image2d<D> res(oln::image2d_size(nrows, ncols, 0));

      for_all(it)
	{
	  if ( !(resf[it] >= 0 && resf[it] <= 255))
	    {
	      std::cout << "- ";
	      std::cout << "err:" << resf[it];
	      // 	    min = (resf[it] < min)? resf[it]:min;
	      // 	    max = (resf[it] >max)? resf[it]:max;
	      res[it] = resf[it] < 0 ? 0 : ntg_max_val(D);
	      std::cout << " -" << std::endl;
	    }
	  else
	    res[it] = resf[it];
	}
      //    std::cout << "min:" << min
      //	      << " max:" << max << std::endl;
      return res;
    }

  }
}
