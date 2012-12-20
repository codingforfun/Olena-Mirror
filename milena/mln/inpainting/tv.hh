#include <mln/core/alias/window2d.hh>
#include <mln/core/concept/iterator.hh>
#include <mln/algebra/vec.hh>
#include <mln/convert/from_to.hh>
#include <mln/norm/l2.hh>

namespace mln
{
  namespace inpainting
  {
    struct tv
    {
      typedef float float_type;
    };

    template <typename T>
    inline
    void interval(T& v,
		  tv::float_type min,
		  tv::float_type max)
    {
      for (unsigned i = 0; i < mln_dim(T); ++i)
	v[i] = std::max(std::min(v[i], max), min);
    }

    template <typename I, typename M>
    void tv(I& u,
	    const I& u_x,
	    const I& u_y,
	    const M& mask,
	    tv::float_type& max_diff_norm)
    {
      typedef tv::float_type float_type;

      extension::adjust_duplicate(u_x, 1);
      extension::adjust_duplicate(u_y, 1);

      const float_type* dx_kern = make_dx_kernel<float_type>();
      const float_type* dy_kern = make_dy_kernel<float_type>();

      static window2d full3x3;
      static const bool vals[] = { 1, 1, 1,
				   1, 1, 1,
				   1, 1, 1 };
      
      convert::from_to(vals, full3x3);

      mln_pixter(I) p(u);
      mln_pixter(const M) p_m(mask);
      mln_pixter(const I) p_x(u_x);
      mln_pixter(const I) p_y(u_y);

      mln_qixter(const I, const window2d) q_x(p_x, full3x3);
      mln_qixter(const I, const window2d) q_y(p_y, full3x3);

      for (p.start(), p_m.start(), p_x.start(), p_y.start();
	   p.is_valid();
	   p.next(), p_m.next(), p_x.next(), p_y.next())
	{
	  if (p_m.val())
	    {
	      mln_value(I) old = p.val();

	      mln_value(I) p_x = literal::zero;
	      mln_value(I) p_y = literal::zero;

	      unsigned k = 0;
	      for_all_2 (q_x, q_y)
	      {
		p_x += q_x.val() * dx_kern[k];
		p_y += q_y.val() * dy_kern[k];
		++k;
	      }
	      
	      mln_value(I) dv = p_x + p_y;

	      p.val() += dv;

	      interval(p.val(), 0.0, 255.0);
	      
	      float_type diff = norm::l2(old - p.val());
	      
	      if (diff > max_diff_norm)
		{
		  max_diff_norm = diff;
		}
	    }
	}
    }    

  } /* mln::inpainting */

} /* mln */
