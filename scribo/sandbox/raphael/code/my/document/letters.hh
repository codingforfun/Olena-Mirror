#ifndef INC_CLEAN_LETTER_DOC
#define INC_CLEAN_LETTER_DOC
#include<my/document/document.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>
#include <mln/core/image/vertex_image.hh>
using namespace mln;

namespace mymln
{
  namespace document
  {
    template<typename L, typename F, typename D>
    void clean_letters_aberations_big(document<L,F,D> doc, mln::util::array<box2> bouningbox_letters, mln::util::array<box2> middle_box_lines, mymln::util::union_find<L> letters_union, )
    {
      for(int N = 0; N < letters_union.size(); N++)
      {
	if(letters_union[N])
	{
	  doc.get_letter_middle_height(N) * 3 < doc.get_bbox(N).len(0);
	  doc.add_noise(N);
	}
      }
    }
  }
}