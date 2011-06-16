#ifndef INC_RUNTIME_LIB
#define INC_RUNTIME_LIB

using namespace mln;
using namespace std;
namespace mymln
{
  namespace runtime
  {
    template<typename L, typename F, typename D>
    void load_separators(runtime<L,F,D>& run)
    {
      
	  run.add_function("separators.find_allign_right", &(mymln::document::separators::separators_find_allign_right));
	  run.add_function("separators.make_clean_right", &(mymln::document::separators::separators_make_clean_right));  
	  run.add_function("separators.find_allign_left", &(mymln::document::separators::separators_find_allign));
	  run.add_function("separators.make_clean_left", &(mymln::document::separators::separators_find_allign));
    }
    
    template<typename L, typename F, typename D>
    void load_clean(runtime<L,F,D>& run)
    {
	  run.add_function("clean.containers_items", &(mymln::document::clean_containers_items));
	  run.add_function("clean.letters_items", &(mymln::document::clean_letters_items));
	  run.add_function("clean.get_lines", &(mymln::document::clean_get_lines));
   
	  run.add_function("clean.letters_alone", &(mymln::document::clean_letters_alone));
	  run.add_function("clean.included_letters", &(mymln::document::clean_included_letters));
	  run.add_function("clean.dot_items", &(mymln::document::clean_dot_items));
	  run.add_function("clean.quote_items", &(mymln::document::clean_quote_items));
	  run.add_function("clean.between", &(mymln::document::clean_between));
	  
	  run.add_function("clean.line_link_item", &(mymln::document::clean_line_link_item));
	  run.add_function("clean.proximity_lines", &(mymln::document::clean_proximity_lines));
	  run.add_function("clean.quote_lines", &(mymln::document::clean_quote_lines));
	  run.add_function("clean.alone_letters_lines", &(mymln::document::clean_alone_letters_lines));
	  run.add_function("clean.odd_letters", &(mymln::document::clean_odd_letters));
	  
	  
	  run.add_function("clean.remove_alone_letter", &(mymln::document::remove_alone_letter));
	  run.add_function("clean.paragraph_items", &(mymln::document::clean_paragraph_items));
	  run.add_function("clean.paragraphs_up", &(mymln::document::clean_paragraphs_up));
	  run.add_function("clean.paragraphs_large", &(mymln::document::clean_paragraphs_large));
	  run.add_function("clean.included_paragraphs", &(mymln::document::clean_included_paragraphs));
	  run.add_function("clean.backward_letters", &(mymln::document::clean_backward_letters));
	  run.add_function("clean.paragraphs_tab", &(mymln::document::clean_paragraphs_tab));
	  run.add_function("clean.proximity_letters", &(mymln::document::clean_proximity_letters));
	  
    }

    template<typename L, typename F, typename D>
    void lib_debug_save_all(mymln::document::document<L,F,D>& doc, std::string file)
    { doc.debug_save_all(file); }
    template<typename L, typename F, typename D>
    void lib_debug_save_buffer(mymln::document::document<L,F,D>& doc, std::string file)
    { doc.debug_save_buffer(file); }
    template<typename L, typename F, typename D>
    void lib_debug_create_buffer(mymln::document::document<L,F,D>& doc)
    { doc.debug_create_buffer(); }
    
    template<typename L, typename F, typename D>
    void load_debug(runtime<L,F,D>& run)
    {
      run.add_function_string("debug.save", &(lib_debug_save_all));
      run.add_function("debug.create_buffer", &(lib_debug_create_buffer));
      run.add_function_string("debug.save_buffer", &(lib_debug_save_buffer));
    }


    template<typename L, typename F, typename D>
    void lib_cook_lines(mymln::document::document<L,F,D>& doc)
    { doc.cook_lines(); }
    template<typename L, typename F, typename D>
    void lib_recook_lines(mymln::document::document<L,F,D>& doc)
    { doc.recook_lines(); }
    template<typename L, typename F, typename D>
    void lib_cook_separators_left(mymln::document::document<L,F,D>& doc)
    { doc.cook_separators(); }
    template<typename L, typename F, typename D>
    void lib_cook_separators_right(mymln::document::document<L,F,D>& doc)
    { doc.cook_separators_right(); }
    template<typename L, typename F, typename D>
    void lib_cook_line_splitting(mymln::document::document<L,F,D>& doc)
    { doc.cook_line_splitting(); }
    template<typename L, typename F, typename D>
    void lib_reset_implicit_separators(mymln::document::document<L,F,D>& doc)
    { doc.reset_implicit_separators(); }
    
    template<typename L, typename F, typename D>
    void lib_cook_line_splitting_exclusive(mymln::document::document<L,F,D>& doc)
    { doc.cook_line_splitting_exclusive(); }
    
    template<typename L, typename F, typename D>
    void lib_cook_paragraphs(mymln::document::document<L,F,D>& doc)
    { doc.cook_paragraphs(); }
    
    template<typename L, typename F, typename D>
    void lib_recook_paragraphs(mymln::document::document<L,F,D>& doc)
    { doc.recook_paragraphs(); }

    template<typename L, typename F, typename D>
    void load_cooking(runtime<L,F,D>& run)
    {
      run.add_function("cook.lines", &(lib_cook_lines));
      run.add_function("cook.separators_right", &(lib_cook_separators_right));
      run.add_function("cook.separators_left", &(lib_cook_separators_left));
      run.add_function("recook.lines", &(lib_recook_lines));
      run.add_function("cook.line_splitting", &(lib_cook_line_splitting));
      run.add_function("cook.line_splitting_inclusive", &(lib_cook_line_splitting));
      run.add_function("cook.line_splitting_exclusive", &(lib_cook_line_splitting_exclusive));
      run.add_function("cook.reset_implicit_separators", &(lib_reset_implicit_separators));      
      run.add_function("cook.paragraphs", &(lib_cook_paragraphs));
      run.add_function("recook.paragraphs", &(lib_recook_paragraphs));

    }
    
    
    template<typename L, typename F, typename D>
    void lib_compute_letter_middle_height(mymln::document::document<L,F,D>& doc)
    { doc.compute_letter_middle_height(); }
     template<typename L, typename F, typename D>
    void lib_compute_letter_middle_width(mymln::document::document<L,F,D>& doc)
    { doc.compute_letter_middle_width(); }
    template<typename L, typename F, typename D>
    void load_compute(runtime<L,F,D>& run)
    {
      run.add_function("compute.letter_middle_height", &(lib_compute_letter_middle_height));
      run.add_function("compute.letter_middle_width", &(lib_compute_letter_middle_width));
    }
    
    
    template<typename L, typename F, typename D>
    void lib_string_print(mymln::document::document<L,F,D>& doc, std::string str)
    { std::cout << str; }
    template<typename L, typename F, typename D>
    void lib_string_print_line(mymln::document::document<L,F,D>& doc, std::string str)
    { std::cout << str << std::endl; }
    template<typename L, typename F, typename D>
    void lib_string_concat(runtime<L,F,D>& run, std::string A, std::string B)
    {  run.add_variable(A, B); }
    
    template<typename L, typename F, typename D>
    void load_string(runtime<L,F,D>& run)
    {
      run.add_function_string("string.print", &(lib_string_print));
      run.add_function_string("string.print_line", &(lib_string_print_line));
      run.add_function_string_string("string.concat", &(lib_string_concat));
      run.add_function_string_string("string.clone", &(lib_string_concat));
    }
    
    
    template<typename L, typename F, typename D>
    void lib_system_set(runtime<L,F,D>& run, std::string A, std::string B)
    {  run.add_variable(A, B); }
    template<typename L, typename F, typename D>
    void lib_system_test_equal(runtime<L,F,D>& run, std::string A, std::string B)
    {  if(!A.compare("true")){run.call_function(B);} }
    
    template<typename L, typename F, typename D>
    void lib_system_test_nequal(runtime<L,F,D>& run, std::string A, std::string B)
    {  if(!A.compare("false")){run.call_function(B);} }
    
    template<typename L, typename F, typename D>
    void lib_system_not(runtime<L,F,D>& run, std::string A, std::string B)
    {  
      if(!A.compare("false")){run.add_variable(A, "true");}
      else{run.add_variable(A, "false");}
    }
    
    template<typename L, typename F, typename D>
    void load_system(runtime<L,F,D>& run)
    {
      run.add_function_string_string("system.set", &(lib_system_set));
      run.add_function_string_string("system.equal", &(lib_string_concat));
      run.add_function_string_string("system.nequal", &(lib_string_concat));
    }   
    
  }
}
#endif