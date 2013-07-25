#ifndef SCALE_SPACE_HH
# define SCALE_SPACE_HH

# include <string>
# include <vector>

#include <mln/core/image/image2d.hh>
#include <mln/data/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/literal/all.hh>
#include <mln/value/int_s8.hh>

#include "vrac/vrac.hh"

using namespace mln;

template<typename I>
class ScaleSpace
{
  public:
    ScaleSpace(unsigned octave_level, unsigned gradient_level)
    {
      o = octave_level;
      g = gradient_level;
    }

    ~ScaleSpace()
    {
      typedef typename std::vector< std::vector<I> >::iterator PIT;

      for (PIT it = pyramid.begin(); it != pyramid.end(); ++it)
        it->clear();

      pyramid.clear();
    }

    std::vector<I>& getOctave(unsigned i)
    {
      return pyramid[i];
    }


    void build(const I& original)
    {
      // Upscaled octave
      I upscaled = vrac::upscaling2(original);
      addOctave(upscaled);

      // 1st octave
      addOctave(original);

      // 2nd octave
      I downscaled = vrac::downscaling2(original);
      addOctave(downscaled);

      for (unsigned i = 3; i < o; ++i)
      {
        downscaled = vrac::downscaling2(downscaled);
        addOctave(downscaled);
      }
    }

    void save()
    {
      std::stringstream name;

      for (unsigned i = 0; i < pyramid.size(); ++i)
      {
        for (unsigned j = 0; j < pyramid[i].size(); ++j)
        {
          name << "output/ss-o" << i << "-" << j << ".pgm";
          io::pgm::save(pyramid[i][j], name.str());
          name.str("");
        }
      }
    }

    inline unsigned octave_count() { return pyramid.size(); }
    inline unsigned gradient_count() { return pyramid[0].size(); }
    
  private:
    unsigned o;
    unsigned g;
    std::vector< std::vector<I> > pyramid;
    
    void addOctave(const I& original)
    {
      I blured;
      std::vector<I> octave;
      //float variance = sqrt(2);
      float variance = 0.84089642;

      for (unsigned i = 0; i < g; ++i)
      {
        blured = vrac::blur(original, variance);
        variance *= sqrt(2);
        octave.push_back(blured);
      }

      pyramid.push_back(octave);
    }
};

#endif /* ! SCALE_SPACE_HH */
