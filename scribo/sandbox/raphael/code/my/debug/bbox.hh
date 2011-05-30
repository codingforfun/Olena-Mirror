#ifndef INC_DEBUG_BBOX
#define INC_DEBUG_BBOX
#include<my/util/vector_bbox_group.hh>
#include<my/util/vector_bbox.hh>
#include<iostream>
namespace mymln
{
  namespace debug
  {
    template <unsigned int D, typename C>
    inline
    void printdb(util::vector_bbox<D,C> bbox);
    
    template <unsigned int D, typename C>
    inline
    void printdb(mln::algebra::vec<D,C> vector);
    
    template<unsigned int D, typename C> inline void printdb(mln::algebra::vec<D,C> vector)
    {
      std::cout << std::endl;
      std::cout << "Vector " << D << " : " << std::endl;
       for(unsigned int N = 0;N < D; N++)
      {
	  std::cout << "      " << vector[N] << std::endl;
      }
    }
    
    template<unsigned int D, typename C> inline void printdb(util::vector_bbox<D,C> bbox)
    {
      std::cout << std::endl;
      std::cout << "Vector Bounding Box " << D << " : " << std::endl;
      std::cout << "   Start: " << std::endl;
      for(unsigned int N = 0;N < D; N++)
      {
	  std::cout << "      " << bbox.start(N) << std::endl;
      }

      std::cout << "   End: " << std::endl;
      for(unsigned int N = 0;N < D; N++)
      {
	  std::cout << "      " << bbox.end(N) << std::endl;
      }
    }
  }
}
#endif