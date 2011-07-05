#ifndef INC_DOCUMENT_DOC
#define INC_DOCUMENT_DOC
#include <mln/accu/shape/bbox.hh>
#include<my/util/vector_bbox_group.hh>
#include<my/util/union.hh>
#include<my/debug/pict.hh>
#include <mln/util/graph.hh>

using namespace mln;
namespace mymln
{
  namespace document
  {

    template<typename Label, typename Float, typename Data>
    class document
    {
      typedef 
      mln::p_graph_piter<
	mln::p_vertices<
	    mln::util::graph,
	    fun::i2v::array<point2d>
	>,
	mln::internal::vertex_fwd_iterator<mln::util::graph>
      >
      graph_itt;
    
      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;

      public:
	// Create a document
	// A document enable the user to work on several elements like
	// separatororletter
	// to work correctly a document need a labelized image that represent
	// the structure of the document.( - usualy created with labeling::blobs - )
	
	// NOTE : The type Float is used toperform floating point operation on data
	// with type Label. So a conversion betweem Data and Float must exist.
	
	
	// WARNING: Methods on lines like get_end_of_line can be used only after a line cooking.
	document(){}
	document(image2d<Label>& ima, image2d<Label>& ima_influ,mln::util::array<box2d>& bboxgp, g_vertices_p& area_graph, Label Areas)
	{
	  img = ima;
	  _bboxgp = bboxgp;
	  _bboxgp_influ = mln::util::array<box2d>(Areas + 1);
	  _area_graph = area_graph;
	  separators_mask = fun::i2v::array<bool>(Areas + 1);
	  containers_mask = fun::i2v::array<bool>(Areas + 1);
	  letters_mask = fun::i2v::array<bool>(Areas + 1);
	  all_letters_mask = fun::i2v::array<bool>(Areas + 1);
	  Hseparator_mask =  fun::i2v::array<bool>(Areas + 1);
	  Vseparator_mask =  fun::i2v::array<bool>(Areas + 1);
	  image_mask =  fun::i2v::array<bool>(Areas + 1);
	  noise_mask = fun::i2v::array<bool>(Areas + 1);
	  temp_letter = fun::i2v::array<bool>(Areas + 1);
	  alone_letters_mask = fun::i2v::array<bool>(Areas + 1);
	  implicit_separators_left_mask = fun::i2v::array<bool>(Areas + 1);
	  implicit_separators_right_mask = fun::i2v::array<bool>(Areas + 1);
	  anomaly_mask = fun::i2v::array<bool>(Areas + 1);
	  kill_mask = fun::i2v::array<bool>(Areas + 1);
	  all_mask = fun::i2v::array<bool>(Areas + 1);
	  CImpSep = 1;
	  NImpSep = 2;
	  lines_union = mymln::util::union_find<Label>(Areas + 1);
	  implicit_separators_union = mymln::util::union_find<Label>(Areas + 1);
	  debug_buffer_enable = false;
	  paragraphs_union = mymln::util::union_find<Label>(Areas + 1);
	  tag_lbl = mln::util::array<std::string>(Areas + 1);
	  Btag_lbl = mln::util::array<bool>(Areas + 1);
	  lines_split = mln::util::array<Label>(Areas + 1);
	  lines_split.fill(0);
	  tag_lbl.fill("");
	  img_influ = ima_influ;
	  CSep = 0;
	  CSepH = 0;
	  CSepV = 0;
	  CLet = 0;
	  CLine = 1;
	  NLine = 2;
	  CPar = 1;
	  NPar = 2;
	  Areas_Number_ = Areas + 1;
	  sep_right_cooked = false;
	  lines_cooked = false;
	  Enable_Debug_Buffer = false; // Remanant version of debug_buffer_enable
	}
	inline bool killed(const Label lbl)
	{return kill_mask(lbl);}
	inline void kill(const Label lbl)
	{kill_mask(lbl) = true;all_mask(lbl) = false;}
	inline unsigned int count()
	{return Areas_Number_;}
	/* OPERATION ON PAGE */
	inline bool in_header(const point2d& p)
	{ return p[0] < (img_influ.domain().len(0) / 8);}
	inline bool in_header(Label lbl)
	{ return in_header(_bboxgp[lbl]); }
	
	inline bool in_footer(const point2d& p)
	{ return p[0] > ((img_influ.domain().len(0) / 8) * 7);}
	inline bool in_footer(Label lbl)
	{ return in_footer(_bboxgp[lbl]); }
	
	/* OPERATION ON PARAGRAPH */
	inline bool link_paragraphs()
	{
	  paragraphs_union[0] = 0;
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {    
	    paragraphs_union.invalidate_link(N);
	    if (!contain_paragraph(N) && contain_line(N))
	    {
	      jump_to_paragraph(N);
	       if(start_lines_mask(N))
	       {
		add_to_paragraph(N);
		paragraphs_union.add_self_link(N);
	       }
	    }
	  }
	   for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	 
	    if (!contain_paragraph(N) && contain_line(N))
	    {
	      jump_to_paragraph(get_beginning_of_line(N));
	      add_to_paragraph(N);
	      if(contain_line(N) && get_beginning_of_line(N) != 0)
	      paragraphs_union.add_link(get_beginning_of_line(N), N);
	    }
	  }
	}
	
	
	
	
	
	inline bool contain_paragraph(const point2d& point)
	{return contain_paragraph(img_influ(point));}
	inline bool contain_paragraph(const Label lbl)
	{return paragraphs_union[lbl] != 0;}
	inline void add_to_paragraph(const point2d& point)
	{add_to_paragraph(img_influ(point));}
	inline void add_to_paragraph(const Label lbl)
	{paragraphs_union[lbl] = CPar;}
	
	inline void add_new_paragraph(const point2d& point)
	{add_new_paragraph(img_influ(point));}
	inline void add_new_paragraph(const Label lbl)
	{CPar = NPar; NPar++;}
	
	inline void add_to_paragraph_link(const point2d& A, const point2d& B)
	{add_to_paragraph_link(img_influ(A),img_influ(B));}
	inline void add_to_paragraph_link(const Label A, const Label B)
	{paragraphs_union.add_link(A, B);}
	
	inline void add_to_paragraph_self_link(const point2d& A)
	{add_to_paragraph_self_link(img_influ(A));}
	inline void add_to_paragraph_self_link(const Label A)
	{paragraphs_union.add_self_link(A);}
	
	inline void propage_paragraph_link()
	{paragraphs_union.propage_links();}
	
	inline void jump_to_paragraph(const point2d& point)
	{
	  jump_to_paragraph(img_influ(point));
	}
	inline void jump_to_paragraph(const Label lbl)
	{ 
	  if(paragraphs_union[lbl] != 0)
	    CPar = paragraphs_union[lbl];
	  else
	    add_new_paragraph(lbl);
	}
	/* OPERATION ON LINES */
	inline void split_line_exclusive(const point2d& point)
	{split_line_exclusive(img_influ(point));}
	inline void split_line_exclusive(const Label lbl)
	{
	  if(lbl == 0){return;}
	  if(!lines_union.is_self_link(lbl))
	    lines_union.add_self_link(lbl);
	  Label pos = get_end_of_line(lbl);
	  if(pos == lbl){return;}
	 
	  while(lines_split[pos] && lines_split[pos] != lbl && pos != lbl)
	  {
	    if(_bboxgp[lines_split[pos]].pmin()[1] < _bboxgp[lbl].pmin()[1])
	    { 
	      lines_split[lbl] = lines_split[pos];
	      lines_split[pos] = lbl;
	      return;
	    }
	    pos = lines_split[pos];
	  }
	  if(pos == lbl || lines_split[pos] == lbl){return;}
	  lines_split[pos] = lbl;
	  
	}
	
	
	inline void split_line(const point2d& point)
	{split_line(img_influ(point));}
	inline void split_line(const Label lbl)
	{
	  if(lbl == 0){return;}
	  if(!lines_union.is_self_link(lbl))
	    lines_union.add_self_link(lbl);
	  Label pos = get_beginning_of_line(lbl);
	  if(pos == lbl){return;}
	 
	  while(lines_split[pos] && lines_split[pos] != lbl && pos != lbl)
	  {
	    if(_bboxgp[lines_split[pos]].pmin()[1] > _bboxgp[lbl].pmin()[1])
	    { 
	      lines_split[lbl] = lines_split[pos];
	      lines_split[pos] = lbl;
	      return;
	    }
	    pos = lines_split[pos];
	  }
	  if(pos == lbl || lines_split[pos] == lbl){return;}
	  lines_split[pos] = lbl;
	  
	}
	
	
	inline void cook_line_splitting_exclusive()
	{
	  std::cout << "--> start union exclusive" << std::endl;
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    lines_union.invalidate_link(N);
	    if(end_lines_mask(N) || implicit_separators_right_mask(N))
	      split_line_exclusive(N);
	  }
	   std::cout << "--> start linking" << std::endl;
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(lines_union.is_self_link(N))
	    {
	      add_new_line(N);
	      add_to_line(N);
	    }
	    else if(end_lines_mask(N))
		if(!lines_union.is_self_link(N))
		  lines_union.add_self_link(N);
	    else
	    {lines_union.invalidate_link(N);}
	  }
	  lines_union[0] = 0;
	  lines_union.invalidate_link(0);
	  std::cout << "--> propage union " << std::endl;
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(!contain_line(N) || lines_union.is_self_link(N))
	      continue;    
	    Label pos = get_end_of_line(N);
	    Label oldpos = pos;
	    while(lines_split[pos] && _bboxgp[lines_split[pos]].pmin()[1] >= _bboxgp[N].pmin()[1])
	      {oldpos = pos; pos = lines_split[pos];}
	    if(pos != 0 && pos != N && pos < Areas_Number_ )
	    {lines_union[N] = lines_union[pos]; lines_union.add_link(pos,N);}
	  }
	 std::cout << "--> end propage union " << std::endl;
	  //lines_union.propage_links();lines_union
	  cook_lines();
	}
	
	inline void cook_line_splitting()
	{
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    lines_union.invalidate_link(N);
	    if(start_lines_mask(N) || implicit_separators_left_mask(N))
	      split_line(N);
	  }
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(lines_union.is_self_link(N))
	    {
	      add_new_line(N);
	      add_to_line(N);
	    }
	    else if(start_lines_mask(N))
	      	if(!lines_union.is_self_link(N))
		lines_union.add_self_link(N);
	    else
	    {lines_union.invalidate_link(N);}
	  }
	  lines_union[0] = 0;
	  lines_union.invalidate_link(0);
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(!contain_line(N) || lines_union.is_self_link(N))
	      continue;    
	    Label pos = get_beginning_of_line(N);
	    while(lines_split[pos] && _bboxgp[lines_split[pos]].pmin()[1] < _bboxgp[N].pmin()[1])
	      pos = lines_split[pos];
	    if(pos != 0 && pos != N && pos < Areas_Number_ )
	    {lines_union[N] = lines_union[pos]; lines_union.add_link(pos,N);}
	  }
	  //lines_union.propage_links();lines_union
	  cook_lines();
	}
	inline void add_to_line_self_link(const point2d& point)
	{ add_to_line_self_link(img_influ(point));}
	inline void add_to_line(const point2d& point)
	{ add_to_line(img_influ(point)); }
	
  
	inline void add_to_line_link(const point2d& A, const point2d& B)
	{ add_to_line_link(img_influ(A), img_influ(B)); }
	inline void invalidate_line_link(const point2d& A)
	{ invalidate_line_link(img_influ(A)); }
	inline bool same_line(const point2d& A, const point2d& B)
	{ return  same_line(img_influ(A), img_influ(B)); }
	inline bool same_line(const Label A, const Label B)
	{ return  lines_union[A] == lines_union[B]; }
	inline bool same_paragraph(const point2d& A, const point2d& B)
	{ return  same_paragraph(img_influ(A), img_influ(B)); }
	inline bool same_paragraph(const Label A, const Label B)
	{ return  paragraphs_union[A] == paragraphs_union[B]; }
	
	inline bool in_beginning_of_line(const point2d& A)
	{return in_beginning_of_line(img_influ(A));}
	inline bool in_beginning_of_line(const Label A)
	{return lines_bbox[lines_union[A]].len(1) / 8 + lines_bbox[lines_union[A]].pmin()[1] > _bboxgp[A].pmax()[1];}


	inline bool is_line_representative(const point2d& A)
	{return is_line_representative(img_influ(A));}
	inline bool is_line_representative(const Label A)
	{return  lines_bbox[lines_union[A]].len(0) < _bboxgp[A].len(0) * 2 ;}


	inline bool in_end_of_line(const point2d& A)
	{return in_end_of_line(img_influ(A));}
	inline bool in_end_of_line(const Label A)
	{return  lines_bbox[lines_union[A]].pmax()[1] - lines_bbox[lines_union[A]].len(1) / 8 < _bboxgp[A].pmax()[1];}

	inline unsigned int space(const point2d& A,const point2d& B)
	{return space(img_influ(A), img_influ(B));}
	inline unsigned int space(const Label A, const Label B)
	{
	      box2d LB = _bboxgp[A];
	      box2d RB = _bboxgp[B];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; } 
	      return  DisA;
	}



	inline void add_new_line(const point2d& point)
	{ add_new_line(img_influ(point)); }
	
	inline void jump_to_line(const point2d& point)
	{ jump_to_line(img_influ(point)); }

	inline bool contain_start_paragraph(const point2d& point)
	{ return contain_start_paragraph(img_influ(point)); }

	inline bool contain_start_line(const point2d& point)
	{ return contain_start_line(img_influ(point)); }
	
	inline bool contain_end_line(const point2d& point)
	{ return contain_end_line(img_influ(point)); }

	inline bool contain_line(const point2d& point)
	{ return contain_line(img_influ(point)); }
	
	inline bool contain_line_start(const point2d& point)
	{ return contain_line_start(img_influ(point)); }
	inline bool contain_line_start(const Label lbl)
	{ return lines_first_label(lbl); }
	
	inline void add_to_line(const Label lbl)
	{ lines_union[lbl] = CLine; }
	
	inline void add_new_line(const Label lbl)
	{ CLine = NLine; NLine++; }
	
	
	inline void add_to_line_self_link(const Label A)
	{lines_union.add_self_link(A);}
	
	inline void add_to_line_link(const Label A, const Label B)
	{lines_union.add_link(A, B);}
	inline void invalidate_line_link(const Label A)
	{lines_union.invalidate_link(A);}
	inline void jump_to_line(const Label lbl)
	{ 
	  if(lines_union[lbl] != 0)
	    CLine = lines_union[lbl];
	  else
	    add_new_line(lbl);
	}
	

	
	inline bool contain_line_self_link(const Label lbl)
	{ return lines_union[lbl].is_self_link();}
	inline bool move_line_self_link(const Label lbl)
	{ 
	  if(lines_union[lbl] && lines_union.is_self_link(lbl))
	  {
	    if(lines_first_label[lines_union[lbl]] == lbl)
	    {
	      if(lines_union[lines_last_label[lines_union[lbl]]] == 0) // CHECK IF THE LAST LABEL HAS NOT BEEN REMOVED
		recook_lines();
	      
	      lines_union.add_link(lines_last_label[lines_union[lbl]], lbl);
	      lines_union.add_self_link(lines_last_label[lines_union[lbl]]);
	    }
	    else if(lines_last_label[lines_union[lbl]] == lbl)
	    {
	      if(lines_union[lines_first_label[lines_union[lbl]]] == 0) // CHECK IF THE FIRST LABEL HAS NOT BEEN REMOVED
		recook_lines();
	      
	      lines_union.add_link(lines_first_label[lines_union[lbl]], lbl);
	      lines_union.add_self_link(lines_first_label[lines_union[lbl]]);
	    }
	    else
	    {
	      if(lines_union[lines_first_label[lines_union[lbl]]])
	      {
		lines_union.add_link(lines_first_label[lines_union[lbl]], lbl);
		lines_union.add_self_link(lines_first_label[lines_union[lbl]]);
	      }
	      else if(lines_first_label[lines_union[lbl]])
	      {
		lines_union.add_link(lines_last_label[lines_union[lbl]], lbl);
		lines_union.add_self_link(lines_last_label[lines_union[lbl]]);
	      }
	      else
	      {
		recook_lines();
		lines_union.add_link(lines_first_label[lines_union[lbl]], lbl);
		lines_union.add_self_link(lines_first_label[lines_union[lbl]]);
	      }
	    }
	    return true;
	  }
	  return false;
	}
	
	inline bool contain_line(const Label lbl)
	{ return lines_union[lbl] != 0;}
	
	inline bool contain_start_line(const Label lbl)
	{ return  start_lines_mask(lbl);}
	
	inline bool contain_start_paragraph(const Label lbl)
	{ return  paragraphs_first_line[paragraphs_union[lbl]] == lines_union[lbl];}
	
	inline bool contain_end_line(const Label lbl)
	{ return  start_lines_mask(lbl);}
	
	inline void add_noise(const point2d& point)
	{add_noise(img_influ(point));}
	
	/// DEPRECIATED
	/// THE METHOD SIZE IS HERE ONLY TO USE THE SAME SYNTAX AS MILENA ARRAYS
	/// USE count() INSTEED
	inline unsigned int size(){return Areas_Number_;} 
	
	
	inline unsigned int line_count(){return lines_bbox.size();}
	inline unsigned int paragraph_count(){return paragraphs_bbox.size();}
	
	void add_noise(Label lbl)
	{
	  separators_mask(lbl) = false;
	  letters_mask(lbl) = false;
	  alone_letters_mask(lbl) = false;
	  containers_mask(lbl) = false;
	  Hseparator_mask(lbl) = false;
	  Vseparator_mask(lbl) = false;
	  alone_letters_mask(lbl) = false;
	  all_letters_mask(lbl) = false;
	  if(lines_cooked)
	  {
	    start_end_lines_mask(lbl) = false;
	    end_lines_mask(lbl) = false;
	    start_lines_mask(lbl) = false;
	  }
	  noise_mask(lbl) = true;
	  lines_union[lbl] = 0;
	}
	void inline add(Label lbl, int link)
	{
	  image_mask(lbl) = false;
	  all_mask(lbl) = true;
	  if (link == 0){add_noise(lbl);}
	  else if (link > 30){ add_separator(lbl);}
	  else 
	  {
	    add_letter(lbl);
	  }
	  
	  /* SET UP SPECIAL MASK TO FALSE */
	  implicit_separators_left_mask(lbl) = false;
	  implicit_separators_right_mask(lbl) = false;
	  anomaly_mask(lbl) = false;
	  kill_mask(lbl) = false;
	  temp_letter(lbl) = false;
	}
	void inline invalid_letter(const point2d& point)
	{invalid_letter(img_influ(point));}
	void invalid_letter(Label lbl)
	{letters_mask(lbl) = false;}
	void inline invalid_separator(const point2d& point)
	{invalid_separator(img_influ(point));}
	void invalid_separator(Label lbl)
	{
	  separators_mask(lbl) = false;
	  Vseparator_mask(lbl) = false;
	  Hseparator_mask(lbl) = false;
	}
	void inline invalid_container(const point2d& point)
	{invalid_container(img_influ(point));}
	void invalid_container(const Label lbl)
	{containers_mask(lbl) = false;}
	void inline add_letter(const point2d& point)
	{add_letter(img_influ(point)); }
	void inline add_letter_coerce(const point2d& point)
	{add_letter_coerce(img_influ(point)); }
	
	
	void add_image(const Label lbl)
	{
	  image_mask(lbl) = true;
	  separators_mask(lbl) = false;
	  containers_mask(lbl) = false;
	  Vseparator_mask(lbl) = false;
	  Hseparator_mask(lbl) = false;
	  alone_letters_mask(lbl) = false;
	  noise_mask(lbl) = false;
	  all_letters_mask(lbl) = false;
	  temp_letter = false;
	}
	
	
	
	void add_anomaly(const point2d& point)
	{add_anomaly(img_influ(point));}
	void add_anomaly(const Label lbl)
	{anomaly_mask(lbl) = true;}
	
	
	void remove_anomaly(const point2d& point)
	{remove_anomaly(img_influ(point));}	
	void remove_anomaly(const Label lbl)
	{anomaly_mask(lbl) = false;}	
	
	
	bool contain_anomaly(const point2d& point)
	{return contain_anomaly(img_influ(point));}
	void contain_anomaly(const Label lbl)
	{return anomaly_mask(lbl);}		
	
	void add_alone_letter(const point2d& point)
	{add_alone_letter(img_influ(point));}
	void add_alone_letter(const Label lbl)
	{
	  letters_mask(lbl) = false;
	  separators_mask(lbl) = false;
	  containers_mask(lbl) = false;
	  Vseparator_mask(lbl) = false;
	  Hseparator_mask(lbl) = false;
	  alone_letters_mask(lbl) = true;
	  noise_mask(lbl) = false;
	  all_letters_mask(lbl) = true;
	  temp_letter = false;
	}
	void add_letter_coerce(const Label lbl)
	{
	  
	      letters_mask(lbl) = true;
	      all_letters_mask(lbl) = true;
	      separators_mask(lbl) = false;
	      containers_mask(lbl) = false;
	      Vseparator_mask(lbl) = false;
	      Hseparator_mask(lbl) = false;
	      alone_letters_mask(lbl) = false;	      
	      noise_mask(lbl) = false;
	      temp_letter = false;
	}
	void add_letter(const Label lbl)
	{ 
	      CLet++;
	      if(label_valid_size_Min_(lbl, 3) || label_valid_size_Min_Large_(lbl, 2))
	      {
		if(letter_ratio_XY(lbl) > 20)
		{add_separator(lbl); return;}
		if(letter_ratio_YX(lbl) > 10)
		{add_separator(lbl); return;}
		
	      img_influ(_bboxgp[lbl].pcenter()) = lbl;
	      letters_mask(lbl) = true;
	      all_letters_mask(lbl) = true;
	      separators_mask(lbl) = false;
	      containers_mask(lbl) = false;
	      Vseparator_mask(lbl) = false;
	      Hseparator_mask(lbl) = false;
	      alone_letters_mask(lbl) = false;
	      temp_letter = false;
	       noise_mask(lbl) = false;
	      }
	      else
		add_noise(lbl);
	}
	inline bool is_big_element_V(const point2d& point)
	{return is_big_element_V(img_influ(point));}
	inline bool is_big_element_V(const Label lbl)
	{
	  return _bboxgp[lbl].len(0) > img_influ.domain().len(0) / 13;
	}
	inline bool is_big_element_H(const point2d& point)
	{return is_big_element_H(img_influ(point));}
	inline bool is_big_element_H(const Label lbl)
	{
	  return _bboxgp[lbl].len(1) > img_influ.domain().len(1) / 13;
	}
	
	
	
	
	
	inline bool is_very_big_element_V(const point2d& point)
	{return is_very_big_element_V(img_influ(point));}
	inline bool is_very_big_element_V(const Label lbl)
	{
	  return _bboxgp[lbl].len(0) > img_influ.domain().len(0) / 6;
	}
	inline bool is_very_big_element_H(const point2d& point)
	{return is_very_big_element_H(img_influ(point));}
	inline bool is_very_big_element_H(const Label lbl)
	{
	  return _bboxgp[lbl].len(1) > img_influ.domain().len(1) / 6;
	}
	
	
	
	
	
	
	void inline add_container(const point2d& point)
	{add_container(img_influ(point)); }
	void add_container(const Label lbl)
	{
	  if(label_valid_size_Min_(lbl, 2))
	  {
	   CSep++;
	   containers_mask(lbl) = true;
	   Vseparator_mask(lbl) = false;
	   Hseparator_mask(lbl) = false;
	   letters_mask(lbl) = false;
	   separators_mask(lbl) = false;	   
	    noise_mask(lbl) = false;
	    alone_letters_mask(lbl) = false;
	    all_letters_mask(lbl) = false;
	    
	  }
	  else
		add_noise(lbl);
	}
	void add_Hseparator(const Label lbl)
	{
	  _bboxgp_influ[lbl] = _bboxgp[lbl].to_larger(4);
	   CSep++;
	   containers_mask(lbl) = false;
	   Vseparator_mask(lbl) = false;
	   Hseparator_mask(lbl) = true;
	   letters_mask(lbl) = false;
	   separators_mask(lbl) = true;
	   alone_letters_mask(lbl) = false;
	    noise_mask(lbl) = false;
	    all_letters_mask(lbl) = false;
	}
	void add_Vseparator(const Label lbl)
	{
	  _bboxgp_influ[lbl] = _bboxgp[lbl].to_larger(4);
	   CSep++;
	   containers_mask(lbl) = false;
	   Vseparator_mask(lbl) = true;
	   Hseparator_mask(lbl) = false;
	   letters_mask(lbl) = false;
	   separators_mask(lbl) = true;
	   alone_letters_mask(lbl) = false;
	    noise_mask(lbl) = false;
	    all_letters_mask(lbl) = false;
	}
	bool inline separator_has(const point2d& A, const point2d& B)
	{
	  return _bboxgp_influ[img_influ(A)].has(B) || separator_has(img_influ(A), img_influ(B));
	}
	bool inline separator_has(const Label A,const Label B)
	{
	  return _bboxgp_influ[A].has(_bboxgp[B].pmin()) || _bboxgp_influ[A].has(_bboxgp[B].pmax());
	}
	void inline add_separator(const point2d& point)
	{add_letter(img_influ(point)); }
	void add_separator(const Label lbl)
	  { 
	    
	    if(label_valid_ratio_(lbl, _VSepRatio_Min,_VSepRatio_Max))
	      add_Vseparator(lbl);
	    else if(label_valid_ratio_(lbl, _HSepRatio_Min,_HSepRatio_Max))
	      add_Hseparator(lbl);
	    else 
	      add_container(lbl);
	  }
	  
	  bool inline contain_alone_letter(const Label lbl)
	  {return contain_(lbl, alone_letters_mask);}
	  
	  bool inline contain_alone_letter(const point2d& point)
	  {return contain_alone_letter(img_influ(point));}
	  
	  bool inline contain_separator(const Label lbl)
	  {return contain_(lbl, separators_mask);}
	  
	  bool inline contain_Vseparator(const Label lbl)
	  {return contain_(lbl, Vseparator_mask);}
	  	  bool inline contain_Hseparator(const Label lbl)
	  {return contain_(lbl, Hseparator_mask);}
	  
	  bool inline contain_letter(const Label lbl)
	  {return contain_(lbl, letters_mask);}
	  
	  
	  
	  bool inline contain_container(const Label lbl)
	  {return contain_(lbl, containers_mask);}
	  
	  bool inline contain_separator(const point2d& point)
	  {return contain_separator(img_influ(point));}
	  
	  bool inline contain_letter(const point2d& point)
	  {return contain_letter(img_influ(point));}
	  
	  bool inline contain_container(const point2d& point)
	  {return contain_container(img_influ(point));}
	  
	  box2d inline get_bbox(const point2d& point)
	  {return _bboxgp(img_influ(point));}
	  
	  box2d inline get_bbox(const Label lbl)
	  {return _bboxgp(lbl);}
	  
	  inline void vertical_separator_ratio_range(Float min, Float max)
	  {_VSepRatio_Min = min; _VSepRatio_Max = max;}
	  
	  inline void horizontal_separator_ratio_range(Float min, Float max)
	  {_HSepRatio_Min = min; _HSepRatio_Max = max;}
	  
	  inline void letter_volume_range(Data min, Data max)
	  {_LeterVolume_Min = min; _LeterVolume_Max = max;}
	  
	  inline void container_volume_range(Data min, Data max)
	  {_ContainerVolume_Min = min; _ContainerVolume_Max = max;}
	  
	  /* ALLIGN FUNCTION */
	  
	  inline bool allign_top( const point2d& Left, const point2d& Right)
	  {return allign_top(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_top_line( const point2d& Left, const point2d& Right)
	  {return allign_top_line(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_top_line( const Label Left, const Label Right)
	  {
	    short int allignV = lines_bbox[lines_union[Left]].pcenter()[0] - lines_bbox[lines_union[Right]].pcenter()[0];
	    return  (!allignV < 0) && allignV * 2 > lines_bbox[lines_union[Left]].len(0);
	  }
			  
	  
	  inline bool allign_top( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right);
	    return  allignV < label_size_(0, Left) && (_bboxgp[Left].pcenter()[0]) > (_bboxgp[Right].pcenter()[0]);
	  }
	  
	  inline bool allign_top_large( const point2d& Left, const point2d& Right)
	  {return allign_top_large(img_influ(Left), img_influ(Right));}
	  
	    inline bool allign_top_large( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right);
	    return  allignV < lines_bbox[lines_union[Left]].len(0) * 2 && (_bboxgp[Left].pcenter()[0]) > (_bboxgp[Right].pcenter()[0]);
	  }
	  
	  inline bool allign_bottom_large( const point2d& Left, const point2d& Right)
	  {return allign_bottom_large(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_bottom_large( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right);
	    return  allignV < lines_bbox[lines_union[Left]].len(0) * 2 && (_bboxgp[Left].pcenter()[0]) < (_bboxgp[Right].pcenter()[0]);
	  }
	  
	  inline bool allign_up_line( const point2d& Left, const point2d& Right)
	  {return allign_up_line(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_up_line( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right) * 1.3f;
	    return  allignV < label_size_(0, Left) && (_bboxgp[Left].pcenter()[0]) > (_bboxgp[Right].pcenter()[0]);
	  }
	  
	  
	  inline bool allign_paragraph_center(const point2d& Left, const point2d& Right)
	  {return allign_paragraph_center(img_influ(Left), img_influ(Right));}
	  inline  bool allign_paragraph_center(const Label Left, const Label Right)
	  {
	    short int Diff = paragraphs_bbox[paragraphs_union[Left]].pcenter()[1] - paragraphs_bbox[paragraphs_union[Right]].pcenter()[1];
	    if(Diff < 0){Diff = -Diff;}
	    return Diff < paragraphs_bbox[paragraphs_union[Left]].len(1)/ 30 && Diff < paragraphs_bbox[paragraphs_union[Right]].len(1) / 30;
	  }
	  inline bool allign_paragraph_center_strict(const point2d& Left, const point2d& Right)
	  {return allign_paragraph_center_strict(img_influ(Left), img_influ(Right));}
	  inline  bool allign_paragraph_center_strict(const Label Left, const Label Right)
	  {
	    short int Diff = paragraphs_bbox[paragraphs_union[Left]].pcenter()[1] - paragraphs_bbox[paragraphs_union[Right]].pcenter()[1];
	    if(Diff < 0){Diff = -Diff;}
	    return Diff < paragraphs_bbox[paragraphs_union[Left]].len(1)/ 60 && Diff < paragraphs_bbox[paragraphs_union[Right]].len(1) / 60;
	  }  
	
	  inline bool allign_line_center(const point2d& Left, const point2d& Right)
	  {return allign_line_center(img_influ(Left), img_influ(Right));}
	  inline  bool allign_line_center(const Label Left, const Label Right)
	  {
	    short int Diff = lines_bbox[lines_union[Left]].pcenter()[1] - lines_bbox[lines_union[Right]].pcenter()[1];
	    if(Diff < 0){Diff = -Diff;}
	    return Diff < lines_bbox[lines_union[Left]].len(1)/ 30 && Diff < lines_bbox[lines_union[Right]].len(1) / 30;
	  }
	
	
	
	  
	  inline bool allign_up_line_line( const point2d& Left, const point2d& Right)
	  {return allign_up_line_line(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_up_line_line( const Label Left, const Label Right)
	  {
	    short int allignV = lines_bbox[lines_union[Left]].pcenter()[0] - lines_bbox[lines_union[Right]].pcenter()[0];
	    if(allignV < 0){allignV = -allignV; }
	    allignV *= 1.4f;
	    return  
	      allignV < lines_bbox[lines_union[Left]].len(0) &&
	      (lines_bbox[lines_union[Left]].pcenter()[0]) > (lines_bbox[lines_union[Left]].pcenter()[0]);
	  }
	  
	  inline bool allign_left( const point2d& Left, const point2d& Right)
	  {return allign_left(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_left( const Label Left, const Label Right)
	  {
	      
	    return  _bboxgp[Left].pmin()[1] >  _bboxgp[Right].pmin()[1];
	  }
	  	  
	  inline bool allign_right( const point2d& Left, const point2d& Right)
	  {return allign_right(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_right( const Label Left, const Label Right)
	  {
	      
	    return  _bboxgp[Left].pmin()[1] <  _bboxgp[Right].pmin()[1];
	  }
	  
	  
	  
	  inline bool allign_H_large_one( const point2d& Left, const point2d& Right)
	  {return allign_H_large_one(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_large_one( const Label Left, const Label Right)
	  {
	      short int allignV = label_allign_(1, Left, Right) * 1.5f;
	    return  allignV <= label_size_(1, Left) + 2;
	  }
	  
	  inline bool allign_H_large( const point2d& Left, const point2d& Right)
	  {return allign_H_large(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_large( const Label Left, const Label Right)
	  {
	      short int allignV = label_allign_(1, Left, Right) * 1.5f;
	    return  allignV < label_size_(1, Left);
	  }
	  
	  inline bool allign_H_tube( const point2d& Left, const point2d& Right)
	  {return allign_H_tube(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_tube( const Label Left, const Label Right)
	  {
	    return  _bboxgp[Left].pmin()[1] <= _bboxgp[Right].pcenter()[1] &&
	    _bboxgp[Left].pmax()[1] >= _bboxgp[Right].pcenter()[1];
	  }
	  


	  inline bool allign_H_strict( const point2d& Left, const point2d& Right)
	  {return allign_H(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_strict( const Label Left, const Label Right)
	  {
	      short int allignH = label_allign_(1, Left, Right) * 5;
	    return  allignH < label_size_(1, Left) && allignH < label_size_(1, Right);
	  }


	  inline bool allign_H( const point2d& Left, const point2d& Right)
	  {return allign_H(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H( const Label Left, const Label Right)
	  {
	      short int allignH = label_allign_(1, Left, Right) * 2;
	    return  allignH < label_size_(1, Left) && allignH < label_size_(1, Right);
	  }
	  
	  
	  inline bool allign_H_min_paragraph( const point2d& Left, const point2d& Right)
	  {return allign_H_min_paragraph(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_min_paragraph( const Label Left, const Label Right)
	  {
	    short int allignH = paragraphs_bbox[paragraphs_union[Left]].pmin()[1] - paragraphs_bbox[paragraphs_union[Right]].pmin()[1];
	    allignH *= 2;
	    return  allignH < paragraphs_bbox[paragraphs_union[Left]].len(0);
	  }
	  
	  
	  inline bool allign_H_max_paragraph( const point2d& Left, const point2d& Right)
	  {return allign_H_max_paragraph(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_max_paragraph( const Label Left, const Label Right)
	  {
	    short int allignH = paragraphs_bbox[paragraphs_union[Left]].pmin()[1] - paragraphs_bbox[paragraphs_union[Right]].pmin()[1];
	    allignH *= 2;
	    return  allignH < paragraphs_bbox[paragraphs_union[Left]].len(0);
	  }
	  
	  
	  inline bool allign_H_min( const point2d& Left, const point2d& Right)
	  {return allign_H_min(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_min( const Label Left, const Label Right)
	  {
	      short int allignH = label_allign_min_(1, Left, Right) * 2;
	    return  allignH < label_size_(1, Left) && allignH < label_size_(1, Right);
	  }
	  
	  inline bool allign_H_max( const point2d& Left, const point2d& Right)
	  {return allign_H_max(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_max( const Label Left, const Label Right)
	  {
	      short int allignH = label_allign_max_(1, Left, Right) * 2;
	    return  allignH < label_size_(1, Left) && allignH < label_size_(1, Right);
	  }
	  
	  
	  inline bool allign_size_height( const point2d& Left, const point2d& Right)
	  {return allign_size_height(img_influ(Left), img_influ(Right));}


	 inline bool allign_proximity_top_strict( const point2d& Left, const point2d& Right)
	  {return allign_proximity_top(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_top_strict( const Label Left, const Label Right)
	  {
	      box2d LB = _bboxgp[Left];
	      box2d RB = _bboxgp[Right];
	      
	      int DisA = LB.pmax()[0] - RB.pmin()[0];
	      int DisB = RB.pmax()[0] - LB.pmin()[0];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = LB.len(1);

	      if(HB > HA)
	      { HA = HB; }
	      return  (DisA * 2) < HA;
	  }


	 inline bool allign_proximity_top( const point2d& Left, const point2d& Right)
	  {return allign_proximity_top(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_top( const Label Left, const Label Right)
	  {
	      box2d LB = _bboxgp[Left];
	      box2d RB = _bboxgp[Right];
	      
	      int DisA = LB.pmax()[0] - RB.pmin()[0];
	      int DisB = RB.pmax()[0] - LB.pmin()[0];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = LB.len(1);

	      if(HB > HA)
	      { HA = HB; }
	      return  (DisA * 2) < HA * 3;
	  }
	
	
	
	  inline bool allign_proximity_V( const point2d& Left, const point2d& Right)
	  {return allign_proximity_V(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_V( const Label Left, const Label Right)
	  {
	      short int SizeL0 = label_size_(0, Left);
	      short int SizeL1 = label_size_(1, Left);
	      short int Swap = 0;
	      if(SizeL0 < SizeL1)
	      { SizeL0 = SizeL1; }
	      short int Dis = _bboxgp[Left].pmin()[0] - _bboxgp[Right].pmin()[0];
	      if(Dis < 0)
		Dis = -Dis; 
	    return  Dis < SizeL0 * 1.5f;
	  }
	
	
	  inline bool allign_proximity( const point2d& Left, const point2d& Right)
	  {return allign_proximity(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity( const Label Left, const Label Right)
	  {
	      box2d LB = _bboxgp[Left];
	      box2d RB = _bboxgp[Right];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = LB.len(1);

	      if(HB > HA)
	      { HA = HB; }
	      return  (DisA * 2) < HA * 3;
	  }
	  
	  
	  
	  
	  inline bool allign_proximity_strict( const point2d& Left, const point2d& Right)
	  {return allign_proximity_strict(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_strict( const Label Left, const Label Right)
	  {
	      box2d LB = _bboxgp[Left];
	      box2d RB = _bboxgp[Right];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = RB.len(0);
	      unsigned int VA = LB.len(1);
	      unsigned int VB = RB.len(1);

	      if(VA > HA)
	      { HA = VA; }
	      if(VB > HB)
	      { HB = VB; }	      
	      return  (DisA * 2) < HA && (DisA * 2) < HB;
	  }
	  


	  inline bool allign_proximity_paragraph_up_large( const point2d& Left, const point2d& Right)
	  {return allign_proximity_paragraph_up_large(img_influ(Left), img_influ(Right));}
	  
	   inline bool allign_proximity_paragraph_up_large( const Label Left, const Label Right)
	  {
	      box2d LB = paragraphs_bbox[paragraphs_union[Left]];
	      box2d RB = paragraphs_bbox[paragraphs_union[Right]];
	      
	      int DisA = LB.pmax()[0] - RB.pmin()[0];
	      int DisB = RB.pmax()[0] - LB.pmin()[0];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = lines_bbox[paragraphs_first_line[paragraphs_union[Left]]].len(0);
	      unsigned int HB = lines_bbox[paragraphs_first_line[paragraphs_union[Right]]].len(0);

	      if(HA < HB)
	      { HA = HB; }
	      return  (DisA) < HA;
	  }
	  
	  
	  


	  inline bool allign_proximity_paragraph_up_medium( const point2d& Left, const point2d& Right)
	  {return allign_proximity_paragraph_up_medium(img_influ(Left), img_influ(Right));}
	  
	   inline bool allign_proximity_paragraph_up_medium( const Label Left, const Label Right)
	  {
	      box2d LB = paragraphs_bbox[paragraphs_union[Left]];
	      box2d RB = paragraphs_bbox[paragraphs_union[Right]];
	      
	      int DisA = LB.pmax()[0] - RB.pmin()[0];
	      int DisB = RB.pmax()[0] - LB.pmin()[0];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = lines_bbox[paragraphs_first_line[paragraphs_union[Left]]].len(0);
	      unsigned int HB = lines_bbox[paragraphs_first_line[paragraphs_union[Right]]].len(0);

	      if(HA < HB)
	      { HA = HB; }
	      return  (DisA * 2) < HA;
	  }



	  inline bool allign_proximity_paragraph_up( const point2d& Left, const point2d& Right)
	  {return allign_proximity_paragraph_up(img_influ(Left), img_influ(Right));}
	  
	   inline bool allign_proximity_paragraph_up( const Label Left, const Label Right)
	  {
	      box2d LB = paragraphs_bbox[paragraphs_union[Left]];
	      box2d RB = paragraphs_bbox[paragraphs_union[Right]];
	      
	      int DisA = LB.pmax()[0] - RB.pmin()[0];
	      int DisB = RB.pmax()[0] - LB.pmin()[0];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = lines_bbox[paragraphs_first_line[paragraphs_union[Left]]].len(0);
	      unsigned int HB = lines_bbox[paragraphs_first_line[paragraphs_union[Right]]].len(0);

	      if(HA < HB)
	      { HA = HB; }
	      return  (DisA * 5) < HA;
	  }

	  
	  inline bool allign_proximity_line_large( const point2d& Left, const point2d& Right)
	  {return allign_proximity_line_large(img_influ(Left), img_influ(Right));}


	  inline bool allign_size_height_paragraph_line( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_height_paragraph_line(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_height_paragraph_line( const Label Left, const Label Right)
	  {
	      short int SizeL = lines_bbox[paragraphs_first_line[paragraphs_union[Left]]].len(0);
	      short int SizeR = lines_bbox[paragraphs_first_line[paragraphs_union[Right]]].len(0);
	    return  SizeR > (SizeL / 2.2f) && SizeR < (SizeL * 2.2);
	  }



	  inline bool allign_size_height_line_medium( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_height_line_medium(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_height_line_medium( const Label Left, const Label Right)
	  {
	      short int SizeL = lines_bbox[lines_union[Left]].len(0);
	      short int SizeR = lines_bbox[lines_union[Right]].len(0);
	    return  SizeR > (SizeL / 1.8f) && SizeR < (SizeL * 1.8f);
	  }


	  inline bool allign_size_height_line( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_height_line(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_height_line( const Label Left, const Label Right)
	  {
	      short int SizeL = lines_bbox[lines_union[Left]].len(0);
	      short int SizeR = lines_bbox[lines_union[Right]].len(0);
	    return  SizeR > (SizeL / 2.2f) && SizeR < (SizeL * 2.2);
	  }
	  
	  inline bool allign_size_height_line_strict( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_height_line_strict(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_height_line_strict( const Label Left, const Label Right)
	  {
	      short int SizeL = lines_bbox[lines_union[Left]].len(0);
	      short int SizeR = lines_bbox[lines_union[Right]].len(0);
	    return  SizeR > (SizeL / 1.3f) && SizeR < (SizeL * 1.3);
	  }
	  
	  	  inline bool allign_proximity_line( const point2d& Left, const point2d& Right)
	  {return allign_proximity_line(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_size_width_large( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_width_large(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_width_large( const Label Left, const Label Right)
	  {
	      short int SizeL = _bboxgp[lines_union[Left]].len(1);
	      short int SizeR = _bboxgp[lines_union[Right]].len(1);
	    return  SizeR >= (SizeL / 5) && SizeR <= (SizeL * 5);
	  }

	  inline bool allign_size_width( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_width(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_width( const Label Left, const Label Right)
	  {
	      short int SizeL = _bboxgp[Left].len(1);
	      short int SizeR = _bboxgp[Right].len(1);
	    return  SizeR >= (SizeL / 2) && SizeR <= (SizeL * 2);
	  }
	  

	  
	  inline bool allign_size_width_strict( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_width_strict(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_width_strict( const Label Left, const Label Right)
	  {
	      short int SizeL = _bboxgp[Left].len(1);
	      short int SizeR = _bboxgp[Right].len(1);
	    return  SizeR >= (SizeL / 1.5f) && SizeR <= (SizeL * 1.5f);
	  }
	  
	  
	  
	  
	  inline bool allign_size_width_line( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_width_line(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_width_line( const Label Left, const Label Right)
	  {
	      short int SizeL = lines_bbox[lines_union[Left]].len(1);
	      short int SizeR = lines_bbox[lines_union[Right]].len(1);
	    return  SizeR > (SizeL / 4) && SizeR < (SizeL * 4);
	  }
	  
	  inline bool allign_size_width_paragraph( const point2d& Left, const point2d& Right)
	  {
	    return  allign_size_width_paragraph(img_influ(Left), img_influ(Right));
	  }

	  inline bool allign_size_width_paragraph( const Label Left, const Label Right)
	  {
	      short int SizeL = paragraphs_bbox[paragraphs_union[Left]].len(1);
	      short int SizeR = paragraphs_bbox[paragraphs_union[Right]].len(1);
	    return  SizeR > ((SizeL * 2) / 3) && SizeR < ((SizeL * 3) / 2);
	  }
	  
	  
	  inline bool allign_proximity_line( const Label Left, const Label Right)
	  {
	      box2d LB = lines_bbox[lines_union[Left]];
	      box2d RB = lines_bbox[lines_union[Right]];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = RB.len(0);

	      if(HA < HB)
	      { HA = HB; }
	      return  (DisA * 5) < HA;
	  }
	  
	  
	  inline bool allign_proximity_line_large( const Label Left, const Label Right)
	  {
	      box2d LB = lines_bbox[lines_union[Left]];
	      box2d RB = lines_bbox[lines_union[Right]];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = RB.len(0);

	      if(HA < HB)
	      { HA = HB; }
	      return  (DisA * 2) < HA;
	  }
	  
	  
	  inline bool allign_proximity_V_line( const point2d& Left, const point2d& Right)
	  {return allign_proximity_V_line(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_V_line( const Label Left, const Label Right)
	  {
	      box2d LB = lines_bbox[lines_union[Left]];
	      box2d RB = lines_bbox[lines_union[Right]];
	      
	      int DisA = LB.pmax()[0] - RB.pmin()[0];
	      int DisB = RB.pmax()[0] - LB.pmin()[0];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = RB.len(0);

	      if(HA < HB)
	      { HA = HB; }
	      return  (DisA * 1.5f) < HA;
	  }

	  inline bool allign_H_paragraph( const point2d& Left, const point2d& Right)
	  {return allign_H_paragraph(img_influ(Left), img_influ(Right));}
	  inline bool allign_H_paragraph( const Label Left, const Label Right )
	  {
	    short int Dis = paragraphs_bbox[paragraphs_union[Left]].pcenter()[1] - paragraphs_bbox[paragraphs_union[Right]].pcenter()[1];
	    if(Dis < 0){Dis = -Dis;}
	    return
		Dis * 2 < paragraphs_bbox[paragraphs_union[Right]].len(1) &&
		Dis * 2 < paragraphs_bbox[paragraphs_union[Left]].len(1);
	  }
	  
	  inline bool allign_top_paragraph( const point2d& Left, const point2d& Right)
	  {return allign_top_paragraph(img_influ(Left), img_influ(Right));}
	  inline bool allign_top_paragraph( const Label Left, const Label Right )
	  {return  paragraphs_bbox[paragraphs_union[Left]].pmin()[0] > paragraphs_bbox[paragraphs_union[Right]].pmax()[0]; }
	  
	  inline bool decal_left_paragraph(const point2d& Left, const point2d& Right)
	  {return decal_left_paragraph(img_influ(Left), img_influ(Right));}
	  inline bool decal_left_paragraph( const Label Left, const Label Right )
	  {
	    return  paragraphs_bbox[paragraphs_union[Left]].pmin()[1] > paragraphs_bbox[paragraphs_union[Right]].pmin()[1]
	    + (paragraphs_bbox[paragraphs_union[Right]].len(1) / 40) ;
	  }
	  
	  inline bool decal_left_paragraph_strong(const point2d& Left, const point2d& Right)
	  {return decal_left_paragraph_strong(img_influ(Left), img_influ(Right));}
	  inline bool decal_left_paragraph_strong( const Label Left, const Label Right )
	  {
	    return  paragraphs_bbox[paragraphs_union[Left]].pmin()[1] > paragraphs_bbox[paragraphs_union[Right]].pmin()[1]
	     + (paragraphs_bbox[paragraphs_union[Right]].len(1) / 20) ;
	  }




	  inline bool allign_proximity_left( const point2d& Left, const point2d& Right)
	  {return allign_proximity_left(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_left( const Label Left, const Label Right)
	  {
	      box2d LB = _bboxgp[Left];
	      box2d RB = _bboxgp[Right];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int VA = LB.len(1);


	      if(VA > HA)
	      { HA = VA; }    
	      return  (DisA) * 3 < HA * 2;
	  }

	  inline bool allign_proximity_strict_left( const point2d& Left, const point2d& Right)
	  {return allign_proximity_strict_left(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_strict_left( const Label Left, const Label Right)
	  {
	      box2d LB = _bboxgp[Left];
	      box2d RB = _bboxgp[Right];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }      
	      unsigned int HA = LB.len(0);
	      unsigned int VA = LB.len(1);
	      if(VA > HA)
	      { HA = VA; }	      
	      return  (DisA) * 2 < HA;
	  }


	  inline bool allign_proximity_large_left( const point2d& Left, const point2d& Right)
	  {return allign_proximity_large_left(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_large_left( const Label Left, const Label Right)
	  {
	      box2d LB = _bboxgp[Left];
	      box2d RB = _bboxgp[Right];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = RB.len(0);
	      unsigned int VA = LB.len(1);
	      unsigned int VB = RB.len(1);

	      if(VA > HA)
	      { HA = VA; }
	      if(VB > HB)
	      { HB = VB; }	      
	      return  (DisA) < HA * 2;
	  }


	  inline bool allign_proximity_large( const point2d& Left, const point2d& Right)
	  {return allign_proximity_large(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_proximity_large( const Label Left, const Label Right)
	  {
	      box2d LB = _bboxgp[Left];
	      box2d RB = _bboxgp[Right];
	      
	      int DisA = LB.pmax()[1] - RB.pmin()[1];
	      int DisB = RB.pmax()[1] - LB.pmin()[1];
	      if(DisA < 0){DisA = -DisA;}
	      if(DisB < 0){DisB = -DisB;}
	      if(DisA > DisB)
	      { DisA = DisB; }
	      
	      unsigned int HA = LB.len(0);
	      unsigned int HB = RB.len(0);
	      unsigned int VA = LB.len(1);
	      unsigned int VB = RB.len(1);

	      if(VA > HA)
	      { HA = VA; }
	      if(VB > HB)
	      { HB = VB; }	      
	      return  (DisA) < HA * 2 && (DisA) < HB * 2;
	  }
	  
	  
	  
	  inline bool allign_size_height( const Label Left, const Label Right)
	  {
	     short int SizeL = label_size_(0, Left);
	      short int SizeR = label_size_(0, Right);
	    return  SizeR > (SizeL / 3) && SizeR < (SizeL * 3);
	  }

	  inline bool allign_size_strict( const point2d& Left, const point2d& Right)
	  {return allign_size(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_size_strict( const Label Left, const Label Right)
	  {
	     short int SizeL0 = label_size_(0, Left);
	      short int SizeR0 = label_size_(0, Right);
	      short int SizeL1 = label_size_(1, Left);
	      short int SizeR1 = label_size_(1, Right);
	      short int Swap = 0;
	      if(SizeL0 < SizeL1)
	      { SizeL0 = SizeL1; }
	      if(SizeR0 < SizeR1){SizeR0 = SizeR1;}
	    return  SizeR0 > (SizeL0 / 2) && SizeR0 < (SizeL0 * 2);
	  }

	  inline bool allign_size( const point2d& Left, const point2d& Right)
	  {return allign_size(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_size( const Label Left, const Label Right)
	  {
	     short int SizeL0 = label_size_(0, Left);
	      short int SizeR0 = label_size_(0, Right);
	      short int SizeL1 = label_size_(1, Left);
	      short int SizeR1 = label_size_(1, Right);
	      short int Swap = 0;
	      if(SizeL0 < SizeL1)
	      { SizeL0 = SizeL1; }
	      if(SizeR0 < SizeR1){SizeR0 = SizeR1;}
	    return  SizeR0 > (SizeL0 / 4) && SizeR0 < (SizeL0 * 4);
	  }
	  
	  
	  inline bool allign_size_medium( const point2d& Left, const point2d& Right)
	  {return allign_size_medium(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_size_medium( const Label Left, const Label Right)
	  {
	     short int SizeL0 = label_size_(0, Left);
	      short int SizeR0 = label_size_(0, Right);
	      short int SizeL1 = label_size_(1, Left);
	      short int SizeR1 = label_size_(1, Right);
	      short int Swap = 0;
	      if(SizeL0 < SizeL1)
	      { SizeL0 = SizeL1; }
	      if(SizeR0 < SizeR1){SizeR0 = SizeR1;}
	    return  SizeR0 > (SizeL0 / 3) && SizeR0 < (SizeL0 * 3);
	  }
	  
	  
	  inline bool allign_size_height_max( const point2d& Left, const point2d& Right)
	  {return allign_size_height_max(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_size_height_max( const Label Left, const Label Right)
	  {
	     short int SizeL = label_size_(0, Left);
	      short int SizeR = label_size_(0, Right);
	    return  SizeR < (SizeL * 3);
	  }
	  
	  
	  inline bool allign_V( const point2d& Left, const point2d& Right)
	  {return allign_V(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right) * 2;
	    return  allignV < label_size_(0, Left) && allignV < label_size_(0, Right);
	  }
	  
	  inline bool allign_V_line( const point2d& Left, const point2d& Right)
	  {return allign_V_line(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V_line( Label Left, Label Right)
	  {
	    short int allignV = lines_bbox[lines_union[Left]].pcenter()[0] - lines_bbox[lines_union[Right]].pcenter()[0];
	    if(allignV<0){allignV = -allignV;}
	    allignV *= 2;
	    return  allignV < lines_bbox[lines_union[Left]].len(0) && allignV < lines_bbox[lines_union[Right]].len(0);
	  }
	  
	  	  inline bool allign_V_line_strict( const point2d& Left, const point2d& Right)
	  {return allign_V_line_strict(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V_line_strict( Label Left, Label Right)
	  {
	    short int allignV = lines_bbox[lines_union[Left]].pcenter()[0] - lines_bbox[lines_union[Right]].pcenter()[0];
	    if(allignV<0){allignV = -allignV;}
	    allignV *= 4;
	    return  allignV < lines_bbox[lines_union[Left]].len(0) && allignV < lines_bbox[lines_union[Right]].len(0);
	  }
	  
	  
	  inline bool allign_center_line( const point2d& Left, const point2d& Right)
	  {return allign_center_line(img_influ(Left), img_influ(Right));}
	  inline bool allign_center_line( Label Left, Label Right)
	  {
	    short int allignC = lines_bbox[lines_union[Left]].pcenter()[0] - lines_bbox[lines_union[Right]].pcenter()[0];
	    if(allignC<0){allignC = -allignC;}
	    return  allignC * 5 < lines_bbox[lines_union[Left]].len(0);
	  }
	  

	  inline bool allign_half_line_letter( const point2d& Left, const point2d& Right)
	  {return allign_half_line_letter(img_influ(Left), img_influ(Right));}
	    inline bool allign_half_line_letter( Label Left, Label Right)
	  {
	    return  lines_bbox[lines_union[Left]].len(0) > (_bboxgp[Right].len(0) * 8);
	  }

	  inline bool allign_small_item( const point2d& Left, const point2d& Right)
	  {return allign_small_item(img_influ(Left), img_influ(Right));}
	    inline bool allign_small_item( Label Left, Label Right)
	  {
	    return  
	      lines_bbox[lines_union[Left]].len(0) < (_bboxgp[Right].len(0) * 12) &&
	      lines_bbox[lines_union[Left]].len(0) > (_bboxgp[Right].len(0) * 2);
	  }
	  inline bool allign_small_item_large( const point2d& Left, const point2d& Right)
	  {return allign_small_item_large(img_influ(Left), img_influ(Right));}
	    inline bool allign_small_item_large( Label Left, Label Right)
	  {
	    return  
	      lines_bbox[lines_union[Left]].len(0) < (_bboxgp[Right].len(0) * 12) &&
	      lines_bbox[lines_union[Left]].len(0)*2 > (_bboxgp[Right].len(0) * 3);
	  }
	  inline bool allign_small_item_line( const point2d& Left, const point2d& Right)
	  {return allign_small_item(img_influ(Left), img_influ(Right));}
	    inline bool allign_small_item_line( Label Left, Label Right)
	  {
	    return  
	      lines_bbox[lines_union[Left]].len(0) < (lines_bbox[lines_union[Right]].len(0) * 12) &&
	      lines_bbox[lines_union[Left]].len(0) > (lines_bbox[lines_union[Right]].len(0) * 2);
	  }




	  inline bool allign_smaller_paragraph( const point2d& Left, const point2d& Right)
	  {return allign_smaller_paragraph(img_influ(Left), img_influ(Right));}
	  inline bool allign_smaller_paragraph( Label Left, Label Right)
	  {
	    return  paragraphs_bbox[paragraphs_union[Left]].len(1) > (paragraphs_bbox[paragraphs_union[Right]].len(1));
	  }


	  inline bool allign_smaller_line( const point2d& Left, const point2d& Right)
	  {return allign_smaller_line(img_influ(Left), img_influ(Right));}
	  inline bool allign_smaller_line( Label Left, Label Right)
	  {
	    return  lines_bbox[lines_union[Left]].len(0) > (lines_bbox[lines_union[Right]].len(0) * 2);
	  }
	  inline bool allign_smaller_line_strict( const point2d& Left, const point2d& Right)
	  {return allign_smaller_line_strict(img_influ(Left), img_influ(Right));}
	  inline bool allign_smaller_line_strict( Label Left, Label Right)
	  {
	    return  lines_bbox[lines_union[Left]].len(0) > (lines_bbox[lines_union[Right]].len(0) * 3);
	  }
	  inline bool allign_smaller_line_letter( const point2d& Left, const point2d& Right)
	  {return allign_smaller_line_letter(img_influ(Left), img_influ(Right));}
	  inline bool allign_smaller_line_letter( Label Left, Label Right)
	  {
	    return  lines_bbox[lines_union[Left]].len(0) > (_bboxgp[Right].len(0) * 1.5f);
	  }

	  inline bool allign_V_large( const point2d& Left, const point2d& Right)
	  {return allign_V_large(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V_large( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right);
	    return  allignV < label_size_(0, Left) && allignV < label_size_(0, Right);
	  }
	  
	  	  inline bool allign_V_side( const point2d& Left, const point2d& Right)
	  {return allign_V_side(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V_side( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right);
	    return  allignV * 4 < label_size_(0, Left);
	  }
	  
	  
	   inline bool allign_V_extra_large( const point2d& Left, const point2d& Right)
	  {return allign_V_extra_large(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V_extra_large( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right) / 2;
	    return  allignV < label_size_(0, Left) && allignV < label_size_(0, Right);
	  }
	  
	  inline bool allign_base_line_line(const point2d& Left, const point2d& Right)
	  {return allign_base_line_line(img_influ(Left), img_influ(Right));}
	  inline bool allign_base_line_line(const Label Left, const Label Right)
	  {
	    short int allignV = lines_bbox[lines_union[Left]].pcenter()[0] - lines_bbox[lines_union[Right]].pcenter()[0];
	     if(allignV<0){allignV = -allignV;}
	    allignV *= 1.5f;
	    return  
	      allignV < lines_bbox[lines_union[Left]].len(0) &&
	      lines_bbox[lines_union[Left]].pcenter()[0] < lines_bbox[lines_union[Right]].pcenter()[0];
	  }
	  inline bool allign_base_line_line_strict(const point2d& Left, const point2d& Right)
	  {return allign_base_line_line_strict(img_influ(Left), img_influ(Right));}
	  inline bool allign_base_line_line_strict(const Label Left, const Label Right)
	  {
	    short int allignV = lines_bbox[lines_union[Left]].pcenter()[0] - _bboxgp[Right].pcenter()[0];
	     if(allignV<0){allignV = -allignV;}
	    allignV *= 3;
	    return  
	      allignV < lines_bbox[lines_union[Left]].len(0) &&
	      lines_bbox[lines_union[Left]].pcenter()[0] < lines_bbox[lines_union[Right]].pcenter()[0];
	  }
	  inline bool is_start_end_line(const point2d& point)
	  {return is_start_end_line(img_influ(point));}
	  inline bool is_start_end_line(const Label lbl)
	  {return start_end_lines_mask(lbl);}
	  inline bool allign_bottom(const point2d& Left, const point2d& Right)
	  {return allign_bottom(img_influ(Left), img_influ(Right));}
	  inline bool allign_bottom(const Label Left, const Label Right)
	  {
	    return _bboxgp[Left].pmin()[0] < _bboxgp[Right].pmin()[0];
	  }
	  
	  inline bool allign_bottom_line(const point2d& Left, const point2d& Right)
	  {return allign_bottom_line(img_influ(Left), img_influ(Right));}
	  inline bool allign_bottom_line(const Label Left, const Label Right)
	  {
	    return lines_bbox[lines_union[Left]].pmin()[0] < lines_bbox[lines_union[Right]].pmin()[0];
	  }
	  
	  inline bool allign_base_line_strict(const point2d& Left, const point2d& Right)
	  {return allign_base_line_strict(img_influ(Left), img_influ(Right));}
	  inline bool allign_base_line_strict(const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right) * 1.7f;
	    return  allignV < label_size_(0, Left) && _bboxgp[Left].pcenter()[0] < _bboxgp[Right].pcenter()[0];
	  }	  
	  
	  inline bool allign_base_line(const point2d& Left, const point2d& Right)
	  {return allign_base_line(img_influ(Left), img_influ(Right));}
	  inline bool allign_base_line(const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right) * 1.5f;
	    return  allignV < label_size_(0, Left) && _bboxgp[Left].pcenter()[0] < _bboxgp[Right].pcenter()[0];
	  }
	  inline bool allign_Space_Factor(const point2d& Left, const point2d& Right)
	  {return allign_Space_Factor(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_Space_Factor( const Label Left, const Label Right)
	  {
	    Float AFactor = label_allign_(1, Left, Right);
	    return AFactor < label_size_(1,Left);
	  }
	  
	  
	   inline bool paragraph_start_with_tab_strict(const point2d& Point)
	  { return paragraph_start_with_tab_strict(img_influ(Point));}
	  inline bool paragraph_start_with_tab_strict(const Label Paragraph)
	  {
	    Label FirstLine = paragraphs_first_line[paragraphs_union[Paragraph]];
	    return
	      lines_bbox[FirstLine].pmin()[1] > paragraphs_bbox[paragraphs_union[Paragraph]].pmin()[1] +
	      (paragraphs_bbox[paragraphs_union[Paragraph]].len(1) / 40) &&
	      lines_bbox[FirstLine].pmax()[1] > paragraphs_bbox[paragraphs_union[Paragraph]].pmax()[1] -
	      (paragraphs_bbox[paragraphs_union[Paragraph]].len(1) / 40)
	      ;
	  }
	  
	  inline bool paragraph_start_with_tab(const point2d& Point)
	  { return paragraph_start_with_tab(img_influ(Point));}
	  inline bool paragraph_start_with_tab(const Label Paragraph)
	  {
	    Label FirstLine = paragraphs_first_line[paragraphs_union[Paragraph]];
	    
	    if(lines_bbox[FirstLine].pmin()[1] > paragraphs_bbox[paragraphs_union[Paragraph]].pmin()[1] +
	      (paragraphs_bbox[paragraphs_union[Paragraph]].len(1) / 30))
	    {
	      debug_draw_box_green_influence_buffer(lines_first_label[FirstLine]); // ERASE ME
	    }
	    else
	    {
	      debug_draw_box_red_influence_buffer(lines_first_label[FirstLine]); // ERASE ME
	    }
	    return
	      lines_bbox[FirstLine].pmin()[1] > paragraphs_bbox[paragraphs_union[Paragraph]].pmin()[1] +
	      (paragraphs_bbox[paragraphs_union[Paragraph]].len(1) / 40) 
	      ;
	  }
	  
	  void stat()
	  {
	    std::cout << "document :" << std::endl;
	    std::cout << "   letter(s) : " << CLet << std::endl;
	    std::cout << "   separator(s) : " << CSep << std::endl;
	    std::cout << "      vertical separator(s) : " << CSepV << std::endl;
	    std::cout << "      horizontal separator(s) : " << CSepH << std::endl;
	    std::cout << "      lines(s) : " << CLine << std::endl;
	  
	  }
	  
	  
	  void debug_breakpoint()
	  {
	    if(debug_buffer_enable)
	    {

	      debug_save_buffer("break.ppm");
	      std::system("eog break.ppm");
	      debug_buffer_enable = true;
	      Enable_Debug_Buffer = true;
	    }
	    else
	    {
	      debug_save_all("break.ppm");
	      std::system("eog break.ppm");
	    }
	  }
	  void debug_set_image(image2d<bool>& source)
	  {debug_source = source;}
	  
	  /// ADD TEMP LETTER
	  /// description : add a label to the letter mask. The label will remain a letter while
	  /// reset_temp_letter is not called
	  /// WARNING: The old type of the label is still activated
	  inline void add_temp_letter(const point2d& Lbl)
	  {
	    add_temp_letter(img_influ(Lbl));
	  }
	  inline void add_temp_letter(const Label lbl)
	  {
	    if(!all_letters_mask(lbl))
	    {
	      letters_mask(lbl) = true;
	      all_letters_mask(lbl) = true;
	      temp_letter(lbl) = true;
	    }
	  }
	  inline void reset_temp_letter()
	  {
	    for(int N = 0; N < Areas_Number_; N++)
	    {
	      if(temp_letter(N))
	      {
		  letters_mask(N) = false;
		  all_letters_mask(N) = false;
		  start_end_lines_mask(N) = false;
		  end_lines_mask(N) = false;
		  start_lines_mask(N) = false;
	      } 
	    }
	  }
	   inline void debug_disable_buffer()
	  {
	      debug_buffer_enable = false;
	  }
	  inline void debug_enable_buffer()
	  {
	    if(Enable_Debug_Buffer)
	    {
	      debug_buffer_enable = true;
	    }
	    else
	    {
	      debug_create_buffer();
	    }
	  }
	  inline void debug_create_buffer()
	  {
	    mln::initialize(debug_buffer,img_influ);
	    debug_buffer_enable = true;
	    Enable_Debug_Buffer = true;
	  }
	  inline void debug_save_dot_graph(std::string file)
	  {
	    fstream filestream(file.c_str(), fstream::in | fstream::out);
	    filestream << "graph 1 { " << std::endl;  
	      typedef vertex_image<point2d,bool> v_ima_g;
	      typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	      v_ima_g mask = fun_mask_all_letters();
	      mln_piter_(v_ima_g) v(mask.domain());
	      typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	      nbh_t nbh(mask);
	      mln_niter_(nbh_t) q(nbh, v);
	      for(int N = 0; N < Areas_Number_; N++)
	      {
		filestream 
		<< "_" 
		<< N 
		<< " [pos=\""<<_bboxgp[N].pcenter()[1] 
		<< ".0," << img_influ.domain().pmax()[0] - _bboxgp[N].pcenter()[0] 
		<< ".0\" "
		<< "shape=\"box\" "
		<< "width=\"" << ((Float)_bboxgp[N].len(1)) / 70.0f  << ".0\" "
		<< "height=\"" <<  ((Float)_bboxgp[N].len(0)) / 70.0f << ".0\" "
		<< "label=\""<< get_tag(N) <<"\" "
		<< "];"
		
		<< std::endl;
	      }
	      for_all(v)
	      {
		  for_all(q)
		  {
		    if(contain_letter(q) && contain_letter(v))
		    {
		      if(contain_alone_letter(q) || contain_alone_letter(v))
		      {
			
			filestream<< "_" << img_influ(q) << " -- _" << img_influ(v) << " [style=\"dotted\"]";
		      }
		      else
		      {
			if(same_line(q,v))
			{filestream<< "_" << img_influ(q) << " -- _" << img_influ(v) << " [color=\"green\"]";}
			else
			{filestream<< "_" << img_influ(q) << " -- _" << img_influ(v) << ";" << std::endl;}
		      }
		    }
		    else
		    {
		      filestream<< "_" << img_influ(q) << " -- _" << img_influ(v) << " [style=\"dotted\" color=\"red\"]";
		    }
		      
		  }
	      }
	    filestream<< "}" << std::endl;
	    filestream.close();
	  }
	  
	  inline void debug_save_buffer(std::string file)
	  {
	    debug_buffer_enable = false;
	    Enable_Debug_Buffer = false;
	    io::ppm::save(mln::debug::superpose(debug_buffer, debug_source, literal::white) , file);
	  }
	  inline void debug_save_buffer_paragraphs(std::string file)
	  {
	    Enable_Debug_Buffer = false;
	    debug_buffer_enable = false;
	    for(unsigned int N = 0; N < paragraphs_bbox.size(); N++)
	    {
	      if(paragraphs_bbox[N].is_valid())
	      {
		draw::box(debug_buffer, paragraphs_bbox[N], mln::literal::red);
		if(paragraphs_bbox_influ[N].is_valid())
		{
		  draw::box(debug_buffer, paragraphs_bbox_influ[N], mln::literal::orange);
		}
	      }

	    }
	    io::ppm::save(mln::debug::superpose(debug_buffer, debug_source, literal::white) , file);
	  }
	  inline void debug_save_buffer_lines(std::string file)
	  {
	    Enable_Debug_Buffer = false;
	    debug_buffer_enable = false;
	    for(unsigned int N = 0; N < lines_bbox.size(); N++)
	    {
	      if(lines_bbox[N].is_valid())
	      {
		draw::box(debug_buffer, lines_bbox[N], mln::literal::blue);
	      }

	    }
	    io::ppm::save(mln::debug::superpose(debug_buffer, debug_source, literal::white) , file);
	  }
	  inline void debug_save_buffer_paragraphs_lines(std::string file)
	  {
	    Enable_Debug_Buffer = false;
	    debug_buffer_enable = false;
	    for(unsigned int N = 0; N < lines_bbox.size(); N++)
	    {
	      if(lines_bbox[N].is_valid())
	      {
		draw::box(debug_buffer, lines_bbox[N], mln::literal::blue);
	      }

	    }
	    	    for(unsigned int N = 0; N < paragraphs_bbox.size(); N++)
	    {
	      if(paragraphs_bbox[N].is_valid())
	      {
		draw::box(debug_buffer, paragraphs_bbox[N], mln::literal::red);
		if(paragraphs_bbox_influ[N].is_valid())
		{
		  draw::box(debug_buffer, paragraphs_bbox_influ[N], mln::literal::orange);
		}
	      }

	    }
	    io::ppm::save(mln::debug::superpose(debug_buffer, debug_source, literal::white) , file);
	  }
	  inline void debug_draw_box_red_buffer(const point2d& L)
	  {debug_draw_box_red_buffer(img_influ(L));}
	  inline void debug_draw_box_green_buffer(const point2d& L)
	  {debug_draw_box_green_buffer(img_influ(L));}
	  inline void debug_draw_box_red_buffer(const Label L)
	  {
	    if(debug_buffer_enable)
	      draw::box(debug_buffer, _bboxgp[L], mln::literal::red);
	  }
	  inline void debug_draw_box_green_buffer(const Label L)
	  {
	    if(debug_buffer_enable)
	      draw::box(debug_buffer,_bboxgp[L], mln::literal::green);
	  }	  
	  inline void debug_draw_box_green_influence_buffer(const Label L)
	  {
	    if(debug_buffer_enable)
	    {
	      draw::box(debug_buffer,_bboxgp[L], mln::literal::green);
	      draw::box(debug_buffer,_bboxgp_influ[L], mln::literal::green);
	    }
	  }	
	  inline void debug_draw_box_red_influence_buffer(const Label L)
	  {
	    if(debug_buffer_enable)
	    {
	      draw::box(debug_buffer,_bboxgp[L], mln::literal::red);
	      draw::box(debug_buffer,_bboxgp_influ[L], mln::literal::red);
	    }
	  }
	  inline void debug_draw_line_red_buffer(const point2d& A,const point2d& B )
	  {
	    if(debug_buffer_enable)
	      draw::line(debug_buffer, A, B, mln::literal::red);
	  }
	  inline void debug_draw_line_green_buffer(const point2d& A,const point2d& B )
	  {
	    if(debug_buffer_enable)
	      draw::line(debug_buffer, A, B, mln::literal::green);
	  }
	  inline void debug_draw_line_orange_buffer(const point2d& A,const point2d& B )
	  {
	    if(debug_buffer_enable)
	      draw::line(debug_buffer, A, B, mln::literal::orange);
	  }
	  template<typename UnionData>
	  void debug_union(mymln::util::union_find<UnionData> dat)
	  {
	    if(debug_buffer_enable)
	    {
	      for(unsigned int N = 1; N < dat.size(); N++)
	      {
		if(dat[N])
		{
		  if(N == dat.link(N))
		    draw::box(debug_buffer, _bboxgp[N], mln::literal::blue);
		  else
		    draw::line(debug_buffer, _bboxgp[N].pcenter(), _bboxgp[dat.link(N)].pcenter(), mln::literal::blue);
		}
		else if(dat.link(N) != 0)
		{
		  draw::line(debug_buffer, _bboxgp[N].pcenter(), _bboxgp[dat.link(N)].pcenter(), mln::literal::red);
		}
	      }
	    }
	  }
	  void debug_save_lines(std::string file)
	  {debug_save_lines(file, debug_source);}
	  void debug_save_lines(std::string file, image2d<bool> source)
	  {
	    	    image2d<value::rgb8> ima_color;
	    mln::initialize(ima_color,img_influ);
	    
	    for(unsigned int N = 0; N < lines_bbox.size(); N++)
	    {
	      if(lines_bbox[N].is_valid())
	      {
		draw::box(ima_color, lines_bbox[N], mln::literal::blue);
	      }
	    }
	    for(unsigned int N = 0; N < _bboxgp.size(); N++)
	    {
	      if(_bboxgp[N].is_valid() && contain_letter(N))
	      {
		draw::box(ima_color, _bboxgp[N], mln::literal::cyan);
	      }
	    }
	     io::ppm::save(mln::debug::superpose(ima_color, source, literal::white) , file);
	  }
	  
	  
	  void debug_save_all_separators(std::string file)
	  {debug_save_all_separators(file, debug_source);}
	  void debug_save_all_separators(std::string file, image2d<bool> source)
	  {
	    image2d<value::rgb8> ima_color;
	    mln::initialize(ima_color,img_influ);
	    
	    for(unsigned int N = 0; N < _bboxgp.size(); N++)
	    {
	      if(_bboxgp[N].is_valid() && (contain_letter(N) || contain_alone_letter(N)))
	      {
		if(sep_right_cooked)
		{
		  if(implicit_separators_right_mask(N))
		    draw::box(ima_color, _bboxgp[N], mln::literal::red);
		  else if(implicit_separators_left_mask(N))
		    draw::box(ima_color, _bboxgp[N], mln::literal::cyan);
		  else if(temp_letter(N))
		    draw::box(ima_color, _bboxgp[N], mln::literal::olive);
		  else
		    draw::box(ima_color, _bboxgp[N], mln::literal::green);
		}
		else
		{
		  if(implicit_separators_left_mask(N))
		    draw::box(ima_color, _bboxgp[N], mln::literal::cyan);
		  else if(implicit_separators_right_mask(N))
		    draw::box(ima_color, _bboxgp[N], mln::literal::red);
		  else if(temp_letter(N))
		    draw::box(ima_color, _bboxgp[N], mln::literal::olive);
		  else
		    draw::box(ima_color, _bboxgp[N], mln::literal::green);
		}
	      }
	      else if(_bboxgp[N].is_valid() && contain_separator(N))
	      {
		  draw::box(ima_color, _bboxgp[N], mln::literal::yellow);
	      }
	    }
	    
	     io::ppm::save(mln::debug::superpose(ima_color, source, literal::white) , file);
	  }
	  
	  
	  void debug_save_all(std::string file)
	  {debug_save_all(file, debug_source);}
	  void debug_save_all(std::string file, image2d<bool> source)
	  {
	    image2d<value::rgb8> ima_color;
	    mln::initialize(ima_color,img_influ);
	    
	    	    for(unsigned int N = 0; N < _bboxgp.size(); N++)
	    {
	      if(_bboxgp[N].is_valid() && contain_letter(N))
	      {
		if(temp_letter(N))
		  draw::box(ima_color, _bboxgp[N], mln::literal::teal);
		else
		  draw::box(ima_color, _bboxgp[N], mln::literal::cyan);
	      }
	      else if(_bboxgp[N].is_valid() && contain_separator(N))
	      {
		draw::box(ima_color, _bboxgp[N], mln::literal::green);
		draw::box(ima_color, _bboxgp_influ[N], mln::literal::green);
	      }
	    }
	    
	    for(unsigned int N = 0; N < lines_first_label.size(); N++)
	    {
	       if(_bboxgp[lines_first_label[N]].is_valid())
	      {
		draw::box(ima_color, _bboxgp[lines_first_label[N]], mln::literal::yellow);
	      }
	    }
	    
	    for(unsigned int N = 0; N < lines_last_label.size(); N++)
	    {
	       if(_bboxgp[lines_first_label[N]].is_valid())
	      {
		draw::box(ima_color, _bboxgp[lines_last_label[N]], mln::literal::orange);
	      }
	    }
	    
	    
	    for(unsigned int N = 0; N < lines_bbox.size(); N++)
	    {
	      if(lines_bbox[N].is_valid())
	      {
		draw::box(ima_color, lines_bbox[N], mln::literal::blue);
	      }
	    }
	    for(unsigned int N = 0; N < lines_influ_bbox.size(); N++)
	    {
	      if(lines_influ_bbox[N].is_valid())
	      {
		//draw::box(ima_color, lines_influ_bbox[N], mln::literal::cyan);
	      }
	    }
	    for(unsigned int N = 0; N < paragraphs_bbox.size(); N++)
	    {
	      if(paragraphs_bbox[N].is_valid())
	      {
		draw::box(ima_color, paragraphs_bbox[N], mln::literal::red);
		if(paragraphs_bbox_influ[N].is_valid())
		{
		  draw::box(ima_color, paragraphs_bbox_influ[N], mln::literal::orange);
		}
	      }

	    }
	     for(unsigned int N = 0; N < _bboxgp.size(); N++)
	    {
	      if(_bboxgp[N].is_valid() && (implicit_separators_left_mask(N) || implicit_separators_right_mask(N)))
	      {
	//	draw::box(ima_color, _bboxgp[N], mln::literal::yellow);
	      }
	    }

	    io::ppm::save(mln::debug::superpose(ima_color, source, literal::white) , file);
	  }
	  void debug_save_paragraphs(std::string file)
	  {  mymln::debug::save_label_image(img, paragraphs_union , file);}
	  void debug_save_separators(std::string file)
	  {  mymln::debug::save_label_image(img, implicit_separators_union , file);}
	  vertex_image<point2d,bool> fun_mask_separators()
	  { return fun_mask_(separators_mask); }
	  vertex_image<point2d,bool> fun_mask_V_separators()
	  { return fun_mask_(Vseparator_mask); }
	  vertex_image<point2d,bool> fun_mask_containers()
	  { return fun_mask_(containers_mask); }
	  vertex_image<point2d,bool> fun_mask_alone_letters()
	  { return fun_mask_(alone_letters_mask); }
	   vertex_image<point2d,bool> fun_mask_implicit_separators_left()
	  { return fun_mask_(implicit_separators_left_mask); }
	  vertex_image<point2d,bool> fun_mask_implicit_separators_right()
	  { return fun_mask_(implicit_separators_right_mask); }
	  vertex_image<point2d,bool> fun_mask_all_letters()
	  {return fun_mask_(all_letters_mask);}
	  vertex_image<point2d,bool> fun_mask_all()
	  {  
	    return fun_mask_(all_mask);
	  }
	  vertex_image<point2d,bool> fun_mask_image()
	  { return fun_mask_(image_mask); }
	  vertex_image<point2d,bool> fun_mask_letters()
	  { return fun_mask_(letters_mask); }
	  vertex_image<point2d,bool> fun_mask_anomalies()
	  { return fun_mask_(anomaly_mask); }
	   vertex_image<point2d,bool> fun_mask_start_lines()
	  { return fun_mask_(start_lines_mask); }
	  vertex_image<point2d,bool> fun_mask_end_lines()
	  { return fun_mask_(end_lines_mask); }
	  vertex_image<point2d,bool> fun_mask_start_end_lines()
	  { return fun_mask_(start_end_lines_mask); }
	  
	  image2d<bool> image_mask_containers()
	  { return image_mask_(containers_mask); }
	  image2d<bool> image_mask_separators()
	  { return image_mask_(separators_mask); }
	  image2d<bool> image_mask_implicit_separators_left()
	  { return image_mask_(implicit_separators_left_mask); }
	  image2d<bool> image_mask_implicit_separators_right()
	  { return image_mask_(implicit_separators_right_mask); }
	  image2d<bool> image_mask_letters()
	  { return image_mask_(letters_mask); }
	  image2d<bool> image_mask_noise()
	  { return image_mask_(noise_mask); }
	  image2d<bool> image_mask_alone_letters()
	  { return image_mask_(alone_letters_mask); }
	  image2d<bool> image_mask_start_lines()
	  { return image_mask_(start_lines_mask); }
	  image2d<bool> image_mask_end_lines()
	  { return image_mask_(end_lines_mask); }
	  
	  mln::util::array<box2d> bbox_mask_lines()
	  { return lines_bbox; }	  
	  mln::util::array<box2d> bbox_mask_containers()
	  { return bbox_mask_(containers_mask); }
	  mln::util::array<box2d> bbox_mask_separators()
	  { return bbox_mask_(separators_mask); }
	  mln::util::array<box2d> bbox_mask_letters()
	  { return bbox_mask_(letters_mask); }
	  mln::util::array<box2d> bbox_mask_noise()
	  { return bbox_mask_(noise_mask); }
	  
	  mln::util::array<box2d> bbox_enlarge_mask_containers(short int x, short int y)
	  { return bbox_mask_enlarge_(containers_mask, x, y); }
	  mln::util::array<box2d> bbox_enlarge_mask_separators(short int x, short int y)
	  { return bbox_mask_enlarge_(separators_mask, x, y); }
	  mln::util::array<box2d> bbox_enlarge_mask_letters(short int x, short int y)
	  { return bbox_mask_enlarge_(letters_mask, x, y); }
	  mln::util::array<box2d> bbox_enlarge_mask_noise(short int x, short int y)
	  { return bbox_mask_enlarge_(noise_mask, x, y); }
	  
	  Label get_label(const point2d& point)
	  { return img_influ(point); }

	  inline box2d get_paragraph_bbox(const point2d& point)
	  { return get_paragraph_bbox(img_influ(point)); }
	  
	  inline box2d get_paragraph_bbox(Label L)
	  { return paragraphs_bbox[paragraphs_union[L]]; }
	  
	  /// USE THIS METHOD ONLY IF YOU KNOW THE PARAGRAPH ID
	  inline box2d get_paragraph_bbox_direct(unsigned int ID)
	  { return paragraphs_bbox[ID]; }


	  inline box2d get_line_bbox(const point2d& point)
	  { return get_line_bbox(img_influ(point)); }
	  
	  inline box2d get_line_bbox(Label L)
	  { return lines_bbox[lines_union[L]]; }

	  inline unsigned int get_paragraph_length(const point2d& point)
	  { return get_paragraph_length(img_influ(point)); }
	  
	  inline unsigned int get_paragraph_length(Label L)
	  { return paragraphs_len[paragraphs_union[L]]; }
	  
	  /// USE THIS METHOD ONLY IF YOU KNOW THE PARAGRAPH ID
	  inline unsigned int get_paragraph_length_direct(unsigned int ID)
	  { return paragraphs_len[ID]; }

	  inline unsigned int get_line_length(const point2d& point)
	  { return get_line_length(img_influ(point)); }
	  
	  inline unsigned int get_line_length(Label L)
	  { return lines_len[lines_union[L]]; }

	  /// USE THIS METHOD ONLY IF YOU KNOW THE LINE ID
	  inline unsigned int get_line_length_direct(unsigned int ID)
	  { return lines_len[ID]; }

	  inline unsigned int get_line_width(point2d point)
	  { return get_line_width(img_influ(point)); }
	  
	  inline unsigned int get_line_width(Label L)
	  { return lines_bbox[lines_union[L]].len(1); }
	  

	  inline Float letter_ratio_YX(const point2d& point)
	  {return letter_ratio_YX(img_influ(point));}
	  inline Float letter_ratio_YX(Label Letter)
	  {
	    return (Float)_bboxgp[Letter].len(0) / (Float)_bboxgp[Letter].len(1);
	  }
	  inline Float letter_ratio_XY(const point2d& point)
	  {return letter_ratio_XY(img_influ(point));}
	  inline Float letter_ratio_XY(Label Letter)
	  {
	    return (Float)_bboxgp[Letter].len(1) / (Float)_bboxgp[Letter].len(0);
	  }
	  inline bool line_median(const point2d& point)
	  { return line_median(img_influ(point)); }
	 
	  inline bool line_median(Label Letter)
	  { 
	    short int D = _bboxgp[Letter].pcenter()[0] - get_line_bbox(Letter).pcenter()[0];
	    if(D<0)D=-D;
	    return D * 3 < get_line_bbox(Letter).len(0);
	  }
	  
	  
	  
	  
	  
	  inline bool line_size_small(const point2d& point)
	  { return line_size_small(img_influ(point)); }
	   inline bool line_size_small(Label Letter)
	  { 
	    return _bboxgp[Letter].len(0) * 3 < get_line_bbox(Letter).len(0);
	  }

	   inline bool line_base(const point2d& point)
	  { return line_base(img_influ(point)); }
	  inline bool line_base(Label Letter)
	  { 
	    short int D = _bboxgp[Letter].pcenter()[0] - get_line_bbox(Letter).pcenter()[0];
	    if(D<0)D=-D;
	    return 
	    D * 2 < get_line_bbox(Letter).len(0) && 
	    get_line_bbox(Letter).pcenter()[0] + (get_line_bbox(Letter).len(0) / 5) < _bboxgp[Letter].pcenter()[0];
	  }

	  inline bool letter_included(point2d Par1, point2d Par2)
	  { return letter_included(img_influ(Par1), img_influ(Par2)); }
	  inline bool letter_included(Label Par1, Label Par2)
	  { 
	    return 
	      _bboxgp[Par1].has(_bboxgp[Par2].pmin()) &&
	      _bboxgp[Par1].has(_bboxgp[Par2].pmax()) ;
	  }

	  inline bool letter_included_center(point2d Par1, point2d Par2)
	  { return letter_included_center(img_influ(Par1), img_influ(Par2)); }
	  inline bool letter_included_center(Label Par1, Label Par2)
	  { 
	    return 
	      _bboxgp[Par1].has(_bboxgp[Par2].pcenter());
	  }


	  inline bool paragraph_included_influence(point2d Par1, point2d Par2)
	  { return paragraph_included_influence(img_influ(Par1), img_influ(Par2)); }
	  inline bool paragraph_included_influence(Label Par1, Label Par2)
	  { 
	    return 
	      paragraphs_bbox_influ[paragraphs_union[Par1]].has(paragraphs_bbox[paragraphs_union[Par2]].pmin()) &&
	      paragraphs_bbox_influ[paragraphs_union[Par1]].has(paragraphs_bbox[paragraphs_union[Par2]].pmax()) ;
	  }

	  inline bool paragraph_included_influence_line(point2d Par1, point2d Line2)
	  { return paragraph_included_influence_line(img_influ(Par1), img_influ(Line2)); }
	  inline bool paragraph_included_influence_line(Label Par1, Label Line2)
	  { 
	    return 
	      paragraphs_bbox_influ[paragraphs_union[Par1]].has(lines_bbox[lines_union[Line2]].pmin()) &&
	      paragraphs_bbox_influ[paragraphs_union[Par1]].has(lines_bbox[lines_union[Line2]].pmax()) ;
	  }


	  inline bool paragraph_included(point2d Par1, point2d Par2)
	  { return paragraph_included(img_influ(Par1), img_influ(Par2)); }
	  inline bool paragraph_included(Label Par1, Label Par2)
	  { 
	    return 
	      paragraphs_bbox[paragraphs_union[Par1]].has(paragraphs_bbox[paragraphs_union[Par2]].pmin()) &&
	      paragraphs_bbox[paragraphs_union[Par1]].has(paragraphs_bbox[paragraphs_union[Par2]].pmax()) ;
	  }
	  
	  inline bool line_reciprocal(const point2d& L1, const point2d& L2)
	  {return line_reciprocal(img_influ(L1), img_influ(L2));}
	  
	  inline bool line_reciprocal(Label L1, Label L2)
	  {
	    return 
	      lines_bbox[lines_union[L1]].has(lines_bbox[lines_union[L2]].pmin()) ||
	      lines_bbox[lines_union[L1]].has(lines_bbox[lines_union[L2]].pmax()) ||
	      lines_bbox[lines_union[L2]].has(lines_bbox[lines_union[L1]].pmin()) ||
	      lines_bbox[lines_union[L2]].has(lines_bbox[lines_union[L1]].pmax()) ;
	  }
	  
	  inline bool line_influence_reciprocal(const point2d& L1, const point2d& L2)
	  {return line_influence_reciprocal(img_influ(L1), img_influ(L2));}
	  
	  inline bool line_influence_reciprocal(Label L1, Label L2)
	  {
	    return 
	      lines_influ_bbox[lines_union[L1]].has(lines_influ_bbox[lines_union[L2]].pmin()) ||
	      lines_influ_bbox[lines_union[L1]].has(lines_influ_bbox[lines_union[L2]].pmax()) ||
	      lines_influ_bbox[lines_union[L2]].has(lines_influ_bbox[lines_union[L1]].pmin()) ||
	      lines_influ_bbox[lines_union[L2]].has(lines_influ_bbox[lines_union[L1]].pmax()) ;
	  }
	  
	  
	   inline bool paragraph_influence_reciprocal(const point2d& L1, const point2d& L2)
	  {return paragraph_influence_reciprocal(img_influ(L1), img_influ(L2));}
	  
	  inline bool paragraph_influence_reciprocal(Label L1, Label L2)
	  {
	    return 
	      paragraphs_bbox_influ[paragraphs_union[L1]].has(paragraphs_bbox_influ[paragraphs_union[L2]].pmin()) ||
	      paragraphs_bbox_influ[paragraphs_union[L1]].has(paragraphs_bbox_influ[paragraphs_union[L2]].pmax()) ||
	      paragraphs_bbox_influ[paragraphs_union[L2]].has(paragraphs_bbox_influ[paragraphs_union[L1]].pmin()) ||
	      paragraphs_bbox_influ[paragraphs_union[L2]].has(paragraphs_bbox_influ[paragraphs_union[L1]].pmax()) ;
	  }
	  
	  
	  
	  inline bool allign_size_x_height( const point2d& Left, const point2d& Right)
	  {return allign_size_x_height(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_size_x_height( const Label Left, const Label Right)
	  {
	     short int SizeL0 = label_size_(0, Left);
	      short int SizeR0 = label_size_(0, Right);
	      short int SizeL1 = label_size_(1, Left);
	      short int SizeR1 = label_size_(1, Right);
	      short int Swap = 0;
	      if(SizeL0 < SizeL1)
	      { SizeL0 = SizeL1; }
	      if(SizeR0 < SizeR1){SizeR0 = SizeR1;}
	    return  SizeR0 > (SizeL0 / 3) && SizeR0 < (SizeL0);
	  }
	  
	  inline bool allign_size_large_inside( const point2d& Left, const point2d& Right)
	  {return allign_size_large_inside(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_size_large_inside( const Label Left, const Label Right)
	  {
	     short int SizeL0 = label_size_(0, Left);
	      short int SizeR0 = label_size_(0, Right);
	      short int SizeL1 = label_size_(1, Left);
	      short int SizeR1 = label_size_(1, Right);
	      short int Swap = 0;
	      if(SizeL0 < SizeL1)
	      { SizeL0 = SizeL1; }
	      if(SizeR0 < SizeR1){SizeR0 = SizeR1;}
	    return  SizeR0 > (SizeL0 / 5) && SizeR0 < (SizeL0);
	  }
	  inline bool paragraph_has(point2d Par, point2d Point)
	  { return paragraph_has(img_influ(Par), Point); }

	  inline bool paragraph_has(Label Par, point2d Point)
	  { return paragraph_has[paragraphs_union[Par]].has(Point); }

	  inline bool line_has(point2d Line, point2d Point)
	  { return line_has(img_influ(Line), Point); }

	  inline bool line_has(Label Line, point2d Point)
	  { return lines_bbox[lines_union[Line]].has(Point); }
	  
	  inline bool line_influence_has(point2d Line, point2d Point)
	  { return line_influence_has(img_influ(Line), Point); }

	  inline bool line_influence_has(Label Line, point2d Point)
	  { return lines_influ_bbox[lines_union[Line]].has(Point); }
	  
	  
	  inline unsigned int get_beginning_of_line(point2d point)
	  { return get_beginning_of_line(img_influ(point)); }
	  
	  inline unsigned int get_beginning_of_line(Label L)
	  { return lines_first_label[lines_union[L]]; }
	  
	  inline unsigned int get_end_of_line(point2d point)
	  { return get_end_of_line(img_influ(point)); }
	  
	  inline unsigned int get_end_of_line(Label L)
	  { return lines_last_label[lines_union[L]]; }
	  
	  
	  inline unsigned int get_parent_line(point2d point)
	  { return lines_union[img_influ(point)]; }
	  
	  
	  inline unsigned int get_parent_line(Label L)
	  { return lines_union[L]; }
	  
	  
	  inline void recook_lines()
	  {
	    lines_first_label.fill(0);
	    lines_last_label.fill(0);
	    lines_len.fill(0);
	    
	    start_lines_mask(0) = false;
	    end_lines_mask(0) = false;
	    
	    cook_lines_();
	  }
	  inline void reset_implicit_separators()
	  { implicit_separators_union.reset(); lines_split.fill(0);}
	  inline void cook_lines()
	  {
	    lines_len = mln::util::array<unsigned int>(NLine + 1);
	    lines_first_label = mln::util::array<unsigned int>(NLine + 1);
	    lines_last_label = mln::util::array<unsigned int>(NLine + 1);
	    start_lines_mask = fun::i2v::array<bool>(Areas_Number_);
	    end_lines_mask = fun::i2v::array<bool>(Areas_Number_);
	    start_end_lines_mask = fun::i2v::array<bool>(Areas_Number_);
	    lines_bbox = mln::util::array<box2d>(NLine + 1);
	    lines_influ_bbox = mln::util::array<box2d>(NLine + 1);
	    lines_len.fill(0);
	    start_lines_mask(0) = false;
	    end_lines_mask(0) = false;
	    cook_lines_();
	  }
	  
	  
	  inline void cook_separators()
	  {
	    separators_len_left = mln::util::array<unsigned int>(NImpSep + 1);
	    separators_middle = mln::util::array<unsigned int>(NImpSep + 1);
	    separators_len_left.fill(0);
	    separators_middle.fill(0);
	    cook_separators_();
	  }
	  
	  inline void cook_separators_right()
	  {
	    separators_len_right = mln::util::array<unsigned int>(NImpSep + 1);
	    separators_middle.resize(NImpSep + 1);
	    separators_len_right.fill(0);
	     separators_middle.fill(0);
	    cook_separators_right_();
	  }

	  inline void propage_line_link()
	  { lines_union.propage_links(); }
	  /*image_if<image2d<Label> masked_image_letters()
	  {return masked_image_(letters_mask); }
	  image_if<image2d<Label> masked_image_separator()
	  {return masked_image_(letters_mask); }*/	 
	  
	 /* IMPLICIT SEPARATORS */ 
	inline void add_new_separator(const point2d& point)
	{ add_new_separator(img_influ(point));}
	inline void add_new_separator(const Label lbl)
	{ CImpSep = NImpSep; NImpSep++; }
	
	inline void add_to_separator_self_link(const point2d& point)
	{ add_to_separator_self_link(img_influ(point));}
	inline void add_to_separator_self_link(const Label A)
	{implicit_separators_union.add_self_link(A);}
	
	inline void add_to_separator_link(const point2d& A, const point2d& B)
	{ add_to_separator_link(img_influ(A), img_influ(B));}
	inline void add_to_separator_link(const Label A, const Label B)
	{implicit_separators_union.add_link(A, B);}
	
	inline bool same_implicit_separator(const point2d& A, const point2d& B)
	{return same_implicit_separator(img_influ(A), img_influ(B));}
	inline bool same_implicit_separator(const Label A, const Label B)
	{return implicit_separators_union[A] == implicit_separators_union[B];}
	
	inline void propage_separator_link()
	{ implicit_separators_union.propage_links(); }
	
	inline void jump_to_separator(const point2d& point)
	{ jump_to_separator(img_influ(point)); }
	inline void jump_to_separator(const Label lbl)
	{ 
	  if(implicit_separators_union[lbl] != 0)
	    CImpSep = implicit_separators_union[lbl];
	  else
	    add_new_separator(lbl);
	}
	inline bool contain_implicit_separator(const point2d& point)
	{ return contain_implicit_separator(img_influ(point)); }
	inline bool contain_noise(const point2d& point)
	{ return contain_noise(img_influ(point)); }
	inline bool contain_implicit_separator(const Label lbl)
	{return implicit_separators_union[lbl] != 0; }
	inline bool contain_noise(const Label lbl)
	{return noise_mask(lbl); }
	
	inline void merge_separators(const point2d& A, const point2d& B)
	{
	  merge_separators(img_influ(A), img_influ(B));
	}
	inline void merge_separators(const Label A, const Label B)
	{
	  
	  if( A && B && !kill_mask(A) && !kill_mask(B) && A != B)
	  {
	    img_influ(_bboxgp[B].pcenter()) = A;
	    _bboxgp[A].merge(_bboxgp[B]);

	    _bboxgp[B] = box2d();

	    _bboxgp_influ[A].merge(_bboxgp_influ[B]);
	    _bboxgp_influ[B] = box2d();
	    separators_mask(B) = false;
	    separators_mask(A) = true;
	    Vseparator_mask(B) = false;
	    Hseparator_mask(B) = false;
	    kill_mask(B) = true;
	    all_mask(B) = false;
	  }
	  
	  
	}
	
	inline void merge(const point2d& A, const point2d& B)
	{
	  merge(img_influ(A), img_influ(B));
	}
	inline void merge(const Label A, const Label B)
	{
	  if( A && B && !kill_mask(A) && !kill_mask(B) && A != B)
	  {
	    img_influ(_bboxgp[B].pcenter()) = A;
	    _bboxgp[A].merge(_bboxgp[B]);
	    _bboxgp[B] = box2d();
	    kill_mask(B) = true;
	    all_mask(B) = false;
	    if(letters_mask(A) && letters_mask(B))
	    {
	      if(lines_union.is_self_link(B))
	      {
		lines_union.add_self_link(A);
		lines_union.add_link(A, B);
	      }
	    }
	    else if(alone_letters_mask(A) && letters_mask(B))
	    {
	      alone_letters_mask(A) = false;
	      letters_mask(A) = true;
	      all_letters_mask(A) = true;
	      if(lines_union.is_self_link(B))
	      {
		lines_union.add_self_link(A);
		lines_union.add_link(A, B);
	      }
	    }
	    else if(letters_mask(B))
	    {
	      add_letter_coerce(A);
	      lines_union.add_link(B, A);
	    }
	    

	    
	    
	    implicit_separators_left_mask(B) = false;
	    implicit_separators_right_mask(B) = false;
	    noise_mask(B) = false;
	    alone_letters_mask(B) = false;
	    all_letters_mask(B) = false;
	    letters_mask(B) = false;
	    separators_mask(B) = false;
	    containers_mask(B) = false;
	    start_end_lines_mask(B) = false;
	    Hseparator_mask(B) = false;
	    Vseparator_mask(B) = false;
	    if(letters_mask(A) && start_lines_mask(B)){start_lines_mask(A) = true;}
	    if(letters_mask(A) && end_lines_mask(B)){end_lines_mask(A) = true;}
	    if(letters_mask(A) && start_end_lines_mask(B)){start_end_lines_mask(A) = true;}
	    
	    
	    start_lines_mask(B) = false;
	    end_lines_mask(B) = false;
	    start_end_lines_mask(B) = false;
	    
	  }
	}

	
	inline void add_to_separator_left(const point2d& point)
	{ add_to_separator_left(img_influ(point));  }
	inline void add_to_separator_left(const Label lbl)
	{ implicit_separators_union[lbl] = CImpSep; implicit_separators_left_mask(lbl) = true; }
	
	inline void add_to_separator_right(const point2d& point)
	{ add_to_separator_right(img_influ(point));  }
	inline void add_to_separator_right(const Label lbl)
	{ implicit_separators_union[lbl] = CImpSep; implicit_separators_right_mask(lbl) = true; }
	
	inline void invalidate_implicit_separator(const point2d& point)
	{ invalidate_implicit_separator(img_influ(point));  }
	inline void invalidate_implicit_separator(Label lbl)
	{
	  implicit_separators_union[lbl] = 0;
	  implicit_separators_left_mask(lbl) = false;
	  implicit_separators_right_mask(lbl) = false;
	}

	inline const Label& operator[](point2d i)
	{ return img_influ(i);}
	
	inline point2d operator[](Label i)
	{ 
	  point2d p = _bboxgp[i].pcenter();
	  return p;
	}
	inline void reset_tag_bool()
	{Btag_lbl.fill(false);}
	inline void tag_label_bool(const point2d& point, bool tag)
	{ tag_label_bool(img_influ(point), tag);}
	inline void tag_label_bool(Label lbl, bool tag)
	{Btag_lbl[lbl] = tag;}
	
	inline bool get_tag_bool(const point2d& point)
	{ return get_tag_bool(img_influ(point));}
	inline bool get_tag_bool(Label lbl)
	{return Btag_lbl[lbl];}
	
	inline std::string get_tag(const point2d& point)
	{ return get_tag(img_influ(point));}
	inline std::string get_tag(Label lbl)
	{return tag_lbl[lbl];}
	
	inline void tag_label(const point2d& point, std::string tag)
	{ tag_label(img_influ(point), tag);}
	inline void tag_label(Label lbl, std::string tag)
	{tag_lbl[lbl] = tag;}
	/* ITER ON LINES */
	inline void cook_lines_iter()
	{cook_lines_iter_();}
	
	inline unsigned int lines_iter_value()
	{return lines_seq[SeqP]; }
	inline void lines_iter_start()
	{ SeqP = 0; }
	inline void lines_iter_next_line()
	{ SeqP = lines_seq_pos[get_beginning_of_line(SeqP) + 1]; }
	inline void lines_iter_next_letter()
	{ SeqP++; while(lines_iter_valid() && !lines_seq[SeqP]){SeqP++;} }
	inline void lines_iter_valid()
	{ return SeqP < Areas_Number_; }
	
	inline void recook_paragraphs()
	{
	  paragraphs_len.fill(0);
	  paragraphs_letter_len.fill(0);
	  cook_paragraphs_();
	}
	inline void cook_paragraphs()
	{
	  paragraphs_bbox = mln::util::array<box2d>(NPar + 1);
	  paragraphs_len = mln::util::array<unsigned int>(NPar + 1);
	  paragraphs_letter_len = mln::util::array<unsigned int>(NPar + 1);
	  
	  paragraphs_first_line = mln::util::array<unsigned int>(NPar + 1);
	  paragraphs_bbox_influ = mln::util::array<box2d>(NPar + 1);
	  paragraphs_len.fill(0);
	   paragraphs_letter_len.fill(0);
	  cook_paragraphs_();
	}
	
	inline void compute_letter_middle_space()
	{
	  lines_space = mln::util::array<unsigned int>(NLine + 1);
	  lines_space.fill(0);
	  compute_letter_middle_space_();
	}
	inline void compute_letter_middle_height()
	{
	  lines_height = mln::util::array<unsigned int>(NLine + 1);
	  lines_height.fill(0);
	  compute_letter_middle_height_();
	}
	inline void compute_letter_middle_width()
	{
	  lines_width = mln::util::array<unsigned int>(NLine + 1);
	  lines_width.fill(0);
	  compute_letter_middle_width_();
	}
	inline void compute_paragraph_middle_height()
	{
	  paragraphs_mid_height = mln::util::array<unsigned int>(NPar + 1);
	  paragraphs_mid_width.fill(0);
	  compute_paragraph_middle_height_();
	}
	inline void compute_paragraph_middle_width()
	{
	  paragraphs_mid_width = mln::util::array<unsigned int>(NPar + 1);
	  paragraphs_mid_width.fill(0);
	  compute_paragraph_middle_width_();
	}
	inline void recompute_letter_middle_space()
	{
	  lines_space.fill(0);
	  compute_letter_middle_height_();
	}
	inline void recompute_letter_middle_height()
	{
	  lines_height.fill(0);
	  compute_letter_middle_height_();
	}
	inline void recompute_letter_middle_width()
	{
	  lines_width.fill(0);
	  compute_letter_middle_height_();
	}
	
	inline unsigned int get_letter_middle_space(const point2d& point)
	{return get_letter_middle_space(img_influ(point));}
	inline unsigned int get_letter_middle_space(const Label lbl)
	{return lines_space[lines_union[lbl]];}
	
	
	inline unsigned int get_letter_middle_height(const point2d& point)
	{return get_letter_middle_height(img_influ(point));}
	inline unsigned int get_letter_middle_height(const Label lbl)
	{return lines_height[lines_union[lbl]];}
	
	inline unsigned int get_letter_middle_width(const point2d& point)
	{return get_letter_middle_width(img_influ(point));}
	inline unsigned int get_letter_middle_width(const Label lbl)
	{return lines_width[lines_union[lbl]];}


	inline bool compatible_paragraph_middle_width(const point2d& A, const point2d& B)
	{ return compatible_paragraph_middle_width(img_influ(A), img_influ(B));}
	inline bool compatible_paragraph_middle_width(const Label A, const Label B)
	{
	  short int LA = lines_bbox[paragraphs_first_line[paragraphs_union[A]]].len(0);
	  short int LB = lines_bbox[paragraphs_first_line[paragraphs_union[A]]].len(0);
	  if(LA > LB)
	  {LA = LB;}
	  return get_paragraph_middle_width_diff(A, B) * 8 < LA;
	}
	inline int get_paragraph_middle_width_diff(const point2d& A, const point2d& B)
	{return get_paragraph_middle_width_diff(img_influ(A), img_influ(B));}
	inline int get_paragraph_middle_width_diff(const Label A, const Label B)
	{
	  int diff = paragraphs_mid_width[paragraphs_union[A]] - paragraphs_mid_width[paragraphs_union[B]];
	  if(diff < 0){diff = -diff;}
	  return diff;
	}

	inline unsigned int get_paragraph_middle_width(const point2d& point)
	{return get_letter_paragraph_width(img_influ(point));}
	inline unsigned int get_paragraph_middle_width(const Label lbl)
	{return paragraphs_mid_width[paragraphs_union[lbl]];}
	
	/// USE THIS METHOD ONLY IF YOU KNOW THE LINE ID
	inline unsigned int get_paragraph_middle_width_direct(const unsigned int ID)
	{return paragraphs_mid_width[ID];}	
	
	inline unsigned int get_line_ID(const point2d& lbl)
	{return get_line_ID(img_influ(lbl));}	
	inline unsigned int get_line_ID(const Label lbl)
	{
	  return lines_union[lbl];
	}
	inline unsigned int get_first_line_ID(const Label lbl)
	{
	  return paragraphs_first_line[paragraphs_union[lbl]];
	}
	inline unsigned int get_first_line()
	{
	  return first_line;
	}
	inline unsigned int get_first_letter(const unsigned int line_ID)
	{
	  return lines_first_label[line_ID];
	}
	inline void get_next_line(int& line_ID)
	{
	  if(lines_seq_pos[line_ID] == line_ID){ line_ID = 0; }
	  line_ID = lines_seq_pos[line_ID];
	}
	inline void get_next_line(unsigned int& line_ID)
	{
	  if(lines_seq_pos[line_ID] == line_ID){ line_ID = 0; }
	  line_ID = lines_seq_pos[line_ID];
	}
	inline void get_next_line(Label& line_ID)
	{
	  if(lines_seq_pos[line_ID] == line_ID){ line_ID = 0; }
	  line_ID = lines_seq_pos[line_ID];
	}
	inline unsigned int return_next_line(const Label& line_ID)
	{
	  if(lines_seq_pos[line_ID] == line_ID){ return 0; }
	  return lines_seq_pos[line_ID];
	}
	inline void get_previous_line(Label& line_ID)
	{
	  if(lines_seq_pos_reverse[line_ID] == line_ID){ line_ID = 0; }
	  line_ID = lines_seq_pos_reverse[line_ID];
	}
	inline unsigned int return_previous_line(const Label& line_ID)
	{
	  if(lines_seq_pos_reverse[line_ID] == line_ID){ return 0; }
	  return lines_seq_pos_reverse[line_ID];
	}
	inline void get_next_letter(Label& lbl)
	{
	  if(lines_seq[lbl] == lbl){ lbl = 0; }
	  lbl = lines_seq[lbl];
	}
	inline void get_next_letter(int& lbl)
	{
	  if(lines_seq[lbl] == lbl){ lbl = 0; }
	  lbl = lines_seq[lbl];
	}
	inline void get_next_letter(unsigned int& lbl)
	{
	  if(lines_seq[lbl] == lbl){ lbl = 0; }
	  lbl = lines_seq[lbl];
	}
	inline std::string get_line_string(const unsigned int ID)
	{
	  std::string line = "";
	  unsigned int Last = 0;
	 for(int N = get_first_letter(ID); N != 0; get_next_letter(N))
	 {
	   if(Last)
	    if(space(Last,N) > get_letter_middle_space(N) * 2)
	      line += " ";
	    
	   if(!get_tag(N).compare("")){line += "?";}
	   else{line += get_tag(N);}
	   Last = N;
	 }
	 return line;
	}
	inline void reserve_previous_next_line()
	{
	  lines_seq_pos = mln::util::array<unsigned int>(NLine + 1);
	  lines_seq_pos_reverse = mln::util::array<unsigned int>(NLine + 1);
	}
	inline void add_line_previous(const point2d& A,const point2d& Prev)
	{add_line_previous(img_influ(A),img_influ(Prev)); }
	inline void add_line_next(const point2d& A, const point2d& Next)
	{add_line_next(img_influ(A),img_influ(Next)); }
	inline void add_line_previous(const Label A,const Label Prev)
	{lines_seq_pos_reverse[get_line_ID(A)] = get_line_ID(Prev); }
	inline void add_line_next(const Label A, const Label Next)
	{lines_seq_pos[get_line_ID(A)] = get_line_ID(Next); }
      private:
	bool Enable_Debug_Buffer;
	fun::i2v::array<bool> temp_letter;
	
	fun::i2v::array<bool> implicit_separators_left_mask;
	fun::i2v::array<bool> implicit_separators_right_mask;
	mln::util::array<unsigned int> separators_len_right;
	mln::util::array<unsigned int> separators_len_left;
	mln::util::array<unsigned int> separators_middle;
	mln::util::array<unsigned int> separators_marging;
	
	bool sep_right_cooked;
	bool lines_cooked;
	inline void cook_separators_()
	{
	  implicit_separators_left_mask(0) = false;
	  for(int N = 1; N < implicit_separators_union.size(); N++)
	  {
	      if(implicit_separators_union[N] != 0)
	      {
		 separators_len_left[implicit_separators_union[N]]++;
		 separators_middle[implicit_separators_union[N]] += _bboxgp[N].pmin()[1];
	      }
	  }
	  
	  /* WARNING : This method to compute the middle value is correct */
	  /* and faster than merge the computing of the middle value and the */
	  /* computing of the lenght of the line . However this doesn't works */
	  /* if you are trying to use very big image or if you work with a 16 BITS*/
	  /* processor */
	  for(unsigned int N = 1; N < NImpSep + 1; N++)
	  {
		if(separators_len_left[N] != 0)
		 separators_middle[N] /= separators_len_left[N]; 
	  }

	  
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(separators_len_left[implicit_separators_union[N]] < 3)
	    {
	      separators_len_left[implicit_separators_union[N]] = 0;
	      implicit_separators_union[N] = 0;
	      implicit_separators_left_mask(N) = false;
	    }
	    else if (
	      _bboxgp[N].pmin()[1] < separators_middle[implicit_separators_union[N]] - _bboxgp[N].len(1) * 2 ||
	      _bboxgp[N].pmin()[1] > separators_middle[implicit_separators_union[N]] + _bboxgp[N].len(1) * 2
	      )
	    {
	      /*
	      separators_len_left[implicit_separators_union[N]]--;
	      implicit_separators_union[N] = 0;
	      implicit_separators_left_mask(N) = false;*/
	    }
	  }
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(!start_lines_mask(N) || implicit_separators_union[N] == 0)
	    {
	     if( separators_len_left[implicit_separators_union[N]] > 0)
		separators_len_left[implicit_separators_union[N]]--;
	    }
	  }
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(separators_len_left[implicit_separators_union[N]] < 1)
	    {
	      separators_len_left[implicit_separators_union[N]] = 0;
	      implicit_separators_union[N] = 0;
	      implicit_separators_left_mask(N) = false;
	    }
	  }
	}

	inline void cook_separators_right_()
	{
	  sep_right_cooked = true;
	  implicit_separators_right_mask(0) = false;
	  for(unsigned int N = 1; N < implicit_separators_union.size(); N++)
	  {
	      if(implicit_separators_union[N] != 0)
	      {
		 separators_len_right[implicit_separators_union[N]]++;
		 separators_middle[implicit_separators_union[N]] += _bboxgp[N].pmax()[1];
	      }
	  }
	  
	  /* WARNING : This method to compute the middle value is correct */
	  /* and faster than merge the computing of the middle value and the */
	  /* computing of the lenght of the line . However this doesn't works */
	  /* if you are trying to use very big image or if you work with a 16 BITS*/
	  /* processor */
	  for(unsigned int N = 1; N < NImpSep + 1; N++)
	  {
		if(separators_len_right[N] != 0)
		 separators_middle[N] /= separators_len_right[N]; 
	  }

	  
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(separators_len_right[implicit_separators_union[N]] < 3)
	    {
	      separators_len_right[implicit_separators_union[N]] = 0;
	      implicit_separators_union[N] = 0;
	      implicit_separators_right_mask(N) = false;
	    }
	    else if (
	      _bboxgp[N].pmax()[1] < separators_middle[implicit_separators_union[N]] - 10 ||
	      _bboxgp[N].pmax()[1] > separators_middle[implicit_separators_union[N]] + 10
	      )
	    {
	      
	      /*separators_len_right[implicit_separators_union[N]]--;
	      implicit_separators_union[N] = 0;
	      implicit_separators_right_mask(N) = false;*/
	    }
	  }
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(!end_lines_mask(N) || implicit_separators_union[N] == 0)
	    {
	      if( separators_len_right[implicit_separators_union[N]] > 0)
	      separators_len_right[implicit_separators_union[N]]--;
	    }
	  }
	  for(unsigned int N = 1; N < Areas_Number_; N++)
	  {
	    if(separators_len_right[implicit_separators_union[N]] < 1)
	    {
	      separators_len_right[implicit_separators_union[N]] = 0;
	      implicit_separators_union[N] = 0;
	      implicit_separators_right_mask(N) = false;
	    }
	  }
	}
	
	
	// PRIVATE DATA ON LINES
	mln::util::array<unsigned int> lines_len;
	mln::util::array<unsigned int> lines_height;
	mln::util::array<unsigned int> lines_width;
	mln::util::array<unsigned int> lines_space;
	mln::util::array<unsigned int> lines_first_label;
	mln::util::array<unsigned int> lines_last_label;
	mln::util::array<unsigned int> lines_seq;
	mln::util::array<unsigned int> lines_seq_pos;
	mln::util::array<unsigned int> lines_seq_pos_reverse;
	mln::util::array<box2d> lines_bbox;
	mln::util::array<box2d> lines_influ_bbox;
	mln::util::array<Label> lines_split;
	fun::i2v::array<bool> start_lines_mask;
	fun::i2v::array<bool> end_lines_mask;
	fun::i2v::array<bool> start_end_lines_mask;
	unsigned int first_line;
	unsigned int SeqP;
	  inline void  compute_letter_middle_width_()
	  {
	    for(unsigned int N = 1; N < Areas_Number_; N++)
	    {
	      if(lines_union[N])
	      {
			lines_width[lines_union[N]] += _bboxgp[N].len(1);
	      }
	    }
	    for(unsigned int N = 1; N < lines_height.size(); N++)
	    {
		    if(lines_len[N])
		    	lines_width[N] /= lines_len[N]; 
	    }
	  }
	  
	  inline void compute_letter_middle_height_()
	  {
	    
	    for(unsigned int N = 1; N < Areas_Number_; N++)
	    {
	      if(lines_union[N])
	      {
			lines_height[lines_union[N]] += _bboxgp[N].len(0);
	      }
	    }
	    for(unsigned int N = 1; N < lines_height.size(); N++)
	    {
		    if(lines_len[N])
		    	lines_height[N] /= lines_len[N]; 
	    }
	  }
	  
	  inline void compute_letter_middle_space_()
	  {
	    
	    for(unsigned int N = 1; N < Areas_Number_; N++)
	    {
	      if(lines_union[N])
	      {
			lines_space[lines_union[N]] += _bboxgp[N].len(1);
	      }
	    }
	    for(unsigned int N = 1; N < lines_space.size(); N++)
	    {
		    if(lines_len[N] - 1 > 0)
		    {
		    	
		    	if(lines_space[N] > lines_bbox[N].len(1))
			  lines_space[N] = 0;
			else
			{
			  lines_space[N] = (lines_bbox[N].len(1) - lines_space[N]) / (lines_len[N] - 1);
			}
		    }
		    else
		    {
		      lines_space[N] = 0;
		    }
		    
	
	    }
	  }
	  inline void cook_lines_iter_()
	  {
	    first_line = 0;
	    lines_seq = mln::util::array<unsigned int>(Areas_Number_);
	    lines_seq_pos = mln::util::array<unsigned int>(NLine + 1);
	    
	    lines_seq.fill(0);
	    lines_seq_pos.fill(0);
	    
	    typedef vertex_image<point2d,bool> v_ima_g;
	    typedef p_vertices<mln::util::graph, fun::i2v::array<mln::point2d> > g_vertices_p;
	    v_ima_g mask = fun_mask_letters();
	    mln_piter_(v_ima_g) v(mask.domain());
	    typedef graph_elt_neighborhood_if<mln::util::graph, g_vertices_p, v_ima_g> nbh_t;
	    nbh_t nbh(mask);
	    mln_niter_(nbh_t) q(nbh, v);
	    for_all(v)
	    {
	      if(contain_letter(v))
	      for_all(q)
	      {
		  if(contain_letter(q))
		  if(same_line(q, v))
		  {
		    if(_bboxgp[img_influ(q)].pmax()[1] < _bboxgp[img_influ(v)].pmin()[1])
		    {
		      if(lines_seq[img_influ(q)])
		      {
			if(_bboxgp[lines_seq[img_influ(q)]].pmin()[1] > _bboxgp[img_influ(v)].pmin()[1])
			  lines_seq[img_influ(q)] = img_influ(v);
		      }
		      else
			lines_seq[img_influ(q)] = img_influ(v);
		    }
		  }
		  else
		  {
		    if(get_line_bbox(q).pmax()[0] < get_line_bbox(v).pmin()[0])
		    {
		       if(lines_seq_pos[lines_union[img_influ(q)]])
		       {
			if(lines_bbox[lines_seq_pos[lines_union[img_influ(q)]]].pmin()[0] > get_line_bbox(v).pmin()[0])
			  lines_seq_pos[lines_union[img_influ(q)]] =lines_union[img_influ(v)];
		       }
			else
			  lines_seq_pos[lines_union[img_influ(q)]] = lines_union[img_influ(v)];
		       
		    }
		  }
	      }
	    }
	    std::cout << "end graph cooking";
	    unsigned int Last = 0;
	    int count = 0;
	    for(unsigned int N = 1; N < lines_seq_pos.size() && N < lines_len.size(); N++)
	    {
	      if(lines_len[N] && !first_line)
	      {first_line = N;}
	      if(lines_len[N] && Last)
	      {lines_seq_pos[Last] = N;}
	      if(lines_len[N])
	      {Last = N; std::cout << lines_len[N] << endl; count++;}
	      
	    }
	    std::cout << count << endl;
	     std::cout << "linear";
	  }
	  //NOTE: THIS FUNCTION IS ONE VERY COMMON FUNCTION
	  // PLEASE OPTIMIZE ME
	  inline void cook_lines_()
	  {
	    lines_cooked = true;
	    Cooked_CLine = CLine;
	    for(unsigned int N = 1; N < lines_union.size(); N++)
	    {
	      if(lines_union[N] != 0 && !kill_mask(N))
	      {
		/* APPROXIMATE THE NUMBER OF CHAR IN THE LINE */
		  lines_len[lines_union[N]]++;
		/* COOK THE FIRST AND THE LAST LABEL OF THE LINE */
		if(lines_first_label[lines_union[N]] == 0)
		  lines_first_label[lines_union[N]] = N;
		else if(_bboxgp[N].pmin()[1] < _bboxgp[lines_first_label[lines_union[N]]].pmin()[1])
		    lines_first_label[lines_union[N]] = N;
		
		if(lines_last_label[lines_union[N]] == 0)
		  lines_last_label[lines_union[N]] = N;
		else if(_bboxgp[N].pmax()[1] > _bboxgp[lines_last_label[lines_union[N]]].pmax()[1])
		    lines_last_label[lines_union[N]] = N;
		
		/* FILL THE MASK WITH FALSE:MAYBE USELESS IF THE MASK IS INITIALIZED */
		start_lines_mask(N) = false;
		end_lines_mask(N) = false;
		start_end_lines_mask(N) =false;
	      }
	    }

	    /* SECOND STEP OF THE COOKING */
	    for(unsigned int N = 0; N < lines_first_label.size(); N++)
	    {
	      if( lines_first_label[N] != 0)
	      {
		 lines_bbox[N] = box2d();
		 
		 start_lines_mask(lines_first_label[N]) = true;
		 end_lines_mask(lines_last_label[N]) = true;
		 start_end_lines_mask(lines_first_label[N]) = true;
		 start_end_lines_mask(lines_last_label[N]) = true;
	      }
	      else
	      {
		// USEFULL ONLY FOR DEBUG WHEN WE NEED TO DRAW ALL THE BOUNDING BOX
		// NOTE:REMOVE IT FOR THE FINAL RELEASE 
		lines_bbox[N] = box2d();
	      }
	    }
 
	    /* THE LAST STEP OF COOKING */
	    for(unsigned int N = 1; N < lines_union.size(); N++)
	    {
	      if(lines_union[N] && lines_first_label[lines_union[N]])
	      {
		lines_bbox[lines_union[N]].merge(_bboxgp[N]);
	      }
	      if(lines_len[lines_union[N]] == 1)
	      { 
		letters_mask(N) = false;
		alone_letters_mask(N) = true; 
		all_letters_mask(N) = true;
		end_lines_mask(N) = true;
		start_lines_mask(N)= true;
		start_end_lines_mask(N) = true;
	      } 
	      else if(lines_union[N])
	      { 
		letters_mask(N) = true;
		alone_letters_mask(N) = false;
		all_letters_mask(N) = true;
	      }
	    }
	    for(unsigned int N = 1; N < lines_bbox.size(); N++)
	    {
	      lines_influ_bbox[N] = lines_bbox[N].to_larger(lines_bbox[N].len(0) / 3);
	    }
	  }
	
	
	
	// GENERIC CONTAIN TEST
	inline bool contain_(const Label& lbl, const fun::i2v::array<bool>& array)
	{
	 return array(lbl);
	}
	// VECTOR MUST BE VECTOR 2 OTHERWISE THE RATION WILL BE  Z / X
	// THIS CAN BE A PROBLEM FOR GENERICITY
	inline Float label_ratio_(Label label)
	{
	  return ((Float)_bboxgp[label].len(0)) / ((Float)_bboxgp[label].len(1));
	}
	inline short int label_allign_min_(const unsigned int N, const Label l1, const Label l2)
	{
	   short int AFactor = _bboxgp[l1].pmin()[N] - _bboxgp[l2].pmin()[N];
	  return AFactor < 0 ? -AFactor : AFactor;
	}
	inline short int label_allign_max_(const unsigned int N, const Label l1, const Label l2)
	{
	   short int AFactor = _bboxgp[l1].pmax()[N] - _bboxgp[l2].pmax()[N];
	  return AFactor < 0 ? -AFactor : AFactor;
	}
	inline short int label_allign_(const unsigned int N, const Label l1, const Label l2)
	{
	   short int AFactor = _bboxgp[l1].pcenter()[N] - _bboxgp[l2].pcenter()[N];
	  return AFactor < 0 ? -AFactor : AFactor;
	}
	inline Data label_size_(const unsigned int N, const Label label)
	{
	  return _bboxgp[label].len(N);
	}
	inline bool label_valid_size_(Label label, Data Min, Data Max)
	{
	  Data SX = label_size_(0, label);
	  Data SY = label_size_(1, label);
	  return !(SX < Min|| SX > Max || SY < Min || SX > Max);
	}
	inline bool label_valid_size_Min_(Label label, Data Min)
	{
	  Data SX = label_size_(0, label);
	  Data SY = label_size_(1, label);
	  return SX >= Min && SY >= Min ;
	}
	inline bool label_valid_size_Min_Large_(Label label, Data Min)
	{
	  Data SX = label_size_(0, label);
	  Data SY = label_size_(1, label);
	  return SX >= Min && SY >= Min || SX >= Min * 2 || SY >= Min * 2;
	}
	inline bool label_valid_ratio_(Label label, Float Min, Float Max)
	{
	  Float Ratio = label_ratio_(label);
	  return !(Ratio < Min || Ratio > Max);
	}
	
	
	// MASK & ITER
	  inline image_if<image2d<Label>, fun::i2v::array<bool> > masked_image_(const fun::i2v::array<bool>& fun)
	  {
	    return (img | fun).rw();
	  }
	  inline image2d<bool> image_mask_(const fun::i2v::array<bool>& fun)
	  {
	    typedef image2d<Label> I;
	    image2d<bool> mask;
	    mln::initialize(mask, img);
	    mln_piter(I) p(img.domain());
	    p.start();
	    while(p.is_valid())
	    {
	     mask(p) = contain_(img(p), fun);
	      p.next();
	    }
	    return mask;
	  }
	 vertex_image<point2d,bool> fun_mask_(const fun::i2v::array<bool>& mask)
	 {
	    typedef vertex_image<point2d,bool> v_ima_g;
	    v_ima_g result(_area_graph, mask);
	    return result;
	 }
	 mln::util::array<box2d> bbox_mask_(fun::i2v::array<bool>& mask)
	 {
	   mln::util::array<box2d> bbox_mask;
	   bbox_mask.reserve(_bboxgp.size());
	   
	   for(unsigned int u = 0; u < _bboxgp.size(); u++)
	   {
	     if(mask(u))
	       bbox_mask.append(_bboxgp[u]);
	   }
	   return bbox_mask;
	 }
	  mln::util::array<box2d> bbox_mask_enlarge_(fun::i2v::array<bool>& mask, short int x, short int y)
	 {
	   mln::util::array<box2d> bbox_mask;
	   bbox_mask.reserve(_bboxgp.size());	   
	   for(unsigned int u = 0; u < _bboxgp.size(); u++)
	   {
	     if(mask(u))
	     {
	       box2d box = _bboxgp[u];
	       box.enlarge(1, x);box.enlarge(0, y);
	       bbox_mask.append(box);
	     }
	   }
	   return bbox_mask;
	 }

	fun::i2v::array<bool> Hseparator_mask;
	fun::i2v::array<bool> Vseparator_mask;
	fun::i2v::array<bool> separators_mask;
	fun::i2v::array<bool> letters_mask;
	fun::i2v::array<bool> alone_letters_mask;
	fun::i2v::array<bool> all_letters_mask;
	fun::i2v::array<bool> containers_mask;
	fun::i2v::array<bool> noise_mask;
	fun::i2v::array<bool> kill_mask;
	fun::i2v::array<bool> all_mask;
	fun::i2v::array<bool> image_mask; 
	fun::i2v::array<bool> anomaly_mask;
	mln::util::array<std::string> tag_lbl;
	mln::util::array<bool> Btag_lbl;
	
	unsigned int Cooked_CLine;
	unsigned int CLine;
	unsigned int NLine;
	unsigned int CImpSep;
	unsigned int NImpSep;
	
	mymln::util::union_find<Label> lines_union;
	unsigned int CLet ;
	unsigned int CSep ;
	unsigned int CSepH ;
	unsigned int CSepV ;

	
	mymln::util::union_find<Label> paragraphs_union;
	unsigned int CPar ;
	unsigned int NPar ;
	mln::util::array<unsigned int> paragraphs_first_label;
	mln::util::array<unsigned int> paragraphs_last_label;
	mln::util::array<unsigned int> paragraphs_mid_width;
	mln::util::array<unsigned int> paragraphs_mid_height;
	mln::util::array<unsigned int> paragraphs_assoc;
	mln::util::array<unsigned int> paragraphs_len;
	mln::util::array<unsigned int> paragraphs_letter_len;
	/* NOTE THESE ARRAYS MUST BE INITIALIZEDD WITH THE NUMBER OF PARAGRAPH */


	mln::util::array<box2d> paragraphs_bbox;
	mln::util::array<box2d> paragraphs_bbox_influ;
	mln::util::array<unsigned int> paragraphs_first_line;
	
	
	  inline void  compute_paragraph_middle_width_()
	  {
	    for(unsigned int N = 1; N < Areas_Number_; N++)
	    {
	      if(paragraphs_union[N])
	      {
			paragraphs_mid_width[paragraphs_union[N]] += _bboxgp[N].len(1);
	      }
	    }
	    for(unsigned int N = 1; N < paragraphs_mid_width.size() && N < paragraphs_letter_len.size(); N++)
	    {
		    if(paragraphs_letter_len[N])
		    {
		      
		    	paragraphs_mid_width[N] /= paragraphs_letter_len[N];
		    }
	    }
	  }
	  inline void  compute_paragraph_middle_height_()
	  {
	    for(unsigned int N = 1; N < Areas_Number_; N++)
	    {
	      if(paragraphs_union[N])
	      {
			paragraphs_mid_height[paragraphs_union[N]] += _bboxgp[N].len(0);
	      }
	    }
	    for(unsigned int N = 1; N < paragraphs_mid_height.size() && N < paragraphs_letter_len.size(); N++)
	    {
		    if(paragraphs_len[N])
		    	paragraphs_mid_height[N] /= paragraphs_letter_len[N]; 
	    }
	  }
	inline void first_recognition()
	{
	  
	}
	
	
	inline void cook_paragraphs_()
	{
	 /* mln::util::array<unsigned int> paragraphs_assoc(lines_union.size());
	 
	  for(int N = 1; N < paragraphs_union.size(); N++)
	  {
	      if(paragraphs_union[N] && lines_union[N] && !start_lines_mask)
	      {
		if(paragraphs_assoc[lines_union[N]])
		  { paragraphs_union.add_link(paragraphs_assoc[lines_union[N]], N); }
		else
		  {paragraphs_assoc[lines_union[N]] = get_beginning_of_line(N);}
	      }
	  }
	  paragraphs_union.propage_links();*/
	  
	  for(int N = 0; N < paragraphs_bbox.size(); N++)
	  {
	    paragraphs_bbox[N] = box2d();
	  }

	  for(int N = 0; N < lines_len.size(); N++)
	  {
	    if(lines_len[N] && paragraphs_union[lines_first_label[N]])
	    {
	      paragraphs_len[paragraphs_union[lines_first_label[N]]]++;
	      paragraphs_letter_len[paragraphs_union[lines_first_label[N]]] += lines_len[N];
	      if(paragraphs_first_line[paragraphs_union[lines_first_label[N]]])
	      {
		if(
		  lines_bbox[paragraphs_first_line[paragraphs_union[lines_first_label[N]]]].pmin()[0] > 
		  lines_bbox[N].pmin()[0]
		  )
		{
		    paragraphs_first_line[paragraphs_union[lines_first_label[N]]] = N;
		}
	      }
	      else
		paragraphs_first_line[paragraphs_union[lines_first_label[N]]] = N;
	    }
	  }
	  
	  for(int N = 0; N < paragraphs_union.size(); N++)
	  {
	    if(paragraphs_union[N] &&  paragraphs_len[paragraphs_union[N]])
	    {
	      paragraphs_bbox[paragraphs_union[N]].merge(lines_bbox[lines_union[N]]);
	    }
	    else
	    {
	      paragraphs_union[N] = 0;
	    }
	    
	  }
	  
	  
	  for(int N = 0; N < paragraphs_len.size(); N++)
	  {
	    if(paragraphs_len[N])
	    {
	      paragraphs_bbox_influ[N] = paragraphs_bbox[N].to_larger(lines_bbox[paragraphs_first_line[N]].len(0) / 10);
	    }
	  }
	}
	  
	template<typename T> void debug_assert_array_(mln::util::array<T>& array, int N, const std::string& name)
	{
	  if(N >= array.size())
	  {
	    std::cout << "WARNING : " << name << " " << N << " " << "is invalid" << endl;
	  }
	}
	
	
	
	/* RANGE DATA */
	Float _VSepRatio_Min; // The ratio is computed with the bounding box
	Float _VSepRatio_Max; 
	Float _HSepRatio_Min;
	Float _HSepRatio_Max;
	Float _LeterRatio_Min;
	Float _LeterRatio_Max;
	Data _LeterVolume_Min;
	Data _LeterVolume_Max;
	
	Data _ContainerVolume_Min;
	Data _ContainerVolume_Max;
	
	
	 mln::util::array<box2d> _bboxgp;
	 mln::util::array<box2d> _bboxgp_influ;
	
	/* DOCUMENT DATA */
	g_vertices_p _area_graph;
	mln::image2d<Label> img;
	mln::image2d<Label> img_influ;
		mln::image2d<bool> debug_source;
		mln::image2d<value::rgb8> debug_buffer;
		bool debug_buffer_enable;
	Label Areas_Number_;
	
	/* IMPLICIT SEPARATOR DETECTION */
	mymln::util::union_find<Label> implicit_separators_union;
	mymln::util::union_find<Label> implicit_separators_union_right;
    };
  }
}
#endif