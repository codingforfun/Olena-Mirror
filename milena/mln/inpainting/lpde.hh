#include <algorithm>
#include <vector>

#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/fill.hh>
#include <mln/literal/colors.hh>
#include <mln/literal/zero.hh>

#include <mln/linear/convolve.hh>
#include <mln/linear/gaussian.hh>
#include <mln/core/alias/w_window2d_float.hh>

#include <mln/pw/value.hh>

#include <mln/value/int_u.hh>
#include <mln/algebra/vec.hh>

#include <mln/data/paste.hh>

#include <mln/core/alias/vec2d.hh>

namespace mln
{
  namespace inpainting
  {
    struct lpde
    {
    public:
      lpde(float dt = 1.0f, float dx = 1.0f, float t_f = 20.0);

      template <template <typename> class I, typename T>
      void operator()(I<T>& src,
		      I<bool>& mask);
      
      void set(const std::vector<float>& w);
      
    private:
      std::vector<float> w;
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

    template <typename I>
    void compute_diff_invariants(const I& u,
				 std::vector<I>& invs,
				 const std::vector<float>& w)
    {
      const float sigma = 0.8;

      invs[0] = linear::gaussian_1st_derivative(u, sigma, 1);
      invs[1] = linear::gaussian_1st_derivative(u, sigma, 0);
      invs[2] = linear::gaussian_2nd_derivative(u, sigma, 1);
      invs[3] = linear::gaussian_2nd_derivative(u, sigma, 0);
      invs[4] = linear::gaussian_2nd_derivative(u, sigma);
      
      { // TV regularization term div ( \nabla u / | \nabla u | )

	I u_xnx( invs[5].domain() );
	I u_yny( invs[5].domain() );

	{
	  mln_piter(I) p( invs[5].domain() );

	  for_all(p)
	  {
	    for (unsigned i = 0; i < mln_dim(mln_value(I)); ++i)
	      {
		float norm;
		{
		  vec2d_f v;
	      
		  v[0] = invs[0](p)[i];
		  v[1] = invs[1](p)[i];
	      
		  norm = norm::l2(v);
		}

		u_xnx(p)[i] = invs[0](p)[i] / norm;
		u_yny(p)[i] = invs[1](p)[i] / norm;
	      }
	  }
	}

	u_xnx = linear::gaussian_1st_derivative(u_xnx, sigma, 1);
	u_yny = linear::gaussian_1st_derivative(u_yny, sigma, 0);	

	mln_piter(I) p( invs[5].domain() );
	for_all(p)
	{
	  invs[5](p) = u_xnx(p) + u_yny(p);
	}
      }
    }

    template <template <typename> class I, typename T>
    void lpde::operator()(I<T>& src,
			  I<bool>& mask)
    {

      data::fill((src | pw::value(mask)).rw(), literal::zero);

      std::vector<I<T> > invs(w.size(), src.domain());

      I<T>& u_t = src;

      float r = this->dt / (this->dx * this->dx);

      for (float t = 0; t < this->t_f; t += dt)
	{
	  compute_diff_invariants(u_t, invs, this->w);

	  mln_piter(I<T>) p(u_t.domain());

	  for_all(p)
	  {
	    if (mask(p))
	      {
		T contribution = literal::zero;

		for (unsigned k = 0; k < w.size(); ++k)
		  {
		    contribution += this->w[k] * invs[k](p);
		  }

		u_t(p) += r * contribution;
	      }
	  }
	}
    }

    void lpde::set(const std::vector<float>& w)
    {
      this->w = w;
    }
  } /* mln::inpainting */

} /* mln*/
