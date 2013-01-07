#ifndef DERIVATIVE_KERNELS_HH_
# define DERIVATIVE_KERNELS_HH_

namespace mln
{
  namespace inpainting
  {
    namespace internal
    {
      const float a = 2;
      const float b = 1;
    }

    template <typename T>
    const T* make_dx_kernel()
    {
      static const T ws[] = { internal::b, internal::a, internal::b,
			      0, 0, 0,
			      -internal::b, -internal::a, -internal::b };
      return ws;
    }

    template <typename T>
    const T* make_dy_kernel()
    {
      static const T ws[] = { internal::b, 0, -internal::b,
			      internal::a, 0, -internal::a,
			      internal::b, 0, -internal::b };
      
      return ws;
    }

    template <typename T>
    const T* make_forward_horizontal_kernel()
    {
      static const T ws[] = { 0, 0, 0,
			      0, -1, 1,
			      0, 0, 0 };
      return ws;
    }

    template <typename T>
    const T* make_forward_vertical_kernel()
    {
      static const T ws[] = { 0, 0, 0,
			      0, -1, 0,
			      0, 1, 0 };
      
      return ws;
    }

    template <typename T>
    const T* make_backward_horizontal_kernel()
    {
      static const T ws[] = { 0, 0, 0,
			      -1, 1, 0,
			      0, 0, 0 };
      return ws;
    }

    template <typename T>
    const T* make_backward_vertical_kernel()
    {
      static const T ws[] = { 0, -1, 0,
			      0, 1, 0,
			      0, 0, 0 };
      
      return ws;
    }

    template <typename T>
    const T* make_dxx_kernel()
    {
      static const T ws[] = {0.0804, 0.1597, 0.0804,
				 -0.1608, -0.3194, -0.1608,
				 0.0804, 0.1597, 0.0804};

      return ws;
    }

    template <typename T>
    const T* make_dyy_kernel()
    {
      static const T ws[] = {0.0804, -0.1608, 0.0804,
				 0.1597, -0.3194, 0.1597,
				 0.0804, -0.1608, 0.0804};
		    
      return ws;
    }

    template <typename T>
    const T* make_dxy_kernel()
    {
      static const T ws[] = {0.1051, 0, -0.1051,
			     0, 0, 0,
			     -0.1051, 0, 0.1051};

      return ws;
    }
  }
}

#endif
