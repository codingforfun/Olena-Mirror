#ifndef MLN_DEBUG_LOGGER
#define MLN_DEBUG_LOGGER
#include <mln/display/image_viewer.hh>
#include <mln/data/convert.hh>
#include <mln/convert/to_qimage_nocopy.hh>
  namespace mln
  {
    namespace debug
    {
      
      
      namespace internal
      {
	// THIS CLASS CAN BE USED FOR MULTITHREADING
	// TODAY IT IS NIT AVIABLE SO THE OBJECT
	// //WILL PERFORM THE OPPERATIONS ON THE SAME THREAD
	class logger_thread
	{
	  public:
	    logger_thread()
	    {

	    }
	    void update_image(std::list<QImage>* ima_list)
	    {      
	      int argc = 1;
	      char* prog = "this";
	      char** argv = &prog;
	      app = new QApplication(argc, argv);
	      view = new mln::display::image_viewer();
	      QVBoxLayout *layout = new QVBoxLayout;
	      layout->addWidget(view);
	      view->window()->setLayout(layout);
	      view->window()->show();
	      
	      
	       std::list<QImage>::iterator it;
	       for ( it=ima_list->begin() ; it != ima_list->end(); it++ )
	       {
		 std::cout << "TATA"<< std::endl;
		 view->push_image(*it);
	       }      
	      app->exec();
	    }
	    ~logger_thread()
	    {
	    }
	  private:
	    mln::display::image_viewer* view;
	    QApplication* app;
	};
      }
      
      
      class logger
      {
	public:
	  logger()
	  {
	    _GUI = false;
	    ima_list_ = new std::list<QImage>();
	  }
	  template <typename I>
	  void log_image(const image2d<I> ima, const char *name)
	  {
	     ima_list_->push_back(mln::convert::to_qimage_nocopy(ima));

	     
	  }
	  void log_image(const image2d<bool> ima, const char *name)
	  {
	      log_image(mln::data::convert(mln::value::qt::rgb32(), ima), name);
	  }
	  
	  void start_gui()
	  {
	    if(!_GUI)
	    {
	       async_view = new mln::debug::internal::logger_thread();
	       async_view->update_image(ima_list_);
	      _GUI = true;
	    }
	  }
	  
	private:
	  bool _GUI;
	  mln::debug::internal::logger_thread* async_view;
	  std::list<QImage>* ima_list_;
	  
      };
      

      
    }
  }
  #endif