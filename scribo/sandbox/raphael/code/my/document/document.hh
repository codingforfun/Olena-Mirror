#ifndef INC_DOCUMENT_DOC
#define INC_DOCUMENT_DOC
#include<my/util/vector_bbox_group.hh>
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

	  lines_mark = mln::util::array<unsigned int>(Areas + 1);
	lines_mark.fill(0);
	  img_influ = ima_influ;
	  CSep = 0;
	  CSepH = 0;
	  CSepV = 0;
	  CLet = 0;
	  CLine = 1;
	  NLine = 2;
	  
	}
	/* OPERATION ON LINES */
	inline void add_to_line(const point2d& point)
	{ add_to_line(img_influ(point)); }
	
	inline void add_new_line(const point2d& point)
	{ add_new_line(img_influ(point)); }
	
	inline void jump_to_line(const point2d& point)
	{ jump_to_line(img_influ(point)); }
	
	inline bool contain_line(const point2d& point)
	{ return contain_line(img_influ(point)); }
	
	inline void add_to_line(const Label lbl)
	{ lines_mark[lbl] = CLine; std::cout << "add : " <<  CLine; }
	
	inline void add_new_line(const Label lbl)
	{ CLine = NLine; NLine++; }
	
	inline void jump_to_line(const Label lbl)
	{ 
	  if(lines_mark[lbl] != 0)
	    CLine = lines_mark[lbl];
	  else
	    add_new_line(lbl);
	}
	
	inline bool contain_line(const Label lbl)
	{ return lines_mark[lbl] != 0;}
	
	/* LABELS MUST ALLWAYS BE SORTED */
	inline void add_noise(const point2d& point)
	{add_noise(img_influ(point));}
	

	
	void add_noise(Label lbl)
	{
	  separators_mask(lbl) = false;
	  letters_mask(lbl) = false;
	  containers_mask(lbl) = false;
	  Hseparator_mask(lbl) = false;
	  Vseparator_mask(lbl) = false;
	  
	  noise_mask(lbl) = true;
	}
	void inline add(Label lbl, int link)
	{
	  if (link == 0){add_noise(lbl);}
	  else if (link > 40){ add_separator(lbl);}
	  else { add_letter(lbl);}
	}
	void inline invalid_letter(const point2d& point)
	{invalid_letter(img_influ(point));}
	void invalid_letter(Label lbl)
	{letters_mask(lbl) = true;}
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
	  
	  inline bool allign_H( const point2d& Left, const point2d& Right)
	  {return allign_H(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_H( const Label Left, const Label Right)
	  {
	    return label_allign_(1, Left, Right) < label_size_(1 ,Left);
	  }
	  inline bool allign_V( const point2d& Left, const point2d& Right)
	  {return allign_V(img_influ(Left), img_influ(Right));}
	  
	  inline bool allign_V( const Label Left, const Label Right)
	  {
	    short int allignV = label_allign_(0, Left, Right) * 4;
	    return  allignV < label_size_(0, Left) && allignV < label_size_(0, Right);
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
	    mymln::debug::save_label_image(img, lines_mark , "outfileline.pbm");
	  }

	  vertex_image<point2d,bool> fun_mask_separators()
	  { return fun_mask_(separators_mask); }
	  vertex_image<point2d,bool> fun_mask_containers()
	  { return fun_mask_(containers_mask); }
	  vertex_image<point2d,bool> fun_mask_all()
	  {  
	    typedef vertex_image<point2d,bool> v_ima_g;
	    v_ima_g result(_area_graph);
	    return result; 
	  }
	  vertex_image<point2d,bool> fun_mask_letters()
	  { return fun_mask_(letters_mask); }
	  image2d<bool> image_mask_containers()
	  { return image_mask_(containers_mask); }
	  image2d<bool> image_mask_separators()
	  { return image_mask_(separators_mask); }
	  image2d<bool> image_mask_letters()
	  { return image_mask_(letters_mask); }
	  image2d<bool> image_mask_noise()
	  { return image_mask_(noise_mask); }
	  
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
	  
	  
	  
	  /*image_if<image2d<Label> masked_image_letters()
	  {return masked_image_(letters_mask); }
	  image_if<image2d<Label> masked_image_separator()
	  {return masked_image_(letters_mask); }*/	  

      private:
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
	fun::i2v::array<bool> containers_mask;
	fun::i2v::array<bool> noise_mask;
	unsigned int CLine;
	unsigned int NLine;
	mln::util::array<unsigned int> lines_mark;
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
    };
  }
}
#endif