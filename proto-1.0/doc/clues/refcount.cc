
#include <assert.h>
#include <iostream>
#include <string>
#include <set>
#include <map>


struct array2d
{
  friend
  std::ostream& operator<<(std::ostream& ostr, const array2d& arr)
  {
    return ostr << "array2d at " << &arr;
  }

  array2d()
  {
    std::cout << "\t+ " << *this << std::endl;
  }
  ~array2d()
  {
    std::cout << "\t- " << *this << std::endl;
  }
  // w/o impl:
  array2d(const array2d&);
  void operator=(const array2d&);
};



template <typename T> class tracked_ptr; // fwd decl



// tracked_ptr_proxy
// there is exactly one instance of proxy per ptr_


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

  tracked_ptr_proxy()
  {
    set_ptr(0);
  }

  tracked_ptr_proxy(T* ptr)
  {
    set_ptr(ptr);
  }

  void set_ptr(T* ptr)
  {
    this->ptr_ = ptr;
    if (ptr_ != 0)
      {
	assert(proxy_of(ptr_) == 0);
	proxy_of(ptr_) = this;
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
	proxy_of(ptr_) = 0;
	delete ptr_;
	ptr_ = 0;
	return true;
      }
    return false;
  }

  // procedures

  typedef std::map<const T*, const tracked_ptr_proxy*> map_type;

  static const tracked_ptr_proxy*& proxy_of(const T* ptr)
  {
    static map_type proxy_of_;
    return proxy_of_[ptr];
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





// tracked_ptr


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
  const tracked_ptr_proxy<T>* proxy() const { return proxy_; }


protected:

  // ctors

  tracked_ptr() :
    proxy_(0)
  {
    invariant_();
  }

  tracked_ptr(T* ptr) :
    proxy_(new tracked_ptr_proxy<T>(ptr))
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
    assert(proxy_->holders_.size() <= 1);
    this->release_proxy_();
    this->proxy_->set_ptr(ptr);
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

  // accessors

  const tracked_ptr<T>& ptr_() const
  {
    invariant_();
    return *this;
  }

  tracked_ptr<T>& ptr_()
  {
    invariant_();
    return *this;
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

  tracked_ptr_proxy<T>* proxy_;

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



// env

struct image2d;
std::set<const image2d*> env;



// image2d


struct image2d : private tracked_ptr<array2d>
{
  typedef tracked_ptr<array2d> super_type;

  const tracked_ptr<array2d>& data() const
  {
    return ptr_();
  }

  tracked_ptr<array2d>& data()
  {
    return ptr_();
  }

  image2d(const std::string& name) :
    name_(name)
  {
    env.insert(this);
  }

  image2d(const std::string& name, int) :
    tracked_ptr<array2d>(new array2d),
    name_(name)
  {
    env.insert(this);
  }

  image2d(const image2d& rhs)
  {
    this->name_ = std::string("cpy_of_") + rhs.name_;
    env.insert(this);
  }

  /*
    the code below is only written to make sure that this->name_ is unchanged
   */
  image2d operator=(const image2d& rhs)
  {
    if (&rhs == this)
      return *this;
    super_type::operator=(rhs);
    // nothing to do with "this->name_"
    return *this;
  }

  ~image2d()
  {
    env.erase(this);
  }

  void clear()
  {
    reset_();    
  }

  std::string name_;
};


std::ostream& operator<<(std::ostream& ostr, const image2d& ima)
{
  ostr << ima.name_ << ": at=" << &ima << " proxy=" << ima.data().proxy();
  if (ima.data().proxy() != 0)
    std::cout << " " << *(ima.data().proxy());
  return ostr;
}

void display()
{
  std::set<const image2d*>::const_iterator i;
  for (i = env.begin(); i != env.end(); ++i)
    std::cout << **i << std::endl;
  std::cout << std::endl;
}


int main()
{
  image2d ima1("ima1", 1);
  image2d ima2("ima2", 2);
  image2d ima3("ima3");
  display();

  ima2 = ima1;
  display();

  ima2.clear();
  display();

//   ima = ima3;
//   display();

//   ima.clear();
//   display();
}
