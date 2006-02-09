#include <oln/io/write_image.hh>
#include <oln/io/read_image.hh>
#include <oln/basics2d.hh>
#include <ntg/all.hh>
#include <oln/appli/denoising/nl_means.hh>

using ntg::int_u8;

int main(int argc, char **argv)
{
  using namespace oln;

  image2d<int_u8> ima;
  image2d<int_u8> out;

  ima = io::read(argv[1]);

  // 7 20 3
  out = denoising::nl_means(ima, atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));

  io::write(out, "nl_means.pgm");
}








// 	for_all_p(p)
// 	  {
// 	    Z = 0;
// 	    val = 0;

// 	    for_all_n_of_p(h, p)
// 	      {
// 		pt1 = h;
// 		dist = 0;
// 		w = 0;

// 		for_all_n_of_p(w, p)
// 		  {
// 		    pt2 = w;
// 		    k2 + fenetre / 2 - k

// 		    dist += gaussian_mask(pt2.col() + fenetre / 2 - pt1.col(), pt2.row() + fenetre / 2 - pt1.row()) *
// 		      ((Pix(input, i2, j2) - Pix(input, k2, l2)) * (Pix(input, i2, j2) - Pix(input, k2, l2)));
// 		  }

// 		w = exp(-dist/(h*h));
// 		val += w * input[h];
// 		Z += w;
// 	      }

// 	    nl[p] = val / Z;
// 	  }

