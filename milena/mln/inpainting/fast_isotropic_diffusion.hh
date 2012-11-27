#include <mln/core/macros.hh>
#include <mln/core/concept/image.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/pw/value.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/literal/zero.hh>
#include <mln/norm/l2.hh>

#include <algorithm>
#include <limits>

#define CONVERGENCE_THRESHOLD	0.05

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
		      const I<bool>& mask);
    };

    template <template <typename> class I, typename T>
    void fast_isotropic_diffusion::operator()(I<T>& src,
					      const I<bool>& mask)
    {
      I<T> src_next(src);

      I<T>* u_ = &src;
      I<T>* u_next_ = &src_next;

      float_type max_diff_norm = std::numeric_limits<float_type>::max();
      unsigned iterations = 0;

      while (max_diff_norm > CONVERGENCE_THRESHOLD)
	{
	  ++iterations;
	  max_diff_norm = 0;

	  const I<T>& u = *u_;
	  I<T>& u_next = *u_next_;

	  mln_pixter(const I<T>) p(u);
	  mln_pixter(I<T>) p_next(u_next);
	  mln_pixter(const I<bool>) p_m(mask);
	  mln_nixter(const I<T>, const neighb2d) n(p, c8());

	  for_all_3(p, p_next, p_m)
	  {
	    if (p_m.val())
	      {
	    	T diff = p.val();

		T accu = literal::zero;

	    	for_all(n)
		  accu += ((float_type) 0.125) * n.val();

		p_next.val() = accu;

	    	diff -= p_next.val();

		float_type diff_norm = norm::l2(diff);
	    	if (diff_norm > max_diff_norm)
		  {
	    	    max_diff_norm = diff_norm;
	    	  }
	      }
	  }

	  std::swap(u_, u_next_);
	  
	}

      std::cout << iterations << "\t";
    }

  } /* mln::inpainting */

} /* mln */
