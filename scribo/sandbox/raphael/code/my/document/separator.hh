#ifndef INC_DOCUMENT_SEPARATOR
#define INC_DOCUMENT_SEPARATOR
#include <my/util/vector_bbox_group.hh>
#include <mln/util/graph.hh>
#include <my/document/document.hh>
using namespace mln;
namespace mymln
{
  namespace document
  {
      namespace separators
      {
	template<typename L, typename F, typename D>
	void separators_find_allign(mymln::document::document<L,F,D>& doc, std::string dgb_out, image2d<bool> s)
	{
	    image2d<value::rgb8> out;
	    mln::initialize(out, s);
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_letters();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	    for_all(v)
	    {

	      if(doc.contain_letter(v))
	      {
		doc.jump_to_separator(v);
		if((!doc.contain_implicit_separator(v)))
		{
		  doc.add_to_separator(v); 
		  doc.add_to_separator_self_link(v);
		}
		bool All_Alone = true;
		for_all(q)
		{
		  
		    if((!doc.contain_implicit_separator(q)))
		    {
		      // draw::line(out, q,v, mln::literal::blue);
		      if(doc.allign_H_Large(q,v) && doc.allign_size(q, v))
		      {
			doc.add_to_separator_link(v, q);
			  draw::line(out, q,v, mln::literal::magenta);
			  All_Alone = false;
		      }
		    }
		    else
		    {
		      if(doc.allign_H_Large(q,v) && doc.allign_size(q, v))
		      {
			doc.add_to_separator_link(q, v);
			draw::line(out, q,v, mln::literal::green);
			 All_Alone = false;
		      }
		    }
		}
		if(All_Alone){doc.invalidate_implicit_separator(v);}

	      }
	      
	    }
	    doc.propage_separator_link();
	    io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
	}
	
	template<typename L, typename F, typename D>
	void separators_make_clean(mymln::document::document<L,F,D>& doc)
	{
	  
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = doc.fun_mask_letters();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	     mln::util::array<unsigned> count = mln::util::array<unsigned>(doc.size());
	     count.fill(0);
	    for_all(v)
	    {

	    	if(doc.contain_implicit_separator(v))
		{
		  bool All_Alone = true;
		  doc.jump_to_line(v);
		  if((!doc.contain_line(v)))
		  {
		    doc.add_to_line(v);
		    doc.add_to_line_self_link(v); 
		  }

		  for_all(q)
		  {
		    
		      if(doc.contain_implicit_separator(q) && doc.same_implicit_separator(q,v))
		      {
			// draw::line(out, q,v, mln::literal::blue);
			if(doc.allign_V(q,v) && doc.allign_size(q, v))
			{
			  count[doc[q]]++;
			}
		      }
		      
		  }
		}
	    }   
	    for(unsigned int N = 0; N  < doc.size();N++)
	    {
		if(count[N] > 1)
		  doc.invalidate_implicit_separator(N);
	    }
	}
      }
  }
}
#endif