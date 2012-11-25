#include <mln/core/macros.hh>
#include <mln/core/concept/image.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/pw/value.hh>
#include <mln/core/alias/neighb2d.hh>

#define CONVERGENCE_THRESHOLD	1

namespace mln
{
  namespace inpainting
  {
    struct fast_isotropic_diffusion
    {
    public:
      typedef double float_type;

      template <template <typename> class I, typename T>
      void operator()(I<T>& src,
		      I<bool>& mask);
    };

    template <template <typename> class I, typename T>
    void fast_isotropic_diffusion::operator()(I<T>& src,
					      I<bool>& mask)
    {
      mln_piter(I<T>) p(src.domain());
      mln_niter(neighb2d) n(c8(), p);

      int max_diff = 1;
      int iterations = 0;

      while (max_diff > 0)
	{
	  ++iterations;

	  max_diff = 0;

	  for_all(p)
	  {
	    if (mask(p))
	      {
		int diff = src(p)[0];

		float_type r = 0;
		float_type g = 0;
		float_type b = 0;

		for_all(n)
		{
		  r += 0.125f * src(n)[0];
		  g += 0.125f * src(n)[1];
		  b += 0.125f * src(n)[2];
		}

		src(p)[0] = r;
		src(p)[1] = g;
		src(p)[2] = b;

		diff -= src(p)[0];

		if (diff < 0)
		  diff = -diff;

		if (diff > max_diff)
		  {
		    max_diff = diff;
		  }
	      }
	  }
	}

      std::cout << iterations << "\t";
    }

  } /* mln::inpainting */

} /* mln */
