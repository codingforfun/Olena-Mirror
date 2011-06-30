#ifndef MLN_DEBUG_DOCUMENT_REMOTE_LIB
#define MLN_DEBUG_DOCUMENT_REMOTE_LIB
#include <my/debug/remote/document_remote.hh>
using namespace mln;
namespace mymln
{
  namespace document
  {
    namespace debug
    {
      
      template<typename L, typename F, typename D>
      bool allign_V(const document<L,F,D>& doc, const point2d& A,const point2d& B){return doc.allign_V(A,B);}
      template<typename L, typename F, typename D>
      bool allign_V_line(const document<L,F,D>& doc, const point2d& A,const point2d& B){return doc.allign_V_line(A,B);}
      template<typename L, typename F, typename D>
      bool allign_V_line_strict(const document<L,F,D>& doc, const point2d& A,const point2d& B){return doc.allign_V_line_strict(A,B);}
      template<typename L, typename F, typename D>
      bool allign_V_large(const document<L,F,D>& doc, const point2d& A,const point2d& B){return doc.allign_V_large(A,B);}
      
      
      template<typename L, typename F, typename D>
      void load(const remote<L,F,D>& rem)
      {
	typedef document< L, F, D > doc;
	rem.add_function_pp("allign_V_large", &(allign_V_large));
	rem.add_function_pp("allign_V_line_strict", &(allign_V_line_strict));
	rem.add_function_pp("allign_V_line", &(allign_V_line));
	rem.add_function_pp("allign_V", &(allign_V));
      }
    }
  }
}
#endif