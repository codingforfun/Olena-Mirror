// Copyright (C) 2009 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#include <iostream>

#include <mln/essential/2d.hh>

#include <scribo/text/extract_bboxes.hh>
#include <scribo/text/grouping/group_with_single_left_link.hh>
#include <scribo/text/grouping/group_with_single_right_link.hh>
#include <scribo/text/grouping/group_from_single_link.hh>

#include <scribo/debug/save_textbboxes_image.hh>
#include <scribo/debug/save_linked_textbboxes_image.hh>
#include <scribo/make/debug_filename.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}

int main(int argc, char* argv[])
{
  using namespace scribo;
  using namespace mln;

  if (argc < 1)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = "extract_text_single_link";

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nbboxes;
  scribo::util::text<image2d<value::label_16> > text
    = text::extract_bboxes(input, c8(), nbboxes);

  {
    std::cout << "* Left grouping" << std::endl;
    mln::util::array<unsigned> left_link
	= text::grouping::group_with_single_left_link(text, 30);

    std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;
    scribo::debug::save_linked_textbboxes_image(input,
						text, left_link,
						literal::red, literal::cyan,
						scribo::make::debug_filename("left_links.ppm"));
//    io::ppm::save(mln::labeling::colorize(value::rgb8(),
//				       text.label_image(),
//				       text.nbboxes()),
//				       scribo::make::debug_filename("lbl_before.ppm"));

    scribo::util::text<image2d<value::label_16> > grouped_text
	  = text::grouping::group_from_single_link(text, left_link);

    std::cout << "AFTER - nbboxes = " << grouped_text.bboxes().nelements() << std::endl;
    io::ppm::save(mln::labeling::colorize(value::rgb8(),
				       grouped_text.label_image(),
				       grouped_text.nbboxes()),
				       scribo::make::debug_filename("left_label_color.ppm"));
    scribo::debug::save_textbboxes_image(input, grouped_text.bboxes(),
					 literal::red,
					 scribo::make::debug_filename("left_bboxes.ppm"));
  }

  {
    std::cout << "* Left grouping" << std::endl;
    mln::util::array<unsigned> right_link
	= text::grouping::group_with_single_right_link(text, 30);

    std::cout << "BEFORE - nbboxes = " << nbboxes << std::endl;
    scribo::debug::save_linked_textbboxes_image(input,
						text, right_link,
						literal::red, literal::cyan,
						scribo::make::debug_filename("right_links.ppm"));
//    io::ppm::save(mln::labeling::colorize(value::rgb8(),
//				       text.label_image(),
//				       text.nbboxes()),
//				       scribo::make::debug_filename("lbl_before.ppm"));

    scribo::util::text<image2d<value::label_16> > grouped_text
	  = text::grouping::group_from_single_link(text, right_link);

    io::ppm::save(mln::labeling::colorize(value::rgb8(),
				       grouped_text.label_image(),
				       grouped_text.nbboxes()),
				       scribo::make::debug_filename("right_label_color.ppm"));
    std::cout << "AFTER - nbboxes = " << grouped_text.bboxes().nelements() << std::endl;

    scribo::debug::save_textbboxes_image(input, grouped_text.bboxes(),
					 literal::red,
					 scribo::make::debug_filename("right_bboxes.ppm"));
  }


}

