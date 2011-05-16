// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef SCRIBO_TOOLCHAIN_INTERNAL_CONTENT_IN_HDOC_FUNCTOR_HH
# define SCRIBO_TOOLCHAIN_INTERNAL_CONTENT_IN_HDOC_FUNCTOR_HH

# include <scribo/core/def/lbl_type.hh>
# include <scribo/core/document.hh>
# include <scribo/core/line_set.hh>
# include <scribo/core/paragraph_set.hh>

# include <scribo/primitive/extract/non_text_hdoc.hh>
# include <scribo/primitive/extract/components.hh>
// # include <scribo/primitive/extract/separators.hh>
// # include <scribo/primitive/extract/vertical_separators.hh>
// # include <scribo/primitive/extract/horizontal_separators.hh>
# include <scribo/primitive/extract/lines_h_thick_and_thin.hh>

# include <scribo/primitive/extract/alignments.hh>

# include <scribo/primitive/identify.hh>

# include <scribo/primitive/remove/separators.hh>

# include <scribo/preprocessing/rotate_90.hh>

# include <scribo/filter/line_links_x_height.hh>
# include <scribo/filter/object_links_bbox_h_ratio.hh>
# include <scribo/filter/objects_small.hh>

# include <scribo/primitive/group/from_single_link.hh>

# include <scribo/primitive/link/merge_double_link.hh>
# include <scribo/primitive/link/internal/dmax_width_and_height.hh>
# include <scribo/primitive/link/with_single_left_link_dmax_ratio.hh>
# include <scribo/primitive/link/with_single_right_link_dmax_ratio.hh>

# include <scribo/preprocessing/denoise_fg.hh>

# include <scribo/text/recognition.hh>
# include <scribo/text/merging.hh>
# include <scribo/text/link_lines.hh>
# include <scribo/text/paragraphs.hh>

# include <scribo/make/debug_filename.hh>

# include <scribo/debug/decision_image.hh>
# include <scribo/debug/bboxes_image.hh>
# include <scribo/debug/linked_bboxes_image.hh>
# include <scribo/debug/bboxes_enlarged_image.hh>
# include <scribo/debug/mean_and_base_lines_image.hh>
# include <scribo/debug/looks_like_a_text_line_image.hh>

# include <scribo/toolchain/internal/toolchain_functor.hh>

# include <scribo/io/xml/save.hh>


namespace scribo
{

  namespace toolchain
  {

    namespace internal
    {


      template <typename I>
      struct content_in_hdoc_functor
	: public Toolchain_Functor
      {
	typedef scribo::def::lbl_type V;
	typedef mln_ch_value(I,V) L;

	content_in_hdoc_functor(const char *doc_filename);

	virtual int nsteps() const;

	virtual void on_xml_saved();

	//===============
	// Core function
	//===============

	template <typename J>
	scribo::document<L> operator()(const Image<J>& original_image,
				       const Image<I>& processed_image);


	//=========
	// Options
	//=========
	bool enable_denoising;
	bool enable_line_seps;
	bool enable_whitespace_seps;
	bool enable_ocr;
	bool save_doc_as_xml;
	scribo::io::xml::Format xml_format;

	//============
	// Parameters
	//============

	std::string ocr_language;
	std::string output_file;

	//=========
	// Results
	//=========
	document<L> doc;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      content_in_hdoc_functor<I>::content_in_hdoc_functor(const char *doc_filename)
	: enable_denoising(true),
	  enable_line_seps(true),
	  enable_whitespace_seps(true),
	  enable_ocr(true),
	  save_doc_as_xml(false),
	  xml_format(scribo::io::xml::PageExtended),
	  ocr_language("eng"),
	  output_file("/tmp/foo.xml"),
	  doc(doc_filename)
      {
      }


      //===============
      // Core function
      //===============

      template <typename I>
      template <typename J>
      scribo::document<typename content_in_hdoc_functor<I>::L>
      content_in_hdoc_functor<I>::operator()(const Image<J>& original_image,
					     const Image<I>& processed_image)
      {
	mln_precondition(exact(original_image).is_valid());
	mln_precondition(exact(processed_image).is_valid());

	doc.set_image(exact(original_image));
	doc.set_binary_image(exact(processed_image));

	// Remove separators
	mln_ch_value(I,bool)
	  separators,
	  input_cleaned = exact(processed_image);
	if (enable_line_seps)
	{
	  // FIXME: SLOW
	  on_new_progress_label("Find vertical and horizontal separators...");

	  // Vertical and horizontal separators
	  {
	    mln_ch_value(I,bool)
	      vseparators = preprocessing::rotate_90(
	       	primitive::extract::lines_h_thick_and_thin(
		  preprocessing::rotate_90(processed_image), 101, 3, 0.05, 0.80, 2), false),
	      hseparators = primitive::extract::lines_h_thick_and_thin(
		processed_image, 101, 3);

	    doc.set_vline_separators(vseparators);
	    doc.set_hline_separators(hseparators);

	    separators = vseparators;
	    separators += hseparators;

	    border::resize(processed_image, border::thickness);
	  }

	  on_progress();

	  on_new_progress_label("Remove separators...");

	  input_cleaned = primitive::remove::separators(processed_image,
							separators);

	  doc.set_binary_image_wo_seps(input_cleaned);
	  on_progress();
	}


#  ifndef SCRIBO_NDEBUG
	// Debug
	if (enable_line_seps)
	{
	  debug::logger().log_image(debug::Special,
				    doc.vline_seps(),
				    "vseparators");

	  debug::logger().log_image(debug::Special,
				    doc.hline_seps(),
				    "hseparators");

	  debug::logger().log_image(debug::Special,
				    input_cleaned,
				    "input_wo_separators");
	}
#  endif // ! SCRIBO_NDEBUG


	// Denoise
	if (enable_denoising)
	{
	  on_new_progress_label("Denoise...");

	  input_cleaned = preprocessing::denoise_fg(input_cleaned, c8(), 3);

	  // Debug
#  ifndef SCRIBO_NDEBUG
	  debug::logger().log_image(debug::AuxiliaryResults,
				    input_cleaned, "denoised");
#  endif // ! SCRIBO_NDEBUG

	  on_progress();
	}

	/// Finding components.
	on_new_progress_label("Finding components...");

	V ncomponents;
	component_set<L>
	  components = scribo::primitive::extract::components(original_image,
							      input_cleaned,
							      c8(),
							      ncomponents);

	on_progress();


	/// Set separator components.
	if (enable_line_seps)
	  components.add_separators(separators);

	// Debug
#  ifndef SCRIBO_NDEBUG
	debug::logger().log_image(debug::AuxiliaryResults,
				  components.separators(),
				  "all_separators");
#  endif // ! SCRIBO_NDEBUG


	on_new_progress_label("Filtering components");

	components = scribo::filter::components_small(components, 3);

	on_progress();


	/// Linking objects
	on_new_progress_label("Linking objects...");

	object_links<L> left_link
	  = primitive::link::with_single_left_link_dmax_ratio(
	    components,
	    primitive::link::internal::dmax_default(1),
	    anchor::MassCenter);


	object_links<L> right_link
	  = primitive::link::with_single_right_link_dmax_ratio(
	    components,
	    primitive::link::internal::dmax_default(1),
	    anchor::MassCenter);

	// Debug
#  ifndef SCRIBO_NDEBUG
	if (debug::logger().is_enabled())
	{
	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    debug::linked_bboxes_image(processed_image,
				       left_link,
				       right_link,
				       literal::blue,
				       literal::cyan,
				       literal::yellow,
				       literal::green,
				       anchor::MassCenter),
	    "object_links");
	}
#  endif // ! SCRIBO_NDEBUG

	// Validating left and right links.
	object_links<L>
	  merged_links = primitive::link::merge_double_link(left_link,
							    right_link);

	on_progress();


	on_new_progress_label("Filtering objects");

	// Remove links if bboxes have too different sizes.
	object_links<L> hratio_filtered_links
	  = filter::object_links_bbox_h_ratio(merged_links, 2.5f);


#  ifndef SCRIBO_NDEBUG
	if (debug::logger().is_enabled())
	{
	  mln_ch_value(I,value::rgb8)
	    hratio_decision_image = scribo::debug::decision_image(processed_image,
								  merged_links,
								  hratio_filtered_links,
								  anchor::MassCenter);
	  // Debug
	  debug::logger().log_image(debug::AuxiliaryResults,
				    hratio_decision_image,
				    "hratio_links_decision_image");
	}
#  endif // ! SCRIBO_NDEBUG

	on_progress();


	on_new_progress_label("Rebuilding lines");

	object_groups<L>
	  groups = primitive::group::from_single_link(hratio_filtered_links);



	// Construct a line set.
	line_set<L>
	  lines = scribo::make::line_set(groups);


	// Extract whitespace to improve text merging results afterwards.
	mln_ch_value(L,bool) whitespaces;
	if (enable_whitespace_seps)
	{
	  scribo::paragraph_set<L> parset = scribo::make::paragraph(lines);
	  doc.set_paragraphs(parset);

	  // Whitespace separators
	  on_new_progress_label("Find whitespace separators...");

	  mln::util::couple<component_set<L>, mln_ch_value(L,bool)>
	    res =  primitive::extract::alignments(doc, 3, 3);
	  whitespaces = res.second();

	  on_progress();

	  components.add_separators(res.second());
	  doc.set_whitespace_separators(res.second(), res.first());
	}


	//===== DEBUG =====
#  ifndef SCRIBO_NDEBUG
	if (debug::logger().is_enabled())
	{
	  if (enable_whitespace_seps)
	    debug::logger().log_image(debug::Special,
				      whitespaces, "whitespaces");

	  // Bboxes image.
	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    scribo::debug::bboxes_image(processed_image, lines),
	    "step1_bboxes");

	  // Bboxes enlarged
	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    scribo::debug::bboxes_enlarged_image(processed_image, lines),
	    "step1_bboxes_enlarged");

	  // Looks like a text line
	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    scribo::debug::looks_like_a_text_line_image(processed_image, lines),
	    "step1_looks_like_a_text_line");

	  // mean and base lines.
	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    scribo::debug::mean_and_base_lines_image(processed_image, lines),
	    "step1_x_height");
	}
#  endif // ! SCRIBO_NDEBUG
	//===== END OF DEBUG =====



	lines = scribo::text::merging(lines);


	//===== DEBUG =====
#  ifndef SCRIBO_NDEBUG
	if (debug::logger().is_enabled())
	{

	  // mean and base lines.
	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    scribo::debug::mean_and_base_lines_image(processed_image, lines),
	    "step2_x_height");

	  // Looks like a text line
	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    scribo::debug::looks_like_a_text_line_image(processed_image, lines),
	    "step2_looks_like_a_text_line");

	  // Bboxes image.
	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    scribo::debug::bboxes_image(processed_image, lines),
	    "step2_bboxes");
	}
#  endif // ! SCRIBO_NDEBUG
	//===== END OF DEBUG =====

	on_progress();


 	//===== DEBUG =====
#  ifndef SCRIBO_NDEBUG
	{
	  image2d<bool> tmp = duplicate(input_cleaned);
	  for_all_lines(l, lines)
	    if (lines(l).is_textline())
	      mln::draw::box_plain(tmp, lines(l).bbox(), false);

	  debug::logger().log_image(
	    debug::AuxiliaryResults,
	    tmp,
	    "input_wo_text");
	}
#  endif // ! SCRIBO_NDEBUG
	//===== END OF DEBUG =====

	// Text recognition
	if (enable_ocr)
	{
	  on_new_progress_label("Recognizing text");

	  scribo::text::recognition(lines, ocr_language.c_str());

	  on_progress();
	}

// 	// Link text lines
// 	on_new_progress_label("Linking text lines");
// 	line_links<L> llinks = scribo::text::link_lines(lines);


// 	//===== DEBUG =====
// #  ifndef SCRIBO_NDEBUG
// 	if (debug::logger().is_enabled())
// 	{
// 	  image2d<value::rgb8>
// 	    debug = data::convert(value::rgb8(), original_image);
// 	  for_all_lines(l, lines)
// 	  {
// 	    if (! lines(l).is_textline())
// 	      continue;

// 	    mln::draw::box(debug, lines(l).bbox(), literal::blue);
// 	    mln::draw::line(debug, lines(l).bbox().pcenter(),
// 			    lines(llinks(l)).bbox().pcenter(), literal::green);
// 	  }

// 	  debug::logger().log_image(debug::AuxiliaryResults,
// 					      debug, "links_raw");
// 	}
// #  endif // ! SCRIBO_NDEBUG
// 	//===== END OF DEBUG =====

// 	on_progress();


// 	// Filter line links.
// 	on_new_progress_label("Filter line links");
// 	llinks = scribo::filter::line_links_x_height(llinks);

// 	//===== DEBUG =====
// #  ifndef SCRIBO_NDEBUG
// 	if (debug::logger().is_enabled())
// 	{
// 	  image2d<value::rgb8>
// 	    debug = data::convert(value::rgb8(), original_image);
// 	  for_all_links(i, llinks)
// 	    if (llinks(i) && llinks(i) != i)
// 	      mln::draw::line(debug, lines(i).bbox().pcenter(),
// 			      lines(llinks(i)).bbox().pcenter(), literal::red);

// 	  debug::logger().log_image(debug::AuxiliaryResults,
// 					      debug, "links");

// 	  for (unsigned i = 1; i < llinks.nelements(); ++i)
// 	    llinks(i) = scribo::make::internal::find_root(llinks, i);

// 	  debug = data::convert(value::rgb8(), original_image);
// 	  mln::util::array<accu::shape::bbox<point2d> >
// 	    nbbox(llinks.nelements());

// 	  for_all_lines(i, lines)
// 	  {
// 	    if (! lines(i).is_textline())
// 	      continue;

// 	    mln::draw::box(debug, lines(i).bbox(), literal::red);
// 	    nbbox(llinks(i)).take(lines(i).bbox());
// 	  }

// 	  for (unsigned i = 1; i < nbbox.nelements(); ++i)
// 	    if (nbbox(i).is_valid())
// 	    {
// 	      box2d b = nbbox(i).to_result();
// 	      mln::draw::box(debug, b, literal::green);
// 	      b.enlarge(1);
// 	      mln::draw::box(debug, b, literal::green);
// 	      b.enlarge(1);
// 	      mln::draw::box(debug, b, literal::green);
// 	    }

// 	  debug::logger().log_image(debug::AuxiliaryResults,
// 					      debug, "par");
// 	}
// #  endif // ! SCRIBO_NDEBUG
// 	//===== END OF DEBUG =====

// 	on_progress();


// 	// Construct paragraphs
// 	on_new_progress_label("Constructing paragraphs");
// 	scribo::paragraph_set<L> parset = scribo::make::paragraph(llinks);

	scribo::paragraph_set<L>
	  parset = extract_paragraphs(lines, doc.binary_image());
	doc.set_paragraphs(parset);

	on_progress();


	// Extract other Elements
	on_new_progress_label("Extracting Elements");
	component_set<L>
	  elements = scribo::primitive::extract::non_text_hdoc(doc, 31);

	on_progress();


	// Identify other Elements
	on_new_progress_label("Identifying Elements");
	elements = scribo::primitive::identify(elements);

	doc.set_elements(elements);

	on_progress();



	// Saving results
	if (save_doc_as_xml)
	{
	  on_new_progress_label("Saving results");

	  scribo::io::xml::save(doc, output_file, xml_format);
	  on_xml_saved();

	  on_progress();
	}

	return doc;
      }



      template<typename I>
      int
      content_in_hdoc_functor<I>::nsteps() const
      {
	return 10 + enable_denoising + enable_line_seps
	  + enable_whitespace_seps + enable_ocr + save_doc_as_xml;
      }


      template<typename I>
      void
      content_in_hdoc_functor<I>::on_xml_saved()
      {
	// Nothing
      }

# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::toolchain::internal

  } // end of namespace scribo::toolchain

} // end of namespace scribo

#endif // ! SCRIBO_TOOLCHAIN_INTERNAL_CONTENT_IN_HDOC_FUNCTOR_HH