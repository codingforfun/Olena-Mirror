# include <mln/core/image2d.hh>
# include <mln/core/p_mutable_array_of.hh>
# include <mln/core/p_run.hh>
# include <mln/convert/from_to.hh>

// Used by display.
# include <mln/geom/bbox.hh>
# include <mln/debug/println.hh>
# include <mln/core/sub_image.hh>


template <typename S>
void display(const S& s)
{
  using namespace mln;

  image2d<unsigned> ima(geom::bbox(s));
  mln_fwd_piter(S) p(s);
  unsigned c = 0;
  for_all(p)
    ima(p) = ++c;
  debug::println(ima | s);
}


int main()
{
  using namespace mln;

  bool vals[6][5] = {
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0}
  };
  image2d<bool> ima = make::image2d(vals);
  debug::println(ima);
  
  typedef p_mutable_array_of< p_run<point2d> > Runs;
  Runs rs;
  convert::from_to(ima, rs);
  std::cout << rs << std::endl;

  display(rs);

  // FIXME: Use the "mutable" feature of rs...
}
