#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>
#include <mln/io/pgm/load.hh>

#include "k6.hh"



namespace mln
{


  point2d z_find_root(image2d<point2d>& zpar, point2d x)
  {
    if (zpar(x) == x)
      return x;
    return zpar(x) = z_find_root(zpar, zpar(x));
  }


  struct data_t
  {
    data_t()
    {
      L_sum = R_sum = 0;
      L_len = R_area = 0;
    }
    unsigned L_sum, R_sum;
    unsigned L_len, R_area;

    void operator+=(const data_t& d)
    {
      L_sum += d.L_sum;
      R_sum += d.R_sum;
      L_len += L_len;
      R_area += R_area;
    }
  };


  template <typename T, typename R, typename N>
  void
  doit(const image2d<T>& K, const image2d<R>& Kr, const N& nbh)
  {
    typedef point2d P;

    box2d b = Kr.domain();
    std::vector<P> S;
    S.reserve(b.nsites());

    image2d<bool> deja_vu(b), in_queue(b);
    image2d<int> ordering(b);
    image2d<T> K_(b);

    dh_queue<T, point2d> q;
    q.Kr = Kr;

    
    // Initialisation.
    
    data::fill(deja_vu, false);
    data::fill(in_queue, false);
    data::fill(ordering, 0);
    unsigned index = 0;

    q.l = 0;
    q.going_up = true;

    {
      mln_piter(box2d) p(b);
      mln_niter(N) n(nbh, p);
      for_all(p)
	if (k6::is_external_border(p, b))
	  {
	    deja_vu(p) = true;
	    for_all(n)
	      if (b.has(n) && ! k6::is_external_border(n, b) && ! in_queue(n))
		{
		  q.push(n);
		  in_queue(n) = true;
		}
	  }
    }

    // Propagation.

    P p;
    mln_niter(N) n(nbh, p);

    while (! q.is_empty())
      {
	p = q.pop();
	S.push_back(p);
	ordering(p) = ++index;
	K_(p) = q.l;

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
	    std::cout << "level = " << q.l << std::endl;
	    k6::pretty_println("deja-vu:", deja_vu);
	    // debug::println("ordering:", ordering);
	    // k6::pretty_println("in-queue:", in_queue);
 	    // k6::pretty_println("K:", K, deja_vu);
	  }
      }
   

    unsigned n_faces = S.size();
    std::cout << "n_faces = " << n_faces << std::endl;



    // gradient

    image2d<T> grad = k6::grad(K);

//     debug::println("K", K);
//     debug::println("grad", grad);




    image2d<point2d> parent(b);
    image2d<point2d> zpar(b);

    image2d<bool> contour(b);
    data::fill(contour, false);

    image2d<data_t> dta(b);

    data::fill(deja_vu, false);
    unsigned n_trees = 0;

    T l = K_(S[n_faces - 1]);
    for (int i = n_faces - 1; i >= 0; --i)
      {
	p = S[i];

	if (K_(p) != l)
	  {
	    std::cout << "deja-vu (" << n_trees << (n_trees > 1 ? " trees)" : " tree)");
	    k6::pretty_println(" ", deja_vu);
	    std::cout << "contour at " << l;
	    k6::pretty_println(" ", contour);
	    l = K_(p);
	  }

	parent(p) = p;
	zpar(p) = p;
	++n_trees;

	for_all(n) if (deja_vu(n))
	  {
	    P r = z_find_root(zpar, n);
	    if (r != p)
	      {
		--n_trees;
		parent(r) = p;
		zpar(r) = p;
		dta(p) += dta(r);
	      }
	  }
	
	deja_vu(p) = true;

	if (k6::is_1_face(p))
	  {
	    if (contour(p))
	      {
		// exterior contour is now interior
		contour(p) = false;
		dta(p).L_sum -= grad(p);
		--dta(p).L_len;
		dta(p).R_sum += grad(p);
		++dta(p).R_area;
	      } 
	  }
	else if (k6::is_2_face(p))
	  {
	    for_all(n)
	      if (! contour(n) && ! deja_vu(n))
		{
		  contour(n) = true;
		  dta(p).L_sum += grad(n);
		  ++dta(p).L_len;
		}
	  }

      }

  }
      

} // mln




int main(int argc, char* argv[])
{
  using namespace mln;
  using namespace mln::k6;

  image2d<value::int_u8> ima, K;
  io::pgm::load(ima, argv[1]);

  K = immerge(ima, 0);
  pretty_println(K, 3);

  // debug::println(to_range(K));

  doit(K, to_range(K), k6::nbh());
}
