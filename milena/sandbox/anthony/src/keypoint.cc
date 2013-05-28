#include "keypoint.hh"

// Constructor
Keypoint::Keypoint(int i,     int j,
                   int scale, int size, bool maximum)
{
  this->i = i;
  this->j = j;
  this->scale = scale;
  this->size = size;
  this->maximum = maximum;
}

bool Keypoint::add(Matrix& offset, Keypoint& old)
{
  if (offset.get(0, 0) >= 1 || offset.get(0, 0) <= -1
      || offset.get(1, 0) >= 1 || offset.get(1, 0) <= -1)
  {
    i += offset.get(0, 0);
    j += offset.get(1, 0);
    scale += offset.get(2, 0);

    return (i != old.getI() && j != old.getJ());
  }
  else
    return false;
}

//mln::point2d Keypoint::getPoint()
//{
  //mln::point2d p(i, j);

  //return p;
//}

// Getters
int Keypoint::getI() { return i; }
int Keypoint::getJ() { return j; }
int Keypoint::getScale() { return scale; }
int Keypoint::getSize() { return size; }

// Setters
void Keypoint::setI(int offsetI) { i = offsetI; }
void Keypoint::setJ(int offsetJ) { j = offsetJ; }
void Keypoint::setScale(int offsetScale) { scale = offsetScale; }
