#include <mln/core/image/image2d.hh>
#include <mln/data/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/literal/all.hh>

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
    for (int j = -(size / 2); j < (size / 2) + 1; ++j)
      win.insert(i, j);

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
void buildOctave(const I& original,
                 unsigned n,
                 unsigned b,
                 std::vector< std::vector<I> >& scaleSpace)
{
  std::vector<I> octave;
  std::stringstream name;
  I blured;
  initialize(blured, original);

  name << "output/SS-o" << n << "-0.pgm";
  io::pgm::save(original, name.str());

  octave.push_back(original);

  for(unsigned i = 1; i <= b; i *= 2)
  {
    blured = blur(original, i);
    octave.push_back(blured);

    name.str("");
    name << "output/SS-o" << n << "-" << i << ".pgm";
    io::pgm::save(blured, name.str());
  }

  scaleSpace.push_back(octave);
}

// Build n octaves on b blur levels
template<typename I>
void buildScaleSpace(std::vector< std::vector<I> >& scaleSpace,
                     const I& original,
                     unsigned n,
                     unsigned b)
{
  if (n > 2)
  {
    // 1st octave
    buildOctave(original, 1, b, scaleSpace);

    // 2nd octave
    I reduced = downscaleResolution(original);
    buildOctave(reduced, 2, b, scaleSpace);

    for (unsigned i = 3; i <= n; ++i)
    {
      // i-th octave
      reduced = downscaleResolution(reduced);
      buildOctave(reduced, i, b, scaleSpace);
    }
  }
  else
  {
    std::cerr << "ERROR: buildScaleSpace: Wrong level number (" << n << ")"
              << std::endl;
  }
}

// Build the DoG pyramid of the scale space
template<typename I>
void buildDifferenceOfGaussianSpace(std::vector< std::vector<I> >& scaleSpace,
                                    std::vector< std::vector<I> >& dogSpace)
{
  for (unsigned i = 0; i < scaleSpace.size(); ++i)
  {
    std::vector<I> octave = scaleSpace.at(i);
    std::vector<I> dogOctave;

    for (unsigned j = 0; j < octave.size() - 1; ++j)
    {
      std::stringstream name;
      I level1 = octave.at(j);
      I level2 = octave.at(j + 1);
      I dog;

      initialize(dog, level1);

      mln_piter(I) p(level1.domain());

      for_all(p)
        dog(p) = level1(p) - level2(p);

      dogOctave.push_back(dog);
      name << "output/DoG_o" << i << "-" << j << ".pgm";
      io::pgm::save(dog, name.str());
    }

    dogSpace.push_back(dogOctave);
  }
}

// Find the extremum value in the c8 neighborhood
template<typename T, typename I, typename N>
void find_extremum(T& min, T& max, const I& current, N n)
{
  for_all(n)
  {
    if (current(n) > max)
      max = current(n);

    if (current(n) < min)
      min = current(n);
  }
}

// Find the minimum value in the upper and lower layers around p
template<typename T, typename I, typename P>
void find_min(T& min, const I& upper, const I& lower, const P& p)
{
  mln_niter(neighb2d) n(c8(), p);

  for_all(n)
  {
    if (lower(n) < min)
      min = lower(n);

    if (upper(n) < min)
      min = upper(n);
  }
}

// Find the maximum value in the upper and lower layers around p
template<typename T, typename I, typename P>
void find_max(T& max, const I& upper, const I& lower, const P& p)
{
  mln_niter(neighb2d) n(c8(), p);

  for_all(n)
  {
    if (lower(n) > max)
      max = lower(n);

    if (upper(n) > max)
      max = upper(n);
  }
}

// Build the extrema image
template<typename I, typename C>
void buildExtrema(C extrema,
                  I original,
                  std::vector< std::vector<I> >& dogSpace)
{
  extrema = data::convert(value::rgb8(), original);
  value::int_u8 min = 255;
  value::int_u8 max = 0;
  value::int_u8 center = 0;

  for (unsigned i = 0; i < dogSpace.size(); ++i)
  {
    std::vector<I> octave = dogSpace.at(i);

    for (unsigned j = 1; j < octave.size() - 1; ++j)
    {
      I current = octave.at(j);
      I upper = octave.at(j+1);
      I lower = octave.at(j-1);

      mln_piter(I) p(current.domain());

      for_all(p)
      {
        mln_niter(neighb2d) n(c8(), p);
        center = current(p);

        find_extremum(min, max, current, n);

        if (center <= min)
        {
          find_min(min, upper, lower, p);

          if (center <= min)
            extrema(p) = literal::blue;
        }
        else if (center >= max)
        {
          find_max(max, upper, lower, p);

          if (center >= max)
            extrema(p) = literal::red;
        }
      }
    }
  }
  io::ppm::save(extrema, "output/extrema.ppm");
}

// Main entry point
int main(int argc, char** argv)
{
  typedef image2d<value::int_u8> I;
  typedef image2d<value::rgb8> C;

  const unsigned blur_level = 4;
  const unsigned octave_level = 4;
  std::vector< std::vector<I> > scaleSpace;
  std::vector< std::vector<I> > dogSpace;
  I original;
  C extrema;

  io::pgm::load(original, "images/keith512.pgm");

  buildScaleSpace(scaleSpace, original, octave_level, blur_level);
  buildDifferenceOfGaussianSpace(scaleSpace, dogSpace);
  buildExtrema(extrema, original, dogSpace);

  return 0;
}
