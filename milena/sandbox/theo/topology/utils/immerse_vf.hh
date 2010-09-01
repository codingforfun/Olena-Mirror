#ifndef UTILS_IMMERSE_VF
# define UTILS_IMMERSE_VF

# include "range.hh"
# include <mln/core/image/image2d.hh>


namespace mln
{

  namespace utils
  {


    // So that pixels can communicate:
    //                                        _                    _
    // [ min(|_ (f(p) + f(n_i)) / 2 _|, max( | (f(p) + f(n_i)) / 2) | ]

    template <typename T, typename N>
    image2d< range<T> >
    immerse_vf(const image2d<T>& f, const N& nbh)
    {
      typedef image2d<T> I;
      typedef range<T>   R;

      image2d<R> K;
      initialize(K, f);

      util::array<int> dp = offsets_wrt(K, nbh);
      const unsigned n_nbhs = dp.nelements();

      T min, max, vp, vn;

      mln_pixter(const I) p(f);
      mln_nixter(const I, N) n(p, nbh);
      for_all(p)
	{
	  vp = p.val();
	  min = max = vp;
	  for_all(n)
	    {
	      vn = n.val();
	      if (vn < min)
		min = vn;
	      if (vn > max)
		max = vn;
	    }
	  unsigned upper = (max + vp) / 2;
	  // if (max % 2 != vp % 2)    // ex: (3 + 5) / 2 = 4, yet we shall round up...
	    if (upper < mln_max(T)) // ...if possible!
	      ++upper;
	  unsigned lower = (min + vp) / 2;
	  if (lower > 0)
	    --lower;
	  K.element(p.offset()) = R(lower, upper);
	}
      return K;
    }



    // Similar as the dynamic (so gradient):
    // interval = [ inf, sup ]

    template <typename T, typename N>
    image2d< range<T> >
    immerse_vf__dyn(const image2d<T>& f, const N& nbh)
    {
      typedef image2d<T> I;
      typedef range<T>   R;

      image2d<R> K;
      initialize(K, f);

      util::array<int> dp = offsets_wrt(K, nbh);
      const unsigned n_nbhs = dp.nelements();

      T min, max, v;

      mln_pixter(const I) p(f);
      mln_nixter(const I, N) n(p, nbh);
      for_all(p)
	{
	  v = p.val();
	  min = max = v;
	  for_all(n)
	    {
	      if (n.val() < min) min = n.val();
	      if (n.val() > max) max = n.val();
	    }
	  K.element(p.offset()) = R(min, max);
	}

      return K;
    }



    // 'half' looks good but it's not grounded/sound...

    template <typename T, typename N>
    image2d< range<T> >
    immerse_vf__half(const image2d<T>& f, const N& nbh)
    {
      typedef image2d<T> I;
      typedef range<T>   R;

      image2d<R> K;
      initialize(K, f);

      util::array<int> dp = offsets_wrt(K, nbh);
      const unsigned n_nbhs = dp.nelements();

      T min, max, v;

      mln_pixter(const I) p(f);
      mln_nixter(const I, N) n(p, nbh);
      for_all(p)
	{
	  v = p.val();
	  min = max = v;
	  for_all(n)
	    {
	      if (n.val() < min) min = n.val();
	      if (n.val() > max) max = n.val();
	    }
	  unsigned dmin = v - min, dmax = max - v;
	  K.element(p.offset()) = R(v - (++dmin / 2),
				    v + (++dmax / 2));
	}

      return K;
    }



  } // end of namespace mln::utils

} // end of namespace mln


#endif // ! UTILS_IMMERSE_VF
