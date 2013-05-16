#include <mln/binarization/all.hh>

#include <mln/core/image/image2d.hh>

#include <mln/data/all.hh>
#include <mln/draw/line.hh>

#include <mln/fun/v2v/rgb_to_luma.hh>

#include <mln/io/pgm/all.hh>

#include <mln/labeling/all.hh>
#include <mln/literal/all.hh>
#include <mln/logical/and.hh>

#include <mln/value/all.hh>

#include <tesseract/baseapi.h>

#include <scribo/binarization/sauvola.hh>
#include <scribo/core/component_set.hh>
#include <scribo/preprocessing/denoise_fg.hh>
#include <scribo/primitive/extract/all.hh>

using namespace mln;

int main(int argc, char** argv)
{
  image2d<value::int_u8> original;

  io::pgm::load(original, "images/keith.pgm");

  io::pgm::save(original, "final.pgm");

  return 0;
}
