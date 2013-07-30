#ifndef DOG_SPACE_HH
# define DOG_SPACE_HH

# include <string>
# include <vector>
# include <list>

#include <mln/core/image/image2d.hh>
#include <mln/data/all.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/literal/all.hh>
#include <mln/value/int_s16.hh>

#include "scale-space.hh"
#include "keypoint.hh"

using namespace mln;

template<typename I, typename S>
class DoGSpace : protected ScaleSpace<S>
{
  public:
    DoGSpace(ScaleSpace<I>* _ss)
    {
      ss = _ss;
    }

    ~DoGSpace()
    {
      ScaleSpace<S>::~ScaleSpace();
    }

    void build(void)
    {
      ScaleSpace<S>::o = ss->octave_count();
      ScaleSpace<S>::g = ss->gradient_count() - 1;

      typedef typename std::vector< std::vector<I> >::iterator PIT;
      typedef typename std::vector<I>::iterator IIT;

      std::vector< std::vector<I> > sspyramid = ss->getPyramid();
      S dog;

      for (PIT pit = sspyramid.begin(); pit != sspyramid.end(); ++pit)
      {
        std::vector<S> octave;
        for (IIT iit = pit->begin(); (iit + 1) != pit->end(); ++iit)
        {
          initialize(dog, *iit);
          mln_piter(I) p(iit->domain());

          for_all(p)
          {
            value::int_s16 up = (value::int_s16) (*iit)(p);
            value::int_s16 down = (value::int_s16) (*(iit+1))(p);
            value::int_s16 diff = down - up;
            //value::int_s16 diff = ( (int) (*iit)(p) ) - ( (int) ((*(iit+1))(p)) );

            //std::cout << up << " - " << down << " = " << diff << std::endl;

            dog(p) = diff;
          }
          octave.push_back(dog);
        }
        ScaleSpace<S>::pyramid.push_back(octave);
      }
    }

    void findKeypoints(std::list<Keypoint>& keypoints)
    {
      typedef typename std::vector< std::vector<S> >::iterator PIT;
      typedef typename std::vector<S>::iterator IIT;

      unsigned octave = 0;
      for (PIT pit = ScaleSpace<S>::pyramid.begin(); pit != ScaleSpace<S>::pyramid.end(); ++pit)
      {
        unsigned gradient = 1;
        for (IIT iit = pit->begin() + 1; (iit + 1) != pit->end(); ++iit)
        {
          S upper = *(iit-1);
          S current = *iit;
          S lower = *(iit+1);

          mln_piter(I) p(current.domain());

          for_all(p)
          {
            value::int_s16 center = current(p);
            value::int_s16 min = 255;
            value::int_s16 max = -256;
            mln_niter(neighb2d) n(c8(), p);

            int extrema = isExtrema(current, center, n);

            if (extrema == -1) // Is a minimum
            {
              findMin(min, lower, upper, p, n);
              if (center < min)
                keypoints.push_back(Keypoint(p.col(), p.row(), octave, gradient, false));
            }
            else if (extrema == 1) // Is a maximum
            {
              findMax(max, lower, upper, p, n);
              if (center > max)
                keypoints.push_back(Keypoint(p.col(), p.row(), octave, gradient, true));
            }
          }

          ++gradient;
        }

        ++octave;
      }
    }

    void discardLowContrastKeypoints(std::list<Keypoint>& keypoints)
    {
      keypoints = keypoints;
    }

    void eliminateEdgeResponses(std::list<Keypoint>& keypoints)
    {
      const static float bound_ratio = 12.1;

      for (std::list<Keypoint>::iterator k = keypoints.begin();
           k != keypoints.end(); ++k)
      {
        unsigned octave = k->getOctave();
        unsigned gradient = k->getGradient();
        S dog = ScaleSpace<S>::pyramid[octave][gradient];
        unsigned col = k->getX();
        unsigned row = k->getY();

        /* Set the neighbors:
         *  n5  n1  n6
         *  n2  p   n3
         *  n7  n4  n8
         */
        float p = dog(mln_psite(S)(row, col));

        float n1 = dog(mln_psite(S)(row - 1, col));
        float n2 = dog(mln_psite(S)(row, col - 1));
        float n3 = dog(mln_psite(S)(row, col + 1));
        float n4 = dog(mln_psite(S)(row + 1, col));

        float n5 = dog(mln_psite(S)(row - 1, col - 1));
        float n6 = dog(mln_psite(S)(row - 1, col + 1));
        float n7 = dog(mln_psite(S)(row + 1, col - 1));
        float n8 = dog(mln_psite(S)(row + 1, col + 1));

        // Set Hessian matrix and find the ratio

        float dxx = n3 - 2.0 * p + n2;
        float dyy = n4 - 2.0 * p + n1;
        float dxy = (n8 - n6 - n7 + n5) / 4.0;

        float trace = dxx + dyy;
        float determinant = dxx * dyy - dxy * dxy;
        float ratio = (trace * trace) / determinant;

        if (determinant <= 0 || ratio >= bound_ratio)
          keypoints.erase(k);
      }
    }

    virtual void save()
    {
      //std::stringstream name;

      //for (unsigned i = 0; i < ScaleSpace<S>::pyramid.size(); ++i)
      //{
        //for (unsigned j = 0; j < ScaleSpace<S>::pyramid[i].size(); ++j)
        //{
          //name << "output/dogs-o" << i << "-" << j << ".pgm";
          //io::pgm::save(ScaleSpace<I>::pyramid[i][j], name.str());
          //name.str("");
        //}
      //}
    }

  private:
    ScaleSpace<I>* ss;

    int isExtrema(S& current, value::int_s16& center,
                  typename neighb2d::fwd_niter n)
    {
      bool min = true;
      bool max = true;

      for_all(n)
      {
        min = (min && center < current(n));
        max = (max && center > current(n));
      }

      if (min)
        return -1;
      else if (max)
        return 1;
      else
        return 0;
    }

    void findMin(value::int_s16& min, S& lower, S& upper,
                 typename S::piter& p, typename neighb2d::fwd_niter& n)
    {
      min = (lower(p) < min) ? lower(p) : min;
      min = (upper(p) < min) ? upper(p) : min;

      for_all(n)
      {
        min = (lower(n) < min) ? lower(n) : min;
        min = (upper(n) < min) ? upper(n) : min;
      }
    }

    void findMax(value::int_s16& max, S& lower, S& upper,
                 typename S::piter& p, typename neighb2d::fwd_niter& n)
    {
      max = (lower(p) > max) ? lower(p) : max;
      max = (upper(p) > max) ? upper(p) : max;

      for_all(n)
      {
        max = (lower(n) > max) ? lower(n) : max;
        max = (upper(n) > max) ? upper(n) : max;
      }
    }
};

#endif /* ! DOG_SPACE_HH */
