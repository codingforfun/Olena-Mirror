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
      v_ima_g mask = doc.fun_mask_all_letters();
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
		 if(doc.align_V(q,v) && doc.align_size_medium(q, v) && doc.align_proximity_large(q, v) )
		 {
		   doc.add_to_line_link(v, q);
		   All_Alone = false;
		 }
	       }
	       else
	       {
		 if(doc.align_V(q,v) && doc.align_size_medium(q, v) && doc.align_proximity_large(q, v))
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
	      if(
		!doc.align_H(q, v) &&
		doc.align_base_line_line_strict(v, q) &&
		doc.align_proximity(v,q) &&
		doc.align_smaller_line(v, q) &&
		doc.get_line_length(v) > 2)
	      {
		doc.add_to_line_link(v, q);
		doc.add_letter_coerce(q);
	      }
	      else if(
		doc.is_start_end_line(v) &&
		doc.align_base_line(v, q) &&
		doc.align_small_item(v,q) &&
		!doc.contain_alone_letter(v) &&
		doc.align_proximity_large_left(v,q) &&
		doc.letter_ratio_YX(q) >= 1
		)
	      {
		  doc.debug_draw_line_green_buffer(v,q);
		  doc.add_to_line_link(v,q);
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
	  if(doc.contain_line(v2) && !doc.contain_alone_letter(v2))
	  {
	    for_all(q2)
	    {
	      if (
		doc.align_H_large(v2, q2) &&
		
		doc.line_has(v2, q2) &&
		doc.letter_ratio_XY(q2) < 2 &&
		doc.align_size_width_large(v2, q2))
	      {
		if(doc.align_top(v2, q2))
		{
		
		doc.add_to_line_link(v2, q2);
		doc.add_letter_coerce(q2);
		if(doc.align_H(v2,q2))
		  {doc.merge(v2,q2); doc.tag_label(v2, "i");doc.debug_draw_line_green_buffer(q2,v2);}
		else if(doc.align_H_tube(v2, q2))
		  {doc.merge(v2,q2); doc.tag_label(v2, "i");doc.debug_draw_line_green_buffer(q2,v2);}
		}
	      }
	    }
	  }
	}
	 doc.propage_line_link();
      }
      
      template<typename L, typename F, typename D>
      void clean_quote_items(mymln::document::document<L,F,D>& doc)
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
	      if(doc.get_line_length(q) < 4)
	      {
		if(doc.align_size(v,q))
		{
		  if(!doc.contain_alone_letter(v) && !doc.contain_alone_letter(q)  && doc.align_proximity_large_left(v,q) )
		  {
		    if(
			(doc.align_top(v, q) || doc.align_top(doc.get_beginning_of_line(v), doc.get_label(q)) ) &&
			doc.align_smaller_line_letter(v,q)
		      )
		    {
		      if(doc.get_line_length(q) < 3 || doc.align_V_line(v, q))
		      {
			doc.add_to_line_link(v, q);
			doc.tag_label(v, "'");
		      }
		    }  
		  }
		  else if(doc.align_top(v, q) && !doc.align_H(v, q) && doc.align_proximity_large_left(v,q) && doc.align_smaller_line_letter(v,q))
		  {
		    if(doc.get_line_length(q) < 3 || doc.align_V_line(v, q))
		    {
		      doc.add_to_line_link(v, q);
		      doc.tag_label(v, "'");
		    }
		  }
		}
		else if (doc.align_H_large(q, v) && doc.align_top(v, q) && doc.align_size_width_large(v, q))
		{
		  doc.add_to_line_link(v, q);
		  if(doc.align_H(v,q))
		  {doc.merge(v,q); doc.tag_label(v, "i");}
		}
	      }

	    }
	  }
	}
	 doc.propage_line_link();
	 doc.recook_lines();
      }
      
      
      
      
      
      template<typename L, typename F, typename D>
      void clean_apostrophe_items(mymln::document::document<L,F,D>& doc)
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
	  if(doc.contain_line(v) )
	  {
	    for_all(q)
	    { 
	      if(
		doc.get_line_length(q) == 1 &&
		doc.line_reciprocal(q,v) &&
		doc.align_top(v,q) &&
		doc.letter_ratio_YX(q) > 1 &&
		!doc.align_H_tube(v,q) &&
		doc.align_proximity_strict_left(v,q) &&
		doc.align_small_item_large(v,q)
		
		)
	      {
		doc.debug_draw_line_green_buffer(q,v);
		doc.add_to_line_link(v,q);
	      }
	    }
	  }
	}
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
		  doc.align_V_line(v,q) &&
		  doc.align_center_line(v, q) &&
		  doc.align_smaller_line(v,q) &&
		  doc.get_line_length(q) < 3 &&
		  doc.align_proximity_line(v,q)
		  )
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
    void clean_proximity_letters(mymln::document::document<L,F,D>& doc)
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
	    if(doc.contain_line(v))
	    {
	      for_all(q)
	      {
		  if(
		    doc.contain_line(q) &&
		    !doc.same_line(q,v) &&
		    doc.get_line_length(q) == 1 &&
		    doc.line_influence_has(v,q) &&
		    !doc.align_H_large(v,q) &&
		    (doc.align_base_line_strict(v,q) || doc.align_V(v,q))
		    )
		  {
		    doc.debug_draw_line_green_buffer(q,v);
		      doc.add_to_line_link(q,v); 
		  }
	      }
	    }
	}
	doc.propage_line_link();
    }


    template<typename L, typename F, typename D>
    void clean_proximity_lines(mymln::document::document<L,F,D>& doc)
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
	    if(doc.contain_line(v))
	    {
	      for_all(q)
	      {
		
		if(doc.same_line(q,v)){ continue;}
		
		if(doc.contain_alone_letter(q))
		{
		  
		  if(doc.align_V(q,v) && doc.align_proximity_strict(q, v) && doc.align_size_height(q, v))
		  {
		    doc.add_to_line_link(v, q);
		
		  }
		  		    
		}
		else if(doc.contain_line(q))
		{ 
		  
		  if(doc.align_V(q,v) && doc.align_size_height_line_strict(q, v) && doc.align_proximity_strict(q,v))
		  {
		    doc.add_to_line_link(v, q);
		    
		  }
		  else if(doc.align_size_height_line(q,v))
		  {
		   
		    if(doc.align_proximity_line(q,v) && doc.align_V_line_strict(q, v))
		    {
			doc.add_to_line_link(v, q);
			doc.debug_draw_line_green_buffer(v,q);
		    }
		    else if(doc.line_influence_reciprocal(q, v) && doc.align_V_line_strict(q, v) && doc.align_size_height_line(q,v))
		    {
		      doc.add_to_line_link(v, q);
		      doc.debug_draw_line_red_buffer(v,q);
		    }
		    else if(
		      
		      doc.line_influence_reciprocal(q, v) &&
		      !doc.same_line(q,v) &&
		      doc.align_V(q,v) &&
		      doc.align_size_x_height(v,q) &&
		      doc.get_line_length(v) > 4 &&
		      doc.align_proximity_line(v,q)
		      )
		    {
		      doc.debug_draw_box_red_buffer(v);
		       doc.debug_draw_box_green_buffer(q);
		   doc.add_to_line_link(v, q);
		    }
		    
		    else if(doc.align_V(q,v) && doc.align_proximity_strict(q,v))
		    {
		      doc.debug_draw_line_orange_buffer(q,v);
		      doc.add_to_line_link(v, q);
		    }
		  }
		  
		  

		}
		
	      }
	    }
	}
	doc.propage_line_link();
    }
     template<typename L, typename F, typename D>
    void clean_between(mymln::document::document<L,F,D>& doc)
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
	    L Start = 0;
	    L End = 0;
	      for_all(q)
	      {
		if(doc.align_V(q,v) && doc.align_size_medium(q, v) && doc.align_proximity(q,v))
		{
		  if(doc[q] == doc.get_beginning_of_line(q))
		  {Start = doc[q]; }
		  else
		  {End = doc[q]; }
		}
		
	      }
	      if(Start && End){doc.add_to_line_link(Start, doc[v]);}
	}
	doc.propage_line_link();
	
    }
 
 
 
    template<typename L, typename F, typename D>
    void clean_paragraph_items(mymln::document::document<L,F,D>& doc)
    {
       typedef vertex_image<point2d,bool> v_ima_g;
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	v_ima_g mask = doc.fun_mask_start_end_lines();
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
	      if(!doc.contain_paragraph(v))
	      { doc.add_to_paragraph(v); }
	      
	      for_all(q)
	      {
		if(
		  doc.align_H_large(q,v) &&
		  doc.align_size_height_line_medium(q, v) &&
		  doc.align_proximity_V_line(v,q) &&
		  doc.align_size_width_line(q, v)
		  )
		{
		  if(doc.contain_paragraph(q))
		  {
		     doc.add_to_paragraph(q);
		     doc.add_to_paragraph_link(q, v);
		  }
		  else
		  {
		    doc.add_to_paragraph(q);
		    doc.add_to_paragraph_link(v, q);
		  }
		  doc.debug_draw_line_red_buffer(v,q);
		}
	      }
	    }
	    else if(doc.contain_line(v) && doc.get_end_of_line(v) == doc[v]){}
	    else if(doc.contain_line(v))
	    {
	      for_all(q)
	      {
		if(
		  doc.get_beginning_of_line(q) == doc[q] &&
		  doc.align_H_large(q,v) &&
		  doc.align_size_height_line_medium(q, v) &&
		  doc.align_size_width_line(q, v) &&
		  doc.align_proximity_V_line(v,q) &&
		  doc.align_bottom_line(q,v)
		  )
		{
		  if(doc.contain_paragraph(q))
		  {
		    doc.jump_to_paragraph(q);
		    if(!doc.contain_paragraph(v))
		    {
		      doc.add_to_paragraph(v);
		      doc.add_to_paragraph_link(q, v);
		    }
		    else
		    {
		      doc.add_to_paragraph_link(v, q);
		    }
		  }
		  else
		  {
		    doc.jump_to_paragraph(v);
		    if(!doc.contain_paragraph(v))
		    {
		      doc.add_to_paragraph(v);
		    }
		    doc.add_to_paragraph(q);
		    doc.add_to_paragraph_link(v, q);
		    
		   
		  }
		  
		   doc.debug_draw_line_green_buffer(v,q);
		}
	      }
	      
	    }
	  }
	}
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
		doc.align_smaller_line(v,q) && 
		doc.get_line_length(v) > 3 &&
		doc.align_proximity_line(v,q) 
		)
	      {
		if(doc.align_base_line_line(v,q) && doc.get_line_length(q) < 3)
		{doc.add_to_line_link(v, q); doc.debug_draw_line_green_buffer(q,v);}
		else if(doc.align_up_line_line(v,q))
		{doc.add_to_line_link(v, q); doc.debug_draw_line_red_buffer(q,v);}
		else if (doc.line_influence_has(v,q) && doc.align_top_large(v,q) && doc.align_small_item_line(v,q) && !doc.align_up_line_line(v,q))
		{ doc.add_to_line_link(v, q); doc.debug_draw_line_orange_buffer(q,v);}
	      }
	    }
	  }
      }
      doc.propage_line_link();
      }
      template<typename L, typename F, typename D>
      void clean_alone_letters_lines(mymln::document::document<L,F,D>& doc)
      {
	//image2d<value::rgb8> out;
	//mln::initialize(out, s);
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
	       //draw::line(out, q,v, mln::literal::red);
	      if(
		((doc.line_influence_has(v,q) && doc.is_line_representative(q)) ||
		doc.line_has(v,q)) &&
		doc.align_V(v, q)
		)
	      {doc.add_to_line_link(v, q);}
	      else if(doc.line_has(v,q))
	      {doc.add_to_line_link(v, q);}
	    }
	  }
	}
	doc.propage_line_link();
	//io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
      }
      
  template<typename L, typename F, typename D>
  void remove_alone_letter(mymln::document::document<L,F,D>& doc)
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
		    for_all(q)
		    {
		      if(doc.in_header(q) || doc.in_footer(q)){continue;}
		      if(doc.contain_alone_letter(q)) {doc.add_noise(q);}
		    }
		}
  }
  
  template<typename L, typename F, typename D>
  void clean_odd_letters(mymln::document::document<L,F,D>& doc)
  {
		for(unsigned int N = 1;N < doc.count(); N++)
		{
			if(doc.contain_line(N))
				if (doc.get_letter_middle_height(N) * 3 < doc.get_bbox(N).len(0))
				{doc.move_line_self_link(N); doc.add_noise(N); doc.invalidate_line_link(N);}
				else if(doc.get_letter_middle_width(N) * 4 < doc.get_bbox(N).len(1))
				{doc.move_line_self_link(N); doc.add_noise(N); doc.invalidate_line_link(N);}
		}
  } 
  
  template<typename L, typename F, typename D>
  void clean_paragraphs_up(mymln::document::document<L,F,D>& doc)
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
		  if(doc.contain_paragraph(v))
		  {
		    for_all(q)
		    {
		      if(
			doc.contain_paragraph(q) &&
			!doc.same_paragraph(v, q) &&
			doc.align_top_paragraph(q, v) &&
			doc.decal_left_paragraph(q, v) &&
			doc.align_size_height_line(q, v) &&
			doc.align_size_width_paragraph(q, v) &&
			doc.get_paragraph_length(v) == 1 &&
			doc.align_H_paragraph(v, q) &&
			doc.align_proximity_V_line(v,q)
			)
		      {
			doc.add_to_paragraph_link(q,v);
		      }
		    }
		  }
		}
		doc.propage_paragraph_link();
  }
 
  template<typename L, typename F, typename D>
  void clean_paragraphs_large(mymln::document::document<L,F,D>& doc)
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
		  if(doc.contain_paragraph(v) && doc.get_paragraph_length(v) > 1)
		  {
		    for_all(q)
		    {
		      if(
			doc.contain_paragraph(q) &&
			doc.get_paragraph_length(q) > 1 &&	
			!doc.same_paragraph(v, q) &&
			doc.align_top_paragraph(q, v))
		      {
			
			if(
			doc.decal_left_paragraph_strong(q, v) &&
			doc.align_size_height_line(q, v) &&
			doc.align_size_width_paragraph(q, v) &&
			doc.align_H_paragraph(v, q) &&
			 doc.align_proximity_V_line(v,q)
			)
		      {
			doc.add_to_paragraph_link(q,v);
			
		      }
		      }
		    }
		  }
		}
		doc.propage_paragraph_link();
		
		

  }
      template<typename L, typename F, typename D>
  void clean_included_paragraphs(mymln::document::document<L,F,D>& doc)
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
		  if(doc.contain_paragraph(v) && doc.get_paragraph_length(v) > 1)
		  {
		    for_all(q)
		    {
		      if(
			doc.contain_paragraph(q) &&
			doc.get_paragraph_length(q) == 1 &&
			!doc.same_paragraph(q, v) &&
			doc.paragraph_included_influence(v, q) &&
			doc.align_size_height_line(v, q))
		      {
			if(doc.line_influence_reciprocal(q, v))
			{
			  doc.add_to_paragraph_link(v,q);
			  doc.debug_draw_line_green_buffer(v,q);
			}
			else if
			  (
			  doc.return_next_line(doc[v]) == doc.return_next_line(doc[q]) &&
			  doc.return_previous_line(doc[v]) == doc.return_previous_line(doc[q]) &&
			  doc.same_paragraph( doc.return_previous_line(doc[v]), doc.return_next_line(doc[v]))
			  )
			{
			  doc.debug_draw_line_red_buffer(v,q);
			}
		      }
		    }
		  }
		}
		doc.propage_paragraph_link();
  }
  
    template<typename L, typename F, typename D>
  void clean_line_space(mymln::document::document<L,F,D>& doc)
  {
		mln::util::array<L> KillMe = mln::util::array<L>(doc.size());
		unsigned int Killer = 0;
		if(doc.size() > 0){KillMe[0] = 0;}
		
    		typedef vertex_image<point2d,bool> v_ima_g;
		typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
		v_ima_g mask = doc.fun_mask_letters();
		mln_piter_(v_ima_g) v(mask.domain());
		typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
		nbh_t nbh(mask);
		mln_niter_(nbh_t) q(nbh, v);
		for_all(v)
		{
		    for_all(q)
		    {
		      if(doc.same_line(q, v))
		      {
			if(doc.in_beginning_of_line(q) || doc.in_end_of_line(q))
			{
			  if(doc.space(q, v) > doc.get_letter_middle_space(q) * 4 && doc.align_half_line_letter(v,q))
			  {
			    if(doc[q] == doc.get_beginning_of_line(q) && doc.align_V_side(v,q))
			    {
			      doc.add_to_line_self_link(v);
			      doc.add_to_line_link(v, q);
			      KillMe[Killer++] = doc[q];
			      KillMe[Killer] = 0;
			      doc.debug_draw_line_green_buffer(q,v);
			      doc.debug_draw_box_red_buffer(q);
			    }
			    else
			    {
			      doc.debug_draw_line_red_buffer(q,v);
			    }
			  }
			}
		      }
		    }
		}
		doc.propage_line_link();
		Killer = 0;
		while(Killer < doc.size() && KillMe[Killer])
		{
		  doc.add_noise(KillMe[Killer]);
		  doc.kill(KillMe[Killer]);
		  doc.invalidate_line_link(KillMe[Killer]);
		  Killer++;
		}
		doc.propage_line_link();
  }


      template<typename L, typename F, typename D>
      void clean_included_letters(mymln::document::document<L,F,D>& doc)
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
			for_all(q)
			{
			  if(doc.letter_included(q, v) &&  doc.align_size_large_inside(q, v))
			  {
			    
			    
			    doc.merge(q,v);
			  }
			}
		    }
		    doc.propage_line_link();
		
      }
      
      
      
      template<typename L, typename F, typename D>
      void clean_backward_letters(mymln::document::document<L,F,D>& doc)
      {
		    typedef vertex_image<point2d,bool> v_ima_g;
		    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
		    v_ima_g mask = doc.fun_mask_start_lines();
		    mln_piter_(v_ima_g) v(mask.domain());
		    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
		    nbh_t nbh(mask);
		    mln_niter_(nbh_t) q(nbh, v);
		    for_all(v)
		    {
		      if(!doc.contain_line(v))
		      {
			for_all(q)
			{
			  if(
			    doc.contain_line(q) && 
			    doc.get_line_length(q)> 5 &&
			    doc.align_V(v, q) &&
			    doc.align_proximity(v, q) &&
			    doc.align_size_height(v, q) &&
			    !doc.contain_separator(v)
			    )	  
			  {
			      doc.add_to_line_link(q,v);
			      doc.debug_draw_line_green_buffer(v, q);
			  }
			}
		      }
		    }
		    doc.propage_line_link();
      }
      
      template<typename L, typename F, typename D>
      void clean_paragraphs_tab(mymln::document::document<L,F,D>& doc)
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
		      if(doc.contain_paragraph(v))
		      {
			for_all(q)
			{
			  if(doc.paragraph_start_with_tab(q) && doc.same_paragraph(q,v))
			  {doc.debug_draw_line_red_buffer(v, q);}
			  if(
			    doc.contain_paragraph(q) && 
			    !doc.same_paragraph(q,v) &&
			    !doc.paragraph_start_with_tab(q) &&
			    doc.align_top_paragraph(q, v) &&
			    doc.get_paragraph_length(q) > 1 &&
			    doc.get_first_line_ID(doc[q]) == doc.get_line_ID(doc[q]) &&
			    doc.align_size_width_paragraph(q,v) &&
			    doc.align_proximity_paragraph_up(q,v) &&
			    doc.align_H_paragraph(q,v)
			    )
			  {
			      doc.add_to_paragraph_link(q,v);
			      doc.debug_draw_line_green_buffer(v, q);
			  }
			}
		      }
		    }
		    doc.propage_paragraph_link();
      }
      template<typename L, typename F, typename D>
      void find_previous_next_line(mymln::document::document<L,F,D>& doc)
      {
		    doc.reserve_previous_next_line();
		    typedef vertex_image<point2d,bool> v_ima_g;
		    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
		    v_ima_g mask = doc.fun_mask_all_letters();
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
			    doc.contain_line(q) &&
			    !doc.same_line(q, v) &&
			    doc.align_H_large(q, v) &&
			    doc.align_size_height_line(q,v))
			  {
			    if(doc.align_top_large(q,v))
			    {
			      doc.debug_draw_line_green_buffer(q,v);
			      doc.add_line_previous(q,v);
			    }
			    else if(doc.align_bottom_large(q,v))
			    {
			       doc.debug_draw_line_red_buffer(q,v);
			       doc.add_line_next(q,v);
			    }
			  }
			}
		      }
		    }

      }
      
      template<typename L, typename F, typename D>
      void clean_letter_previous_next_line(mymln::document::document<L,F,D>& doc)
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
		      if(doc.contain_line(v) && doc.return_next_line(doc.get_line_ID(v)))
		      {
			for_all(q)
			{
			  if(
			    
			    doc.contain_line(q) &&
			    !doc.same_line(q, v) &&
			    doc.return_next_line(doc.get_line_ID(v)) == doc.return_next_line(doc.get_line_ID(q)) &&
			    doc.return_previous_line(doc.get_line_ID(v)) == doc.return_previous_line(doc.get_line_ID(q)) &&
			    doc.align_V_line(v, q) &&
			    doc.align_size_height_line_medium(v, q) &&
			    !doc.killed(doc[q])
			    )
			  {
			      doc.debug_draw_line_green_buffer(v,q);
			      
			      doc.add_to_line_link(v,q);
			  }
			}
		      }
		    }
		    doc.propage_line_link();
	
      }
  
        template<typename L, typename F, typename D>
      void clean_V_lines(mymln::document::document<L,F,D>& doc)
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
			for_all(q)
			{
			  if(
			    doc.align_H(v,q) &&
			    doc.align_size_width_strict(v,q)
			    )
			  {
			    doc.add_temp_letter(v);
			    doc.debug_draw_line_green_buffer(v,q);
			  }                    
			}
		    }
		    doc.propage_line_link();
	
      }
      
      
      template<typename L, typename F, typename D>
      void clean_paragraphs_end_line(mymln::document::document<L,F,D>& doc)
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
		      if(doc.contain_paragraph(v))
		      {
			for_all(q)
			{
			  if(
			    doc.contain_paragraph(q) &&
			    !doc.same_paragraph(q,v) &&
			    doc.get_paragraph_length(q) == 1)
			  {
			      if(
			      doc.align_top_paragraph(q,v) &&
			      doc.align_H_min_paragraph(q,v) &&
			      doc.align_size_height_paragraph_line(q,v) &&
			      doc.align_smaller_paragraph(v, q) &&
			      doc.compatible_paragraph_middle_width(v,q) &&
			      doc.align_proximity_paragraph_up_large(q,v) &&
			      doc.get_line_length(q) > 3
			      )
			      {
				  doc.debug_draw_line_green_buffer(q,v);
				  doc.add_to_paragraph_link(v, q);
			      }
			      else if(
				doc.align_top_paragraph(v,q) &&
				!doc.paragraph_start_with_tab(v) &&
				doc.align_size_height_paragraph_line(q,v) &&
				doc.align_H_max_paragraph(q,v) &&
				doc.align_smaller_paragraph(v, q) &&
				 doc.compatible_paragraph_middle_width(v,q) &&
				 doc.align_proximity_paragraph_up_large(q,v) &&
				 doc.get_line_length(q) > 3
				)
			      {
				  doc.debug_draw_line_green_buffer(q,v);
				  doc.add_to_paragraph_link(v, q);
			      }
			  }
			}
		      }
		    }
		    doc.propage_paragraph_link();
      }
      
      template<typename L, typename F, typename D>
      void clean_finalize_line(mymln::document::document<L,F,D>& doc)
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
			  if(doc.align_V_line(q,v) && !doc.same_line(q,v))
			  {
			    if(doc.same_paragraph(q,v))
			    {
			      if(doc.compatible_paragraph_middle_width(q,v))
			      {
				doc.debug_draw_line_orange_buffer(q,v);
				doc.add_to_line_link(v, q);
			      }
			    }
			    else
			    {
			      if(doc.paragraph_included_influence(q,v) || doc.paragraph_included_influence(v,q))
			      {
				if(doc.compatible_paragraph_middle_width(q, v))
				{
				  if(
				      doc.return_next_line(doc[q]) == doc.return_next_line(doc[v]) ||
				      doc.return_previous_line(doc[q]) == doc.return_previous_line(doc[v])
				    )
				  {
				   //if(doc.get_line_length(q) < 4 || doc.get_line_length(v) < 4  )
				     
				     if(
				       (doc.get_paragraph_length(v) < 3 && doc.align_V_tube_line(q,v))  ||
				       (doc.get_line_length(q) < 3 && doc.align_V_tube_line(v,q))
				       )
				     {
				      if(doc.align_size_height_line_strict(q,v))
				      {
					doc.debug_draw_line_red_buffer(q,v);
					doc.add_to_line_link(v, q);
					doc.add_to_paragraph_link(v, q);
				      }
				     }
				  }
				}
			      }
			     
			    }
			  }
			}
		      }
		    }
		     doc.propage_line_link();
		     doc.propage_paragraph_link();
      }
  
  
  
  
  
  
        template<typename L, typename F, typename D>
      void clean_rebuild_letters(mymln::document::document<L,F,D>& doc)
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
			  if(doc.contain_line(q) && doc.same_line(q,v))
			  {
			     if(
			       doc.letter_included_center(v,q) && doc.align_small_item(v,q))
			     {
			       doc.debug_draw_line_orange_buffer(v,q);
			       doc.merge(q,v);
			     }
			     
			  }
			}
		      }
		    }
		     doc.propage_line_link();
      }
  
  
        template<typename L, typename F, typename D>
      void clean_center_paragraphs(mymln::document::document<L,F,D>& doc)
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
		      for_all(q)
		      {
			if(!doc.same_paragraph(q,v) && doc.align_size_height_line(q,v))
			{
			 if(
			   doc.align_paragraph_center(q,v) &&
			   doc.align_proximity_paragraph_up_large(q,v) &&
			   doc.align_top_paragraph(q,v) &&
			   doc.align_size_height_line_strict(q,v) &&
			   doc.compatible_paragraph_middle_width(q,v) &&
			   !doc.paragraph_start_with_tab(q)
			   )  
			 {
			   if(doc.get_paragraph_length(q) < 2)
			   {
			     if(!doc.decal_left_paragraph(q,v))
			     {
			      doc.debug_draw_line_green_buffer(q,v);
			      doc.add_to_paragraph_link(v, q);
			     }
			     else if(doc.align_paragraph_center_strict(q,v))
			     {
				doc.debug_draw_line_orange_buffer(q,v);
				doc.add_to_paragraph_link(v, q);
			     }
			   }
			   else
			   {
			      doc.debug_draw_line_green_buffer(q,v);
			      doc.add_to_paragraph_link(v, q);
			   }
			 }
			}
		      }
		    }
		    doc.propage_line_link();
		     doc.propage_paragraph_link();
		    
      }
  
 
         template<typename L, typename F, typename D>
      void clean_paragraphs_first_line(mymln::document::document<L,F,D>& doc)
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
		      if(doc.contain_paragraph(v) && doc.get_paragraph_length(v) > 1 && doc.contain_start_paragraph(v))
		      {
			for_all(q)
			{
			  if(
			    doc.same_paragraph(q,v) &&
			    !doc.same_line(q,v) &&
			    doc.align_left(v,q) &&
			    doc.align_V_line(v,q)
			    )
			  {
			    doc.debug_draw_line_green_buffer(q,v);
			      doc.debug_draw_box_green_buffer(q);
			      doc.add_to_line_link(v,q);
			  }
			}
		      }
		    }
		    doc.propage_line_link();
      }
 
 
        template<typename L, typename F, typename D>
      void clean_paragraphs_couple(mymln::document::document<L,F,D>& doc)
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
		      if(doc.contain_paragraph(v) && doc.get_paragraph_length(v) == 1)
		      {
			for_all(q)
			{
			  if(
			    doc.contain_paragraph(q) &&
			    doc.get_paragraph_length(q) == 1 &&
			    !doc.same_paragraph(q,v) &&
			    doc.align_proximity_paragraph_up_medium(q,v) &&
			    doc.compatible_paragraph_middle_width(q,v) &&
			    !doc.decal_left_paragraph(q, v) &&
			    doc.align_top_paragraph(q,v) &&
			    doc.align_size_height_paragraph_line(q,v) &&
			    (doc.contain_start_line(q) || doc.contain_start_line(v)) && // THESE TWO LINES CONATIN MAYBE SOME ERROR
			    doc.align_H_paragraph(q,v) // FIX THE PROBLEM BEFORE TO START IT
			    )
			  {
			    doc.debug_draw_line_green_buffer(q,v);
			    doc.add_to_paragraph_link(q,v);
			  }
			}
		      }
		    }
		    doc.propage_paragraph_link();
      }
  
  
      template<typename L, typename F, typename D>
      void clean_finalize_paragraph(mymln::document::document<L,F,D>& doc)
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
		      for_all(q)
		      {

			if(!doc.same_paragraph(q,v) && doc.paragraph_influence_reciprocal(q,v))
			{
			  if(
			      doc.paragraph_included_influence_line(q,v)||
			      doc.paragraph_included_influence_line(v, q))
			  {
			    if(
			      doc.align_V_line(v,q) &&
			      doc.align_size_height_line(v, q) &&
			      !doc.paragraph_included_influence(v, q) &&
			      !doc.paragraph_included_influence(q, v)
			      
			      )
			      
			    {
			      
			      if(	(doc.contain_implicit_separator_right(q) && doc.contain_implicit_separator_left(v)) ||
					(doc.contain_implicit_separator_right(v) && doc.contain_implicit_separator_left(q))	)
			      {
				doc.debug_draw_line_orange_buffer(q,v);
				continue;
			      }
			  
			      doc.debug_draw_line_green_buffer(q,v);
			      doc.add_to_paragraph_link(q,v);
			      doc.add_to_line_link(q,v);
			    }
			  }
			  else if( doc.get_paragraph_length(q) == 1 && doc.align_size_width_line(q,v))
			  {
			    if(doc.align_V_line(q,v))
			    {
			    doc.debug_draw_line_red_buffer(q,v);
			    }
			  }
			}
		      }
		    }
		    doc.propage_line_link();
		     doc.propage_paragraph_link();
		    
      }
	template<typename L, typename F, typename D>
      void clean_ellipsis(mymln::document::document<L,F,D>& doc)
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
		      for_all(q)
		      {
			if(
			  doc.get_line_length(q) == 3 &&
			  doc.align_base_line_line_strict(v, q) &&
			  doc.align_smaller_line_strict(v,q) &&
			  doc.align_proximity_large_left(v,q)
			  )
			{
			  doc.debug_draw_line_green_buffer(q,v);
			  doc.add_to_line_link(v,q);
			}
		      }
		    }
		    doc.propage_line_link();
      }
  

      template<typename L, typename F, typename D>
      void clean_finalize_letters(mymln::document::document<L,F,D>& doc)
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
		      
		      if(doc.contain_noise(v))
		      {
			unsigned int Count = 0;
			bool HasLink = false;
			L line = 0;
			for_all(q)
			{
			    if(doc.contain_paragraph(q) && doc.contain_line(q) && doc.paragraph_included_letter(q,v))
			    {
			      if(doc.align_V_line_artefact(q,v) && doc.align_size_height_line_artefact(q,v))
			      {
				HasLink = true;
				if(!line)
				  line = doc[q];
				else if(doc.get_line_distance(q,v) < doc.get_line_distance(doc[line], v))
				  line = doc[q];
			      }
			      else if(doc.align_top_large(q,v) && !doc.align_V(q,v))
			      {
				Count++;
			      }
			    }

			}
			if(Count > 0 && HasLink)
			{
			  doc.debug_draw_box_green_buffer(v);
			  doc.debug_draw_line_green_buffer(v, doc[line]);
			    doc.debug_draw_string(v, Count);
			    doc.add_to_line_link(doc[line],v);
			    doc.add_to_paragraph_link(doc[line], v);
			}
		      }
		    }
		    
		    doc.propage_line_link();
		     doc.propage_paragraph_link();
		    
      }
      
      template<typename L, typename F, typename D>
      void clean_lines_artefacts(mymln::document::document<L,F,D>& doc)
      {
	//image2d<value::rgb8> out;
	//mln::initialize(out, s);
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
	       //draw::line(out, q,v, mln::literal::red);
	      if(doc.same_line(v,q) && doc.is_line_artefact(q))
	      {
		doc.debug_draw_string(q, "ARTEFACT");
		doc.add_noise(q);
	      }
	    }
	  }
	}
	doc.clean_noise_lines();
	doc.propage_line_link();
	//io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
      }
      
      
      
      template<typename L, typename F, typename D>
      void clean_finalize_separators(mymln::document::document<L,F,D>& doc)
      {
	for(int Id = 1; Id < doc.size(); Id++) 
	{
	  if(
	    doc.contain_letter(Id) &&
	    doc.contain_implicit_separator_fixed(Id) &&
	    !(doc.contain_start_line(Id) || doc.contain_end_line(Id))
	    )
	  {
	    doc.invalidate_all_implicit_separator(Id);
	  }  
	}
	
	typedef vertex_image<point2d,bool> v_ima_g;
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	v_ima_g mask = doc.fun_mask_all_letters();
	mln_piter_(v_ima_g) v(mask.domain());
	typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	nbh_t nbh(mask);
	mln_niter_(nbh_t) q(nbh, v);
	for_all(v)
	{
	  if(doc.contain_implicit_separator_fixed(v))
	  {
	    bool FindAFriend = false;
	    if(doc.contain_implicit_separator_right(v))
	    {
	      for_all(q)
	      {
		if(doc.contain_implicit_separator_right(q))
		{FindAFriend = true; break;}
	      }
	    }
	    else
	    {
	      for_all(q)
	      {
		if(doc.contain_implicit_separator_left(q))
		{FindAFriend = true; break;}
	      }
	    }
	    if(!FindAFriend)
	    {
	      doc.debug_draw_box_green_buffer(v);
	      doc.invalidate_all_implicit_separator(v);
	    }
	  }
	}
	
      }
      
      
      template<typename L, typename F, typename D>
      void clean_finalize_little_paragraphs(mymln::document::document<L,F,D>& doc)
      {
	typedef vertex_image<point2d,bool> v_ima_g;
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	v_ima_g mask = doc.fun_mask_all_letters();
	mln_piter_(v_ima_g) v(mask.domain());
	typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	nbh_t nbh(mask);
	mln_niter_(nbh_t) q(nbh, v);
	bool Change = false;
	for_all(v)
	{
	  if(doc.contain_paragraph(v) && doc.get_paragraph_length(v) == 1 && doc.get_line_length(v) < 7 )
	  {
	     for_all(q)
	      {
		if(
		  !doc.same_paragraph(q,v) &&
		  doc.align_V_line(q,v) &&
		  doc.align_proximity_line_extra_large(v,q) &&
		  doc.align_size_height_line(q,v)
		  )
		{
		  
		  if(doc.paragraph_influence_reciprocal(v,q))
		  {if(doc.contain_implicit_separator_fixed(v) && doc.contain_implicit_separator_fixed(q)) continue;}
		  else
		  {if(doc.contain_implicit_separator_fixed(v) || doc.contain_implicit_separator_fixed(q)) continue;}
		  if(doc.get_line_length(v) + doc.get_line_length(q) > 40)
		  {
		    continue;
		    doc.debug_draw_box_red_buffer(q);
		    doc.debug_draw_box_red_buffer(v);
		  }
		  else if(doc.get_line_length(v) > 20 || doc.get_line_length(q) > 20)
		  {if(doc.contain_implicit_separator_fixed(v) || doc.contain_implicit_separator_fixed(q)) continue;}
		  
		    doc.debug_draw_line_green_buffer(q,v);
		    doc.add_to_line_link(v,q);
		    doc.add_to_paragraph_link(v,q);
		    Change = true;
		}
	      }
	  }
	  
	}
	if(Change)
	{
	  doc.propage_line_link();
	  doc.propage_paragraph_link();
	}
      }
      template<typename L, typename F, typename D>
      void clean_finalize_punctuation(mymln::document::document<L,F,D>& doc)
      {
	typedef vertex_image<point2d,bool> v_ima_g;
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	v_ima_g mask = doc.fun_mask_all_letters();
	mln_piter_(v_ima_g) v(mask.domain());
	typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	nbh_t nbh(mask);
	mln_niter_(nbh_t) q(nbh, v);
	bool Change = false;
	for_all(v)
	{
	  if(
	    doc.contain_paragraph(v) &&
	    doc.get_paragraph_length(v) == 1 &&
	    doc.contain_line(v) &&
	    doc.get_line_length(v) > 1 &&
	    doc.get_line_length(v) < 4)
	  {
	    for_all(q)
	    {
	      if(
		!doc.same_paragraph(q,v) &&
		doc.paragraph_included_influence(q,v) &&
		doc.line_included(q,v)
		)
	      {
		doc.debug_draw_line_green_buffer(q,v);
		doc.add_to_line_link(v,q);
		doc.add_to_paragraph_link(v,q);
		Change = true;
	      }
	    }
	  }
	}
	if(Change)
	{
	  doc.propage_line_link();
	  doc.propage_paragraph_link();
	}
      }
  
  
      template<typename L, typename F, typename D>
      void clean_finalize_alone_paragraphs(mymln::document::document<L,F,D>& doc)
      {
	typedef vertex_image<point2d,bool> v_ima_g;
	typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	v_ima_g mask = doc.fun_mask_all_letters();
	mln_piter_(v_ima_g) v(mask.domain());
	typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	nbh_t nbh(mask);
	mln_niter_(nbh_t) q(nbh, v);
	bool Change = false;
	for_all(v)
	{
	  if(
	    doc.contain_paragraph(v) &&
	    doc.get_paragraph_length(v) == 1 &&
	    doc.get_line_length(v) > 5)
	  {
	    for_all(q)
	    {
	      if(
		!doc.same_paragraph(q,v) &&
		doc.contain_paragraph(q) &&
		doc.get_paragraph_length(q) > 1 &&
		(doc.contain_start_paragraph(q) || doc.contain_end_paragraph(q)) &&
		doc.align_proximity_paragraph_up_large(q,v) &&
		doc.compatible_paragraph_middle_width(q,v) &&
		doc.align_paragraph_H_tube(q,v) &&
		doc.compatible_paragraph_height(q,v)
		)
	      {
		if(!doc.paragraph_start_with_tab(q) && doc.align_top_paragraph(q,v))
		{
		  doc.add_to_paragraph_link(v,q);
		  doc.debug_draw_line_green_buffer(q,v);
		  Change = true;
		}
		else if(!doc.paragraph_end_with_return(q) && doc.align_top_paragraph(v,q))
		{
		  if(doc.align_half_paragraph(q,v))
		  {
		   doc.add_to_paragraph_link(v,q);
		   doc.debug_draw_line_green_buffer(q,v);
		   Change = true;
		  }
		}
		
		
	      }
	    }
	  }
	}
	if(Change)
	{
	  doc.propage_paragraph_link();
	}
      }
  
  
  
  }
  }
  

#endif