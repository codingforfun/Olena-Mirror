#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/iota.hh>

// Example specific header.
#include <doc/tools/sample_utils.hh>


int main()
{
  using namespace mln;

  //! [Input image]
  typedef image2d<bool> I;
  I ima(5,5);
  debug::iota(ima, 100);
  //! [Input image]

  {
    //! [Iteration over a site]
    point2d p(2,2);
    mln_niter_(neighb2d) q(c4(), p);
    for_all(q)
      ; // Do something here.
    //! [Iteration over a site]
  }

  {
    //! [Iteration over a domain]
    mln_piter_(I) p(ima.domain());
    mln_niter_(neighb2d) q(c4(), p);

    for_all(p)
      for_all(q)
        ; // Do something here.
    //! [Iteration over a domain]
  }
}

