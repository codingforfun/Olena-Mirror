#ifndef INC_CLEAN_DOC
#define INC_CLEAN_DOC
#include<my/document/document.hh>
#include <mln/core/image/graph_elt_neighborhood.hh>
#include <mln/core/image/vertex_image.hh>
using namespace mln;

namespace mymln
{
  namespace document
  {
    template<typename L, typename F, typename D>
    void clean_containers_items(mymln::document::document<L,F,D>& doc)
    {
      typedef vertex_image<point2d,bool> v_ima_g;
      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
      v_ima_g mask = doc.fun_mask_containers();
      mln_piter_(v_ima_g) v(mask.domain());
      typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
      nbh_t nbh(mask);
      mln_niter_(nbh_t) q(nbh, v);
      for_all(v)
      {
	  
	  for_all(q)
	  { 
	    if(!doc.contain_container(v) && doc.get_bbox(q).has(v))
	    {
	      doc.add_noise(v);
	    }
	      
	  }
      }
      
    }
    
    template<typename L, typename F, typename D>
    void clean_letters_items(mymln::document::document<L,F,D>& doc)
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
	if(doc.contain_letter(v))
	{
	  unsigned int count = 0;
	  for_all(q)
	  { 
	    if(doc.contain_letter(q))
	      count++;
	  }
	  if(count < 1)
	  { doc.add_noise(v); }
	}
      }
      
    }
    
    template<typename L, typename F, typename D>
     void clean_get_lines(mymln::document::document<L,F,D>& doc, std::string dgb_out,image2d<bool> s)
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
	  bool All_Alone = true;
	  doc.jump_to_line(v);
	  if((!doc.contain_line(v)))
	  {
	    doc.add_to_line(v);
	    doc.add_to_line_self_link(v); 
	  }

	  for_all(q)
	  {
	    
	       if((!doc.contain_line(q)))
	       {
		 // draw::line(out, q,v, mln::literal::blue);
		 if(doc.allign_V(q,v) && doc.allign_size(q, v))
		 {
		   doc.add_to_line_link(v, q);
		    draw::line(out, q,v, mln::literal::magenta);
		   All_Alone = false;
		 }
	       }
	       else
	       {
		 if(doc.allign_V(q,v) && doc.allign_size(q, v))
		 {
		  doc.add_to_line_link(q, v);
		  draw::line(out, q,v, mln::literal::green);
		  All_Alone = false;
		 }
	       }
	  }
	  if(All_Alone)
	  {  
	    doc.add_alone_letter(v);
	  }
	}
	
      }
      doc.propage_line_link();
      io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
    }
    
    template<typename L, typename F, typename D>
    void clean_dot_items(mymln::document::document<L,F,D>& doc, std::string dgb_out,image2d<bool> s)
    {
      image2d<value::rgb8> out;
      mln::initialize(out, s);
      typedef vertex_image<point2d,bool> v_ima_g;
      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
      v_ima_g mask = doc.fun_mask_alone_letters();
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
	      if(!doc.allign_H(q, v) && doc.allign_base_line(v, q))
	      {
		draw::line(out, q,v, mln::literal::green);
		doc.add_to_line_link(v, q);
		doc.add_letter_coerce(q);
	      }
	    }
	  }
	}
	mask = doc.fun_mask_alone_letters();
	mln_piter_(v_ima_g) v2(mask.domain());
	nbh = nbh_t(mask);
	mln_niter_(nbh_t) q2(nbh, v2);
	for_all(v2)
	{
	  if(doc.contain_line(v2))
	  {
	    for_all(q2)
	    {
	      if (doc.allign_H_Large(v2, q2) && doc.allign_top(v2, q2))
	      {
		draw::line(out, q2,v2, mln::literal::magenta);
		doc.add_to_line_link(v2, q2);
		doc.add_letter_coerce(q2);
	      }
	    }
	  }
	}
	 io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
	 doc.propage_line_link();
      }
      
      template<typename L, typename F, typename D>
      void clean_quote_items(mymln::document::document<L,F,D>& doc, std::string dgb_out,image2d<bool> s)
      {
	
      image2d<value::rgb8> out;
      mln::initialize(out, s);
      typedef vertex_image<point2d,bool> v_ima_g;
      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
      v_ima_g mask = doc.fun_mask_start_end_lines();
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
	      if(doc.get_line_length(q) < 4)
	      {
		if(doc.allign_size(v,q))
		{
		  if(!doc.contain_alone_letter(v) && !doc.contain_alone_letter(q) )
		  {
		    if(
			(doc.allign_top(v, q) || doc.allign_top(doc.get_beginning_of_line(v), doc.get_label(q)))
		      )
		    {
		      doc.add_to_line_link(v, q);
		      draw::line(out, q,v, mln::literal::green);
		    }  
		  }
		  else if(doc.allign_top(v, q) && !doc.allign_H(v, q))
		  {
		    doc.add_to_line_link(v, q);	
		  }
		  else
		  {
		    draw::line(out, q,v, mln::literal::magenta);
		  }
		}
		else if (doc.allign_H_Large(q, v) && doc.allign_up_line(v, q))
		{
		  doc.add_to_line_link(v, q);
		  draw::line(out, q,v, mln::literal::blue);
		}
	      }

	    }
	  }
	}
	 io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
	 doc.propage_line_link();
	 doc.recook_lines();
      }
      
     
	
      
    }
  
  
  
  }
  

#endif