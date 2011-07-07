#ifndef INC_IMAGE_DOC
#define INC_IMAGE_DOC
#include<my/document/document.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>
#include <mln/core/image/vertex_image.hh>
using namespace mln;

namespace mymln
{
  namespace document
  {
    	template<typename L, typename F, typename D>
      void image_bad_paragraph(mymln::document::document<L,F,D>& doc)
      {
	typedef vertex_image<point2d,bool> v_ima_g;
		    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
		    v_ima_g mask = doc.fun_mask_all_letters();
		    mln_piter_(v_ima_g) v(mask.domain());
		    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
		    nbh_t nbh(mask);
		    mln_niter_(nbh_t) q(nbh, v);
		    for_all(v)
		    {
		      if(
			doc.contain_paragraph(v) &&
			doc.get_paragraph_length(v) == 1 &&
			doc.get_line_length(v) < 4
			
			)
		      {
			for_all(q)
			{
			  if
			    (
			    doc.contain_noise(q)
			    
			    
			    )
			  {
			    doc.move_line_self_link(doc[v]);
			    doc.debug_draw_line_green_buffer(q,v);
			    doc.debug_draw_box_green_buffer(v);
			    doc.add_noise(v);
			  }
			}
		      }
		    }
		    doc.propage_line_link();
      }
      
    	template<typename L, typename F, typename D>
      void image_anomalies_paragraph(mymln::document::document<L,F,D>& doc)
      {
	typedef vertex_image<point2d,bool> v_ima_g;
		    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
		    v_ima_g mask = doc.fun_mask_all_letters();
		    mln_piter_(v_ima_g) v(mask.domain());
		    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
		    nbh_t nbh(mask);
		    mln_niter_(nbh_t) q(nbh, v);
		    for_all(v)
		    {
		      if(doc.contain_end_line(v))
		      {
			doc.debug_draw_string(v, doc.get_line_length(v));
		      }
		      if(
			doc.contain_paragraph(v) &&
			doc.get_paragraph_length(v) < 3 &&
			doc.get_line_length(v)  < 20
			)
		      {
			unsigned int noise_count = 0;
			for_all(q)
			{
			  if(doc.contain_noise(q))
			  {

			    noise_count++;
			  }
			}
			if(noise_count > 3)
			{
			  doc.move_line_self_link(doc[v]);
			  doc.debug_draw_box_green_buffer(v);
			  doc.add_noise(v);
			}
		      }
		    }
      }
  }
}
#endif