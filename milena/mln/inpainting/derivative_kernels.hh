namespace mln
{
  namespace inpainting
  {
    template <typename T>
    const T* make_dx_kernel()
    {
      static const T ws[] = { 0.1, 0.2, 0.1,
				  0, 0, 0,
				  -0.1, -0.2, -0.1 };
      return ws;
    }

    template <typename T>
    const T* make_dy_kernel()
    {
      static const T ws[] = { 0.1, 0, -0.1,
				  0.2, 0, -0.2,
				  0.1, 0, -0.1 };
      
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
