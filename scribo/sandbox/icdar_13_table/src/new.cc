// INCLUDES OLENA
#include <mln/binarization/all.hh>

#include <mln/core/image/image2d.hh>

#include <mln/data/all.hh>
#include <mln/draw/all.hh>

#include <mln/fun/v2v/rgb_to_luma.hh>

#include <mln/io/pbm/all.hh>
#include <mln/io/pdf/load.hh>
#include <mln/io/ppm/all.hh>

#include <mln/labeling/all.hh>
#include <mln/literal/all.hh>
#include <mln/logical/and.hh>
#include <mln/logical/not.hh>

#include <mln/value/all.hh>

// INCLUDE TESSERACT
#include <tesseract/baseapi.h>

// INCLUDES SCRIBO
#include <scribo/binarization/sauvola.hh>

#include <scribo/core/component_set.hh>
#include <scribo/core/line_set.hh>
#include <scribo/core/paragraph_set.hh>

#include <scribo/debug/links_image.hh>
#include <scribo/draw/groups_bboxes.hh>
#include <scribo/draw/line_components.hh>

#include <scribo/filter/object_links_bbox_h_ratio.hh>

#include <scribo/preprocessing/denoise_fg.hh>
#include <scribo/primitive/extract/lines_h_discontinued.hh>
#include <scribo/primitive/extract/lines_v_discontinued.hh>
#include <scribo/primitive/extract/separators_nonvisible.hh>
#include <scribo/primitive/extract/vertical_separators.hh>
#include <scribo/primitive/group/from_single_link.hh>
#include <scribo/primitive/link/internal/compute_anchor.hh>
#include <scribo/primitive/link/internal/dmax_width_and_height.hh>
#include <scribo/primitive/link/merge_double_link.hh>
#include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
#include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>
#include <scribo/primitive/remove/separators.hh>

#include <scribo/text/extract_paragraphs.hh>
#include <scribo/text/merging.hh>

using namespace mln;

// Open and initialize XML
void  start_xml(std::ofstream& xml, const char* name, const char* pdf)
{
  xml.open(name);
  xml << "<?xml version\"1.0\" encoding=\"UTF-8\"?>" << std::endl
      << "<document filename='" << pdf << "'>" << std::endl;
}

// Finalize an close XML
void  end_xml(std::ofstream& xml)
{
  xml << "</document>" << std::endl;
  xml.close();
}

// Write a new (simple) table in XML file
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

                                /********/
                                /* MAIN */
                                /********/

int main(int argc, char** argv)
{
  typedef value::label_16 V;
  typedef image2d<V> L;

  std::ofstream xml;
  std::ostringstream path;
  image2d<value::rgb8> original, ima_links, ima_groups, ima_valid;
  image2d<value::int_u8> filtered;
  image2d<bool> bin, reverse, reverse_selection, bin_merged, separators, bin_without_separators, whitespaces, comp, denoised;
  scribo::component_set< image2d<unsigned> > components, rcomponents;

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

    // Reverse selection
    reverse = logical::not_(bin);
    initialize(reverse_selection, reverse);
    data::fill(reverse_selection, false);

    unsigned nrcomponents;
    rcomponents = scribo::primitive::extract::components(reverse, c8(), nrcomponents);

    for (unsigned i = 1; i < rcomponents.nelements(); ++i)
    {
      const box2d& b = rcomponents(i).bbox();

      if (b.height() < 20 && b.width() < 20)
        data::fill((reverse_selection | b).rw(), true);
    }

    reverse_selection = logical::and_(reverse, reverse_selection);
    reverse_selection = scribo::preprocessing::denoise_fg(reverse_selection, c8(), 4);

    // Find separators
    bin_without_separators = duplicate(bin);
    separators = separators;
    V nhlines, nvlines;
    unsigned min_width = 31;
    unsigned min_height = 71;
    scribo::component_set<L> hlines =  scribo::primitive::extract::lines_h_discontinued(bin_without_separators, c4(), nhlines, min_width, 2);
    scribo::component_set<L> vlines =  scribo::primitive::extract::lines_v_discontinued(bin_without_separators, c4(), nvlines, min_height, 2);
    for (unsigned i = 1; i <= hlines.nelements(); ++i)
      data::fill((bin_without_separators | hlines(i).bbox()).rw(), false);

    for (unsigned i = 1; i <= vlines.nelements(); ++i)
      data::fill((bin_without_separators | vlines(i).bbox()).rw(), false);

    // Denoise
    denoised = scribo::preprocessing::denoise_fg(bin_without_separators, c8(), 4);

    // Bin merged
    bin_merged = logical::or_(denoised, reverse_selection);

    // Extract components
    unsigned ncomponents;
    components = scribo::primitive::extract::components(bin_merged, c8(), ncomponents);

    initialize(comp, bin_merged);
    data::fill(comp, false);
    for (unsigned i = 1; i <= components.nelements(); ++i)
    {
      const box2d& b = components(i).bbox();
      if (b.width() > 2 && b.height() > 2)
        data::fill((comp | b).rw(), true);
    }

    // Find links
    scribo::object_links< image2d<unsigned> > right_link = scribo::primitive::link::with_single_right_link_dmax_ratio(components,
        scribo::primitive::link::internal::dmax_width_and_height(1),
        scribo::anchor::MassCenter);

    scribo::object_links< image2d<unsigned> > left_link = scribo::primitive::link::with_single_left_link_dmax_ratio(components,
        scribo::primitive::link::internal::dmax_width_and_height(1),
        scribo::anchor::MassCenter);

    scribo::object_links< image2d<unsigned> > merged_links = scribo::primitive::link::merge_double_link(left_link, right_link);

    // Filter links
    scribo::object_links< image2d<unsigned> > hratio_filtered_links = scribo::filter::object_links_bbox_h_ratio(merged_links, 2.5f);

    ima_links = data::convert(value::rgb8(), bin_merged);
    ima_groups = data::convert(value::rgb8(), bin_merged);
    ima_valid = data::convert(value::rgb8(), bin_merged);

    // Write links
    for (unsigned l = 1; l < merged_links.nelements(); ++l)
    {
      point2d p1 = scribo::primitive::link::internal::compute_anchor(merged_links.components(), l, scribo::anchor::MassCenter);
      point2d p2 = scribo::primitive::link::internal::compute_anchor(merged_links.components(), merged_links(l), scribo::anchor::MassCenter);

      draw::line(ima_links, p1, p2, literal::red);
    }

    for (unsigned l = 1; l < hratio_filtered_links.nelements(); ++l)
    {
      point2d p1 = scribo::primitive::link::internal::compute_anchor(hratio_filtered_links.components(), l, scribo::anchor::MassCenter);
      point2d p2 = scribo::primitive::link::internal::compute_anchor(hratio_filtered_links.components(), hratio_filtered_links(l), scribo::anchor::MassCenter);

      draw::line(ima_links, p1, p2, literal::blue);
    }

    // Group components
    scribo::object_groups< image2d<unsigned> > groups = scribo::primitive::group::from_single_link(hratio_filtered_links);
    scribo::draw::groups_bboxes(ima_groups, groups, literal::blue);

    // Compute averages
    unsigned average_height = 0;
    unsigned average_width = 0;

    for (unsigned i = 1; i < groups.nelements(); ++i)
    {
      average_height += groups(i).bbox().height();
      average_width += groups(i).bbox().width();
    }
    average_height /= groups.nelements();
    average_width /= groups.nelements();

    std::vector<short> balance(groups.nelements(), 0);

    // Draw vertical links (red)
    for (unsigned i = 1; i < groups.nelements(); ++i)
    {
      for (unsigned j = 1; j < groups.nelements(); ++j)
      {
        if (i != j)
        {
          const box2d& b1 = groups(i).bbox();
          const box2d& b2 = groups(j).bbox();
          const point2d& p1 = b1.pcenter();
          const point2d& p2 = b2.pcenter();

          unsigned max_height = std::max(b1.height(), b2.height());
          unsigned min_height = std::min(b1.height(), b2.height());

          if (p1[0] < p2[0] // Avoid redundancy
              && max_height * 2 < bin_merged.ncols()
              && min_height + 3 >= max_height // Same heights
              && b1.width() < 2 * average_width && b2.width() < 2 * average_width // Regular width
              && (b1.pmin()[1] == b2.pmin()[1]
                || (b1.pmin()[1] < b2.pmin()[1] && b1.pmax()[1] > b2.pmin()[1])
                || (b1.pmin()[1] > b2.pmin()[1] && b2.pmax()[1] > b1.pmin()[1])) // Boxes are aligned
             && abs(p1[0] - p2[0]) < 3 * max_height // Reduced gap
             && abs(p1[1] - p2[1]) < 20) // Vertical proximity
          {
            draw::line(ima_groups, p1, p2, literal::red);
            balance[i] += 1;
            balance[j] += 1;
            break;
          }
        }
      }
    }

    // Draw horizontal links (green)
    for (unsigned i = 1; i < groups.nelements(); ++i)
    {
      for (unsigned j = 1; j < groups.nelements(); ++j)
      {
        if (i != j)
        {
          const box2d& b1 = groups(i).bbox();
          const box2d& b2 = groups(j).bbox();
          const point2d& p1 = b1.pcenter();
          const point2d& p2 = b2.pcenter();

          if (p1[1] < p2[1] // Avoid redundancy
              && (b1.pmin()[0] == b2.pmin()[0]
                || (b1.pmin()[0] < b2.pmin()[0] && b1.pmax()[0] > b2.pmin()[0]) 
                || (b1.pmin()[0] > b2.pmin()[0] && b2.pmax()[0] > b1.pmin()[0])) // Boxes are aligned
              && abs(p1[0] - p2[0]) < 10) // Reduced gap
          {
            draw::line(ima_groups, p1, p2, literal::green);
            balance[i] += 1;
            balance[j] += 1;
            break;
          }
        }
      }
    }

    // Draw weighted boxes (red < orange < cyan < green) (useless ?)
    for (unsigned i = 0; i < balance.size(); ++i)
    {
      if (balance[i] == 1)
        draw::box(ima_valid, groups(i).bbox(), literal::red);

      if (balance[i] == 2)
        draw::box(ima_valid, groups(i).bbox(), literal::orange);

      if (balance[i] == 3)
        draw::box(ima_valid, groups(i).bbox(), literal::cyan);

      if (balance[i] > 3)
        draw::box(ima_valid, groups(i).bbox(), literal::green);
    }

    // Write images and close XML
    // FIXME To externalize
    path.str(""); path << "output/p" << page << "_0_bin.pbm";
    io::pbm::save(bin, path.str());

    path.str(""); path << "output/p" << page << "_1_bin_without_separators.pbm";
    io::pbm::save(bin_without_separators, path.str());

    path.str(""); path << "output/p" << page << "_2_denoised.pbm";
    io::pbm::save(denoised, path.str());

    path.str(""); path << "output/p" << page << "_3_reverse.pbm";
    io::pbm::save(reverse, path.str());

    path.str(""); path << "output/p" << page << "_4_reverse_selection.pbm";
    io::pbm::save(reverse_selection, path.str());

    path.str(""); path << "output/p" << page << "_5_bin_merged.pbm";
    io::pbm::save(bin_merged, path.str());

    path.str(""); path << "output/p" << page << "_6_components.pbm";
    io::pbm::save(comp, path.str());

    path.str(""); path << "output/p" << page << "_7_links.ppm";
    io::ppm::save(ima_links, path.str());

    path.str(""); path << "output/p" << page << "_8_groups.ppm";
    io::ppm::save(ima_groups, path.str());

    path.str(""); path << "output/p" << page << "_9_valid.ppm";
    io::ppm::save(ima_valid, path.str());
  }

  end_xml(xml);

  return 0;
}
