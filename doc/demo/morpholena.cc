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


void morpholena_base();
void morpholena_grad();
void morpholena_hats();
void morpholena_fast_hats();
void morpholena_opening_closing();
void morpholena_hit_or_miss();
void morpholena_thin_thick();
void morpholena_watershed();
void morpholena_watershed_beucher();
void morpholena_minima_imp();
void morpholena_minima();

int main()
{
  morpholena_base();
  morpholena_grad();
  morpholena_hats();
  morpholena_fast_hats();
  morpholena_opening_closing();
  morpholena_hit_or_miss();
  morpholena_thin_thick();
  morpholena_watershed();
  morpholena_watershed_beucher();
  morpholena_minima_imp();
  morpholena_minima();
}
