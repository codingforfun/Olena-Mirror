// Copyright (C) 2010, 2011, 2012 EPITA Research and Development
// Laboratory (LRDE)
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

#include <iostream>

#include <mln/core/alias/neighb2d.hh>

#include <mln/morpho/gradient.hh>

#include <mln/value/label.hh>
#include <mln/data/stretch.hh>

#include "apps/bench/watershed.hh"
#include "apps/data.hh"


// Shortcut macros for run.

#define WATERSHED_WITH_BUILTIN_WINDOW(Namespace, Suffix, Headline)	\
  do									\
    {									\
      t.start();							\
      w = Namespace::watershed(g, nbasins);				\
      t.stop();								\
      std::cout << Headline << t.read() << " s" << std::endl;		\
      io::pgm::save(data::stretch(int_u8(), w),				\
                    prefix + '-' + length + '-' + Suffix + ".pgm");	\
    }									\
  while (0)

#define WATERSHED(Namespace, Nbh, Suffix, Headline)			\
  do									\
    {									\
      t.start();							\
      w = Namespace::watershed(g, Nbh, nbasins);			\
      t.stop();								\
      std::cout << Headline << t.read() << " s" << std::endl;		\
      io::pgm::save(data::stretch(int_u8(), w),				\
                    prefix + '-' + length + '-' + Suffix + ".pgm");	\
    }									\
  while (0)


void
run(const std::string& filename, const std::string& length)
{
  using namespace mln;
  using value::int_u8;
  using value::label;

  border::thickness = 1;
  image2d<int_u8> lena;
  io::pgm::load(lena, filename);
  image2d<int_u8> g = morpho::gradient(lena, win_c4p());

  typedef label<24> label_t;
  label_t nbasins;
  image2d<label_t> w;
  util::timer t;

  std::string prefix = "watershed-lena-out";
  std::cout << "== " << filename << std::endl;

  WATERSHED_WITH_BUILTIN_WINDOW(nongen,      "nongen",      "nongen\t\t");
  // FIXME: Does not work yet...
  // WATERSHED_WITH_BUILTIN_WINDOW(nongen_3ptr, "nongen_3ptr", "nongen_3ptr\t");

  WATERSHED(gen,           c4(), "gen",           "gen\t\t");
  // // FIXME: Introduce a new test case, gen_static, using a static window
  // // and static_qiters.
  WATERSHED(fast,          c4(), "fast",          "fast\t\t");

  // Static window, neighborhood and qixters.
  const unsigned n = 4;
  mln::dpoint2d dps[n] = { mln::dpoint2d( 0, -1),
  			   mln::dpoint2d(-1,  0),
  			   mln::dpoint2d(+1,  0),
  			   mln::dpoint2d( 0, +1) };
  mln::util::static_array<mln::dpoint2d, n> sa(dps, dps + n);
  mln::static_neighborhood<mln::dpoint2d, n> static_nbh_c4 (sa);

  WATERSHED(fast_static,   static_nbh_c4, "fast_static",   "fast_static\t");

  std::cout << std::endl;
}

int
main ()
{
  run(MLN_IMG_DIR "/lena.pgm",            "512");
  run(MLN_APPS_DIR "/bench/lena1024.pgm", "1024");
  run(MLN_APPS_DIR "/bench/lena2048.pgm", "2048");
}
