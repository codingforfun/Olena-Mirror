#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/data/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/literal/all.hh>
#include <mln/value/all.hh>
#include <mln/make/w_window2d.hh>
#include <mln/core/alias/w_window2d_int.hh>

#include <cmath>

using namespace mln;

// Set the gaussian kernel of size (6 * t + 1)
double *gaussian_kernel(unsigned t)
{
  int size = 6 * t + 1;
  unsigned index = 0;
  double *kernel = new double[size * size];
  static const double pi = 4.0 * atan(1.0);
  double div = (2.0 * pi * t * t);

  for (int i = -(size / 2); i < (size / 2) + 1; ++i)
  {
    for (int j = -(size / 2); j < (size / 2) + 1; ++j)
    {
      double e = exp(- (i * i + j * j) / (2.0 * t * t) );
      kernel[index++] = e / div;
    }
  }

  return kernel;
}

// Apply the convolution of the image by the kernel
template<typename I>
void convolve(const I& original, I& filtered, double *kernel, unsigned t)
{
  mln_piter(I) p(filtered.domain());
  int size = 6 * t + 1;
  window2d win;

  for (int i = -(size / 2); i < (size / 2) + 1; ++i)
  {
    for (int j = -(size / 2); j < (size / 2) + 1; ++j)
    {
      win.insert(i, j);
    }
  }


  // Iterate through all image sites
  for_all(p)
  {
    // Create the window around the site
    mln_qiter(window2d) q(win, p);
    float sum = 0;
    int index = 0;

    // Iterate through all window image sites
    for_all(q)
    {
        if (filtered.has(q))
          sum += original(q) * kernel[index++];
        else
          sum += 127 * kernel[index++];
    }

    filtered(p) = static_cast<int>(sum);
  }
}

// Blur the image thanks to a convolution matrix
template<typename I>
image2d<value::int_u8> blur(const I& original, unsigned t)
{
  I filtered;
  double *kernel = gaussian_kernel(t);

  initialize(filtered, original);
  convolve(original, filtered, kernel, t);

  return filtered;
}

template<typename I>
void downscaleResolution(const I& original, I& toReduce)
{
}

// MAIN ENTRY POINT
int main(int argc, char** argv)
{
  image2d<value::int_u8> original;
  io::pgm::load(original, "images/flower.pgm");

  image2d<value::int_u8> blur1, blur2, blur4;
  initialize(blur1, original);
  initialize(blur2, original);
  initialize(blur4, original);

  io::pgm::save(original, "output/original.pgm");

  blur1 = blur(original, 1);
  io::pgm::save(blur1, "output/blur1.pgm");

  blur2 = blur(original, 2);
  io::pgm::save(blur2, "output/blur2.pgm");

  blur4 = blur(original, 4);
  io::pgm::save(blur4, "output/blur4.pgm");

  return 0;
}
