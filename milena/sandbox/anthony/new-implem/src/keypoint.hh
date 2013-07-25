#ifndef KEYPOINT_HH
# define KEYPOINT_HH

# include <ostream>

class Keypoint
{
  public:
    Keypoint(int _x, int _y,
             unsigned _octave, unsigned _gradient,
             bool _maximum)
    {
      x = _x;
      y = _y;
      octave = _octave;
      gradient = _gradient;
      maximum = _maximum;
    }

    inline int getX() const { return x; }
    inline int getY() const { return y; }
    inline unsigned getOctave() const { return octave; }
    inline unsigned getGradient() const { return gradient; }
    inline bool getType() const { return maximum; }
    inline float getOrientation() const { return orientation; }

    inline void setX(int _x) { x = _x; }
    inline void setY(int _y) { y = _y; }
    inline void setOrientation(float _o) { orientation = _o; }

    friend std::ostream& operator<< (std::ostream& stream, const Keypoint& k)
    {
      stream << "(" << k.getX() << "," << k.getY() << ")"
             << " o:" << k.getOctave() << " g:" << k.getGradient();

      return stream;
    }

  private:
      int       x;
      int       y;
      unsigned  octave;
      unsigned  gradient;
      bool      maximum;
      float     orientation;
};

#endif /* ! KEYPOINT_HH */
