#ifndef INC_VECTOR_BBOX
#define INC_VECTOR_BBOX
#include <mln/draw/box.hh>
namespace mymln
{
  namespace util
  {
    template<unsigned int D, typename C>class vector_bbox
    {
      public :
	vector_bbox()
	{
	  init = false;
	   for(unsigned int N = 0; N < D; N++)
	  {
	    initN[N] = false;
	  }
	}
	void add(const unsigned int N, C value)
	{
	  if(!init && !initN[N])
	  {
	    max[N] = value;
	    min[N] = value;
	    initN[N] = true;
	    for(unsigned int I = 0; I < D; I++)
	    {
	      if(!initN[I]){return;}
	    }
	    init = true;
	    return;
	  }
	  if(value > max[N]){max[N] = value;}
	  if(value < min[N]){min[N] = value;}
	}
	void add(mln::algebra::vec<D,C> vector)
	{
	  if(!init)
	  {
	    for(unsigned int N = 0; N < D; N++)
	    {
	      max[N] = vector[N];
	      min[N] = vector[N];
	    }
	    init = true;
	  return;
	  }
	  for(unsigned int N = 0; N < D; N++)
	  {
	    if(vector[N] > max[N]){max[N] = vector[N];}
	    if(vector[N] < min[N]){min[N] = vector[N];}
	  }
	}
	// one define the value to use as one;
	// it is usualy 1 !
	inline C volume()
	{
	  C Acc = 1;
	  for(unsigned int N = 0; N < D; N++)
	  {
	    Acc *= size(N);
	  }
	  return Acc;
	}
	inline bool contain(const unsigned int N,const C value)
	{
	  return max[N] >= value && min[N] <= value;
	}
	inline C size(const unsigned int N)
	{
	  return max[N] - min[N];
	}
	inline C start(const unsigned int N)
	{
	  return min[N];
	}
	inline C end(const unsigned int N)
	{
	  return max[N];
	}
	
	/* NOT VERY GENERIC : REMOVE AFTER TEST*/
	
	template<typename I, typename V> void draw_to_picture(I& dest, V& value)
	{
	  mln::draw::box(dest, mln::box<mln::point2d>(mln::point2d(min[0],min[1]),mln::point2d(max[0], max[1])), value);
	}
	
      private:
	bool init;
	mln::algebra::vec<D,bool>initN;
	mln::algebra::vec<D,C> max;
	mln::algebra::vec<D,C> min;
    };
  }
}
#endif