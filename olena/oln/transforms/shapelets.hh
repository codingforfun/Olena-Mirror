#include <ntg/int.hh>
#include <oln/basics2d.hh>


#include <oln/core/image.hh>
#include <oln/core/point1d.hh>
#include <oln/core/point2d.hh>
#include <oln/level/fill.hh>
#include <vector>

namespace oln
{
  inline double
  hermite(int n, double x)
  {
    assert(n >= 0);
    if (n == 0)
      return 1;
    if (n == 1)
      return 2. * x;
    return 2. * (x * hermite(n - 1, x) - (n - 1) * hermite(n - 2, x));
  }

  inline double
  fact(double n)
  {
    precondition(n >= 0);

    double res = n;

    if (n <= 1)
      return 1;
    while (n > 1)
      {
	res *= n;
	n--;
      }
    return res;
  }

  inline double
  beta(int n, double x)
  {
    assert(n >= 0);
    const double c = sqrt(pow(2., n) * sqrt(M_PI) * fact(n));

    assert(finite(hermite(n, x) * exp(x * x / -2.0) / c));
    return hermite(n, x) * exp(x * x / -2.0) / c;
  }



  inline double
  shapelet_basis_1d(int n, point1d p, double b)
  {
    assert(n >= 0 && b >= 0);
    return beta(n, p.col() / b) / sqrt(b);
  }

  double
  shapelet_basis_2d(int m, int n, point2d p, double b)
  {
    assert(m >= 0 && n >= 0 && b >= 0);
    return beta(m, p.col() / b) * beta(n, p.row() / b) / b;
  }

  //FIXME: ima should be non_vectorial
  template <class I>
  std::vector<double>
  to_shapelet_basis_2d(const oln::abstract::image_with_dim<2, I>& ima,
		       int m, int n,
		       double b)
  {
    assert(m >= 0 && n >= 0 && b >= 0);

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
		shapelet_basis_2d(k, l, point2d(it.row() - row, it.col() - col), b);
	    }
	  res[k * n + l] = s;
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

    oln::level::fill(resf, ntg_max_val(D) / 2 );
    // For each vector
    for (int k = 0; k < m; k++)
      for (int l = 0; l < n; l++)
	// Add the value at the point
	for_all(it)
	  resf[it] += vec[k * n + l] *
	  shapelet_basis_2d(k, l, point2d(it.row() - nrows / 2, it.col() - ncols / 2), b);

    image2d<D> res(oln::image2d_size(nrows, ncols, 0));

    for_all(it)
      {
	if ( !(resf[it] >= 0 && resf[it] < 256))
	  {
	    std::cout << "err:" << resf[it] << std::endl;
// 	    min = (resf[it] < min)? resf[it]:min;
// 	    max = (resf[it] >max)? resf[it]:max;
	    res[it] = resf[it] < 0 ? 0 : ntg_max_val(D);
	  }
	else
	  res[it] = resf[it];
      }
    //    std::cout << "min:" << min
    //	      << " max:" << max << std::endl;
    return res;
  }

}
