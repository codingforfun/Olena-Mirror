#ifndef METALIC_TRACKED_PTR_HH
# define METALIC_TRACKED_PTR_HH

# include <ostream>
# include <set>
# include <map>


namespace mlc {


  template <typename T> class tracked_ptr; // fwd decl


  namespace internal {


    template <class T>
    class tracked_ptr_proxy
    {

      friend class tracked_ptr<T>;

      // attributes

      T* ptr_;
      std::set<const tracked_ptr<T>*> holders_;

      // w/o impl

      tracked_ptr_proxy(const tracked_ptr_proxy&);
      void operator=(const tracked_ptr_proxy&);

      // methods

      tracked_ptr_proxy() :
	ptr_(0)
      {
      }

      tracked_ptr_proxy(T* ptr) :
	ptr_(0)
      {
	set_ptr(ptr);
      }

      void set_ptr(T* new_ptr)
      {
	if (new_ptr == this->ptr_)
	  return;
	T* old_ptr = this->ptr_;
	if (old_ptr != 0)
	  {
	    assert(proxy_of()[old_ptr] == this);
	    proxy_of().erase(old_ptr);
	  };
	this->ptr_ = new_ptr;
	if (new_ptr != 0)
	  {
	    assert(proxy_of()[new_ptr] == 0);
	    proxy_of()[new_ptr] = this;
	  };
      }

      void register_holder(const tracked_ptr<T>* holder)
      {
	assert(holder != 0);
	assert(holders_.find(holder) == holders_.end());
	holders_.insert(holder);
      }

      bool unregister_holder(const tracked_ptr<T>* holder)
      {
	assert(holder != 0);
	assert(holders_.size() > 0);

	typename std::set<const tracked_ptr<T>*>::iterator e = holders_.find(holder);
	assert(e != holders_.end());
	holders_.erase(e);

	if (holders_.size() == 0)
	  {
	    proxy_of()[ptr_] = 0;
	    delete ptr_;
	    ptr_ = 0;
	    return true;
	  }
	return false;
      }

      // procedures

      typedef std::map<const T*, const tracked_ptr_proxy*> map_type;

      static map_type& proxy_of()
      {
	static map_type proxy_of_;
	return proxy_of_;
      }

      friend
      std::ostream& operator<<(std::ostream& ostr, const tracked_ptr_proxy& proxy)
      {
	ostr << "[ptr=" << proxy.ptr_ << " holders=(";
	typename std::set<const tracked_ptr<T>*>::const_iterator i = proxy.holders_.begin();
	bool ok = i != proxy.holders_.end();
	while (ok)
	  {
	    ostr << *i++;
	    if (ok = i != proxy.holders_.end())
	      ostr << ", ";
	  }
	ostr << ")]";
	return ostr;
      }

    };

  } // end of namspace internal





  template <typename T>
  class tracked_ptr
  {
  public:

    const T& operator*() const
    {
      assert(proxy_ != 0 and proxy_->ptr_ != 0);
      return *(proxy_->ptr_);
    }

    T& operator*()
    {
      assert(proxy_ != 0 and proxy_->ptr_ != 0);
      return *(proxy_->ptr_);
    }

    const T*const operator->() const
    {
      assert(proxy_ != 0);
      return proxy_->ptr_;
    }

    T*const operator->()
    {
      assert(proxy_ != 0);
      return proxy_->ptr_;
    }

    operator bool() const
    {
      return proxy_ != 0 and proxy_->ptr_ != 0;
    }

    bool operator not() const
    {
      return not bool(*this);
    }

    bool operator==(int b) const
    {
      return bool(*this) == bool(b);
    }

    bool operator!=(int b) const
    {
      return bool(*this) != bool(b);
    }


    // hook:
    const internal::tracked_ptr_proxy<T>* proxy() const { return proxy_; }


    // ctors

    tracked_ptr() :
      proxy_(0)
    {
      invariant_();
    }

    tracked_ptr(T* ptr) :
      proxy_(new internal::tracked_ptr_proxy<T>(ptr))
    {
      proxy_->register_holder(this);
      invariant_();
    }

    // shallow copy

    tracked_ptr(const tracked_ptr& rhs) :
      proxy_(rhs.proxy_)
    {
      if (proxy_ != 0)
	proxy_->register_holder(this);
      invariant_();
    }

    // assignment

    tracked_ptr& operator=(const tracked_ptr& rhs)
    {
      invariant_();
      if (rhs == 0)
	return *this;
      if (rhs.proxy_ == this->proxy_)
	return *this;
      if (this->release_proxy_())
	delete this->proxy_;
      this->proxy_ = rhs.proxy_;
      this->proxy_->register_holder(this);
      invariant_();
      return *this;
    }

    tracked_ptr& operator=(T* ptr)
    {
      if (proxy_ != 0)
	{
	  assert(proxy_->holders_.size() <= 1);
	  this->release_proxy_();
	}
      else
	proxy_ = new internal::tracked_ptr_proxy<T>(ptr);
      this->proxy_->register_holder(this);
      return *this;
    }

    // dtor

    ~tracked_ptr()
    {
      invariant_();
      if (release_proxy_())
	delete proxy_;
      proxy_ = 0;
    }

    // dangerous! (?)

    void reset_()
    {
      invariant_();
      if (proxy_ == 0 or proxy_->ptr_ == 0)
	return;
      T* ptr = proxy_->ptr_;
      std::set<const tracked_ptr<T>*>& holders = proxy_->holders_;
      typename std::set<const tracked_ptr<T>*>::iterator i;
      for (i = holders.begin(); i != holders.end(); ++i)
	{
	  if (*i == this)
	    continue;
	  tracked_ptr<T>* holder = const_cast<tracked_ptr<T>*>(*i);
	  holder->proxy_ = 0;
	}
      delete this->proxy_->ptr_;
      delete this->proxy_;
      this->proxy_ = 0;
    }


  private:

    internal::tracked_ptr_proxy<T>* proxy_;

    bool release_proxy_()
    {
      invariant_();
      if (proxy_ == 0)
	return false;
      return proxy_->unregister_holder(this);
    }

    void invariant_() const
    {
      if (proxy_ == 0)
	return;
      const std::set<const tracked_ptr<T>*>& holders = proxy_->holders_;
      typename std::set<const tracked_ptr<T>*>::const_iterator i;
      for (i = holders.begin(); i != holders.end(); ++i)
	assert((*i)->proxy_ == this->proxy_);
    }

  };


} // end of namespace mlc



#endif // ! METALIC_TRACKED_PTR_HH
