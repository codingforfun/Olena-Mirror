#include <mln/binarization/all.hh>

#include <mln/core/image/image2d.hh>

#include <mln/data/all.hh>
#include <mln/draw/line.hh>

#include <mln/fun/v2v/rgb_to_luma.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/pdf/load.hh>
#include <mln/io/ppm/all.hh>

#include <mln/labeling/all.hh>
#include <mln/literal/all.hh>
#include <mln/logical/and.hh>

#include <mln/value/all.hh>

#include <tesseract/baseapi.h>

#include <scribo/binarization/sauvola.hh>
#include <scribo/core/component_set.hh>
#include <scribo/preprocessing/denoise_fg.hh>
#include <scribo/primitive/extract/vertical_separators.hh>
#include <scribo/primitive/remove/separators.hh>
#include <scribo/primitive/extract/separators_nonvisible.hh>

#include <scribo/primitive/link/internal/dmax_width_and_height.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/merge_double_link.hh>

using namespace mln;

void  start_xml(std::ofstream& xml, const char* name, const char* pdf)
{
  xml.open(name);
  xml << "<?xml version\"1.0\" encoding=\"UTF-8\"?>" << std::endl
      << "<document filename='" << pdf << "'>" << std::endl;
}

void  end_xml(std::ofstream& xml)
{
  xml << "</document>" << std::endl;
  xml.close();
}

void  write_table(std::ofstream& xml, const point2d& start, const point2d& end)
{
  static unsigned table = 0;
  static unsigned region = 0;
  static unsigned page = 1;

  xml << "\t<table id='" << table << "'>" << std::endl
      << "\t\t<region id='" << region << "' page='" << page << "'>" << std::endl
      << "\t\t<bounding-box x1='" << start[1] << "' y1='" << start[0] << "' "
      << "x2='" << end[1] << "' y2='" << end[0] << "'/>" << std::endl
      << "\t\t</region>" << std::endl
      << "\t</table>" << std::endl;

  ++table;
}

int main(int argc, char** argv)
{
  typedef value::label_16 V;
  typedef image2d<V> L;

  std::ofstream xml;
  std::ostringstream path;
  image2d<value::rgb8> original;
  image2d<value::int_u8> filtered;
  image2d<bool> bin, separators, bin_without_separators, whitespaces,
                denoised, comp, links;
  scribo::component_set< image2d<unsigned> > components;

  unsigned dpi = 72;

  // Loading and binarization
  start_xml(xml, "final.xml", argv[1]);

  util::array< image2d<value::rgb8> > pdf;
  io::pdf::load(pdf, argv[1], dpi);
  for (unsigned page = 0; page < pdf.nelements(); ++page)
  {
    original = pdf[page];
    filtered = data::transform(original, fun::v2v::rgb_to_luma<value::int_u8>());
    bin = scribo::binarization::sauvola(filtered, 81, 0.44);

    // Find separators
    separators = scribo::primitive::extract::vertical_separators(bin, 81);
    bin_without_separators = scribo::primitive::remove::separators(bin, separators);
    whitespaces = scribo::primitive::extract::separators_nonvisible(bin);

    // Denoise
    denoised = scribo::preprocessing::denoise_fg(bin_without_separators, c8(), 4);

    // Extract components
    unsigned ncomponents;
    components = scribo::primitive::extract::components(denoised, c8(), ncomponents);

    initialize(comp, denoised);
    data::fill(comp, false);
    for (unsigned i = 1; i <= components.nelements(); ++i)
      data::fill((comp | components(i).bbox()).rw(), true);

    scribo::object_links< image2d<unsigned> > right_link = scribo::primitive::link::with_single_right_link_dmax_ratio(components,
        scribo::primitive::link::internal::dmax_width_and_height(1),
        scribo::anchor::MassCenter);

    scribo::object_links< image2d<unsigned> > left_link = scribo::primitive::link::with_single_left_link_dmax_ratio(components,
        scribo::primitive::link::internal::dmax_width_and_height(1),
        scribo::anchor::MassCenter);

    scribo::object_links< image2d<unsigned> > merged_links = scribo::primitive::link::merge_double_link(left_link, right_link);

    initialize(links, denoised);
    data::fill(links, false);
    for (unsigned i = 1; i <= merged_links.components().nelements(); ++i)
      data::fill((links | merged_links.components()(i).bbox()).rw(), true);

    // Write images and close XML
    path.str(""); path << "output/p" << page << "_0_bin.pbm";
    io::pbm::save(bin, path.str());

    path.str(""); path << "output/p" << page << "_1_bin_without_separators.pbm";
    io::pbm::save(bin_without_separators, path.str());

    path.str(""); path << "output/p" << page << "_2_whitespaces.pbm";
    io::pbm::save(whitespaces, path.str());

    path.str(""); path << "output/p" << page << "_3_denoised.pbm";
    io::pbm::save(denoised, path.str());

    path.str(""); path << "output/p" << page << "_4_components.pbm";
    io::pbm::save(comp, path.str());

    path.str(""); path << "output/p" << page << "_5_links.pbm";
    io::pbm::save(links, path.str());
  }

  end_xml(xml);

  return 0;
}
