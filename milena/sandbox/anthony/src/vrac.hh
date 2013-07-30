#ifndef VRAC_HH
# define VRAC_HH

# include <mln/core/image/image2d.hh>
# include <mln/data/all.hh>
# include <mln/io/pgm/all.hh>
# include <mln/io/ppm/save.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/literal/all.hh>
# include <mln/value/int_s8.hh>

# include <cmath>
# include <list>

# include "matrix.hh"
# include "keypoint.hh"

//# include "keypoint.hh"
//# include "matrix.hh"

using namespace mln;

// Set the gaussian kernel
float *gaussian_kernel(float variance)
{
  unsigned index = 0;
  float *kernel = new float[49];
  Matrix matrix(7, 7);
  float sum = 0;

  for (int i = -3; i < 4; ++i)
  {
    for (int j = -3; j < 4; ++j)
    {
      kernel[index] = exp(- (i*i + j*j) / (2.0 * variance * variance) );
      sum += kernel[index++];
    }
  }

  sum = 1. / sum;

  for (int i = 0; i < 49; ++i)
    kernel[i] *= sum;

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
I blur(const I& original, float variance)
{
  I filtered;
  float *kernel = gaussian_kernel(variance);

  initialize(filtered, original);
  convolve(original, filtered, kernel);

  return filtered;
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


template<typename C>
void writeKeypoints(std::list<Keypoint> keypoints, C& black, C& copy)
{
  for (std::list<Keypoint>::iterator it = keypoints.begin(); it != keypoints.end(); ++it)
  {
    unsigned o = it->getOctave();

    if (o == 0)
    {
      mln_psite(C) p(it->getY() / 2, it->getX() / 2);
      copy(p) = literal::red;
      black(p) = literal::red;
    }
    else if (o == 1)
    {
      mln_psite(C) p(it->getY(), it->getX());
      copy(p) = literal::red;
      black(p) = literal::red;
    }
    else
    {
      mln_psite(C) p(it->getY() * pow(2, o-1) - (o-1), it->getX() * pow(2, o-1) - (o-1));
      copy(p) = literal::red;
      black(p) = literal::red;
    }
  }
}

#endif /* ! VRAC_HH */
