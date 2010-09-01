#ifndef UTILS_FRONTIERE_HH
# define UTILS_FRONTIERE_HH

# include "h_median.hh"
# include <mln/core/image/image2d.hh>



namespace mln
{

  namespace utils
  {


    template <typename T>
    T
    frontiere_median(const image2d<T>& f, bool& going_up)
    {
      utils::h_median<T> m;
      const int
	nrows = f.nrows(),
	nrows_1 = nrows - 1,
	ncols = f.ncols(),
	ncols_1 = ncols - 1;

      // ------
      //
      // ------
      for (int col = 0; col < ncols; ++col)
	{
	  m.take(f.at_(0, col));
	  m.take(f.at_(nrows_1, col));
	}

      // ------
      // |    |
      // ------
      for (int row = 1; row < nrows_1; ++row)
	{
	  m.take(f.at_(row, 0));
	  m.take(f.at_(row, ncols_1));
	}

      // m.println();

      going_up = m.goes_up();
      return m.value();
    }



    // initialize_frontiere

    template <typename R, typename Q>
    void
    initialize_frontiere(image2d<R>& K, Q& q, image2d<bool>& deja_vu)
    {
      const int
	nrows = K.nrows(),
	nrows_1 = nrows - 1,
	ncols = K.ncols(),
	ncols_1 = ncols - 1;

      point2d p;
      short& row = p.row();
      short& col = p.col();

      // top
      row = 0;
      for (col = 0; col < ncols; ++col)
	{
	  K(p).set(q.level());
	  q.push_element_at_current_level(p);
	  deja_vu(p) = true;
	}

      // bot
      row = nrows_1;
      for (col = 0; col < ncols; ++col)
	{
	  K(p).set(q.level());
	  q.push_element_at_current_level(p);
	  deja_vu(p) = true;
	}

      // left
      col = 0;
      for (row = 1; row < nrows_1; ++row)
	{
	  K(p).set(q.level());
	  q.push_element_at_current_level(p);
	  deja_vu(p) = true;
	}

      // right
      col = ncols_1;
      for (row = 1; row < nrows_1; ++row)
	{
	  K(p).set(q.level());
	  q.push_element_at_current_level(p);
	  deja_vu(p) = true;
	}
    }



    // set_frontiere

    template <typename T>
    void
    set_frontiere(image2d<T>& f, T v)
    {
      const int
	nrows = f.nrows(),
	nrows_1 = nrows - 1,
	ncols = f.ncols(),
	ncols_1 = ncols - 1;

      // ------
      //
      // ------
      for (int col = 0; col < ncols; ++col)
	{
	  f.at_(0, col) = v;
	  f.at_(nrows_1, col) = v;
	}

      // ------
      // |    |
      // ------
      for (int row = 1; row < nrows_1; ++row)
	{
	  f.at_(row, 0) = v;
	  f.at_(row, ncols_1) = v;
	}
    }


    /*

    template <typename T, typename Q>
    void
    push_frontiere(const image2d<T>& f,
		   Q& q,
		   image2d<bool>& deja_vu)
    {
      const int
	nrows = f.nrows(),
	nrows_1 = nrows - 1,
	ncols = f.ncols(),
	ncols_1 = ncols - 1;
      unsigned p,
	dp = f.delta_index(dpoint2d(1, 0));

      // top
      p = f.index_of_point(point2d(0, 0));
      for (int col = 0; col < ncols; ++col, ++p)
	{
	  q.push(p, f.element(p));
	  deja_vu.element(p) = true;
	}

      // bot
      p = f.index_of_point(point2d(nrows_1, 0));
      for (int col = 0; col < ncols; ++col, ++p)
	{
	  q.push(p, f.element(p));
	  deja_vu.element(p) = true;
	}

      // left
      p = f.index_of_point(point2d(1, 0));
      for (int row = 1; row < nrows_1; ++row, p += dp)
	{
	  q.push(p, f.element(p));
	  deja_vu.element(p) = true;
	}

      // right
      p = f.index_of_point(point2d(1, ncols_1));
      for (int row = 1; row < nrows_1; ++row, p += dp)
	{
	  q.push(p, f.element(p));
	  deja_vu.element(p) = true;
	}
    }

*/


  } // end of namespace mln::utils

} // end of namespace mln


#endif // ! UTILS_FRONTIERE_HH
