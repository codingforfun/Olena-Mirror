#ifndef INC_VECTOR_BBOX_GROUP
#define INC_VECTOR_BBOX_GROUP
#include "vector_bbox.hh"
namespace mymln
{
  namespace util
  {
    template<unsigned int D, typename C, typename Lbl>class vector_bbox_group
    {
      typedef vector_bbox<D,C> vbbox;
      public :
	vector_bbox_group()
	{
	  bboxes = mln::util::array<vbbox>(0);
	}
	vector_bbox_group(unsigned nlabel)
	{
	  bboxes = mln::util::array<vbbox>(nlabel);
	  bboxes.fill(vbbox());
	}
	void add(mln::algebra::vec<D,C> vector, Lbl label)
	{
	  bboxes[label].add(vector);
	}
	void add(const unsigned int N, C value, Lbl label)
	{
	  
	  bboxes[label].add(N, value);
	  
	}
	inline C volume(Lbl label)
	{
	  return bboxes[label].volume();
	}
	template<typename I, typename V> void draw_to_picture(I& dest, V& value)
	{
	  for(unsigned int N = 0; N < bboxes.size(); N++)
	  {
	    draw_to_picture(dest, value);
	  }
	}
	inline bool contain(const unsigned int N,const C value, Lbl label)
	{
	  return bboxes[label].contain(N,value);
	}
	inline C start(const unsigned int N, Lbl label)
	{
	  return  bboxes[label].start(N);
	}
	inline C size(const unsigned int N, Lbl label)
	{
	  return  bboxes[label].size(N);
	}
	inline C end(const unsigned int N, Lbl label)
	{
	  return  bboxes[label].end(N);
	}
	inline unsigned int size()
	{
	  return bboxes.size();
	} 
      private:
	mln::util::array<vbbox> bboxes;
    };
  }
}
#endif