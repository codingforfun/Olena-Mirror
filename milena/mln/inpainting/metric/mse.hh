#ifndef MSE_HH_
# define MSE_HH_

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
      template <typename I, typename I2>
      double mse(const Image<I>& original_,
		  const Image<I2>& altered_)
      {
	const I& original = exact(original_);
	const I2& altered = exact(altered_);

	mln_piter(I) p( altered.domain() );

	double mse = 0;

	const unsigned dim = mln_dim(mln_value(I));
	
	unsigned nelements = 0;

	for_all(p)
	{
	  const mln_sum(mln_value(I)) diff = original(p) - altered(p);
	  
	  for (unsigned i = 0; i < dim; ++i)
	    mse += diff[i] * diff[i];

	  ++nelements;
	}

	mse /= (nelements * dim);

	return mse;
      }
    }
  }
}

#endif
