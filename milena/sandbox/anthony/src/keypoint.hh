#ifndef KEYPOINT_HH
# define KEYPOINT_HH

//# include <mln/core/alias/point2d.hh>
# include <ostream>
# include "matrix.hh"

class Keypoint
{
  public:
    // Constructor
    Keypoint(int i, int j,
             int scale, int size, bool maximum);

    bool add(Matrix& k, Keypoint& old);
    //mln::point2d getPoint();
    
    // Getters
    int getI();
    int getJ();
    int getScale();
    int getSize();

    void setI(int offsetI);
    void setJ(int offsetJ);
    void setScale(int offsetScale);

  private:
    int i;
    int j;
    int scale;
    int size;
    bool maximum;
};

#endif /* ! KEYPOINT_HH */
