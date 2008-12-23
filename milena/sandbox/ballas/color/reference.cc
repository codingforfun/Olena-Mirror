# include <mln/core/var.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/image/extended.hh>
# include <mln/core/routine/extend.hh>

# include <mln/core/alias/window2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>
# include <mln/core/site_set/p_centered.hh>

# include <mln/literal/origin.hh>
# include <mln/literal/black.hh>
# include <mln/literal/white.hh>

# include <mln/value/int_u8.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>

# include <mln/value/rgb8.hh>
# include <mln/io/ppm/load.hh>
# include <mln/io/ppm/save.hh>

# include <mln/accu/min_max.hh>
# include <mln/accu/mean.hh>

# include <mln/fun/i2v/array.hh>
# include <mln/fun/p2v/iota.hh>

# include <mln/data/paste.hh>
# include <mln/data/fill.hh>
# include <mln/level/transform.hh>
# include <mln/extension/fill.hh>
# include <mln/convert/to.hh>

# include <mln/linear/gaussian.hh>

# include <mln/morpho/meyer_wst.hh>
# include <mln/morpho/closing_volume.hh>

# include <mln/make/w_window2d.hh>
# include <mln/opt/at.hh>

# include <mln/debug/println.hh>

// Laplacian method
namespace mln
{
  namespace linear
  {
    // required to deal with a input image that differ from the output since I
    //don't succeed in using a float image in entry.
    template <class I, class O>
    inline
    void
    gaussian_2nd_derivative(const Image<I>& input, float sigma, Image<O>& output)
    {
      mln_precondition(exact(input).has_data());

      impl::recursivefilter_coef_
        coef(-1.331f, 3.661f,
             1.24f, 1.314f,
             0.3225f, -1.738f,
             0.748f, 2.166f,
             sigma, impl::gaussian_2nd_deriv_coef_norm_);
      impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				   input, coef, sigma, output);
    }

    template <class I, class O>
    inline
    void
    gaussian_1st_derivative(const Image<I>& input, float sigma, Image<O>& output)
    {
      mln_precondition(exact(input).has_data());

      impl::recursivefilter_coef_
        coef(-0.6472f, -4.531f,
             1.527f, 1.516f,
             0.6494f, 0.9557f,
             0.6719f, 2.072f,
             sigma, impl::gaussian_1st_deriv_coef_norm_);
      impl::generic_filter_common_(mln_trait_value_nature(mln_value(I))(),
				   input, coef, sigma, output);
    }
  }
}

// Gradient + watershed method
namespace mln
{
  namespace morpho
  {
    template <typename I, typename N>
    mln_concrete(I)
    closing_volume(const I& input, const Neighborhood<N>& nbh, std::size_t lambda)
    {
      mln_concrete(I) output;
      initialize(output, input);
      closing_volume(input, nbh, lambda, output);
      return output;
    }
  }
} // !mln

namespace mln
{
  image2d<value::int_u8> convert_to_grey(const image2d<value::rgb8>& data)
  {
    image2d<value::int_u8> output(data.domain());
    mln_piter_(image2d<value::int_u8>) p(output.domain());
    for_all(p)
      output(p) = (int) (data(p).red() * 0.3 + data(p).green() * 0.58 + data(p).blue()) * 0.12;
    return output;
  }
}  // !mln

// Functions

inline
bool is_row_odd(const mln::point2d& p)
{
  return p.row() % 2;
}

inline
bool is_cell(const mln::point2d& p)
{
  return p.row() % 2 == 0 && p.col() % 2 == 0;
}

inline
bool is_edge(const mln::point2d& p)
{
  return p.row() % 2 + p.col() % 2 == 1;
}

inline
bool is_point(const mln::point2d& p)
{
  return p.row() % 2 && p.col() % 2;
}

inline
bool is_not_edge(const mln::point2d& p)
{
  return ! is_edge(p);
}



namespace mln
{

  namespace border
  {

    template <typename I>
    void
    fill(I& ima, const mln_value(I)& v)
    {
      const int nrows = ima.nrows();
      const int ncols = ima.ncols();
      for (int r = -1; r <= nrows; ++r)
	{
	  opt::at(ima, r, -1) = v;
	  opt::at(ima, r, ncols) = v;
	}
      for (int c = -1; c <= ncols; ++c)
	{
	  opt::at(ima, -1, c) = v;
	  opt::at(ima, nrows, c) = v;
	}
    }

  } // mln::border

  namespace accu
  {

    template <typename I, typename L, typename A, typename V>
    inline
    void
    compute(const Image<I>& input_,
	    const Image<L>& label_,
	    const Accumulator<A>&,
	    V& v)
    {
      trace::entering("accu::compute");

      const I& input = exact(input_);
      const L& label = exact(label_);

      const unsigned n = v.size();
      std::vector<A> a(n);

      mln_piter(I) p(input.domain());
      for_all(p)
	a[label(p)].take(input(p));

      for (unsigned l = 1; l < n; ++l)
	v(l) = a[l].to_result();

      trace::exiting("accu::compute");
    }

  } // mln::accu

  namespace morpho
  {

    template <typename I, typename N>
    mln_concrete(I)
    gradient(const I& input, const N& nbh)
    {
      mln_concrete(I) output;
      initialize(output, input);
      accu::min_max<mln_value(I)> mm;

      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
	mm.init();
	for_all(n) if (input.has(n))
	  mm.take(input(n));
	output(p) = mm.second() - mm.first();
      }
      return output;
    }

    template <typename I, typename N>
    mln_concrete(I)
    dilation(const I& input, const N& nbh)
    {
      typedef mln_value(I) V;

      mln_concrete(I) output;
      initialize(output, input);

      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
	for_all(n)
	  if (input.has(n) && input(n) != value::rgb8(0,0,0))
	    output(p) = input(n);
      }
      return output;
    }
  } // mln::morpho


  struct colorize : Function_v2v< colorize >
  {
    typedef value::rgb8 result;
    colorize(unsigned max)
      : lut(max + 1)
    {
      lut[0] = literal::black;
      for (unsigned i = 1; i <= max; ++i)
	lut[i] = result(100 + std::rand() % 150,
			100 + std::rand() % 150,
			100 + std::rand() % 150);
    }
    result operator()(unsigned i) const
    {
      return lut[i];
    }
    std::vector<result> lut;
  };


  template <typename I>
  I display_edge_with_bg(const I& ima, unsigned zoom, mln_value(I) bg)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
	     ncols * (zoom + 1) - 1);
    data::fill(output, bg);
    mln_VAR( edge, ima | is_edge );
    mln_piter(edge_t) p(edge.domain());
    for_all(p)
      if (p.row() % 2) // horizontal edge
	{
	  unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
	  unsigned col = (p.col() / 2) * (zoom + 1);
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row, col + i) = ima(p);
	}
      else // vertical edge
	{
	  unsigned row = (p.row() / 2) * (zoom + 1);
	  unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row + i, col) = ima(p);
	}
    return output;
  }

  template <typename I>
  I display_edge(const I& ima, unsigned zoom)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
	     ncols * (zoom + 1) - 1);

    mln_VAR( cell, ima | is_cell );
    mln_piter(cell_t) q(cell.domain());
    for_all(q)
    {
      unsigned row = (q.row() / 2) * (zoom + 1);
      unsigned col = (q.col() / 2) * (zoom + 1);
      for (unsigned i = 0; i < zoom; ++i)
	for (unsigned j = 0; j < zoom; ++j)
	  opt::at(output, row + i, col + j) = ima(q);
    }

    mln_VAR( edge, ima | is_edge );
    mln_piter(edge_t) p(edge.domain());
    for_all(p)
      if (p.row() % 2) // horizontal edge
	{
	  unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
	  unsigned col = (p.col() / 2) * (zoom + 1);
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row, col + i) = ima(p);
	}
      else // vertical edge
	{
	  unsigned row = (p.row() / 2) * (zoom + 1);
	  unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row + i, col) = ima(p);
	}
    return output;
  }

} // mln



template <typename T>
mln::image2d<T>
image2cells(const mln::image2d<T>& input)
{
  mln::image2d<T> output(2 * input.nrows() - 1,
			 2 * input.ncols() - 1);
  for (int row = 0; row < input.nrows(); ++row)
    for (int col = 0; col < input.ncols(); ++col)
      mln::opt::at(output, 2 * row, 2 * col) = mln::opt::at(input, row, col);
  return output;
}

namespace mln {

  template <typename I, typename N, typename M>
  mln_concrete(I)
  mean(const I& input, const N& nbh, const M& nbh2)
  {
  mln_concrete(I) output;
  initialize(output, input);

  mln_piter(I) p(input.domain());
  mln_niter(N) n(nbh, p);
  mln_niter(M) m(nbh2, p);
  for_all(p)
  {
    if (is_edge(p))
    {
      int nb = 0;
      int r = 0, g = 0, b = 0;
      for_all(n)
      {
	if (input.has(n))
	{
	  r += input(n).red();
	  g += input(n).green();
	  b += input(n).blue();
	  ++nb;
	}
      }
      output(p) = value::rgb8(r / nb, g / nb, b / nb);
    }
    if (is_point(p))
    {
      int nb = 0;
      int r = 0, g = 0, b = 0;
      for_all(m)
      {
	if (input.has(m))
	{
	  r += input(m).red();
	  g += input(m).green();
	  b += input(m).blue();
	  ++nb;
	}
      }
      output(p) = value::rgb8(r / nb, g / nb, b / nb);
    }
    if (is_cell(p))
      output(p) = input(p);
  }
  return output;
  }

}

template <typename T>
mln::image2d<T>
cells2image(const mln::image2d<T>& input)
{
  mln::image2d<T> output((input.nrows() + 1) / 2,
			 (input.ncols() + 1) / 2);
  for (int row = 0; row < input.nrows(); row += 2)
    for (int col = 0; col < input.ncols(); col += 2)
      mln::opt::at(output, row / 2, col / 2) = mln::opt::at(input, row, col);
  return output;
}




template <typename I>
mln_concrete(I)
do_it(I& input, float lambda, unsigned& nbasins)
{
  using namespace mln;

  /**************************/
  /* Neighborhood defintion */
  /**************************/

  // e2c
  bool e2c_h[] = { 0, 1, 0,
		   0, 0, 0,
		   0, 1, 0 };
  bool e2c_v[] = { 0, 0, 0,
		   1, 0, 1,
		   0, 0, 0 };
  mln_VAR( e2c, make::double_neighb2d(is_row_odd, e2c_h, e2c_v) );

  // e2e
  bool e2e_h[] = { 0, 0, 1, 0, 0,
		   0, 1, 0, 1, 0,
		   0, 0, 0, 0, 0,
		   0, 1, 0, 1, 0,
		   0, 0, 1, 0, 0 };
  bool e2e_v[] = { 0, 0, 0, 0, 0,
		   0, 1, 0, 1, 0,
		   1, 0, 0, 0, 1,
		   0, 1, 0, 1, 0,
		   0, 0, 0, 0, 0 };
  mln_VAR( e2e, make::double_neighb2d(is_row_odd, e2e_h, e2e_v) );

  // e2p
  bool e2p_h[] = { 1, 0, 1,
		   0, 0, 0,
		   1, 0, 1 };
  bool e2p_v[] = { 1, 0, 1,
		   0, 0, 0,
		   1, 0, 1 };
  mln_VAR( e2p, make::double_neighb2d(is_row_odd, e2p_h, e2p_v) );

  /******************/
  /* Initialisation */
  /******************/

  I output = mean(image2cells(input), e2c, e2p);
  io::ppm::save(output, "tmp_input.ppm");
  //image2d<value::int_u8> ima = convert_to_grey(output);
  image2d<value::int_u8> imau = convert_to_grey(output);
  io::pgm::save(imau, "tmp_grey_input.pgm");

  image2d<float> ima(exact(imau).domain());

  // cell
  mln_VAR(cell, imau | is_cell);

  // edge
  mln_VAR(edge, extend((imau | is_edge).rw(), pw::value(imau)));

  // FIXME until laplacian is working use gradient / closing_area / wst

  linear::gaussian_2nd_derivative(imau, lambda, ima);

  mln_piter(edge_t) p(edge.domain());
  for_all(p)
  {
    if (is_row_odd(p))
    {
      mln_value(image2d<float>) t = opt::at(ima, p.row() - 1, p.col());
      mln_value(image2d<float>) b = opt::at(ima, p.row() + 1, p.col());
      if ((t > 0 && b < 0) || (t < 0 && b > 0))
	output(p) = value::rgb8(255,0,0);
    }
    else
    {
      mln_value(image2d<float>) r = opt::at(ima, p.row(), p.col() - 1);
      mln_value(image2d<float>) d = opt::at(ima, p.row(), p.col() + 1);
      if ((r > 0 && d < 0) || (r < 0 && d > 0))
	output(p) = value::rgb8(255,0,0);
    }
  }

#if 0
  data::paste(morpho::gradient(edge, e2c), edge);
  data::paste(morpho::closing_volume(edge, e2e, lambda), edge);
  data::fill(edge, morpho::meyer_wst(edge, e2e, nbasins));

  // Fill regions (with colorize) (won't work with laplacian...)

  colorize colors(nbasins);

  image2d<value::rgb8> cells(ima.domain());
  data::fill(cells, literal::white);
  data::paste(level::transform(edge, colors), cells);
  io::ppm::save(display_edge_with_bg(cells, 3, literal::white), "tmp_edge.ppm");

  // Move the color of an edge which is non black in the cell
  data::paste(morpho::dilation(cells, c4()), cells);
#endif

  //cells = convert_to_rgb8(ima);

  return output;
}

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.ppm" << std::endl;
  std::cerr << "  lambda >= 0" << std::endl;
  abort();
}

int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 4)
    usage(argv);

  float lambda = atof(argv[2]);
  if (lambda < 0)
    usage(argv);

  image2d<value::rgb8> ima;
  io::ppm::load(ima, argv[1]);

  unsigned nbasins;
  image2d<value::rgb8> output = do_it(ima, lambda, nbasins);

  io::ppm::save(display_edge(output, 3), argv[3]);
}
