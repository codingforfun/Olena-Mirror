#ifndef MLN_INPAINTING_METRIC_PSNR_HH
# define MLN_INPAINTING_METRIC_PSNR_HH

# include <cmath>

# include <mln/core/concept/image.hh>

# include <mln/inpainting/metric/mse.hh>

# include <mln/trait/value_.hh>

namespace mln
{
  namespace inpainting
  {
    namespace metric
    {
      template <typename I, typename I2>
      double psnr(const Image<I>& original_,
		  const Image<I2>& altered_)
      {
	double mse = metric::mse(original_, altered_);

	const unsigned dim = mln_dim(mln_value(I));

	const unsigned max = pow(2, mln_nbits(mln_value(I)) / dim) - 1;

	return 20 * log10( max ) - 10 * log10(mse);
      }
    }
  }
}

#endif
