#include <algorithm>
#include <vector>
#include <limits>

#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/fill.hh>
#include <mln/literal/colors.hh>
#include <mln/literal/zero.hh>

#include <mln/linear/convolve.hh>
#include <mln/linear/gaussian.hh>
#include <mln/core/alias/w_window2d_float.hh>

#include <mln/core/alias/neighb2d.hh>

#include <mln/pw/value.hh>

#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/data/paste.hh>

#include <mln/core/alias/vec2d.hh>

#include <mln/inpainting/derivative_kernels.hh>

#include <mln/inpainting/fast_isotropic_diffusion.hh>
#include <mln/inpainting/mean_diffusion.hh>
#include <mln/inpainting/tv.hh>

#include <mln/core/concept/accumulator.hh>

#include <mln/trait/value_.hh>

#include <mln/extension/adjust_duplicate.hh>

#include <mln/norm/l2.hh>
#include <mln/value/int_u8.hh>

namespace mln
{
  namespace inpainting
  {
    struct lpde
    {
    public:
      typedef float float_type;
      
      lpde(float dt = 1.0f, float dx = 1.0f, float t_f = 20.0);

      template <template <typename> class I, typename T>
      void operator()(I<T>& src,
		      const I<bool>& mask);
      
      void set(const std::vector<float_type>& w);
      
    private:
      std::vector<float_type> w;
      float dt;
      float dx;
      float t_f;
    };

    lpde::lpde(float dt, float dx, float t_f)
    {
      this->dt = dt;
      this->dx = dx;
      this->t_f = t_f;
    }

    template <typename T>
    inline
    T components_product(trait::value::nature::scalar,
			 T a,
			 T b)
    {
      return a * b;
    }

    template <typename T>
    T components_product(trait::value::nature::vectorial,
			 T a,
			 T b)
    {
      T c;

      for (unsigned i = 0; i < mln_dim(T); ++i)
	c[i] = a[i] * b[i];

      return c;
    }

    template <typename T>
    inline
    T components_product(T a,
			 T b)
    {
      return components_product(mln_trait_value_nature(T)(), a, b);
    }

    template <typename T>
    T normalize(T v)
    {
      lpde::float_type magnitude = norm::l2(v);

      magnitude = std::max((lpde::float_type)1.0, magnitude);

      return v / magnitude;
    }

    template <typename I, typename M>
    lpde::float_type diffuse(const I& u,
			     I& u_next,
			     const std::vector<lpde::float_type>& w,
			     const M& mask,
			     lpde::float_type& max_diff_norm,
			     I& u_x,
			     I& u_y)
    {
      typedef lpde::float_type float_type;
      typedef mln_value(I) T;

      const float_type activation_threshold = 0;
      
      const unsigned derivative_kernels_size = 5;
      const float_type* derivative_kernel[derivative_kernels_size];
      
      derivative_kernel[0] = make_dx_kernel<float_type>();
      derivative_kernel[1] = make_dy_kernel<float_type>();
      derivative_kernel[2] = make_dxx_kernel<float_type>();
      derivative_kernel[3] = make_dyy_kernel<float_type>();
      derivative_kernel[4] = make_dxy_kernel<float_type>();

      static window2d full3x3;
      static const bool vals[] = { 1, 1, 1,
				   1, 1, 1,
				   1, 1, 1 };
      
      convert::from_to(vals, full3x3);

      mln_pixter(const I) p(u);
      mln_pixter(I) p_next(u_next);
      mln_pixter(const M) p_m(mask);

      mln_pixter(I) pu_x(u_x);
      mln_pixter(I) pu_y(u_y);

      mln_qixter(const I, const window2d) q(p, full3x3);

      float_type sse = 0;

      for (p.start(), p_next.start(), p_m.start(),
	     pu_x.start(), pu_y.start();
      	   p.is_valid();
      	   p.next(), p_next.next(), p_m.next(),
	     pu_x.next(), pu_y.next())
	{
	  if (p_m.val())
	  {
	    const T old = p.val();

	    if (fabs(w[1]) > activation_threshold)
	      p_next.val() = p.val() * w[1];

	    T p_derivative[derivative_kernels_size];

	    for (unsigned i = 0; i < derivative_kernels_size; ++i)
	      p_derivative[i] = literal::zero;

	    unsigned k = 0;
	    for_all (q)
	    {
	      for (unsigned i = 0; i < derivative_kernels_size; ++i)
		{
		  p_derivative[i] += q.val() * derivative_kernel[i][k];
		}
	      ++k;
	    }

	    const T& p_x = p_derivative[0];
	    const T& p_y = p_derivative[1];
	    const T& p_xx = p_derivative[2];
	    const T& p_yy = p_derivative[3];
	    const T& p_xy = p_derivative[4];

	    const T p_x2 = components_product(p_x, p_x);
	    const T p_y2 = components_product(p_y, p_y);
	    const T p_xx2 = components_product(p_xx, p_xx);
	    const T p_yy2 = components_product(p_yy, p_yy);
	    const T p_xy2 = components_product(p_xy, p_xy);

	    pu_x.val() = normalize(p_x);
	    pu_y.val() = normalize(p_y);

	    //||∇u||^2
	    if (fabs(w[2]) > activation_threshold)
	      p_next.val() += w[2] * (p_x2 + p_y2);

	    // laplacian
	    if (fabs(w[3]) > activation_threshold)
	      p_next.val() += w[3] * (p_xx + p_yy);
	
	    // tr(H^2(u))
	    if (fabs(w[4]) > activation_threshold)
	      p_next.val() += w[4] * (p_xx2 + 2 * p_xy2 + p_yy2);

	    // (∇u)^T H_u (∇u)
	    if (fabs(w[5]) > activation_threshold)
	      p_next.val() += w[5] * (components_product(p_x2, p_xx)
				      + 2 * components_product(components_product(p_x, p_y), p_xy)
				      + components_product(p_y2, p_yy));

	    float_type diff = norm::l2(old - p_next.val());
	    sse += diff;

	    if (diff > max_diff_norm)
	      {
		max_diff_norm = diff;
	      }
	  }
	}

      inpainting::tv(u_next, u_x, u_y, mask, max_diff_norm);

      return sse;
    }

    template <template <typename> class I, typename T>
    void lpde::operator()(I<T>& src,
			  const I<bool>& mask)
    {
      // mean_diffusion mean_diff;
      // mean_diff(src, mask);
      
      // fast_isotropic_diffusion fast_iso;
      // fast_iso(src, mask);

      I<T>& u_ = src;
      I<T> u_next_(src);
      
      extension::adjust_duplicate(u_, 1);
      extension::adjust_duplicate(u_next_, 1);

      I<T>* u = &u_;
      I<T>* u_next = &u_next_;

      float_type sse = std::numeric_limits<float_type>::max() / 2.0;

      float_type sse_old = std::numeric_limits<float_type>::max();

      float_type max_diff_norm = std::numeric_limits<float_type>::max();

      float_type min_max_diff_norm = std::numeric_limits<float_type>::max();

      I<T> u_x(u_.domain());
      I<T> u_y(u_.domain());

      float t = 0;
      while (t < this->t_f
	     && max_diff_norm > CONVERGENCE_THRESHOLD
	     && !std::isnan(max_diff_norm)
	     )
	{

	  max_diff_norm = 0;

	  sse_old = sse;

	  sse = diffuse(*u, *u_next, w, mask, max_diff_norm, u_x, u_y);

	  if (max_diff_norm < min_max_diff_norm)
	    min_max_diff_norm = max_diff_norm;

	  std::swap(u, u_next);

	  t += this->dt;
	}

      std::cout << t << "\t";
    }

    void lpde::set(const std::vector<float_type>& w)
    {
      this->w = w;
    }
  } /* mln::inpainting */

} /* mln */
