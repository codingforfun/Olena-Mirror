#include <my/document/document.hh>

// THIS DEMO SHOW YOU HOW TO OBTAIN THE GRAPH
// THAT REPRESENTS RELATION BETWEEN LINES

template<typename L,typename F,typename D>
void demo_draw_line_graph(mymln::document::document<L,F,D>& doc)
{
      typedef vertex_image<point2d,bool> v_ima_g;
      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
      typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
      
   
      std::cout << "   ==   Demo : draw line graph   ==   " << std::endl;


      v_ima_g mask = doc.fun_mask_letters(); // We will work on each nodes that have a link with a letter
      mln_piter_(v_ima_g) v(mask.domain());
      nbh_t nbh(mask);
      mln_niter_(nbh_t) q(nbh, v);
      
      
      doc.debug_create_buffer(); // Create a debug buffer inside the document
      for_all(v)
      {
	  // we work on nodes that have links with a letter so q will be the letter
	  // and v an unknow node. To draw line graph we must filter v and check if v is a letter
	  if(doc.contain_letter(v) && doc.contain_line(v))
	  {
	    for_all(q)
	    {
	      if(doc.contain_line(q))
	      {
		if(doc.same_line(q,v))
		doc.debug_draw_line_green_buffer(v,q);
		else
		doc.debug_draw_line_red_buffer(v,q);
	      }
	    }
	  }
      }
      doc.debug_breakpoint(); // Stop the program and display the current buffer
      std::cout << "PRESS ENTER TO CONTINUE"; std::cin.get();
}