// Copyright (C) 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.

#ifndef OLENA_CORE_DPOINT_HH
# define OLENA_CORE_DPOINT_HH

# include <oln/core/abstract/dpoint.hh>
# include <oln/core/default_props.hh>
# include <oln/core/dim_nd.hh>

namespace oln {

  /// \class Concrete dpoint for dimension grater than 3.
  template <unsigned Dim>
  struct dpoint : public abstract::dpoint_nd<dpoint<Dim> >
  {
  };

  template <unsigned Dim>
  struct point; //fwd decl

  template <unsigned Dim>
  struct props<dpoint<Dim> >: public default_props<dim<Dim> >
  {
  };


} // oln

#endif // ! OLENA_CORE_DPOINT_HH
