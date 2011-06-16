#ifndef INC_RECOGNITION_DOC
#define INC_RECOGNITION_DOC
#include<my/document/document.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>
#include <mln/core/image/vertex_image.hh>
using namespace mln;

namespace mymln
{
  namespace document
  {
          template<typename L, typename F, typename D>
	  void recognize_minus(mymln::document::document<L,F,D>& doc)
	  {
			typedef vertex_image<point2d,bool> v_ima_g;
			typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
			v_ima_g mask = doc.fun_mask_letters();
			mln_piter_(v_ima_g) v(mask.domain());
			typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
			nbh_t nbh(mask);
			mln_niter_(nbh_t) q(nbh, v);
			for_all(v)
			{
			  if(doc.contain_line(v))
			  {
			    for_all(q)
			    {
			      if(doc.contain_line(q) && doc.line_median(q) && doc.letter_ratio_XY(q) >= 3.0f)
			      {
				doc.tag_label(q, "-");
			      }
			    }
			  }
			}
			doc.propage_paragraph_link();
			
	  }
	   template<typename L, typename F, typename D>
	  void recognize_dot(mymln::document::document<L,F,D>& doc)
	  {
			typedef vertex_image<point2d,bool> v_ima_g;
			typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
			v_ima_g mask = doc.fun_mask_letters();
			mln_piter_(v_ima_g) v(mask.domain());
			typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
			nbh_t nbh(mask);
			mln_niter_(nbh_t) q(nbh, v);
			for_all(v)
			{
			  if(doc.contain_line(v))
			  {
			    for_all(q)
			    {
			      if( doc.contain_line(q) && doc.same_line(q,v) && doc.line_base(q) && doc.line_size_small(q))
			      {
				if(doc.letter_ratio_XY(q) > 0.7f && doc.letter_ratio_XY(q) < 1.3f)
				  doc.tag_label(q, ".");
				else if(doc.letter_ratio_XY(q) <= 0.7f)
				  doc.tag_label(q, ",");
				else
				  doc.tag_label(q, "_");
				
			      }
			    }
			  }
			}
			doc.propage_paragraph_link();
			
	  }
  }
}
  

#endif