
#include <mln/core/var.hh>

#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>


#include <mln/value/label_16.hh>
#include <mln/data/convert.hh>
#include <mln/labeling/regional_minima.hh>

#include <mln/data/sort_psites.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/height.hh>
#include <mln/accu/stat/min.hh>
#include <mln/labeling/compute.hh>

#include "cplx2d.hh"
#include "extinct_attributes.hh"
#include "pseudo_tree.hh"


namespace mln
{

  template <typename T>
  image2d<T> quad(const image2d<T>& input)
  {
    image2d<T> output(input.nrows() * 2, input.ncols() * 2);
    mln_piter(box2d) p(output.domain());
    for_all(p)
      output(p) = input.at_(p.row() / 2, p.col() / 2);
    return output;
  }


  template <typename G>
  mln_ch_value(G, unsigned)
  compute_extinct_attributes_of_edges(const G& g)
  {
    typedef p_array<point2d> S;
    S s = data::sort_psites_decreasing(g); // min-tree

    typedef morpho::tree::data<G,S> tree_t;
    tree_t t(g, s, cplx2d::e2e());

    morpho::attribute::height<G> a;
    mln_VAR(h, morpho::tree::compute_attribute_image(a, t));

    return extinct_attributes(t, h, g);
  }



} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm output.pgm [echo = 0]" << std::endl;
  std::cerr << "  Laurent ISMM 2009 scheme." << std::endl
	    << "  echo: 0 (nop) or 1 (yes)." << std::endl;
  abort();
}




int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::label_16;

  if (argc != 3 && argc != 4)
    usage(argv);

  bool echo = false;
  if (argc == 4)
    {
      int echo_ = std::atoi(argv[3]);
      if (echo_ == 1)
	{
	  std::cout << "echo is turned 'on'" << std::endl;
	  echo = true;
	}
    }


  // f: regular image.

  image2d<int_u8> f_raw;
  io::pgm::load(f_raw, argv[1]);

  image2d<int_u8> f = quad(f_raw);
  if (echo)
    {
      debug::println("f:", f);
      io::pgm::save(f, "temp_f.pgm");
    }


  // g: weights on edges.

  mln_VAR(g, cplx2d::f_to_g(f, 255) );

  if (echo)
    {
      debug::println("g:", g);

      // Save g.
      image2d<int_u8> g_(g.unmorph_().domain());
      data::fill(g_, 0);
      data::paste(g, g_);
      io::pgm::save(g_, "temp_g.pgm");
    }


  typedef unsigned L;                        //  <---  Type of labels.
  L l_max;


  // w: minima regional labeling on edges.
  mln_VAR( w, labeling::regional_minima(g, cplx2d::e2e(), l_max) );

 
  std::cout << "l_max = " << l_max << std::endl;

  if (echo)
    debug::println("w:", w);
  // we just have labeled the image flat zones.

  mln_ch_value_(g_t, unsigned) eh = compute_extinct_attributes_of_edges(g);

  if (echo)
    debug::println("eh:", eh);

  
  // a: array "label -> attribute".

  typedef unsigned A;                        //  <---  Type of attributes.
  util::array<A> a = labeling::compute(accu::stat::min<A>(), eh, w, l_max);

  if (echo)
  {
    std::cout << "a = " << std::endl;
    for (unsigned i = 1; i <= l_max; ++i)
      std::cout << '#' << i << ":a" << a[i] << "  ";
    std::cout << std::endl << std::endl;
  }



  util::array<L> l_ = sort_by_increasing_attributes(a, l_max);

  if (echo)
  {
    std::cout << "l_:" << std::endl;
    for (unsigned i = 1; i <= l_max; ++i)
      std::cout << '#' << l_[i] << "(a" << a[l_[i]] << ") ";
    std::cout << std::endl
	      << std::endl;
  }


  mln_VAR(w_ext, cplx2d::extend_w_edges_to_all_faces(w));

  if (echo)
    debug::println("w_ext:", w_ext);


  // e -> (l1, l2)
  mln_VAR( e_to_l1_l2, function_e_to_l1_l2(w_ext, cplx2d::e2p()) );

//   {
//     // Test adjacency "e -> (l1, l2)".
//     L l1, l2;
//     mln_piter_(g_t) e(g.domain());
//     for_all(e)
//       if (w(e) == 0)
// 	{
// 	  e_to_l1_l2(e, l1, l2);
// 	  std::cout << e << "=" << l1 << '|' << l2 << "  ";
// 	}
//     std::cout << std::endl;
//   }



  // Get the attribute values on the edges thanks to the pseudo-tree.

  mln_VAR( aa,  compute_pseudo_tree(w_ext, g, l_, a, e_to_l1_l2, echo) );

  if (echo)
    debug::println("aa:", aa);

  // Outputing.


  {
    A aa_max = data::compute(accu::stat::max<A>(), aa);
    using value::int_u8;
    if (aa_max < 256)
      {
	image2d<int_u8> output(aa.domain());
	data::fill(output, 0);
	data::paste(aa, output);
	io::pgm::save(output, argv[2]);
      }
    else
      {
	std::cerr << "warning: stretching [0," << aa_max << "] to int_u8" << std::endl;
	
	image2d<int_u8> output(aa.domain());
	data::fill(output, 0);
	data::paste(aa, output);
	io::pgm::save(data::stretch(int_u8(), output), argv[2]);
      }
  }


//   {
//     mln_VAR(aa_basins, aa | (pw::value(w) != 0));
//     mln_piter(aa_basins_t) p(aa_basins.domain());
//     for_all(p)
//     {
//       L l = w(p);
//       aa_basins(p) = aa( edge[l] ); // FIXME: was: a[w(p)];
//     }
//   }

}
