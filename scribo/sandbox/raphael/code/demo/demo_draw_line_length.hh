#include <my/document/document.hh>

// THIS DEMO SHOW YOU HOW TO DRAW THE LENGTH OF EACH
// LINE DIRECTLY ON A DEBUG BUFFER

template<typename L,typename F,typename D>
void demo_draw_line_length(mymln::document::document<L,F,D>& doc)
{
      typedef vertex_image<point2d,bool> v_ima_g;
      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
      typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
      
   
      std::cout << "   ==   Demo : draw line length   ==   " << std::endl;


      v_ima_g mask = doc.fun_mask_end_lines(); // We will work on each nodes that have a link with an end of line
      mln_piter_(v_ima_g) v(mask.domain());
      nbh_t nbh(mask);
      mln_niter_(nbh_t) q(nbh, v);
      
      
      doc.debug_create_buffer();
      for_all(v)
      {
	    for_all(q)
	    {
	       doc.debug_draw_string(q, doc.get_line_length(q)); // Here we ask the document to draw near q the length of the line
	    }
      }
      
      doc.debug_breakpoint(); 
      std::cout << "PRESS ENTER TO CONTINUE"; std::cin.get();
}