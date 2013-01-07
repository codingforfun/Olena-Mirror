#ifndef MEAN_GRADIENT_ERROR_HH_
# define MEAN_GRADIENT_ERROR_HH_

# include <mln/core/concept/image.hh>
# include <mln/norm/l1.hh>
# include <mln/trait/value_.hh>
# include <mln/algebra/vec.hh>
# include <mln/linear/gaussian.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>

# include <mln/data/compute.hh>
# include <mln/data/transform.hh>
# include <mln/fun/vv2v/diff_abs.hh>
# include <mln/accu/math/sum.hh>

# include <mln/inpainting/tv.hh>

namespace mln
{
  namespace inpainting
  {
    namespace metric
    {
      template <typename I, typename M>
      float mean_gradient_error(const Image<I>& original_,
				const Image<I>& altered_,
				const Image<M>& mask_)
      {
	I original_dx;
	I original_dy;
	I altered_dx;
	I altered_dy;

	initialize(original_dx, exact(original_));
	initialize(original_dy, exact(original_));
	initialize(altered_dx, exact(original_));
	initialize(altered_dy, exact(original_));

	internal::convolve(exact(original_), original_dx, make_dx_kernel<float>());
	internal::convolve(exact(altered_), altered_dx, make_dx_kernel<float>());
	internal::convolve(exact(original_), original_dy, make_dy_kernel<float>());
	internal::convolve(exact(altered_), altered_dy, make_dy_kernel<float>());

	unsigned card = 0;

	mln_piter(I) p( original_dx.domain() );
	
	float error = 0;

	const M& mask = exact(mask_);

	for_all(p)
	{
	  if (mask(p))
	    {
	      const mln_value(I) diff = original_dx(p) + original_dy(p)
		- (altered_dx(p) + altered_dy(p));
	      
	      for (unsigned i = 0; i < mln_dim(mln_value(I)); ++i)
		error += std::abs(diff[i]);

	      ++card;
	    }
	}

	return error / (card * mln_dim(mln_value(I)));
      }
    }
  }
}


#endif
