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
      v_ima_g mask = doc.fun_mask_letters();
      mln_piter_(v_ima_g) v(mask.domain());
      typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
      nbh_t nbh(mask);
      mln_niter_(nbh_t) q(nbh, v);
      mln::util::array<unsigned> count = mln::util::array<unsigned>(doc.size());
      mln::util::array<bool> inside = mln::util::array<bool>(doc.size());
      count.fill(0);
       inside.fill(false);
      for_all(v)
      {
	  
	unsigned link = 0;
	  for_all(q)
	  {
	    if(doc.contain_container(v) && doc.get_bbox(v).has(q))
	    {
	      
	      inside[doc[q]] = true;
	      link++;
	    }
	    else if(doc.contain_letter(v))
	    {
	      count[doc[q]]++;
	    }
	    
	      
	  }   
      }
      for(unsigned int N = 0; N  < doc.size();N++)
      {
	if(inside[N])
	  if(count[N] < 3)
	    doc.add_noise(N);
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
    void clean_letters_alone(mymln::document::document<L,F,D>& doc)
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
	if(doc.contain_alone_letter(v))
	{
	  unsigned int count = 0;
	  unsigned int real_count = 0;
	  for_all(q)
	  { 
	    if(doc.contain_alone_letter(q))
	      count++;
	    else if(doc.contain_letter(q))
	      real_count++;
	  }
	  if(real_count  < 2 && count > 2)
	  { doc.add_noise(v);}
	  else if(real_count == 0 && count > 1)
	  { doc.add_noise(v);}
	}
      }
      
    }
    
    template<typename L, typename F, typename D>
     void clean_get_lines(mymln::document::document<L,F,D>& doc)
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
		 if(doc.allign_V(q,v) && doc.allign_size(q, v) && (doc.allign_proximity_large(q, v) || doc.allign_proximity_large(v, q)) )
		 {
		   doc.add_to_line_link(v, q);
		   All_Alone = false;
		 }
	       }
	       else
	       {
		 if(doc.allign_V(q,v) && doc.allign_size(q, v) && (doc.allign_proximity_large(q, v) || doc.allign_proximity_large(v, q)))
		 {
		  doc.add_to_line_link(q, v);
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
    }
    
    
    template<typename L, typename F, typename D>
    void clean_dot_items(mymln::document::document<L,F,D>& doc)
    {
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
		doc.add_to_line_link(v2, q2);
		doc.add_letter_coerce(q2);
	      }
	    }
	  }
	}
	 doc.propage_line_link();
      }
      
      template<typename L, typename F, typename D>
      void clean_quote_items(mymln::document::document<L,F,D>& doc, std::string dgb_out,image2d<bool> s)
      {
       #ifndef NGRAPHDEBUG 
      image2d<value::rgb8> out;
      mln::initialize(out, s);
      #endif
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
		else if (doc.allign_H_Large(q, v) && doc.allign_top(v, q))
		{
		  doc.add_to_line_link(v, q);
		  draw::line(out, q,v, mln::literal::blue);
		}
	      }

	    }
	  }
	}
	 #ifndef NGRAPHDEBUG 
	 io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
	 #endif
	 doc.propage_line_link();
	 doc.recook_lines();
      }
    
    template<typename L, typename F, typename D>
    void clean_line_link_item(mymln::document::document<L,F,D>& doc)
    {
	typedef vertex_image<point2d,bool> v_ima_g;
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	v_ima_g mask = doc.fun_mask_start_end_lines();
	mln_piter_(v_ima_g) v(mask.domain());
	typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	nbh_t nbh(mask);
	mln_niter_(nbh_t) q(nbh, v);
	for_all(v)
	{
	  if(doc.contain_letter(v))
	  {
	    if(doc.contain_line(v))
	    {
	      for_all(q)
	      {
		if(
		  doc.allign_V_line(v,q) &&
		  doc.allign_center_line(v, q) &&
		  doc.allign_smaller_line(v,q) &&
		  doc.get_line_length(q) < 3 &&
		  doc.allign_proximity_line(v,q)
		  )
		{
		    doc.add_to_line_link(v, q);
		}
	      }
	    }
	  }
	}
    }


    template<typename L, typename F, typename D>
    void clean_proximity_lines(mymln::document::document<L,F,D>& doc)
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
	    if(doc.contain_line(v))
	    {
	      for_all(q)
	      {
		if(doc.allign_V(q,v) && doc.allign_size(q, v) && doc.allign_proximity(q,v))
		{
		  doc.add_to_line_link(v, q);
		}
		else if(doc.allign_size_height_line(q,v) && doc.allign_proximity_line(q,v) && doc.allign_V_line(q, v))
		{
		   doc.add_to_line_link(v, q);
		}
	      }
	    }
	  }
	}
	doc.propage_line_link();
    }
    
    template<typename L, typename F, typename D>
    void clean_paragraph_items(mymln::document::document<L,F,D>& doc, std::string dgb_out,image2d<bool> s)
    {
             #ifndef NGRAPHDEBUG 
	    image2d<value::rgb8> out;
	    mln::initialize(out, s);
	    #endif
       typedef vertex_image<point2d,bool> v_ima_g;
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	v_ima_g mask = doc.fun_mask_start_lines();
	mln_piter_(v_ima_g) v(mask.domain());
	typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	nbh_t nbh(mask);
	doc.link_paragraphs();
	mln_niter_(nbh_t) q(nbh, v);
	for_all(v)
	{
	  if(doc.contain_letter(v))
	  {
	    if(doc.contain_line(v) && doc.get_beginning_of_line(v) == doc[v])
	    {
	      doc.jump_to_paragraph(v);  
	      for_all(q)
	      {
		if(doc.allign_H_Large(q,v) && doc.allign_size(q, v))
		{
		  if(doc.contain_paragraph(q))
		  {
		    if(!doc.contain_paragraph(v))
		      {
			doc.add_to_paragraph(v);
			
		      }
		    doc.add_to_paragraph_link(q, v);
		     draw::line(out, q,v, mln::literal::green);
		  }
		  else
		  {
		    
		    if(!doc.contain_paragraph(v))
		    {
		      doc.add_to_paragraph(q);
		      doc.add_to_paragraph(v);
		      doc.add_to_paragraph_self_link(q);
		      doc.add_to_paragraph_link(q, v);
		    }
		    else
		    {
		      doc.add_to_paragraph(q);
		      doc.add_to_paragraph_link(v, q);
		    }
		    draw::line(out, q,v, mln::literal::magenta);
		  }
		 
		}
	      }
	    }
	  }
	}
	io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
	doc.propage_paragraph_link();
    }
  
      template<typename L, typename F, typename D>
      void clean_quote_lines(mymln::document::document<L,F,D>& doc)
      {
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
	      if(
		doc.get_line_length(q) < 5 &&
		doc.allign_smaller_line(v,q) && 
		doc.get_line_length(v) > 3 &&
		doc.allign_proximity_line(v,q) &&
		doc.allign_V_line(v,q) 
		)
	      {
		if(doc.allign_base_line_line(v,q) && doc.get_line_length(q) < 3)
		{doc.add_to_line_link(v, q);}
		else if(doc.allign_up_line_line(v,q))
		{doc.add_to_line_link(v, q);}
	      }
	    }
	  }
	}
      }
  }
  }
  

#endif