#ifndef INC_DOCUMENT_DOC
#define INC_DOCUMENT_DOC
#include<my/util/vector_bbox_group.hh>
#include<my/util/union.hh>
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
	
	document(image2d<Label>& ima, image2d<Label>& ima_influ,mln::util::array<box2d>& bboxgp, g_vertices_p& area_graph, Label Areas)
	{
	  img = ima;
	  _bboxgp = bboxgp;
	  _area_graph = area_graph;
	  separators_mask = fun::i2v::array<bool>(Areas + 1);
	  containers_mask = fun::i2v::array<bool>(Areas + 1);
	  letters_mask = fun::i2v::array<bool>(Areas + 1);
	  Hseparator_mask =  fun::i2v::array<bool>(Areas + 1);
	  Vseparator_mask =  fun::i2v::array<bool>(Areas + 1);
	  noise_mask = fun::i2v::array<bool>(Areas + 1);
	  alone_letters_mask = fun::i2v::array<bool>(Areas + 1);
	  CImpSep = 1;
	  NImpSep = 2;
	  lines_union = mymln::util::union_find<Label>(Areas + 1);
	  implicit_separators_union = mymln::util::union_find<Label>(Areas + 1);
	  img_influ = ima_influ;
	  CSep = 0;
	  CSepH = 0;
	  CSepV = 0;
	  CLet = 0;
	  CLine = 1;
	  NLine = 2;
	  Areas_Number_ = Areas + 1;
	  
	}
	/* OPERATION ON LINES */
	inline void add_to_line_self_link(const point2d& point)
	{ add_to_line_self_link(img_influ(point));}
	inline void add_to_line(const point2d& point)
	{ add_to_line(img_influ(point)); }
	

	inline void add_to_line_link(const point2d& A, const point2d& B)
	{ add_to_line_link(img_influ(A), img_influ(B)); }
	inline bool same_line(const point2d& A, const point2d& B)
	{ return  same_line(img_influ(A), img_influ(B)); }
	inline bool same_line(const Label A, const Label B)
	{ return  lines_union[A] == lines_union[B]; }
	inline void add_new_line(const point2d& point)
	{ add_new_line(img_influ(point)); }
	
	inline void jump_to_line(const point2d& point)
	{ jump_to_line(img_influ(point)); }
	
	inline bool contain_line(const point2d& point)
	{ return contain_line(img_influ(point)); }
	
	inline void add_to_line(const Label lbl)
	{ lines_union[lbl] = CLine; }
	
	inline void add_new_line(const Label lbl)
	{ CLine = NLine; NLine++; }
	
	
	inline void add_to_line_self_link(const Label A)
	{lines_union.add_self_link(A);}
	
	inline void add_to_line_link(const Label A, const Label B)
	{lines_union.add_link(A, B);}

	inline void jump_to_line(const Label lbl)
	{ 
	  if(lines_union[lbl] != 0)
	    CLine = lines_union[lbl];
	  else
	    add_new_line(lbl);
	}
	
	inline bool contain_line(const Label lbl)
	{ return lines_union[lbl] != 0;}
	
	inline void add_noise(const point2d& point)
	{add_noise(img_influ(point));}
	
	inline unsigned int size(){return Areas_Number_;}

	
	void add_noise(Label lbl)
	{
	  separators_mask(lbl) = false;
	  letters_mask(lbl) = false;
	  alone_letters_mask(lbl) = false;
	  containers_mask(lbl) = false;
	  Hseparator_mask(lbl) = false;
	  Vseparator_mask(lbl) = false;
	  alone_letters_mask(lbl) = false;
	  
	  noise_mask(lbl) = true;
	}
	void inline add(Label lbl, int link)
	{
	  if (link == 0){add_noise(lbl);}
	  else if (link > 30){ add_separator(lbl);}
	  else { add_letter(lbl);}
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
	}
	void add_letter_coerce(const Label lbl)
	{
	  
	      letters_mask(lbl) = true;
	      separators_mask(lbl) = false;
	      containers_mask(lbl) = false;
	      Vseparator_mask(lbl) = false;
	      Hseparator_mask(lbl) = false;
	      alone_letters_mask(lbl) = false;	      
	      noise_mask(lbl) = false;
	}
	void add_letter(const Label lbl)
	{ 
	      CLet++;
	      if(label_valid_size_Min_(lbl, 2))
	      {
	      letters_mask(lbl) = true;
	      separators_mask(lbl) = false;
	      containers_mask(lbl) = false;
	      Vseparator_mask(lbl) = false;
	      Hseparator_mask(lbl) = false;
	      alone_letters_mask(lbl) = false;
	      
	       noise_mask(lbl) = false;
	      }
	      else
		add_noise(lbl);
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
	  }
	  else
		add_noise(lbl);
	}
	void add_Hseparator(const Label lbl)
	{
	   CSep++;
	   containers_mask(lbl) = false;
	   Vseparator_mask(lbl) = false;
	   Hseparator_mask(lbl) = true;
	   letters_mask(lbl) = false;
	   separators_mask(lbl) = true;
	   alone_letters_mask(lbl) = false;
	    noise_mask(lbl) = false;
	}
	void add_Vseparator(const Label lbl)
	{
	   CSep++;
	   containers_mask(lbl) = false;
	   Vseparator_mask(lbl) = true;
	   Hseparator_mask(lbl) = false;
	   letters_mask(lbl) = false;
	   separators_mask(lbl) = true;
	   alone_letters_mask(lbl) = false;
	    noise_mask(lbl) = false;
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
	  
	  inline bool allign_top( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right);
	    return  allignV < label_size_(0, Left) && (_bboxgp[Left].pcenter()[0]) > (_bboxgp[Right].pcenter()[0]);
	  }
	  
	  
	  inline bool allign_up_line( const point2d& Left, const point2d& Right)
	  {return allign_up_line(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_up_line( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right) * 1.3f;
	    return  allignV < label_size_(0, Left) && (_bboxgp[Left].pcenter()[0]) > (_bboxgp[Right].pcenter()[0]);
	  }
	  
	  inline bool allign_H_Large( const point2d& Left, const point2d& Right)
	  {return allign_H_Large(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H_Large( const Label Left, const Label Right)
	  {
	      short int allignV = label_allign_(1, Left, Right) * 1.5f;
	    return  allignV < label_size_(1, Left);
	  }
	  
	  inline bool allign_H( const point2d& Left, const point2d& Right)
	  {return allign_H(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H( const Label Left, const Label Right)
	  {
	      short int allignH = label_allign_(1, Left, Right) * 2;
	    return  allignH < label_size_(1, Left) && allignH < label_size_(1, Right);
	  }
	  
	    inline bool allign_size_height( const point2d& Left, const point2d& Right)
	  {return allign_size_height(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_size_height( const Label Left, const Label Right)
	  {
	     short int SizeL = label_size_(0, Left);
	      short int SizeR = label_size_(0, Right);
	    return  SizeR > (SizeL / 3) && SizeR < (SizeL * 3);
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
	  inline bool allign_V_large( const point2d& Left, const point2d& Right)
	  {return allign_V_large(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V_large( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right);
	    return  allignV < label_size_(0, Left) && allignV < label_size_(0, Right);
	  }
	  
	  
	   inline bool allign_V_extra_large( const point2d& Left, const point2d& Right)
	  {return allign_V_extra_large(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V_extra_large( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right) / 2;
	    return  allignV < label_size_(0, Left) && allignV < label_size_(0, Right);
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
	  
	  
	  void stat()
	  {
	    std::cout << "document :" << std::endl;
	    std::cout << "   letter(s) : " << CLet << std::endl;
	    std::cout << "   separator(s) : " << CSep << std::endl;
	    std::cout << "      vertical separator(s) : " << CSepV << std::endl;
	    std::cout << "      horizontal separator(s) : " << CSepH << std::endl;
	    std::cout << "      lines(s) : " << CLine << std::endl;
	  
	  }
	  void debug_save_lines(std::string file)
	  {  mymln::debug::save_label_image(img, lines_union , file);}
	  void debug_save_separators(std::string file)
	  {  mymln::debug::save_label_image(img, implicit_separators_union , file);}
	  vertex_image<point2d,bool> fun_mask_separators()
	  { return fun_mask_(separators_mask); }
	  vertex_image<point2d,bool> fun_mask_containers()
	  { return fun_mask_(containers_mask); }
	  vertex_image<point2d,bool> fun_mask_alone_letters()
	  { return fun_mask_(alone_letters_mask); }
	  vertex_image<point2d,bool> fun_mask_all()
	  {  
	    typedef vertex_image<point2d,bool> v_ima_g;
	    v_ima_g result(_area_graph);
	    return result; 
	  }
	  vertex_image<point2d,bool> fun_mask_letters()
	  { return fun_mask_(letters_mask); }
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
	  
	  Label get_label(point2d point)
	  { return img_influ(point); }
	  
	  unsigned int get_line_length(point2d point)
	  { return get_line_length(img_influ(point)); }
	  
	  unsigned int get_line_length(Label L)
	  { return lines_len[lines_union[L]]; }
	  
	  unsigned int get_beginning_of_line(point2d point)
	  { return get_beginning_of_line(img_influ(point)); }
	  
	  unsigned int get_beginning_of_line(Label L)
	  { return lines_first_label[lines_union[L]]; }
	  
	  unsigned int get_end_of_line(point2d point)
	  { return get_end_of_line(img_influ(point)); }
	  
	  unsigned int get_end_of_line(Label L)
	  { return lines_last_label[lines_union[L]]; }
	  
	  
	  unsigned int get_parent_line(point2d point)
	  { return lines_union[img_influ(point)]; }
	  
	  
	  unsigned int get_parent_line(Label L)
	  { return lines_union[L]; }
	  
	  
	  inline void recook_lines()
	  {
	    lines_first_label.fill(0);
	    lines_last_label.fill(0);
	    lines_len.fill(0);
	    cook_lines_();
	  }
	  inline void cook_lines()
	  {
	    lines_len = mln::util::array<unsigned int>(CLine + 1);
	    lines_first_label = mln::util::array<unsigned int>(CLine + 1);
	    lines_last_label = mln::util::array<unsigned int>(CLine + 1);
	    start_lines_mask = fun::i2v::array<bool>(Areas_Number_);
	    end_lines_mask = fun::i2v::array<bool>(Areas_Number_);
	    start_end_lines_mask = fun::i2v::array<bool>(Areas_Number_);
	    lines_len.fill(0);
	    start_lines_mask(0) = false;
	    end_lines_mask(0) = false;
	    cook_lines_();
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

	inline bool contain_implicit_separator(const Label lbl)
	{return implicit_separators_union[lbl] != 0; }

	inline void add_to_separator(const point2d& point)
	{ add_to_separator(img_influ(point)); }
	inline void add_to_separator(const Label lbl)
	{ implicit_separators_union[lbl] = CImpSep; }
	
	inline void invalidate_implicit_separator(const point2d& point)
	{ invalidate_implicit_separator(img_influ(point)); }
	inline void invalidate_implicit_separator(Label lbl)
	{ implicit_separators_union[lbl] = 0; }

	inline Label& operator[](point2d i)
	{ return img_influ(i); }
	
	inline point2d& operator[](Label i)
	{ return _bboxgp[i].pcenter(); }
      private:
	
	// PRIVATE DATA ON LINES
	mln::util::array<unsigned int> lines_len;
	mln::util::array<unsigned int> lines_first_label;
	mln::util::array<unsigned int> lines_last_label;
	fun::i2v::array<bool> start_lines_mask;
	fun::i2v::array<bool> end_lines_mask;
	fun::i2v::array<bool> start_end_lines_mask;
	

	
	  inline void cook_lines_()
	  {
	    for(unsigned int N = 1; N < lines_union.size(); N++)
	    {
	      if(lines_union[N] != 0)
	      {
		/* APPROXIMATE THE NUMBER OF CHAR IN THE LINE */
		  lines_len[lines_union[N]]++;
		/* COOK THE FIRST AND THE LAST LABEL OF THE LINE */
		if(lines_first_label[lines_union[N]] == 0)
		  lines_first_label[lines_union[N]] = N;
		else if(_bboxgp[N].pcenter()[1] < _bboxgp[lines_first_label[lines_union[N]]].pcenter()[1])
		    lines_first_label[lines_union[N]] = N;
		
		if(lines_last_label[lines_union[N]] == 0)
		  lines_last_label[lines_union[N]] = N;
		else if(_bboxgp[N].pcenter()[1] > _bboxgp[lines_last_label[lines_union[N]]].pcenter()[1])
		    lines_last_label[lines_union[N]] = N;
		
		/* FILL THE MASK WITH FALSE:MAYBE USELESS IF THE MASK IS INITIALIZED */
		start_lines_mask(N) = false;
		end_lines_mask(N) = false;
		start_end_lines_mask(N) =false;
	      }
	    }

	    
	    /* SECOND STEP OF THE COOKING */
	    for(unsigned int N = 0; N < CLine + 1; N++)
	    {
	      if( lines_first_label[N] != 0)
	      {
		 start_lines_mask(lines_first_label[N]) = true;
		 end_lines_mask(lines_last_label[N]) = true;
		 start_end_lines_mask(lines_first_label[N]) = true;
		 start_end_lines_mask(lines_last_label[N]) = true;
	      }
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
	  return SX >= Min && SY >= Min;
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
	fun::i2v::array<bool> containers_mask;
	fun::i2v::array<bool> noise_mask;
	
	
	unsigned int CLine;
	unsigned int NLine;
	unsigned int CImpSep;
	unsigned int NImpSep;
	
	mymln::util::union_find<Label> lines_union;
	unsigned int CLet ;
	unsigned int CSep ;
	unsigned int CSepH ;
	unsigned int CSepV ;
	
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
	
	/* DOCUMENT DATA */
	g_vertices_p _area_graph;
	mln::image2d<Label> img;
	mln::image2d<Label> img_influ;
	Label Areas_Number_;
	
	/* IMPLICIT SEPARATOR DETECTION */
	mymln::util::union_find<Label> implicit_separators_union;
    };
  }
}
#endif