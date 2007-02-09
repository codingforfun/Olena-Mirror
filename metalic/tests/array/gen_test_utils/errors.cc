// Copyright (C) 2003  EPITA Research and Development Laboratory
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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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
#include <fstream>
#include <sstream>

using namespace std;


// Warning :
// ---------
// - dimension -> 1-indexed
// - geometry  -> 0-indexed
// - position  -> 0-indexed



//
// Center
//


// If odd -> center
// else   -> no center

int	if_center(ostream&	ofs,
		  int		dimension,
		  int*		geometry,
		  int		position)
{
  // Checks if one of the sizes is even (-> no automatic center)
  for (int i = 0; i < dimension; ++i)
    if ((geometry[i] & 1) == 1) // geometry is 0-indexed !
      {
	if (position == 0)
	  ofs << "x(1), ";
	else
	  ofs << "1, ";
	return 1;
      }
  // All sizes are odd
  ofs << "1, ";
  (void) dimension; (void) geometry; (void) position;
  return 1;
}


// If odd -> middle
// else   -> 1rst position

int	one_center(ostream&	ofs,
		   int		dimension,
		   int*		geometry,
		   int		position)
{
  // Checks if one of the sizes is even (-> no automatic center)
  for (int i = 0; i < dimension; ++i)
    if ((geometry[i] & 1) == 1) // geometry is 0-indexed !
      {
	if (position == 0)
	  ofs << "x(1), ";
	else
	  ofs << "1, ";
	return 1;
      }
  // All sizes are odd
  int middle = geometry[0] + 1;
  for (int i = dimension - 1; 0 < i; --i)
    middle *= (geometry[i] + 1);
  middle = middle >> 1;
  if (position == middle)
    ofs << "x(1), ";
  else
    ofs << "1, ";
  return 1;
}

int	all_centers(ostream&	ofs,
		    int		dimension,
		    int*	geometry,
		    int		position)
{
  ofs << "x(1), ";
  (void) position;
  for (--dimension; dimension >= 0; --dimension)
    if (geometry[dimension] != 0)
      return 0;
  return 1;
}



//
// Comma
//


int	no_comma(ostream&	ofs,
		 int		dimension,
		 int*		geometry,
		 int		position)
{
  if (position == 0)
    ofs << "1 ";
  else
    ofs << "1, ";
  (void) dimension; (void) geometry;
  return 0;
}


//
// No element
//


int	no_element(ostream&	ofs,
		   int		dimension,
		   int*		geometry,
		   int		position)
{
  (void) ofs; (void) dimension; (void) geometry; (void) position;
  return 0;
}



//
// Linebreak
//


int	no_lbreak(ostream&	ofs,
		  int		dimension,
		  int*		geometry,
		  int		position)
{
  (void) ofs; (void) geometry; (void) position;
  return (dimension != 1) ? 0 : 1;
}

int	one_lbreak(ostream&	ofs,
		   int		dimension,
		   int*		geometry,
		   int		position)
{
  if (position == geometry[dimension - 1])
    ofs << "lbrk, ";
  return (dimension > 1) ? 1 : 0;
}

int	all_lbreak(ostream&	ofs,
		   int		dimension,
		   int*		geometry,
		   int		position)
{
  if (((position + 1) % (geometry[dimension - 1] + 1)) == 0)
      ofs << "lbrk, ";
  
  if (dimension == 1)
    return 0;
  for (--(--dimension); dimension >= 0; --dimension)
    if (geometry[dimension] != 0)
      return 0;
  return 1;
}

int	too_much_lbreak(ostream&	ofs,
			int		dimension,
			int*		geometry,
			int		position)
{
  ofs << "lbrk, ";
  (void) dimension; (void) geometry; (void) position;
  if (dimension == 1)
    return 0;
  for (--dimension; dimension >= 0; --dimension)
    if (geometry[dimension] != 0)
      return 0;
  return 1;
}

int	twice_lbreak(ostream&	ofs,
		     int	dimension,
		     int*	geometry,
		     int	position)
{
  if (position == geometry[dimension - 1])
    ofs << "lbrk, lbrk, ";
  return 0;
}


//
// Planebreak
//


int	no_pbreak(ostream&	ofs,
		  int		dimension,
		  int*		geometry,
		  int		position)
{
  (void) ofs; (void) geometry; (void) position;
  return (dimension > 2) ? 0 : 1;
}

int	one_pbreak(ostream&	ofs,
		   int		dimension,
		   int*		geometry,
		   int		position)
{
  if (position == (geometry[1] + 1) * (geometry[2] + 1) - 1)
    ofs << "pbrk, ";
  return (dimension > 2) ? 1 : 0;
}

int	all_pbreak(ostream&	ofs,
		   int		dimension,
		   int*		geometry,
		   int		position)
{
  if ((position + 1) % ((geometry[1] + 1) * (geometry[2] + 1)) == 0)
      ofs << "pbrk, ";
  return (dimension < 3) ? 0 : ((geometry[0] == 0) ? 1 : 0);
}



//
// End
//


int	no_end(ostream&	ofs,
	       int	dimension,
	       int*	geometry,
	       int	position)
{
  (void) ofs; (void) dimension; (void) geometry; (void) position;
  return 0;
}

int	normal_end(ostream&	ofs,
		   int		dimension,
		   int*		geometry,
		   int		position)
{
  ofs << "end);" << endl;  
  (void) dimension; (void) geometry; (void) position;
  return 1;
}

int	end_elt(ostream&	ofs,
		int		dimension,
		int*		geometry,
		int		position)
{
  ofs << "end, 1);" << endl;
  (void) dimension; (void) geometry; (void) position;
  return 0;
}

int	end_lbrk(ostream&	ofs,
		 int		dimension,
		 int*		geometry,
		 int		position)
{
  ofs << "end, lbrk);" << endl;
  (void) dimension; (void) geometry; (void) position;
  return 0;
}

int	end_center(ostream&	ofs,
		   int		dimension,
		   int*		geometry,
		   int		position)
{
  ofs << "end, x());" << endl;
  (void) dimension; (void) geometry; (void) position;
  return 0;
}

int	end_starter(ostream&	ofs,
		    int		dimension,
		    int*	geometry,
		    int		position)
{
  ofs << "end, ints = 1);" << endl;
  (void) dimension; (void) geometry; (void) position;
  return 0;
}
