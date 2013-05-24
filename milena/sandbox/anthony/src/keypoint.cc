#include "keypoint.hh"

// Constructor
Keypoint::Keypoint(unsigned i,     unsigned j,
                   unsigned scale, unsigned size, bool maximum)
{
  this->i = i;
  this->j = j;
  this->scale = scale;
  this->size = size;
  this->maximum = maximum;
}

// Getters
unsigned Keypoint::getI() { return i; }
unsigned Keypoint::getJ() { return j; }
unsigned Keypoint::getScale() { return scale; }
unsigned Keypoint::getSize() { return size; }
