
#include <assert.h>
#include <iostream>
#include <string>
#include <set>


struct array2d
{
  friend
  std::ostream& operator<<(std::ostream& ostr, const array2d& arr)
  {
    return ostr << "array2d at " << &arr;
  }

  array2d()
  {
//     std::cout << "+ " << *this << std::endl;
  }
  ~array2d()
  {
//     std::cout << "- " << *this << std::endl;
  }
  // w/o impl:
  array2d(const array2d&);
  void operator=(const array2d&);
};



template <typename T> class data_holder; // fwd decl



// data_tracker


template <class T>
class data_tracker
{
public:

  operator const T*() const
  {
    return data_;
  }

  operator T*()
  {
    return data_;
  }

  const T& operator*() const
  {
    assert(data_ != 0);
    return *data_;
  }

  T& operator*()
  {
    assert(data_ != 0);
    return *data_;
  }

  const T*const operator->() const
  {
    assert(data_ != 0);
    return data_;
  }

  T*const operator->()
  {
    assert(data_ != 0);
    return data_;
  }

private:

  friend class data_holder<T>;

  // attributes

  T* data_;
  std::set<const void*> holders_;

  // w/o impl

  data_tracker(const data_tracker&);
  void operator=(const data_tracker&);

  // methods

  data_tracker() :
    data_(0)
  {}

  data_tracker(T* data) :
    data_(data)
  {
  }

  void register_holder(const void* holder)
  {
    assert(holder != 0);
    assert(holders_.find(holder) == holders_.end());
    holders_.insert(holder);
  }

  bool unregister_holder(const void* holder)
  {
    assert(holder != 0);
    assert(holders_.size() > 0);

    std::set<const void*>::iterator e = holders_.find(holder);
    assert(e != holders_.end());
    holders_.erase(e);

    if (holders_.size() == 0)
      {
	delete data_;
	return true;
      }
    return false;
  }

  // procedure

  friend
  std::ostream& operator<<(std::ostream& ostr, const data_tracker& tracker)
  {
    ostr << "[data=" << tracker.data_ << " holders=(";
    std::set<const void*>::const_iterator i = tracker.holders_.begin();
    bool ok = i != tracker.holders_.end();
    while (ok)
      {
	ostr << *i++;
	if (ok = i != tracker.holders_.end())
	  ostr << ", ";
      }
    ostr << ")]";
    return ostr;
  }

};





// data_holder


template <typename T>
class data_holder
{
public:

  const T& operator*() const
  {
    assert(tracker_ != 0);
    return tracker_->operator*();
  }

  T& operator*()
  {
    assert(tracker_ != 0);
    return tracker_->operator*();
  }

  const T*const operator->() const
  {
    assert(tracker_ != 0);
    return tracker_->operator*();
  }

  T*const operator->()
  {
    assert(tracker_ != 0);
    return tracker_->operator*();
  }

  operator bool() const
  {
    return tracker_ != 0;
  }

  bool operator==(int b) const
  {
    return bool(tracker_) == bool(b);    
  }

  bool operator!=(int b) const
  {
    return bool(tracker_) != bool(b);
  }

  bool operator not() const
  {
    return not this->operator bool();
  }


  // hook:
  const data_tracker<T>* tracker() const { return tracker_; }


protected:

  // ctors

  data_holder() :
    tracker_(0)
  {
  }

  data_holder(T* data) :
    tracker_(new data_tracker<T>(data))
  {
    tracker_->register_holder(this);
  }

  // shallow copy

  data_holder(const data_holder& rhs) :
    tracker_(rhs.tracker_)
  {
    if (tracker_ != 0)
      tracker_->register_holder(this);
  }

  // assignment

  data_holder& operator=(const data_holder& rhs)
  {
    if (rhs.tracker_ == this->tracker_)
      return *this;
    if (this->release_tracker_())
      delete this->tracker_;
    this->tracker_ = rhs.tracker_;
    this->tracker_->register_holder(this);
    return *this;
  }

//   data_holder& operator=(T* ptr)
//   {
//     this->release_tracker_(); // FIXME: is it ok?
//     this->tracker_->data_ = ptr;
//     this->tracker_->register_holder(this);
//     return *this;
//   }

  // dtor

  ~data_holder()
  {
    if (release_tracker_())
      delete tracker_;
    tracker_ = 0;
  }

  // data access

  const data_holder<T>& data_() const
  {
    return *this;
  }

  data_holder<T>& data_()
  {
    return *this;
  }

private:

  data_tracker<T>* tracker_;

  bool release_tracker_()
  {
    if (tracker_ == 0)
      return false;
    return tracker_->unregister_holder(this);
  }
};



// env

struct image2d;
std::set<const image2d*> env;



// image2d


struct image2d : private data_holder<array2d>
{
  typedef data_holder<array2d> super;
  typedef data_holder<array2d> data_ptr_type;

  const data_ptr_type& data() const
  {
    return data_();
  }

  data_ptr_type& data_ptr()
  {
    return data_();
  }

  image2d(const std::string& name) :
    super(),
    name_(name)
  {
    env.insert(this);
  }

  image2d(const std::string& name, int) :
    super(new array2d),
    name_(name)
  {
    env.insert(this);
  }

  image2d(const image2d& rhs)
    // should call super cpy ctor
  {
    this->name_ = std::string("cpy_of_") + rhs.name_;
    env.insert(this);
  }

  image2d operator=(const image2d& rhs)
  {
    if (&rhs == this)
      return *this;
    super::operator=(rhs);
    // this->name_ is unchanged
    return *this;
  }

  ~image2d()
    // should call super dtor
  {
    env.erase(this);
  }

  std::string name_;
};


std::ostream& operator<<(std::ostream& ostr, const image2d& ima)
{
  ostr << ima.name_ << ": at=" << &ima << " tracker=" << ima.data().tracker();
  if (ima.data().tracker() != 0)
    std::cout << " " << *(ima.data().tracker());
  return ostr;
}

void display()
{
  std::set<const image2d*>::const_iterator i;
  for (i = env.begin(); i != env.end(); ++i)
    std::cout << **i << std::endl;
}


int main()
{
  image2d ima("ima");
  image2d ima2("ima2", 2);

  ima = ima2;

  if (ima.data())
    std::cout << *ima.data() << std::endl;

  display();
}
