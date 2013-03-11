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
#include <scribo/primitive/extract/all.hh>

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

template<typename L>
void  get_horizontal_lines(scribo::component_set<L>&  hlines,
                           image2d<bool>&             ima,
                           image2d<bool>&             bin_without,
                           const unsigned             av_height)
{
  for (unsigned i = 1; i <= hlines.nelements(); ++i)
  {
    const box2d& b = hlines(i).bbox();

    data::fill((bin_without | b).rw(), false);

    if (b.height() != 0 && b.width() != 0 && b.height() < av_height)
      data::fill((ima | b).rw(), true);
  }
}

template<typename L>
void  get_vertical_lines(scribo::component_set<L>&  vlines,
                         image2d<bool>&             ima,
                         image2d<bool>&             bin_without,
                         const unsigned             av_width,
                         const int                  delta_prox_h)
{
  for (unsigned i = 1; i <= vlines.nelements(); ++i)
  {
    const box2d& b = vlines(i).bbox();

    data::fill((bin_without | b).rw(), false);

    if (b.height() != 0 && b.width() != 0
        && b.width() < av_width)
      if (vlines(i).mass_center()[1] - vlines(i - 1).mass_center()[1] > delta_prox_h
          || vlines(i).mass_center()[1] < vlines(i - 1).mass_center()[1])
        data::fill((ima | b).rw(), true);
  }
}

unsigned find_left(image2d<bool>& ima)
{
  bool found = false;

  def::coord col = geom::min_col(ima);
  while (col < geom::max_col(ima) && !found)
  {
    def::coord row = geom::min_row(ima);
    while (row < geom::max_row(ima) && !found)
    {
      found = opt::at(ima, row, col);
      ++row;
    }
    ++col;
  }

  return (col - 1);
}

unsigned  find_right(image2d<bool>& ima)
{
  bool found = false;

  def::coord col = geom::max_col(ima) - 1;
  while (col >= geom::min_col(ima) && !found)
  {
    def::coord row = geom::min_row(ima);
    while (row < geom::max_row(ima) && !found)
    {
      found = opt::at(ima, row, col);
      ++row;
    }
    --col;
  }

  return (col + 1);

}

unsigned  find_top(image2d<bool>& ima)
{
  bool found = false;

  def::coord row = geom::min_row(ima);
  while (row < geom::max_row(ima) && !found)
  {
    def::coord col = geom::min_col(ima);
    while (col < geom::max_col(ima) && !found)
    {
      found = opt::at(ima, row, col);
      ++col;
    }
    ++row;
  }

  return (row - 1);

}

unsigned  find_bottom(image2d<bool>& ima)
{
  bool found = false;

  def::coord row = geom::max_row(ima) - 1;
  while (row >= geom::min_row(ima) && !found)
  {
    def::coord col = geom::min_col(ima);
    while (col < geom::max_col(ima) && !found)
    {
      found = opt::at(ima, row, col);
      ++col;
    }
    --row;
  }

  return (row + 1);
}

void  find_borders(image2d<bool>& ima,
                   unsigned& left, unsigned& right,
                   unsigned& top, unsigned& bottom)
{
  left = find_left(ima);
  right = find_right(ima);
  top = find_top(ima);
  bottom = find_bottom(ima);
}

                                /********/
                                /* MAIN */
                                /********/

int main(int argc, char** argv)
{
  typedef value::label_16 V;
  typedef image2d<V> L;

  image2d<value::rgb8> original, final;
  image2d<value::int_u8> filtered;
  image2d<bool> bin, ima_hlines, ima_vlines, ima_tables, ima_texts,
                bin_without_lines, bin_without_lines_denoised,
                mask;
  image2d<unsigned> labeled;
  scribo::component_set< image2d<unsigned> > masks;
  V nhlines, nvlines;
  L hlines_ima, vlines_ima;
  scribo::component_set<L> hlines, vlines;
  std::ostringstream path;

  // Parameters
  unsigned av_height = 4;
  unsigned av_width = 4;
  int delta_prox_h = 5;
  unsigned min_height = 71;
  unsigned min_width = 31;
  unsigned dpi = 72;

  // Loading and binarization
  std::ofstream xml;
  start_xml(xml, "final.xml", argv[1]);

  //io::ppm::load(original, argv[1]);
  util::array< image2d<value::rgb8> > pdf;
  io::pdf::load(pdf, argv[1], dpi);
  for (unsigned page = 0; page < pdf.nelements(); ++page)
  {
    original = pdf[page];
    filtered = data::transform(original, mln::fun::v2v::rgb_to_luma<value::int_u8>());

    bin = scribo::binarization::sauvola(filtered, 81, 0.44);
    final = data::convert(value::rgb8(), bin);

    initialize(mask, bin);
    initialize(ima_texts, bin);
    initialize(ima_tables, bin);
    data::fill(ima_tables, false);

    bin_without_lines = duplicate(bin);

    // Lines extraction
    hlines = scribo::primitive::extract::lines_h_discontinued(bin, c4(), nhlines, min_width, 2);
    vlines = scribo::primitive::extract::lines_v_discontinued(bin, c4(), nvlines, min_height, 2);

    get_horizontal_lines(hlines, ima_tables, bin_without_lines, av_height);
    get_vertical_lines(vlines, ima_tables, bin_without_lines, av_width, delta_prox_h);

    // Denoising
    bin_without_lines_denoised = scribo::preprocessing::denoise_fg(bin_without_lines, c8(), 4);

    // Set the tables mask
    unsigned n;
    labeled = labeling::blobs(ima_tables, c8(), n);
    masks = scribo::component_set< image2d<unsigned> >(labeled, n);
    data::fill(mask, false);

    for (unsigned i = 1; i <= masks.nelements(); ++i)
      data::fill((mask | masks(i).bbox()).rw(), true);

    // Compose table zones with bin_without_lines_lines
    ima_texts = logical::and_(bin_without_lines_denoised, mask);

    // Isolate texts between tables
    for (unsigned i = 1; i <= masks.nelements(); ++i)
    {
      image2d<bool> table_mask, isolated_text;
      bool empty = true;

      initialize(table_mask, bin);
      data::fill(table_mask, false);
      data::fill((table_mask | masks(i).bbox()).rw(), true);

      isolated_text = logical::and_(bin_without_lines_denoised, table_mask);

      mln_piter_(image2d<bool>) p(isolated_text.domain());

      for_all(p)
        empty = empty && !(isolated_text(p));

      if (!empty)
      {
        path.str("");
        path << "output/p" << page << "_8_" << i << "_isolated.pbm";
        io::pbm::save(isolated_text, path.str());

        // Find coordinates
        unsigned left, right, top, bottom;
        find_borders(isolated_text, left, right, top, bottom);

        point2d p1, p2, p3, p4;

        p1 = point2d(top, left);
        p2 = point2d(top, right);
        p3 = point2d(bottom, right);
        p4 = point2d(bottom, left);

        draw::line(final, p1, p2, literal::green);
        draw::line(final, p2, p3, literal::green);
        draw::line(final, p3, p4, literal::green);
        draw::line(final, p4, p1, literal::green);

        write_table(xml, p1, p3);
      }
    }

    // Get lines images
    hlines_ima = hlines.labeled_image();
    vlines_ima = vlines.labeled_image();
    ima_hlines = data::convert(bool(), hlines_ima);
    ima_vlines = data::convert(bool(), vlines_ima);

    // Write images and close XML
    path.str(""); path << "output/p" << page << "_0_bin.pbm";
    io::pbm::save(bin, path.str());

    path.str(""); path << "output/p" << page << "_1_bin_without_lines.pbm";
    io::pbm::save(bin_without_lines, path.str());

    path.str(""); path << "output/p" << page << "_2_bin_without_lines_denoised.pbm";
    io::pbm::save(bin_without_lines_denoised, path.str());

    path.str(""); path << "output/p" << page << "_3_hlines.pbm";
    io::pbm::save(ima_hlines, path.str());

    path.str(""); path << "output/p" << page << "_4_vlines.pbm";
    io::pbm::save(ima_vlines, path.str());

    path.str(""); path << "output/p" << page << "_5_tables.pbm";
    io::pbm::save(ima_tables, path.str());

    path.str(""); path << "output/p" << page << "_6_mask.pbm";
    io::pbm::save(mask, path.str());

    path.str(""); path << "output/p" << page << "_7_texts.pbm";
    io::pbm::save(ima_texts, path.str());

    /* Save 8_i_isolated */

    path.str(""); path << "output/p" << page << "_9_final.pbm";
    io::ppm::save(final, path.str());
  }

  end_xml(xml);

  return 0;
}
