#ifndef TV_HH_
# define TV_HH_

#include <mln/core/alias/window2d.hh>
#include <mln/core/concept/iterator.hh>
#include <mln/algebra/vec.hh>
#include <mln/convert/from_to.hh>
#include <mln/norm/l1.hh>
#include <mln/norm/l2.hh>
#include <mln/inpainting/derivative_kernels.hh>

#include <mln/extension/adjust_duplicate.hh>
#include <mln/inpainting/fast_isotropic_diffusion.hh>

namespace mln
{
  namespace inpainting
  {
    struct tv
    {
    public:
      tv(unsigned max_iterations = 10000)
	: max_iterations_ (max_iterations)
      {
      }

      typedef float float_type;

      template <template <typename> class I, typename T>
      void operator()(I<T>& src,
		      const I<bool>& mask);

    private:
      unsigned max_iterations_;
    };

    namespace internal
    {
      template <typename T, typename V>
      inline
      void interval(T& v,
		    const V& min,
		    const V& max)
      {
	for (unsigned i = 0; i < mln_dim(T); ++i)
	  v[i] = std::max(std::min((V) v[i], max), min);
      }

      template <typename T>
      inline
      void normalize_linf(T& u,
			  T& v)
      {
	for (unsigned i = 0; i < mln_dim(T); ++i)
	  {
	    float norm = std::sqrt(u[i] * u[i] + v[i] * v[i]);

	    norm = std::max(1.0f, norm);

	    u[i] /= norm;
	    v[i] /= norm;
	  }
      }

      template <typename I, typename T>
      void linear(I& y,
		  const I& x,
		  const T& a,
		  const T& b,
		  const mln_value(I)& c = literal::zero)
      {
	mln_pixter(const I) p_x(x);
	mln_pixter(I) p_y(y);

	for_all_2 (p_x, p_y)
	  {
	    p_y.val() = a * p_y.val() + b * p_x.val() + c;
	  }
      }

      template <typename I>
      void convolve(const I& src,
		    I& ima,
		    const float* kernel)
      {
	static window2d full3x3;
	static const bool vals[] = { 1, 1, 1,
				     1, 1, 1,
				     1, 1, 1 };

	convert::from_to(vals, full3x3);
     
	mln_pixter(I) o(ima);
	mln_pixter(const I) p(src);
	mln_qixter(const I, const window2d) q(p, full3x3);

	for_all_2 (p, o)
	  {
	    mln_value(I) accu = literal::zero;

	    unsigned k = 0;
	    for_all (q)
	    {
	      accu += q.val() * kernel[k];
	      ++k;
	    }

	    o.val() = accu;
	  }
      }

      // debug
      template <typename I, typename M>
      void print_first_value(const I& src, const M& mask)
      {
	for (unsigned i = 0; i < src.nelements(); ++i)
	  {
	    point2d p = src.point_at_index(i);

	    if (mask(p))
	      {
		std::cout << p << std::endl;
		std::cout << src(p) << std::endl;
		return;
	      }
	  }   
      }

      template <typename I>
      float max_diff_norm(const I& u, const I& old)
      {
	float max = 0;

	mln_pixter(const I) p(u);
	mln_pixter(const I) p_old(old);

	for_all_2 (p, p_old)
	  {
	    max = std::max(max, norm::l1(p.val() - p_old.val()));
	  }

	return max * max; // l2 norm
      }
    }

    // TV regularization inpainting with PrimalDual algorithm
    template <template <typename> class I, typename T>
    void tv::operator()(I<T>& src,
			const I<bool>& mask)
    {
      typedef float float_type;

      const float_type* dx_bkern = make_backward_horizontal_kernel<float_type>();
      const float_type* dy_bkern = make_backward_vertical_kernel<float_type>();
      const float_type* dx_fkern = make_forward_horizontal_kernel<float_type>();
      const float_type* dy_fkern = make_forward_vertical_kernel<float_type>();

      const float_type sigma = 1.0f / std::sqrt(8.0f);
      
      I<T> u_x;
      initialize(u_x, src);

      I<T> u_y;
      initialize(u_y, src);
      
      I<T> u_x2;
      initialize(u_x2, src);
      I<T> u_y2;
      initialize(u_y2, src);

      I<T> u = duplicate(src);

      I<T> old = duplicate(u);

      I<T> u_aux = duplicate(u);

      internal::convolve(u, u_x, dx_fkern);
      internal::convolve(u, u_y, dy_fkern);

      float_type max_diff = std::numeric_limits<float_type>::max();

      unsigned iterations = 0;

      while ((max_diff > CONVERGENCE_THRESHOLD)
      	     && (iterations < this->max_iterations_))
      	{
      	  internal::convolve(u_aux, u_x2, dx_fkern);
      	  internal::convolve(u_aux, u_y2, dy_fkern);

      	  internal::linear(u_x, u_x2, 1.0f, sigma);
      	  internal::linear(u_y, u_y2, 1.0f, sigma);

      	  mln_pixter(I<T>) p_x(u_x);
      	  mln_pixter(I<T>) p_y(u_y);

      	  for_all_2 (p_x, p_y)
      	    {
      	      internal::normalize_linf(p_x.val(), p_y.val());
      	    }

      	  // divergence
      	  {
      	    internal::convolve(u_x, u_x2, dx_bkern);
      	    internal::convolve(u_y, u_y2, dy_bkern);

      	    internal::linear(u, u_x2, 0.f, 1.f);
      	    internal::linear(u, u_y2, 1.f, 1.f);
      	  }

      	  internal::linear(u, u, sigma, 0.0f);
      	  internal::linear(u, old, 1.0f, 1.0f);

      	  mln_pixter(I<T>) p(u);
      	  mln_pixter(const I<T>) p_src(src);
      	  mln_pixter(const I<bool>) p_m(mask);

      	  for_all_3 (p, p_src, p_m)
      	    {
      	      if (!p_m.val())
      	      	{
      	      	  p.val() = p_src.val();
      	      	}
      	      else
      		{
      		  internal::interval(p.val(), 0.0f, 1.0f);
      		}
      	    }

      	  internal::linear(u_aux, u, 0.0f, 2.0f);
      	  internal::linear(u_aux, old, 1.0f, -1.0f);

      	  max_diff = internal::max_diff_norm(u, old);

      	  data::paste(u, old);

      	  ++iterations;
      	}

      data::paste(u, src);
    }

    template <template <typename> class I, typename T>
    struct tv_step
    {
    public:
      typedef float float_type;

      tv_step(I<T>& src,
	      const I<bool>& mask_)
	: mask (mask_)
      {
	this->src = duplicate(src);
	
	initialize(u_x, src);
	initialize(u_y, src);
      	initialize(u_x2, src);
	initialize(u_y2, src);

	u = duplicate(src);
	u_aux = duplicate(u);

	dx_bkern = make_backward_horizontal_kernel<float_type>();
	dy_bkern = make_backward_vertical_kernel<float_type>();
	dx_fkern = make_forward_horizontal_kernel<float_type>();
	dy_fkern = make_forward_vertical_kernel<float_type>();

	sigma = 1.0f / std::sqrt(8.0f);
      }

      void operator()(I<T>& ima)
      {
	internal::convolve(u_aux, u_x2, dx_fkern);
	internal::convolve(u_aux, u_y2, dy_fkern);

	internal::linear(u_x, u_x2, 1.0f, sigma);
	internal::linear(u_y, u_y2, 1.0f, sigma);

	mln_pixter(I<T>) p_x(u_x);
	mln_pixter(I<T>) p_y(u_y);

	for_all_2 (p_x, p_y)
	  {
	    internal::normalize_linf(p_x.val(), p_y.val());
	  }

	// divergence
	{
	  internal::convolve(u_x, u_x2, dx_bkern);
	  internal::convolve(u_y, u_y2, dy_bkern);

	  internal::linear(u, u_x2, 0.f, 1.f);
	  internal::linear(u, u_y2, 1.f, 1.f);
	}

	internal::linear(u, u, sigma, 0.0f);
	internal::linear(u, ima, 1.0f, 1.0f);

	mln_pixter(I<T>) p(u);
	mln_pixter(const I<T>) p_src(src);
	mln_pixter(const I<bool>) p_m(mask);

	for_all_3 (p, p_src, p_m)
	  {
	    if (!p_m.val())
	      {
		p.val() = p_src.val();
	      }
	    else
	      {
		internal::interval(p.val(), 0.0f, 1.0f);
	      }
	  }

	internal::linear(u_aux, u, 0.0f, 2.0f);
	internal::linear(u_aux, ima, 1.0f, -1.0f);

	float max_diff = internal::max_diff_norm(u, ima);

	data::paste(u, ima);
      }

    private:
      I<T> src;
      const I<bool>& mask;

      I<T> u_x;
      I<T> u_y;
      
      I<T> u_x2;
      I<T> u_y2;

      I<T> u;
      I<T> u_aux;

      const float_type* dx_bkern;
      const float_type* dy_bkern;
      const float_type* dx_fkern;
      const float_type* dy_fkern;
      
      float_type sigma;
    };

  } /* mln::inpainting */

} /* mln */

#endif
