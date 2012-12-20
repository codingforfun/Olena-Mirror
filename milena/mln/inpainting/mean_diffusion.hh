#include <mln/core/macros.hh>
#include <mln/core/concept/image.hh>
#include <mln/core/alias/point2d.hh>
#include <mln/pw/value.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/core/image/dmorph/image_if.hh>

#include <mln/core/concept/function.hh>

#include <mln/data/transform.hh>

#include <mln/literal/colors.hh>
#include <mln/literal/zero.hh>
#include <mln/pw/image.hh>
#include <mln/convert/to_p_array.hh>
#include <mln/core/var.hh>

namespace mln
{
  
  namespace inpainting
  {
    
    struct mean_diffusion
    {
    public:	
      typedef float float_type;
      
      template <template <typename> class I, typename T>
      void operator()(I<T>& src,
		      const I<bool>& mask_);
    };
    
    namespace
    {
      template <typename I, typename N>
      struct contour : public Function_v2b< contour<I,N> >
      {
	typedef bool result;
	
	contour(const I& ima, const N& neighborhood)
	  : ima_ (ima),
	    neighborhood_ (neighborhood)
	{
	}

	bool operator()(const mln_psite(I)& p) const
	{
	  if (ima_(p))
	    {
	      mln_niter(N) n(neighborhood_, p);

	      for_all(n)
	      {
		if (!ima_(n))
		  return true;
	      }
	    }

	  return false;
	}

      private:
	const I& ima_;
	const N& neighborhood_;
      };

    template <template <typename> class I, typename T>
    void diffuse(I<T>& src,
		 const p_array<mln_psite(I<bool>)>& ctr,
		 I<bool>& mask,
		 const neighb2d& neighborhood)
      {

	mln_piter(p_array<mln_psite(I<bool>)>) p(ctr);
	mln_niter_(neighb2d) n(neighborhood, p);
		
	for_all(p)
	{
	  T accu = literal::zero;

	  unsigned nb = 0;

	  for_all(n)
	  {
	    if (!mask(n))
	      {
		accu += src(n);
		++nb;
	      }
	  }
	  
	  src(p) = accu / nb;
	}
      }

    } /* !mln::inpainting::{anonymous} */

    template <template <typename> class I, typename T>
    void mean_diffusion::operator()(I<T>& src,
				    const I<bool>& mask_)
    {
      const neighb2d neighborhood = c8();
	
      I<bool> mask(mask_.domain());
      data::paste(mask_, mask);

      unsigned contour_nsites = std::numeric_limits<unsigned>::max();

      while (contour_nsites > 0)
	{
	  contour<I<bool>, neighb2d> ctr = contour<I<bool>, neighb2d>(mask, neighborhood);
	  p_array<mln_psite(I<bool>)> ctr_set = convert::to_p_array(mask | ctr);
	  contour_nsites = ctr_set.nsites();

	  if (contour_nsites > 0)
	    {
	      diffuse(src, ctr_set, mask, neighborhood);

	      mln_piter(p_array<mln_psite(I<bool>)>) p(ctr_set);
	      for_all (p)
	      {
		mask(p) = false;
	      }
	    }
	}
    }
    
  } /* mln::inpainting */
  
} /* mln */

