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
#include "matrix.hh"

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
            const static unsigned max_interpolation_depth = 5;
            unsigned count = 0;
            bool enough = false;
            std::list<Keypoint>::iterator k = keypoints.begin();
            Matrix* offset = NULL;
            Matrix* previous_offset = NULL;

            while (k != keypoints.end())
            {
                while (!enough && count < max_interpolation_depth)
                {
                    std::cout << "test1" << std::endl;
                    unsigned octave = k->getOctave();
                    unsigned gradient = k->getGradient();
                    S dog = ScaleSpace<S>::pyramid[octave][gradient];
                    S dog_up = ScaleSpace<S>::pyramid[octave][gradient + 1];
                    S dog_down = ScaleSpace<S>::pyramid[octave][gradient - 1];
                    unsigned col = k->getX();
                    unsigned row = k->getY();
                    std::cout << "test2" << std::endl;

                    /* Set the neighbors of the current level:
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

                    /* Set the neighbors of the upper level:
                     *      u1
                     *  u2  u   u3
                     *      u4
                     */
                    float u = dog_up(mln_psite(S)(row, col));
                    float u1 = dog_up(mln_psite(S)(row - 1, col));
                    float u2 = dog_up(mln_psite(S)(row, col - 1));
                    float u3 = dog_up(mln_psite(S)(row, col + 1));
                    float u4 = dog_up(mln_psite(S)(row + 1, col));

                    /* Set the neighbors of the lower level:
                     *      l1
                     *  l2  l   l3
                     *      l4
                     */
                    float l = dog_down(mln_psite(S)(row, col));
                    float l1 = dog_down(mln_psite(S)(row - 1, col));
                    float l2 = dog_down(mln_psite(S)(row, col - 1));
                    float l3 = dog_down(mln_psite(S)(row, col + 1));
                    float l4 = dog_down(mln_psite(S)(row + 1, col));

                    // Compute the offset

                    float dxx = n3 - 2.0 * p + n2;
                    float dyy = n4 - 2.0 * p + n1;
                    float dss = u - 2.0 * p + l;

                    float dxy = (n8 - n6 - n7 + n5) / 4.0;
                    float dxs = (u3 - u2 - l3 + l2) / 4.0;
                    float dys = (u4 - u1 - l4 + l1) / 4.0;

                    std::cout << "test3" << std::endl;
                    Matrix m1(3, 1);
                    m1.initialize();
                    m1.set(0, 0, n3 - n2);
                    m1.set(1, 0, n4 - n1);
                    m1.set(2, 0, u - l);

                    Matrix m2(3, 3);
                    m2.initialize();
                    m2.set(0, 0, dxx);  m2.set(0, 1, dxy);  m2.set(0, 2, dxs);
                    m2.set(1, 0, dxy);  m2.set(1, 1, dyy);  m2.set(1, 2, dys);
                    m2.set(2, 0, dxs);  m2.set(2, 1, dys);  m2.set(2, 2, dss);

                    std::cout << "test4" << std::endl;
                    previous_offset = offset;
                    offset = m2.solve3x3(m1);
                    std::cout << "test5" << std::endl;

                    if (offset == NULL)
                    {
                        offset = previous_offset;
                        enough = true;
                    }
                    else
                    {
                        if (std::abs(offset->get(0,0)) > 0.5 ||
                                std::abs(offset->get(1,0)) > 0.5 ||
                                std::abs(offset->get(2,0)) > 0.5)
                        {
                            std::cout << "test51" << std::endl;
                            ++count;
                            k->setX(k->getX() + (int) offset->get(0, 0));
                            k->setY(k->getX() + (int) offset->get(1, 0));
                            k->setGradient(k->getGradient() + (int) offset->get(1, 0));

                            //FIXME Check boundaries

                            if (k->getGradient() < 1 ||
                                    k->getGradient() >= ScaleSpace<S>::pyramid[0].size() - 1)
                                count = max_interpolation_depth; // Remove keypoint
                        }
                        else
                        {
                            std::cout << "test52" << std::endl;
                            enough = true;
                        }
                    }
                }

                std::cout << "test6" << std::endl;
                if (count >= max_interpolation_depth)
                {
                    std::cout << "test61" << std::endl;
                    k = keypoints.erase(k);
                }
                else
                {
                    std::cout << "test62" << std::endl;
                    unsigned octave = k->getOctave();
                    unsigned gradient = k->getGradient();

                    S dog = ScaleSpace<S>::pyramid[octave][gradient];
                    S dog_up = ScaleSpace<S>::pyramid[octave][gradient + 1];
                    S dog_down = ScaleSpace<S>::pyramid[octave][gradient - 1];
                    unsigned col = k->getX();
                    unsigned row = k->getY();

                    /* Set the neighbors of the current level:
                     *      n1
                     *  n2  p   n3
                     *      n4
                     */
                    float p = dog(mln_psite(S)(row, col));

                    float n1 = dog(mln_psite(S)(row - 1, col));
                    float n2 = dog(mln_psite(S)(row, col - 1));
                    float n3 = dog(mln_psite(S)(row, col + 1));
                    float n4 = dog(mln_psite(S)(row + 1, col));

                    // Set the neighbors of the upper/lower level:
                    float u = dog_up(mln_psite(S)(row, col));
                    float l = dog_down(mln_psite(S)(row, col));

                    Matrix firstOrder(3, 1);
                    firstOrder.initialize();

                    /************ WARNING ************/
                    /*       Maybe missing /255      */
                    /*********************************/

                    firstOrder.set(0, 0, n3 - n2);
                    firstOrder.set(1, 0, n4 - n1);
                    firstOrder.set(2, 0, u - l);

                    float product = firstOrder.dotProduct(offset);
                    float contrast = p + product / 2.0;

                    if (contrast < 0.03)
                        k = keypoints.erase(k);
                    else
                        k++;
                }

                enough = false;
                count = 0;
            }
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
