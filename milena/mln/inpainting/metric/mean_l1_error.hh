#ifndef MEAN_L1_ERROR_HH_
# define MEAN_L1_ERROR_HH_

# include <mln/core/concept/image.hh>
# include <mln/norm/l1.hh>
# include <mln/trait/value_.hh>
# include <mln/algebra/vec.hh>

# include <mln/data/compute.hh>
# include <mln/accu/math/sum.hh>

#include <mln/core/image/dmorph/image_if.hh>
#include <mln/pw/value.hh>

namespace mln
{
  namespace inpainting
  {
    namespace metric
    {
      template <typename I, typename M>
      float mean_l1_error(const Image<I>& original_,
			  const Image<I>& altered_,
			  const Image<M>& mask_)
      {
	const I& original = exact(original_);
	const I& altered = exact(altered_);
	const M& mask = exact(mask_);

	mln_piter(I) p( original.domain() );
	
	unsigned card = 0;

	float error = 0;

	for_all(p)
	{
	  if (mask(p))
	    {
	      const mln_value(I) diff = original(p) - altered(p);
	      
	      for (unsigned i = 0; i < mln_dim(mln_value(I)); ++i)
		{
		  error += std::abs(diff[i]);
		}

	      ++card;
	    }
	}

	return error / (card * mln_dim(mln_value(I)));
      }
    }
  }
}

#endif
