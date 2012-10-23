#ifndef MLN_INPAINTING_METRIC_PSNR_HH
# define MLN_INPAINTING_METRIC_PSNR_HH

# include <cmath>

# include <mln/core/concept/image.hh>
# include <mln/norm/l1.hh>
# include <mln/trait/value_.hh>
# include <mln/algebra/vec.hh>

namespace mln
{
  namespace inpainting
  {
    namespace metric
    {
      template <typename I>
      double psnr(const Image<I>& original_,
		  const Image<I>& altered_)
      {
	const I& original = exact(original_);
	const I& altered = exact(altered_);

	mln_piter(I) p( altered.domain() );

	double mse = 0;

	const unsigned dim = mln_dim(mln_value(I));
	
	for_all(p)
	{
	  const mln_sum(mln_value(I)) diff = original(p) - altered(p);

	  if (dim == 1)
	    mse += diff * diff;
	  else
	    for (unsigned i = 0; i < dim; ++i)
	      mse += diff[i] * diff[i];
	}

	mse /= ( altered.ncols() * altered.nrows() * dim);
	
	const unsigned max = pow(2, mln_nbits(mln_value(I)) / dim) - 1;

	return 20 * log10( max ) - 10 * log10(mse);
      }
    }
  }
}

#endif
