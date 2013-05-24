#ifndef KEYPOINT_HH
# define KEYPOINT_HH

# include <ostream>

class Keypoint
{
  public:
    // Constructor
    Keypoint(unsigned i, unsigned j,
             unsigned scale, unsigned size, bool maximum);
    
    // Getters
    unsigned getI();
    unsigned getJ();
    unsigned getScale();
    unsigned getSize();

  private:
    unsigned i;
    unsigned j;
    unsigned scale;
    unsigned size;
    bool maximum;
};

#endif /* ! KEYPOINT_HH */
