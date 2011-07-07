#ifndef MLN_DATA_IMAGE
#define MLN_DATA_IMAGE
#include <stdlib.h>
namespace mymln
{
  namespace data
  {
    enum image_type
    {
      Gray,
      Color,
      Black_And_White
    };
    
    /// The class image is use to contain images
    class image
    {
      public:
	image()
	{ }	
      private:
	
    };
  }
}
#endif