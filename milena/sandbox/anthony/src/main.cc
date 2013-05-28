#include <mln/core/image/image2d.hh>
#include <mln/data/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/literal/all.hh>
#include <mln/value/int_s8.hh>

#include <cmath>

#include "keypoint.hh"
#include "matrix.hh"

using namespace mln;

// Set the gaussian kernel
float *gaussian_kernel()
{
  float variance = sqrt(2.0);
  unsigned index = 0;
  float *kernel = new float[49];
  static const float pi = 4.0 * atan(1.0);
  float div = (2.0 * pi * variance);

  for (int i = -3; i < 4; ++i)
  {
    for (int j = -3; j < 4; ++j)
    {
      float e = exp(- (i*i + j*j) / (2.0 * variance) );
      kernel[index++] = e / div;
    }
  }

  return kernel;
}

// Apply the convolution of the image by the kernel
template<typename I>
void convolve(const I& original, I& filtered, float *kernel)
{
  mln_piter(I) p(filtered.domain());
  window2d win;

  for (int i = -3; i < 4; ++i)
    for (int j = -3; j < 4; ++j)
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
I blur(const I& original)
{
  I filtered;
  float *kernel = gaussian_kernel();

  initialize(filtered, original);
  convolve(original, filtered, kernel);

  return filtered;
}

// Downscale by 2 the image resolution
template<typename I>
I downscaling2(const I& original)
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

float *upscalingTL_kernel()
{
  const static float one = 1.0 / 8.0;
  const static float two = 2.0 / 8.0;
  const static float three = 2.0 / 8.0;
  const static float four = 3.0 / 8.0;

  float *kernel = new float[4];

  kernel[0] = one;
  kernel[1] = two;
  kernel[2] = three;
  kernel[3] = four;

  return kernel;
}

float *upscalingTR_kernel()
{
  const static float one = 2.0 / 8.0;
  const static float two = 1.0 / 8.0;
  const static float three = 3.0 / 8.0;
  const static float four = 2.0 / 8.0;

  float *kernel = new float[4];

  kernel[0] = one;
  kernel[1] = two;
  kernel[2] = three;
  kernel[3] = four;

  return kernel;
}

float *upscalingBL_kernel()
{
  const static float one = 2.0 / 8.0;
  const static float two = 3.0 / 8.0;
  const static float three = 1.0 / 8.0;
  const static float four = 2.0 / 8.0;

  float *kernel = new float[4];

  kernel[0] = one;
  kernel[1] = two;
  kernel[2] = three;
  kernel[3] = four;

  return kernel;
}

float *upscalingBR_kernel()
{
  const static float one = 3.0 / 8.0;
  const static float two = 2.0 / 8.0;
  const static float three = 2.0 / 8.0;
  const static float four = 1.0 / 8.0;

  float *kernel = new float[4];

  kernel[0] = one;
  kernel[1] = two;
  kernel[2] = three;
  kernel[3] = four;

  return kernel;
}

template<typename I>
unsigned upscalePixel(const I& original, int i, int j, int type)
{
  float *kernel;
  window2d win;
  mln_psite(I) p(i, j);
  float value = 0;
  unsigned index = 0;
  int iStart = 0;
  int jStart = 0;

  if (type == 0)
  {
    kernel = upscalingTL_kernel();
    iStart = -1;
    jStart = -1;
  }
  else if (type == 1)
  {
    kernel = upscalingTR_kernel();
    iStart = 0;
    jStart = -1;
  }
  else if (type == 2)
  {
    kernel = upscalingBL_kernel();
    iStart = -1;
    jStart = 0;
  }
  else
  {
    kernel = upscalingBR_kernel();
    iStart = 0;
    jStart = 0;
  }

  for (int ii = iStart; ii < iStart + 2; ++ii)
    for (int jj = jStart; jj < jStart + 2; ++jj)
      win.insert(ii, jj);

  mln_qiter(window2d) q(win, p);

  for_all(q)
  {
    if (original.has(q))
    {
      value += original(q) * kernel[index];
    }

    ++index;
  }

  return (static_cast<int>(value));
}

// Upscale by 2 the image resolution
template<typename I>
I upscaling2(const I& original)
{
  box2d size(original.nrows() * 2, original.ncols() * 2);
  I upscaled(size);

  mln_piter(I) p(upscaled.domain());
  int index = 0;

  for_all(p)
  {
    int i = p.col();
    int j = p.row();

    if (i % 2 == 0 && j % 2 == 0) // Top left
      opt::at(upscaled, i, j) = upscalePixel(original, i / 2, j / 2, 0);
    else if (i % 2 == 0) // Bottom left
      opt::at(upscaled, i, j) = upscalePixel(original, i / 2, j / 2, 2);
    else if (j % 2 == 0) // Top right
      opt::at(upscaled, i, j) = upscalePixel(original, i / 2, j / 2, 1);
    else // Bottom right
      opt::at(upscaled, i, j) = upscalePixel(original, i / 2, j / 2, 3);

    ++index;
  }


  return upscaled;
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
  blured = duplicate(original);

  for(unsigned i = 1; i <= b; i *= 2)
  {
    blured = blur(blured);
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
    // Upscaled octave
    I upscaled = upscaling2(original);
    buildOctave(upscaled, 0, b, scaleSpace);

    // 1st octave
    buildOctave(original, 1, b, scaleSpace);

    // 2nd octave
    I downscaled = downscaling2(original);
    buildOctave(downscaled, 2, b, scaleSpace);

    for (unsigned i = 3; i <= n; ++i)
    {
      // i-th octave
      downscaled = downscaling2(downscaled);
      buildOctave(downscaled, i, b, scaleSpace);
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
      {
        int diff = ((int) level1(p)) - ((int) level2(p));
        dog(p) = (diff >= 0) ? diff : 0;
      }

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
void find_min(T& min, const I& upper, const I& lower, const P& pUpper, const P& pLower)
{
  mln_niter(neighb2d) nUpper(c8(), pUpper);
  mln_niter(neighb2d) nLower(c8(), pLower);

  // Upper processing
  for_all(nUpper)
    if (upper(nUpper) < min)
      min = upper(nUpper);

  // Lower processing
  for_all(nLower)
    if (lower(nLower) < min)
      min = lower(nLower);
}

// Find the maximum value in the upper and lower layers around p
template<typename T, typename I, typename P>
void find_max(T& max, const I& upper, const I& lower, const P& pUpper, const P& pLower)
{
  mln_niter(neighb2d) nUpper(c8(), pUpper);
  mln_niter(neighb2d) nLower(c8(), pLower);

  // Upper processing
  for_all(nUpper)
    if (upper(nUpper) > max)
      max = upper(nUpper);

  // Lower processing
  for_all(nLower)
    if (lower(nLower) > max)
      max = lower(nLower);
}

template<typename T, typename I, typename P>
bool hasEdgeResponse(const I& dog, const P& p, const T& center)
{
  const static float bound_ratio = 12.1;

  /* Set the neighbors:
   *  n5  n1  n6
   *  n2  p   n3
   *  n7  n4  n8
   */
  float n1 = dog(mln_psite(I)(p.row() - 1, p.col()));
  float n2 = dog(mln_psite(I)(p.row(), p.col() - 1));
  float n3 = dog(mln_psite(I)(p.row(), p.col() + 1));
  float n4 = dog(mln_psite(I)(p.row() + 1, p.col()));

  float n5 = dog(mln_psite(I)(p.row() - 1, p.col() - 1));
  float n6 = dog(mln_psite(I)(p.row() - 1, p.col() + 1));
  float n7 = dog(mln_psite(I)(p.row() + 1, p.col() - 1));
  float n8 = dog(mln_psite(I)(p.row() + 1, p.col() + 1));

  // Set Hessian matrix and find the ratio

  float dxx = n3 - 2.0 * center + n2;
  float dyy = n4 - 2.0 * center + n1;

  float dxy = (n8 - n6 - n7 + n5) / 4.0;
  float trace = dxx + dyy;
  float determinant = dxx * dyy - dxy * dxy;

  if (determinant <= 0)
    return true;

  float ratio = (trace * trace) / determinant;

  return (ratio >= bound_ratio);
}

// Build the extrema image
template<typename I, typename C>
void buildExtrema(C& extrema,
                  const I& original,
                  std::vector< std::vector<I> >& dogSpace,
                  std::vector<Keypoint>& keypoints)
{
  // Number of scales (gaussian meaning)
  unsigned scales = dogSpace.at(0).size();

  // Iterates through all scales
  for (unsigned i = 0; i < scales; ++i)
  {
    // Iterates through all resolutions
    for (unsigned j = 1; j < dogSpace.size() - 1; ++j)
    {
      I current = dogSpace.at(j).at(i);
      I upper = dogSpace.at(j+1).at(i);
      I lower = dogSpace.at(j-1).at(i);

      mln_piter(I) p(current.domain());

      for_all(p)
      {
        value::int_u8 min = 255;
        value::int_u8 max = 0;
        value::int_u8 center = 0;

        mln_niter(neighb2d) n(c8(), p);
        center = current(p);

        mln_psite(I) pUpper(p.row() / 2, p.col() / 2);
        mln_psite(I) pLower(p.row() * 2, p.col() * 2);

        // Find the min and max in the current neighborhood (c8)
        find_extremum(min, max, current, n);

        if (center <= min)
        {
          // Find the min in other levels of the pyramid
          find_min(min, upper, lower, pUpper, pLower);

          if (center < min && !hasEdgeResponse(current, p, center))
          {
            mln_psite(I) pOriginal(p.row() * pow(2, (j-1)),
                p.col() * pow(2, (j-1)));
            extrema(pOriginal) = literal::red;
            keypoints.push_back(Keypoint(p.row(), p.col(), i, j, false));
          }
        }
        else if (center >= max)
        {
          // Find the max in other levels of the pyramid
          find_max(max, upper, lower, pUpper, pLower);

          if (center > max && !hasEdgeResponse(current, p, center))
          {
            mln_psite(I) pOriginal(p.row() * pow(2, (j-1)),
                p.col() * pow(2, (j-1)));
            extrema(pOriginal) = literal::red;
            keypoints.push_back(Keypoint(p.row(), p.col(), i, j, true));
          }
        }
      }
    }
  }
}

// Compute the first order matrix for interpolation
template<typename I>
void computeFirstOrderMatrix(const std::vector< std::vector<I> >& dogSpace,
                             Keypoint& k,
                             Matrix& matrix)
{
  matrix.initialize();

  std::vector<I> current = dogSpace.at(1);
  std::vector<I> upper = dogSpace.at(2);
  std::vector<I> lower = dogSpace.at(0);

  /* Point naming convention
   *      n1
   *  n2  c   n3
   *      n4
   */
  point2d c(k.getI(), k.getJ());

  point2d n1(k.getI(), k.getJ() - 1);
  point2d n2(k.getI() - 1, k.getJ());
  point2d n3(k.getI() + 1, k.getJ());
  point2d n4(k.getI(), k.getJ() + 1);

  point2d cupper(c.row() / 2, c.col() / 2);
  point2d clower(c.row() * 2, c.col() * 2);

  // Compute partial x
  matrix.set(0, 0, (current.at(0)(n3) - current.at(0)(n2)) / 2.0);

  // Compute partial y
  matrix.set(1, 0, (current.at(0)(n3) - current.at(0)(n2)) / 2.0);

  // Compute partial scale
  matrix.set(2, 0, (lower.at(0)(clower) - upper.at(0)(cupper)) / 2.0);
}

// Compute the inversed second order matrix for interpolation
template<typename I>
void computeSecondOrderMatrix(const std::vector< std::vector<I> >& dogSpace,
                              Keypoint& k,
                              Matrix& matrix)
{
  matrix.initialize();

  const static std::vector<I> current = dogSpace.at(1);
  const static I upper = dogSpace.at(2).at(0);
  const static I lower = dogSpace.at(0).at(0);
  const static I main = current.at(0);

  /* Point naming convention
   *  n5  n1  n6
   *  n2  c   n3
   *  n7  n4  n8
   */
  point2d c(k.getI(), k.getJ());

  point2d n1(k.getI(), k.getJ() - 1);
  point2d n2(k.getI() - 1, k.getJ());
  point2d n3(k.getI() + 1, k.getJ());
  point2d n4(k.getI(), k.getJ() + 1);

  point2d n5(k.getI() - 1, k.getJ() - 1);
  point2d n6(k.getI() + 1, k.getJ() - 1);
  point2d n7(k.getI() + 1, k.getJ() + 1);
  point2d n8(k.getI() - 1, k.getJ() + 1);

  point2d cupper(c.row() / 2, c.col() / 2);
  point2d n1upper(n1.row() / 2, n1.col() / 2);
  point2d n2upper(n2.row() / 2, n3.col() / 2);
  point2d n3upper(n3.row() / 2, n3.col() / 2);
  point2d n4upper(n4.row() / 2, n4.col() / 2);

  point2d clower(c.row() * 2, c.col() * 2);
  point2d n1lower(n1.row() * 2, n1.col() * 2);
  point2d n2lower(n2.row() * 2, n2.col() * 2);
  point2d n3lower(n3.row() * 2, n3.col() * 2);
  point2d n4lower(n4.row() * 2, n4.col() * 2);

  float partial_xx, partial_yy, partial_ss, partial_xy, partial_xs, partial_ys;

  // Compute partial xx
  partial_xx = main(n3) - 2 * main(c) + main(n2);

  // Compute partial yy
  partial_yy = main(n4) - 2 * main(c) + main(n1);

  // Compute partial ss
  partial_ss = lower(clower) - 2 * main(c)
    + upper(cupper);

  // Compute partial xy
  partial_xy = (main(n8) - main(n6) - main(n7) + main(n5)) / 4.0;

  // Compute partial xs
  partial_xs = (upper(n3upper) - lower(n3lower)
      - upper(n2upper) + lower(n2lower)) / 4.0;

  // Compute partial ys
  partial_ys = (upper(n4upper) - lower(n4lower)
      - upper(n1upper) + lower(n1lower)) / 4.0;

  matrix.set(0, 0, partial_xx);
  matrix.set(0, 1, partial_xy);
  matrix.set(0, 2, partial_xs);

  matrix.set(1, 0, partial_xy);
  matrix.set(1, 1, partial_yy);
  matrix.set(1, 2, partial_ys);

  matrix.set(2, 0, partial_xs);
  matrix.set(2, 1, partial_ys);
  matrix.set(2, 2, partial_ss);
}

// Discard low contrast keypoints thanks to taylor interpolation
// See Brown and Lowe paper (2002)
template<typename I, typename C>
void discardLowContrastKeypoints(const std::vector< std::vector<I> >& dogSpace,
                                 std::vector<Keypoint>& keypoints,
                                 C& extrema)
{
  Keypoint k_old(0, 0, 0, 0, true);

  for (unsigned i = 0; i < keypoints.size(); ++i)
  {
    Keypoint k = keypoints.at(i);

    Matrix firstOrder(3, 1);
    Matrix secondOrder(3, 3);
    Matrix inversed(secondOrder.getHeight(), secondOrder.getWidth());

    computeFirstOrderMatrix(dogSpace, k, firstOrder);
    computeSecondOrderMatrix(dogSpace, k, secondOrder);
    bool hasInversed = secondOrder.inverse(inversed);

    if (hasInversed)
    {
      Matrix x = inversed * firstOrder;
      bool isMajorOffset = x.isMajorOffset();

      if (isMajorOffset)
      {
        std::cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << std::endl;
        std::cout << "First order matrix:" << std::endl;
        firstOrder.print();

        std::cout << "Second order matrix:" << std::endl;
        secondOrder.print();

        std::cout << "Offset: " << std::endl;
        x.print();

        std::cout << "k_old: " << k_old.getI() << " " << k_old.getJ() << std::endl;
        std::cout << "k: " << k.getI() << " " << k.getJ() << std::endl;
        bool hasMoved = k.add(x, k_old);
        std::cout << "k++: " << k.getI() << " " << k.getJ()
                  << "\thasMoved: " << hasMoved << std::endl;

        point2d p(k.getI(), k.getJ());
        std::cout << "Final point: " << extrema.domain() << "\t" << p << std::endl;

        if (hasMoved && extrema.has(p)
            && p.row() >= 0 && p.row() < geom::max_row(extrema)
            && p.col() >= 0 && p.col() < geom::max_col(extrema))
        {
          std::cout << "VALID" << std::endl;
          extrema(p) = literal::red;
          k_old = keypoints.at(i);
          keypoints.at(i--) = k;
        }

        std::cout << std::endl;
      }
      else
      {
        point2d p(k.getI(), k.getJ());
        extrema(p) = literal::red;
      }
    }
  }
}

// Main entry point
int main(int argc, char** argv)
{
  typedef image2d<value::int_u8> I;
  typedef image2d<value::rgb8> C;

  const unsigned blur_level = 5;
  const unsigned octave_level = 3;
  std::vector< std::vector<I> > scaleSpace;
  std::vector< std::vector<I> > dogSpace;
  std::vector<Keypoint> keypoints;
  I original;
  C extrema, improved;
  bool black = true;

  io::pgm::load(original, "images/lena.pgm");

  if (black)
  {
    initialize(extrema, original);
    initialize(improved, original);
  }
  else
  {
    extrema = data::convert(value::rgb8(), original);
    improved = data::convert(value::rgb8(), original);
  }

  // Localization
  buildScaleSpace(scaleSpace, original, octave_level, blur_level);
  buildDifferenceOfGaussianSpace(scaleSpace, dogSpace);
  buildExtrema(extrema, original, dogSpace, keypoints);
  discardLowContrastKeypoints(dogSpace, keypoints, improved);

  // Processing
  // TODO

  io::ppm::save(extrema, "output/extrema.ppm");
  io::ppm::save(improved, "output/extrema_improved.ppm");

  return 0;
}
