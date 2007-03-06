// Copyright (C) 2007 EPITA Research and Development Laboratory
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
#include "grid2.hh"

namespace glg
{
  typedef box_iterator_<point2d> box_iterator2d;
  typedef box_iterator_<point1d> box_iterator1d;
}

int main ()
{
  using namespace glg;
  point2d p1(0, 0), p2(3, 3), p3(5, 4);
  point1d pp1(2), pp2(7);
  box2d   b(p1, p2), b2(p2, p3);
  box1d   c(pp1, pp2);
  box_iterator2d bb(b);
  box_iterator1d bc(c);
  pset_std_<point2d> tst;
  image2d<int> aie(b), pwet(b2);
  image2d<int> cocopy(aie);;
  image2d<int>::iter it(aie.bbox());
  signal<int> ouille(c);

  std::cout << "[32mParcours simple image2d[m" << std::endl;

  print(aie);

  std::cout << "[35m~~~[m" << std::endl << std::endl;

  std::cout << "[32mParcours simple signal[m" << std::endl;

  print(ouille);

  std::cout << "[35m~~~[m" << std::endl << std::endl;

  int i = 1;
  for (it.start(); it.is_valid(); it.next(), i++)
    aie(it) += i;

  std::cout << "[32mIncrementation des valeurs de image2d[m" << std::endl;

  print(aie);

  std::cout << "[35m~~~[m" << std::endl << std::endl;

  // dummy instanciations

  std::cout << "[32mInstanciation de variables de divers types[m" << std::endl;

  glg::singleton<int> sing;
  glg::vec<3, int> vec; // voir avec Theo le comportement louche de g++
			// reponse : g++ c'est gland!

  std::cout << "If you can see this message, instanciations worked!" << std::endl;

  std::cout << "[35m~~~[m" << std::endl << std::endl;

  std::cout << "[32mVec fullfillment + display[m" << std::endl;
  vec[0] = 0;
  vec[1] = 5;
  vec[2] = 1;

  std::cout << vec;

  std::cout << "[35m~~~[m" << std::endl << std::endl;


  std::cout << "[32mPolite image[m" << std::endl;

  polite_image< image2d<int> > drucker(aie);
  drucker.talk();
  //  print(drucker);

  std::cout << std::endl << "[35m~~~[m" << std::endl << std::endl;

  std::cout << "[32mValue cast image[m" << std::endl;

  value_cast_image<float, image2d<int> > imaf(aie);
  //  print(imaf);

  std::cout << std::endl << "[35m~~~[m" << std::endl << std::endl;

  std::cout << "[32mImage Stack[m" << std::endl;

  image_stack<2,image2d<int> > s;
  s.image(0) = aie;
  s.image(1) = pwet;

  //  print(s);

  std::cout << std::endl << "[35m~~~[m" << std::endl << std::endl;

  return bidon (p1, p2);
}
