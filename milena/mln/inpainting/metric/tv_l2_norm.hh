#ifndef TV_L2_NORM_HH_
# define TV_L2_NORM_HH_

# include <mln/core/concept/image.hh>
# include <mln/trait/value_.hh>
# include <mln/algebra/vec.hh>

# include <mln/inpainting/derivative_kernels.hh>

namespace mln
{
  namespace inpainting
  {
    namespace metric
    {
      template <typename I, typename M>
      double tv_l2_norm(const Image<I>& ima_,
			const Image<M>& mask_)
      {
	I dx;
	I dy;

	const I& ima = exact(ima_);

	initialize(dx, ima);
	initialize(dy, ima);

	internal::convolve(ima, dx, make_dx_kernel<float>());
	internal::convolve(ima, dy, make_dy_kernel<float>());

	mln_piter(I) p( dx.domain() );
	
	double sum = 0;

	const M& mask = exact(mask_);

	for_all(p)
	{
	  if (mask(p))
	    {
	      for (unsigned i = 0; i < mln_dim(mln_value(I)); ++i)
		{
		  const float& a = dx(p)[i];
		  const float& b = dy(p)[i];
		  
		  sum += std::sqrt(a * a + b * b);
		}
	    }
	}

	return sum;
      }
    }
  }
}

#endif
