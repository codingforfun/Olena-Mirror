// Copyright (C) 2013 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///

#ifndef SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_PARAGRAPH_HH
# define SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_PARAGRAPH_HH

# include <scribo/layout/internal/docstrum/line.hh>


namespace scribo
{

  namespace layout
  {

    namespace internal
    {

      namespace docstrum
      {

	using namespace mln;

	class paragraph
	{
	public:
	  paragraph(const line* l,
		    const unsigned id);

	  void add_line(const line* l);

	  const bbox2d& bbox() const;

	private:
	  std::vector<const line*> lines_;
	  bbox2d bbox_;
	};



# ifndef MLN_INCLUDE_ONLY

	//-------------------------
	// Paragraph
	//-------------------------

	inline
	paragraph::paragraph(const line* l,
			     const unsigned id)
	{
	  const bbox2d& box = l->bbox();

	  bbox_.pmin() = box.pmin();
	  bbox_.pmax() = box.pmax();
	  bbox_.set_bbox_number(id);

	  lines_.push_back(l);
	}

	inline
	void
	paragraph::add_line(const line* l)
	{
	  lines_.push_back(l);

	  const point2d& pmin = l->bbox().pmin();
	  const point2d& pmax = l->bbox().pmax();
	  point2d& bbox_pmin = bbox_.pmin();
	  point2d& bbox_pmax = bbox_.pmax();

	  if (bbox_pmin.col() > pmin.col())
	    bbox_pmin.col() = pmin.col();

	  if (bbox_pmin.row() > pmin.row())
	    bbox_pmin.row() = pmin.row();

	  if (bbox_pmax.col() < pmax.col())
	    bbox_pmax.col() = pmax.col();

	  if (bbox_pmax.row() < pmax.row())
	    bbox_pmax.row() = pmax.row();
	}

	inline
	const bbox2d&
	paragraph::bbox() const
	{
	  return bbox_;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::layout::internal::docstrum

    } // end of namespace scribo::layout::internal

  } // end of namespace scribo::layout

} // end of namespace scribo

#endif // ! SCRIBO_LAYOUT_INTERNAL_DOCSTRUM_PARAGRAPH_HH
