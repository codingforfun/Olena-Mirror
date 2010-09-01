#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/io/pgm/load.hh>

#include "k6.hh"



namespace mln
{


  template <typename T, typename R, typename N>
  void
  doit(const image2d<T>& ima, const image2d<R>& Kr, const N& nbh)
  {
    typedef point2d P;
    P p;
    mln_niter(N) n(nbh, p);

    box2d b = Kr.domain();

    image2d<bool> deja_vu(b), in_queue(b);
    dh_queue<T, point2d> q;
    q.Kr = Kr;
    
    // Initialisation.
    
    data::fill(deja_vu, false);
    data::fill(in_queue, false);

    q.l = 0;
    q.going_up = true;

    {
      mln_piter(box2d) p(b);
      for_all(p)
	if (k6::is_external_border(p, b))
	  {
	    q.push(p);
	    in_queue(p) = true;
	  }
    }


    // Propagation.

    while (! q.is_empty())
      {
	p = q.pop();
	mln_invariant(in_queue(p) && ! deja_vu(p));
	for_all(n)
	  if (b.has(n) && ! deja_vu(n) && ! in_queue(n))
	    {
	      q.push(n);
	      in_queue(n) = true;
	    }
	deja_vu(p)  = true;
	in_queue(p) = false;
	if (q.is_level_completed())
	  {
// 	    if (q.l == 150)
// 	      {
// 		unsigned nlabels;
// 		image2d<unsigned> lab = labeling::background(deja_vu, c4(), nlabels);
// 		k6::pretty_println("lab:", lab);
// 	      }
	    std::cout << "level = " << q.l << std::endl;
	    k6::pretty_println("deja-vu:", deja_vu);
	    // k6::pretty_println("in-queue:", in_queue);
 	    // k6::pretty_println("ima:", ima, deja_vu);
	  }
      }
  }


}



int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::k6;

  image2d<value::int_u8> ima, K;
  io::pgm::load(ima, argv[1]);

  K = immerge(ima, 0);
  pretty_println(K, 3);

  // debug::println(to_range(K));

  doit(ima, to_range(K), k6::nbh());
}
