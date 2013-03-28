#undef MLN_WO_GLOBAL_VARS
#include "xml.hh"

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
#include <scribo/binarization/sauvola_ms.hh>

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

#include "disjoint_set.hh"

using namespace mln;

// Check if two boxes intersected
bool intersect(const box2d& b1, const box2d& b2)
{
  return ((unsigned) abs(b1.pcenter().col() - b2.pcenter().col()) * 2 < b1.width() + b2.width()
      && (unsigned) abs(b1.pcenter().row() - b2.pcenter().row()) * 2 < b1.height() + b2.height());
}

// Draw weighted boxes (red < orange < cyan < green)
//                      1 link < 2 links < 3 links < 3+ links
template<typename T, typename L>
void  draw_adjacency_boxes(const std::vector<short>&                  balance,
                           image2d<T>&                                ima,
                           const scribo::object_groups< image2d<L> >& groups)
{
  for (unsigned i = 0; i < balance.size(); ++i)
  {
    if (balance[i] == 1)
      draw::box(ima, groups(i).bbox(), literal::red);

    if (balance[i] == 2)
      draw::box(ima, groups(i).bbox(), literal::orange);

    if (balance[i] == 3)
      draw::box(ima, groups(i).bbox(), literal::cyan);

    if (balance[i] > 3)
      draw::box(ima, groups(i).bbox(), literal::green);
  }
}

// Write image2d<bool> images
void  write_image(const image2d<bool>& ima,
                  const char*          name,
                  const unsigned       page,
                  unsigned&            number,
                  std::ostringstream&  path)
{
  path.str("");
  path << "output/p" << page
       << "_" << number
       << "_" << name << ".pbm";
  io::pbm::save(ima, path.str());
  ++number;
}

// Write image2d<value::rbg8> images
void  write_image(const image2d<value::rgb8>& ima,
                  const char*                 name,
                  const unsigned              page,
                  unsigned&                   number,
                  std::ostringstream&         path)
{
  path.str("");
  path << "output/p" << page
       << "_" << number
       << "_" << name << ".ppm";
  io::ppm::save(ima, path.str());
  ++number;
}

// Draw vertical links from top to bottom (red)
template<typename L>
void  draw_links_tb(const scribo::object_groups< image2d<unsigned> >& groups,
                    image2d<value::rgb8>&                             ima_groups,
                    std::vector<short>&                               balance,
                    unsigned                                          average_width,
                    const scribo::component_set<L>&                   hlines)
{
  for (unsigned i = 1; i <= groups.nelements(); ++i)
  {
    for (unsigned j = 1; j <= groups.nelements(); ++j)
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
            && max_height * 2 < ima_groups.ncols()
            && min_height + 3 >= max_height // Same heights
            && b1.width() < 2 * average_width && b2.width() < 2 * average_width // Regular width
            && (b1.pmin()[1] == b2.pmin()[1]
              || (b1.pmin()[1] < b2.pmin()[1] && b1.pmax()[1] > b2.pmin()[1])
              || (b1.pmin()[1] > b2.pmin()[1] && b2.pmax()[1] > b1.pmin()[1])) // Boxes are aligned
            && (unsigned) abs(p1[0] - p2[0]) < 3 * max_height // Reduced gap
            && (unsigned) abs(p1[1] - p2[1]) < 20) // Vertical proximity
        {
          unsigned k = 1;
          short separators = 0;

          while (k <= hlines.nelements() && separators < 2)
          {
            const box2d& s = hlines(k).bbox();

            if (s.pmin()[1] <= b1.pmin()[1] && s.pmax()[1] >= b1.pmax()[1]
                && s.pmin()[0] > b1.pmax()[0]
                && s.pmax()[0] < b2.pmin()[0])
              ++separators;

            ++k;
          }

          if (separators < 2)
          {
            draw::line(ima_groups, p1, p2, literal::red);
            balance[i] += 1;
            break;
          }
        }
      }
    }
  }
}

// Draw vertical links from bottom to top (red)
template<typename L>
void  draw_links_bt(const scribo::object_groups< image2d<unsigned> >& groups,
                    image2d<value::rgb8>&                             ima_groups,
                    std::vector<short>&                               balance,
                    unsigned                                          average_width,
                    const scribo::component_set<L>&                   hlines)
{
  for (unsigned i = groups.nelements(); i > 0; --i)
  {
    for (unsigned j = groups.nelements(); j > 0; --j)
    {
      if (i != j)
      {
        const box2d& b1 = groups(i).bbox();
        const box2d& b2 = groups(j).bbox();
        const point2d& p1 = b1.pcenter();
        const point2d& p2 = b2.pcenter();

        unsigned max_height = std::max(b1.height(), b2.height());
        unsigned min_height = std::min(b1.height(), b2.height());

        if (p1[0] > p2[0] // Avoid redundancy
            && max_height * 2 < ima_groups.ncols()
            && min_height + 3 >= max_height // Same heights
            && b1.width() < 2 * average_width && b2.width() < 2 * average_width // Regular width
            && (b1.pmin()[1] == b2.pmin()[1]
              || (b1.pmin()[1] < b2.pmin()[1] && b1.pmax()[1] > b2.pmin()[1])
              || (b1.pmin()[1] > b2.pmin()[1] && b2.pmax()[1] > b1.pmin()[1])) // Boxes are aligned
            && (unsigned) abs(p1[0] - p2[0]) < 3 * max_height // Reduced gap
            && (unsigned) abs(p1[1] - p2[1]) < 20) // Vertical proximity
        {
          unsigned k = 1;
          short separators = 0;

          while (k <= hlines.nelements() && separators < 2)
          {
            const box2d& s = hlines(k).bbox();

            if (s.pmin()[1] <= b1.pmin()[1] && s.pmax()[1] >= b1.pmax()[1]
                && s.pmax()[0] < b1.pmin()[0]
                && s.pmin()[0] > b2.pmax()[0])
              ++separators;

            ++k;
          }

          if (separators < 2)
          {
            draw::line(ima_groups, p1, p2, literal::red);
            balance[i] += 1;
            break;
          }
        }
      }
    }
  }
}

// Draw horizontal links from left to right (green)
template<typename L>
void  draw_links_lr(const scribo::object_groups< image2d<unsigned> >& groups,
                    image2d<value::rgb8>&                             ima_groups,
                    std::vector<short>&                               balance,
                    const scribo::component_set<L>&                   vlines)
{
  for (unsigned i = 1; i <= groups.nelements(); ++i)
  {
    for (unsigned j = 1; j <= groups.nelements(); ++j)
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
            && abs(p1[0] - p2[0]) < 10 // Reduced gap
            && abs(p1[1] - p2[1]) > (b1.width() + b2.width()) / 4) // Consistent gap
        {
          unsigned k = 1;
          short separators = 0;

          while (k <= vlines.nelements() && separators < 2)
          {
            const box2d& s = vlines(k).bbox();

            if (s.pmin()[0] <= b1.pmin()[0] && s.pmax()[0] >= b1.pmax()[0]
                && s.pmin()[1] > b1.pmax()[1]
                && s.pmax()[1] < b2.pmin()[1])
              ++separators;

            ++k;
          }

          if (separators < 2)
          {
            draw::line(ima_groups, p1, p2, literal::green);
            balance[i] += 1;
            break;
          }
        }
      }
    }
  }
}

// Draw horizontal links from right to left (green)
template<typename L>
void  draw_links_rl(const scribo::object_groups< image2d<unsigned> >& groups,
                    image2d<value::rgb8>&                             ima_groups,
                    std::vector<short>&                               balance,
                    const scribo::component_set<L>&                   vlines)
{
  for (unsigned i = groups.nelements(); i > 0; --i)
  {
    for (unsigned j = groups.nelements(); j > 0; --j)
    {
      if (i != j)
      {
        const box2d& b1 = groups(i).bbox();
        const box2d& b2 = groups(j).bbox();
        const point2d& p1 = b1.pcenter();
        const point2d& p2 = b2.pcenter();

        if (p1[1] > p2[1] // Avoid redundancy
            && (b1.pmin()[0] == b2.pmin()[0]
              || (b1.pmin()[0] < b2.pmin()[0] && b1.pmax()[0] > b2.pmin()[0]) 
              || (b1.pmin()[0] > b2.pmin()[0] && b2.pmax()[0] > b1.pmin()[0])) // Boxes are aligned
            && abs(p1[0] - p2[0]) < 10 // Reduced gap
            && abs(p1[1] - p2[1]) > (b1.width() + b2.width()) / 4) // Consistent gap
        {
          unsigned k = 1;
          short separators = 0;

          while (k <= vlines.nelements() && separators < 2)
          {
            const box2d& s = vlines(k).bbox();

            if (s.pmin()[0] <= b1.pmin()[0] && s.pmax()[0] >= b1.pmax()[0]
                && s.pmax()[1] < b1.pmin()[1]
                && s.pmin()[1] > b2.pmax()[1])
              ++separators;

            ++k;
          }

          if (separators < 2)
          {
            draw::line(ima_groups, p1, p2, literal::green);
            balance[i] += 1;
            break;
          }
        }
      }
    }
  }
}

/******************************** MAIN ****************************************/
int main(int argc, char** argv)
{
  typedef value::label_16 V;
  typedef image2d<V> L;
  typedef image2d<value::rgb8> I8;
  typedef image2d<bool> IB;
  typedef scribo::component_set< image2d<unsigned> > CS;

  // Loading and binarization
  XML* xml = new XML("final.xml", argv[1]);

  util::array< image2d<value::rgb8> > pdf;
  unsigned dpi = 72;
  io::pdf::load(pdf, argv[1], dpi);

  // Iterate over all pages
  for (unsigned page = 0; page < pdf.nelements(); ++page)
  {
    I8 original = pdf[page];
    image2d<value::int_u8> filtered = data::transform(original, fun::v2v::rgb_to_luma<value::int_u8>());
    IB bin = scribo::binarization::sauvola_ms(filtered, 81, 2);

    // Reverse selection
    IB reverse = logical::not_(bin);
    IB reverse_selection;
    initialize(reverse_selection, reverse);
    data::fill(reverse_selection, false);

    unsigned nrcomponents;
    CS rcomponents = scribo::primitive::extract::components(reverse, c8(), nrcomponents);

    for (unsigned i = 1; i < rcomponents.nelements(); ++i)
    {
      const box2d& b = rcomponents(i).bbox();

      if (b.height() < 20 && b.width() < 20)
        data::fill((reverse_selection | b).rw(), true);
    }

    reverse_selection = logical::and_(reverse, reverse_selection);
    reverse_selection = scribo::preprocessing::denoise_fg(reverse_selection, c8(), 4);

    // Find separators
    IB bin_without_separators = duplicate(bin);
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
    IB denoised = scribo::preprocessing::denoise_fg(bin_without_separators, c8(), 4);

    // Bin merged
    IB bin_merged = logical::or_(denoised, reverse_selection);

    // Extract components
    unsigned ncomponents;
    CS components = scribo::primitive::extract::components(bin_merged, c8(), ncomponents);
    IB comp;

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

    IB tmp = logical::and_(bin_merged, comp);

    I8 ima_links = data::convert(value::rgb8(), tmp);
    I8 ima_groups = data::convert(value::rgb8(), tmp);
    I8 ima_valid = data::convert(value::rgb8(), tmp);

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

    // Draw and count links
    draw_links_tb(groups, ima_groups, balance, average_width, hlines);
    draw_links_bt(groups, ima_groups, balance, average_width, hlines);
    draw_links_lr(groups, ima_groups, balance, vlines);
    draw_links_rl(groups, ima_groups, balance, vlines);
    draw_adjacency_boxes(balance, ima_valid, groups);

    /* FIXME: The code below duplicates some of the code in the
       draw_links_* routines.  Factor.  */
    // Adjacencies between nodes.  Of course, an actual digraph data
    // structure would be better.
    typedef unsigned node_id;
    typedef std::set<node_id> group_set;
    typedef std::vector<group_set> adjacencies;
    adjacencies nodes_below(groups.nelements());
    adjacencies nodes_above(groups.nelements());
    adjacencies nodes_right(groups.nelements());
    adjacencies nodes_left(groups.nelements());

    // Draw vertical links (red)
    for (unsigned i = 1; i < groups.nelements(); ++i)
    {
      for (unsigned j = i + 1; j < groups.nelements(); ++j)
      {
	const box2d& b1 = groups(i).bbox();
	const box2d& b2 = groups(j).bbox();
	const point2d& p1 = b1.pcenter();
	const point2d& p2 = b2.pcenter();

	unsigned max_height = std::max(b1.height(), b2.height());
	unsigned min_height = std::min(b1.height(), b2.height());

	if (/* p1[0] < p2[0] // Avoid redundancy
	    && */
	    max_height * 2 < bin_merged.ncols()
	    && min_height + 3 >= max_height // Same heights
	    && b1.width() < 2 * average_width && b2.width() < 2 * average_width // Regular width
	    && (b1.pmin()[1] == b2.pmin()[1]
                || (b1.pmin()[1] < b2.pmin()[1] && b1.pmax()[1] > b2.pmin()[1])
                || (b1.pmin()[1] > b2.pmin()[1] && b2.pmax()[1] > b1.pmin()[1])) // Boxes are aligned
	    && abs(p1[0] - p2[0]) < 3 * max_height // Reduced gap
	    && abs(p1[1] - p2[1]) < 20) // Vertical proximity
          {
	    // Build the above/below adjacencies.
	    node_id top_node, bottom_node;
	    if (p1.row() < p2.col())
	      {
		top_node = i;
		bottom_node = j;
	      }
	    else
	      {
		top_node = j;
		bottom_node = i;
	      }
	    nodes_below[top_node].insert(bottom_node);
	    nodes_above[bottom_node].insert(top_node);
          }
      }
    }

    // Draw horizontal links (green)
    for (unsigned i = 1; i < groups.nelements(); ++i)
    {
      for (unsigned j = i + 1; j < groups.nelements(); ++j)
      {
	const box2d& b1 = groups(i).bbox();
	const box2d& b2 = groups(j).bbox();
	const point2d& p1 = b1.pcenter();
	const point2d& p2 = b2.pcenter();

	if (/* p1[1] < p2[1] // Avoid redundancy
	       && */
	    (b1.pmin()[0] == b2.pmin()[0]
                || (b1.pmin()[0] < b2.pmin()[0] && b1.pmax()[0] > b2.pmin()[0])
                || (b1.pmin()[0] > b2.pmin()[0] && b2.pmax()[0] > b1.pmin()[0])) // Boxes are aligned
	    && abs(p1[0] - p2[0]) < 10) // Reduced gap
          {
	    // Build the right/left adjacencies.
	    node_id left_node, right_node;
	    if (p1.col() < p2.col())
	      {
		left_node = i;
		right_node = j;
	      }
	    else
	      {
		left_node = j;
		right_node = i;
	      }
	    nodes_right[left_node].insert(right_node);
	    nodes_left[right_node].insert(left_node);
          }
      }
    }

    // Build the columns.
    // First pass.
    disjoint_set<node_id> columns_set(groups.nelements());
    for (unsigned i = 0; i < groups.nelements(); ++i)
      {
	const group_set& successors = nodes_below[i];
	for (group_set::const_iterator j = successors.begin();
	     j != successors.end(); ++j)
	    columns_set.make_union(i, *j);
      }
    // Second pass: assign labels.  Label 0 is unused and means
    // ``default''.
    std::vector<group_set> descendants(groups.nelements());
    for (unsigned i = 0; i < groups.nelements(); ++i)
      {
	// Process groups in reverse order.
	unsigned j = groups.nelements() - 1 - i;
	if (!columns_set.is_root(j))
	  descendants[columns_set.find(j)].insert(j);
      }

    std::vector<box2d> columns;
    std::set<unsigned> not_valid;

    // Visualization: columns.
    image2d<value::rgb8> ima_columns = data::convert(value::rgb8(), bin_merged);
    for (unsigned i = 0; i < groups.nelements(); ++i)
      if (!descendants[i].empty())
	{
	  box2d column_box = groups(i).bbox();
	  for (group_set::const_iterator j = descendants[i].begin();
	       j != descendants[i].end(); ++j)
	    column_box.merge(groups(*j).bbox());
	  draw::box(ima_columns, column_box, literal::green);
      columns.push_back(column_box);
	}

    for (unsigned i = 0; i < columns.size(); ++i)
    {
      for (unsigned j = 0; j < columns.size(); ++j)
      {
        if (i != j)
        {
          const box2d& c1 = columns[i];
          const box2d& c2 = columns[j];

          if (intersect(c1, c2) || intersect(c2, c1))
          {
            not_valid.insert(i);
            not_valid.insert(j);
          }
        }
      }
    }

    for (unsigned i = 0; i < columns.size(); ++i)
      if (not_valid.find(i) != not_valid.end())
        draw::box(ima_columns, columns[i], literal::red);

    // Write images and close XML
    std::ostringstream path;
    unsigned number = 0;

    write_image(bin, "bin", page, number, path);
    write_image(bin_without_separators, "bin_without_separators", page, number, path);
    write_image(denoised, "denoised", page, number, path);
    write_image(reverse, "reverse", page, number, path);
    write_image(reverse_selection, "", page, number, path);
    write_image(bin_merged, "reverse_selection", page, number, path);
    write_image(comp, "bin_merged", page, number, path);
    write_image(ima_links, "components", page, number, path);
    write_image(ima_groups, "groups", page, number, path);
    write_image(ima_valid, "valid", page, number, path);
    write_image(ima_columns, "columns", page, number, path);
  }

  delete xml;
}
