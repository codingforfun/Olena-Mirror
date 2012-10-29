// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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
/// \brief Helpers to display tree of shapes.

#ifndef MLN_WORLD_KN_INTERNAL_DISPLAY_HH
# define MLN_WORLD_KN_INTERNAL_DISPLAY_HH

# include <sstream>
# include <mln/world/kn/face_dim.hh>
# include <mln/world/k2/is_primary_2_face.hh>
# include <mln/world/kn/is_1_face_horizontal.hh>
# include <mln/world/kn/is_1_face_vertical.hh>


namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace internal
      {


	struct display
	{
	  virtual bool is_on() const = 0;
	  virtual std::string level(const point2d& p) = 0;
	  virtual bool level_changes_at(unsigned i) const = 0;
	  virtual void show(const image2d<bool>& input) = 0;
	  virtual ~display();
	};


	struct display_off : public display
	{
	  virtual bool is_on() const;
	  virtual std::string level(const point2d&);
	  virtual bool level_changes_at(unsigned) const;
	  virtual void show(const image2d<bool>&);
	};


	template <typename Tree>
	struct display_on : public display
	{
	  const Tree& t;
	  std::ostream& ostr;

	  display_on(const Tree& t, std::ostream& ostr);
	  virtual bool is_on() const;
	  virtual std::string level(const point2d& p);
	  virtual bool level_changes_at(unsigned i) const;
	  virtual void print_p(const point2d& p) = 0;
	  virtual void show(const image2d<bool>& input);
	};



	template <typename Tree>
	struct display_in_D : public display_on<Tree>
	{
	  typedef display_on<Tree> super;
	  using super::ostr;
	  using super::t;

	  display_in_D(const Tree& t, std::ostream& ostr = std::cout);
	  virtual void print_p(const point2d& p);
	};



	template <typename Tree>
	struct display_in_K1 : public display_on<Tree>
	{
	  typedef display_on<Tree> super;
	  using super::ostr;
	  using super::t;

	  display_in_K1(const Tree& t, std::ostream& ostr = std::cout);
	  virtual void print_p(const point2d& p);
	};



	template <typename Tree>
	struct display_in_K2 : public display_on<Tree>
	{
	  typedef display_on<Tree> super;
	  using super::ostr;
	  using super::t;

	  display_in_K2(const Tree& t, std::ostream& ostr = std::cout);
	  virtual void print_p(const point2d& p);
	};


# ifndef MLN_INCLUDE_ONLY

	// display

	inline
	display::~display()
	{
	}


	// display_off

	inline
	bool
	display_off::is_on() const
	{
	  return false;
	}

	inline
	std::string
	display_off::level(const point2d&)
	{
	  return "";
	}

	inline
	bool
	display_off::level_changes_at(unsigned) const
	{
	    return false;
	}

	inline
	void
	display_off:: show(const image2d<bool>&)
	{
	}


	// display_on

	template <typename Tree>
	display_on<Tree>::display_on(const Tree& t,
				     std::ostream& ostr)
	    : t(t), ostr(ostr)
	{
	}

	template <typename Tree>
	bool
	display_on<Tree>::is_on() const
	{
	  return true;
	}

	template <typename Tree>
	std::string
	display_on<Tree>::level(const point2d& p)
	{
	  std::ostringstream s;
	  s << t.level(p);
	  return s.str();
	}

	template <typename Tree>
	bool
	display_on<Tree>::level_changes_at(unsigned i) const
	  {
	    return t.level_changes_at(i);
	  }

	template <typename Tree>
	void
	display_on<Tree>::show(const image2d<bool>& input)
	{
	  box2d dom = input.domain();
	  const short
	    min_row = dom.pmin().row(),
	    max_row = dom.pmax().row(),
	    min_col = dom.pmin().col(),
	    max_col = dom.pmax().col();

	  point2d p;
	  short& row = p.row();
	  short& col = p.col();

	  char bdr = '#';

	  for (col = min_col; col <= max_col + 2; ++col)
	    ostr << bdr << ' ';
	  ostr << std::endl;

	  for (row = min_row; row <= max_row; ++row)
	  {
	    ostr << bdr;
	    for (col = min_col; col <= max_col; ++col)
	      if (input(p))
		print_p(p);
	      else
		ostr << "  ";
	    ostr << ' ' << bdr << std::endl;
	  }

	  for (col = min_col; col <= max_col + 2; ++col)
	    ostr << bdr << ' ';
	  ostr << std::endl
	       << std::endl;
	}


	// display_in_D

	template <typename Tree>
	display_in_D<Tree>::display_in_D(const Tree& t,
					 std::ostream& ostr)
	    : super(t, ostr)
	{
	}

	template <typename Tree>
	void
	display_in_D<Tree>::print_p(const point2d& p)
	{
	  ostr << " O";
	}


	// display_in_K1

	template <typename Tree>
	display_in_K1<Tree>::display_in_K1(const Tree& t,
					   std::ostream& ostr)
	  : super(t, ostr)
	{
	}

	template <typename Tree>
	void
	display_in_K1<Tree>::print_p(const point2d& p)
	{
	  switch (face_dim(p))
	  {
	    case 0:
	      ostr << " +";
	      break;
	    case 1:
	      ostr << (kn::is_1_face_horizontal(p) ? " -" : " |");
	      break;
	    case 2:
	      ostr << " O";
	      break;
	  }
	}


	// display_in_K2

	template <typename Tree>
	display_in_K2<Tree>::display_in_K2(const Tree& t,
					   std::ostream& ostr)
	    : super(t, ostr)
	  {
	  }

	template <typename Tree>
	void
	display_in_K2<Tree>::print_p(const point2d& p)
	{
	  switch (face_dim(p))
	  {
	    case 0:
	      ostr << " +";
	      break;
	    case 1:
	      ostr << (kn::is_1_face_horizontal(p) ? " -" : " |");
	      break;
	    case 2:
	      ostr << (k2::is_primary_2_face(p) ? " O" : " x");
	      break;
	  }
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::world::kn::internal

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln


#endif // ! MLN_WORLD_KN_INTERNAL_DISPLAY_HH
