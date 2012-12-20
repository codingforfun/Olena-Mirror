namespace mln
{
  namespace inpainting
  {
    namespace
    {
      const float a = 61;
      const float b = 17;

      template <typename T>
      const T* make_dx_kernel()
      {
	static const T ws[] = { b, a, b,
				0, 0, 0,
				-b, -a, -b };
	return ws;
      }

      template <typename T>
      const T* make_dy_kernel()
      {
	static const T ws[] = { b, 0, -b,
				a, 0, -a,
				b, 0, -b };
      
	return ws;
      }
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
