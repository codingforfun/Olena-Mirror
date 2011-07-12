#include <my/document/document.hh>

// THIS DEMO SHOW YOU HOW TO DRAW THE TEXT TAB
// FOR ALL PARAGRAPH STARTING WITH A RIGHT TAB
// WARNING : IN THIS DEMO WE DON'T CHECK THE ALLIGNEMENT
// SO IN SOME CASE THE METHOD CAN FAIL

template<typename L,typename F,typename D>
void demo_draw_tab(mymln::document::document<L,F,D>& doc)
{
      typedef vertex_image<point2d,bool> v_ima_g;
      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
      typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
      
   
      std::cout << "   ==   Demo : draw tab   ==   " << std::endl;


      v_ima_g mask = doc.fun_mask_start_lines(); // We will work on each nodes that have a link with an end of line
      mln_piter_(v_ima_g) v(mask.domain());
      nbh_t nbh(mask);
      mln_niter_(nbh_t) q(nbh, v);
      
      
      doc.debug_create_buffer();
      for_all(v)
      {
	    for_all(q)
	    {
	      if(doc.contain_paragraph(q) && doc.paragraph_start_with_tab(q))
	      {
		if(doc.get_line_ID(q) == doc.get_first_line_ID(q))
		{
		  doc.debug_draw_box_green_buffer(q);
		  doc.debug_draw_line_green_buffer(doc.get_line_bbox(q).pmin(), doc.get_paragraph_bbox(q).pmin());
		}
		
	      }
	     
	    }
      }
      
      doc.debug_breakpoint(); 
      std::cout << "PRESS ENTER TO CONTINUE"; std::cin.get();
}