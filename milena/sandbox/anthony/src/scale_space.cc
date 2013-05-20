#include <mln/core/image/image2d.hh>
#include <mln/data/all.hh>
#include <mln/io/pgm/all.hh>

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
I blur(const I& original, unsigned t)
{
  I filtered;
  double *kernel = gaussian_kernel(t);

  initialize(filtered, original);
  convolve(original, filtered, kernel, t);

  return filtered;
}

// Downscale by 2 the image resolution
template<typename I>
I downscaleResolution(const I& original)
{
  mln_piter(I) p(original.domain());
  window2d win;
  int index = 0;

  // Initialize the rescaled image
  box2d size(original.nrows() / 2, original.ncols() / 2);
  I reduced(size);

  // Set the 2x2 window matrix
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      win.insert(i, j);

  // Iterate through all image sites
  for_all(p)
  {
    // Get 1/4 of total pixels
    if (index % 2 == 0)
    {
      int i = (index % original.ncols()) / 2;
      int j = (index / original.ncols()) / 2;
      int count = 0;
      int sum = 0;
      mln_qiter(window2d) q(win, p);

      // Iterate through all window image sites
      for_all(q)
      {
        if (original.has(q))
        {
          sum += original(q);
          ++count;
        }
      }

      if (reduced.has(point2d(j, i)))
        opt::at(reduced, j, i) = sum / count;
    }

    ++index;
  }

  return reduced;
}

// Build the n-th octave on b blur levels
template<typename I>
void buildOctave(const I& original, unsigned n, unsigned b)
{
  std::stringstream name;
  I blured;
  initialize(blured, original);

  name << "output/o" << n << "b0.pgm";
  io::pgm::save(original, name.str());

  for(unsigned i = 1; i <= b; i *= 2)
  {
    blured = blur(original, i);
    name.str("");
    name << "output/o" << n << "b" << i << ".pgm";
    io::pgm::save(blured, name.str());
  }
}

// Main entry point
int main(int argc, char** argv)
{
  image2d<value::int_u8> original;
  io::pgm::load(original, "images/flower.pgm");
  const unsigned blur_level = 4;
  unsigned octave = 1;

  // 1st octave
  buildOctave(original, octave++, blur_level);

  // 2nd octave
  image2d<value::int_u8> reduced = downscaleResolution(original);
  buildOctave(reduced, octave++, blur_level);

  // 3rd octave
  reduced = downscaleResolution(reduced);
  buildOctave(reduced, octave++, blur_level);

  // 4th octave
  reduced = downscaleResolution(reduced);
  buildOctave(reduced, octave++, blur_level);

  return 0;
}
