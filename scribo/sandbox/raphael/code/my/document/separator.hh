#ifndef INC_DOCUMENT_SEPARATOR
#define INC_DOCUMENT_SEPARATOR
#include <my/util/vector_bbox_group.hh>
#include <mln/util/graph.hh>
#include <mln/document/document.hh>
using namespace mln;
namespace mymln
{
  namespace document
  {
      namespace separators
      {
	template<typename L, typename F, typename D>
	void clean_containers_items(mymln::document::document<L,F,D>& doc, std::string dgb_out, image2d<bool> s)
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
		if((!doc.contain_implicit_separator(v)))
		{
		  doc.add_to_implicit_separator(v);
		  doc.add_to_implicit_separator_self_link(v); 
		}

		for_all(q)
		{
		  
		    if((!doc.contain_line(q)))
		    {
		      // draw::line(out, q,v, mln::literal::blue);
		      if(doc.allign_H(q,v) && doc.allign_size(q, v))
		      {
			doc.add_to_implicit_separator_link(v, q);
			  draw::line(out, q,v, mln::literal::magenta);
			All_Alone = false;
		      }
		    }
		    else
		    {
		      if(doc.allign_V(q,v) && doc.allign_size(q, v))
		      {
			doc.add_to_implicit_separator_link(q, v);
			draw::line(out, q,v, mln::literal::green);
			All_Alone = false;
		      }
		    }
		}

	      }
	      
	    }
	    doc.propage_implicit_separator_link();
	    io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
	}
      }
  }
}