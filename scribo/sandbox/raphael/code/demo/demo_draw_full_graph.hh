#include <my/document/document.hh>

// THIS DEMO SHOW YOU HOW TO OBTAIN THE GRAPH
// THAT REPRESENTS THE DOCUMENT

template<typename L,typename F,typename D>
void demo_draw_full_graph(mymln::document::document<L,F,D>& doc)
{
      typedef vertex_image<point2d,bool> v_ima_g;
      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
      typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
      
   
      std::cout << "   ==   Demo : draw full graph   ==   " << std::endl;


      v_ima_g mask = doc.fun_mask_all(); // We will work on all nodes
      mln_piter_(v_ima_g) v(mask.domain());
      nbh_t nbh(mask);
      mln_niter_(nbh_t) q(nbh, v);
      
      
      doc.debug_create_buffer(); // Create a debug buffer inside the document
      for_all(v)
      {
	  for_all(q)
	  {
	    doc.debug_draw_line_orange_buffer(v,q);
	  }
      }
      doc.debug_breakpoint(); // display the current buffer
       std::cout << "PRESS ENTER TO CONTINUE"; std::cin.get();
}