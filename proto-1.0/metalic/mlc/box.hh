#ifndef METALIC_BOX_HH
# define METALIC_BOX_HH


namespace mlc 
{

  template <class T>
  struct box {

    box(T ref) :
      ref(ref)
    {}

    box(const box<T>& rhs) :
      ref(rhs.ref)
    {}

    box<T>& operator=(box<T>& rhs)
    {
      this->ref = rhs.ref;
      return *this;
    }

    // FIXME: oldies, should be removed?

    operator T() { return ref; }

    T& unbox() { return ref; }
    const T& unbox() const { return ref; }

    // FIXME: new code, better?

    const T& operator*() const { return ref; }
          T& operator*()       { return ref; }

    const T *const operator->() const { return &ref; }
          T *const operator->()       { return &ref; }

    friend class box<const T>;

  private:
    mutable T ref;
  };



  template <class T>
  struct box <const T>
  {

    box(const T& ref) :
      ref(const_cast<T&>(ref))
    {}

    box(const box<T>& rhs) :
      ref(const_cast<T&>(rhs.ref))
    {}

    box(const box<const T>& rhs) :
      ref(const_cast<T&>(rhs.ref))
    {}

    box<const T>& operator=(const box<const T>& rhs)
    {
      this->ref = const_cast<T&>(rhs.ref);
      return *this;
    }

    // FIXME: oldies, should be removed?

    operator const T() const { return ref; }
    const T& unbox() const { return ref; }

    // FIXME: new code, better?

    const T& operator*() const { return ref; } // FIXME: 'T&' or 'T'?
    const T *const operator->() const { return &ref; }

  private:
    mutable T ref;
  };



  template <class T> const T& unbox(const box<T>& b) {return b.unbox(); }
  template <class T>       T& unbox(      box<T>& b) {return b.unbox(); }


} // end of namespace mlc


#endif // ! METALIC_BOX_HH
