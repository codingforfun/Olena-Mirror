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
		 if(doc.allign_V(q,v) && doc.allign_size(q, v) && doc.allign_proximity_large(q, v) )
		 {
		   doc.add_to_line_link(v, q);
		   All_Alone = false;
		 }
	       }
	       else
	       {
		 if(doc.allign_V(q,v) && doc.allign_size(q, v) && doc.allign_proximity_large(q, v))
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
		!doc.allign_H(q, v) &&
		doc.allign_base_line_line_strict(v, q) &&
		doc.allign_proximity(v,q) &&
		doc.allign_smaller_line(v, q) &&
		doc.get_line_length(v) > 2)
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
	  if(doc.contain_line(v2) && !doc.contain_alone_letter(v2))
	  {
	    for_all(q2)
	    {
	      if (
		doc.allign_H_large(v2, q2) &&
		doc.allign_top(v2, q2) &&
		doc.line_has(v2, q2) &&
		doc.letter_ratio_XY(q2) < 2 &&
		doc.allign_size_width_large(v2, q2))
	      {
		doc.add_to_line_link(v2, q2);
		doc.add_letter_coerce(q2);
		if(doc.allign_H(v2,q2))
		  {doc.merge(v2,q2); doc.tag_label(v2, "i");}
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
		if(doc.allign_size(v,q))
		{
		  if(!doc.contain_alone_letter(v) && !doc.contain_alone_letter(q)  && doc.allign_proximity_large_left(v,q) )
		  {
		    if(
			(doc.allign_top(v, q) || doc.allign_top(doc.get_beginning_of_line(v), doc.get_label(q)) ) &&
			doc.allign_smaller_line_letter(v,q)
		      )
		    {
		      if(doc.get_line_length(q) < 3 || doc.allign_V_line(v, q))
		      {
			doc.add_to_line_link(v, q);
			doc.tag_label(v, "'");
		      }
		    }  
		  }
		  else if(doc.allign_top(v, q) && !doc.allign_H(v, q) && doc.allign_proximity_large_left(v,q) && doc.allign_smaller_line_letter(v,q))
		  {
		    if(doc.get_line_length(q) < 3 || doc.allign_V_line(v, q))
		    {
		      doc.add_to_line_link(v, q);
		      doc.tag_label(v, "'");
		    }
		  }
		}
		else if (doc.allign_H_large(q, v) && doc.allign_top(v, q) && doc.allign_size_width_large(v, q))
		{
		  doc.add_to_line_link(v, q);
		  if(doc.allign_H(v,q))
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
		  if(doc.contain_line(q) && doc.get_line_length(q) == 1 && doc.line_influence_has(v,q))
		  {
		      doc.add_to_line_link(q,v); 
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
		if(doc.same_line(q,v)){continue;}
		if(doc.contain_alone_letter(q))
		{
		  if(doc.allign_V(q,v) && doc.allign_proximity_strict(q, v) && doc.allign_size_height(q, v))
		  {
		    doc.add_to_line_link(v, q);
		
		  }
		  		    
		}
		else if(doc.contain_line(q))
		{
		  if(doc.allign_V(q,v) && doc.allign_size_height_line_strict(q, v) && doc.allign_proximity_strict(q,v))
		  {
		    doc.add_to_line_link(v, q);
		    
		  }
		  else if(doc.allign_size_height_line(q,v))
		  {
		    if(doc.allign_proximity_line(q,v) && doc.allign_V_line_strict(q, v))
		    {
			doc.add_to_line_link(v, q);
			doc.debug_draw_line_green_buffer(v,q);
		    }
		    else if(doc.line_influence_reciprocal(q, v) && doc.allign_V_line_strict(q, v))
		    {
		      doc.add_to_line_link(v, q);
		      doc.debug_draw_line_red_buffer(v,q);
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
		if(doc.allign_V(q,v) && doc.allign_size(q, v) && doc.allign_proximity(q,v))
		{
		  if(doc[q] == doc.get_beginning_of_line(q))
		  {Start = doc[q]; }
		  else
		  {End = doc[q]; }
		}
		
	      }
	      if(Start && End){doc.add_to_line_link(Start, doc[v]);}
	}
	doc.propage_paragraph_link();
	
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
		  doc.allign_H_large(q,v) &&
		  doc.allign_size_height_line(q, v) &&
		  doc.allign_proximity_V_line(v,q) &&
		  doc.allign_size_width_line(q, v)
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
		  doc.allign_H_large(q,v) &&
		  doc.allign_size_height_line(q, v) &&
		  doc.allign_size_width_line(q, v) &&
		  doc.allign_proximity_V_line(v,q) &&
		  doc.allign_bottom_line(q,v)
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
		doc.allign_smaller_line(v,q) && 
		doc.get_line_length(v) > 3 &&
		doc.allign_proximity_line(v,q) 
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
		doc.allign_V(v, q)
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
					doc.add_noise(N);
				else if(doc.get_letter_middle_width(N) * 4 < doc.get_bbox(N).len(1))
					doc.add_noise(N);
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
			doc.allign_top_paragraph(q, v) &&
			doc.decal_left_paragraph(q, v) &&
			doc.allign_size_height_line(q, v) &&
			doc.allign_size_width_paragraph(q, v) &&
			doc.get_paragraph_length(v) == 1 &&
			doc.allign_H_paragraph(v, q) &&
			doc.allign_proximity_V_line(v,q)
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
			doc.allign_top_paragraph(q, v))
		      {
			
			if(
			doc.decal_left_paragraph_strong(q, v) &&
			doc.allign_size_height_line(q, v) &&
			doc.allign_size_width_paragraph(q, v) &&
			doc.allign_H_paragraph(v, q) &&
			 doc.allign_proximity_V_line(v,q)
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
			doc.allign_size_height_line(v, q))
		      {
			if(doc.line_influence_reciprocal(q, v))
			doc.add_to_paragraph_link(v,q);
		      }
		    }
		  }
		}
		doc.propage_paragraph_link();
  }
  
    template<typename L, typename F, typename D>
  void clean_lines_space(mymln::document::document<L,F,D>& doc, std::string dgb_out,image2d<bool> s)
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
		    for_all(q)
		    {
		      if(doc.same_line(q, v))
		      {
			draw::line(out, q,v, mln::literal::red);
			if(doc.in_beginning_of_line(q) || doc.in_end_of_line(q))
			{
			  draw::line(out, q,v, mln::literal::green);
			  if(doc.space(q, v) > doc.get_letter_middle_space(q) * 10)
			  {
			    draw::line(out, q,v, mln::literal::blue);
			    /*if(doc[q] == doc.get_beginning_of_line(q))
			      doc.add_to_line_link(v, q); */
			    //doc.add_noise(q);
			  }
			}
		      }
		    }
		}
		doc.propage_line_link();
		io::ppm::save(mln::debug::superpose(out, s, literal::white),dgb_out);
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
			  if(doc.letter_included(q, v) &&  doc.allign_size_large_inside(q, v))
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
			    doc.allign_V(v, q) &&
			    doc.allign_proximity(v, q) &&
			    doc.allign_size_height(v, q)
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
			  if(
			    doc.contain_paragraph(q) && 
			    !doc.same_paragraph(q,v) &&
			    !doc.paragraph_start_with_tab(q) &&
			    doc.allign_top_paragraph(q, v) &&
			    doc.get_paragraph_length(q) > 1 &&
			    doc.get_first_line_ID(doc[q]) == doc.get_line_ID(doc[q]) &&
			    doc.allign_size_width_paragraph(q,v) &&
			    doc.allign_proximity_paragraph_up(q,v) &&
			    doc.allign_H_paragraph(q,v)
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

  
  }
  }
  

#endif