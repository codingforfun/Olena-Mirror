// Copyright (C) 2005 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef METALIC_TRACKED_PTR_HH
# define METALIC_TRACKED_PTR_HH

# include <iostream>
# include <set>
# include <map>

# include <mlc/contract.hh>

namespace mlc {


  template <typename T> class tracked_ptr; // fwd decl


  namespace internal {


    /*! \class tracked_ptr_proxy<T>
    **
    ** Class that effectively holds a pointer.  For use in class
    ** tracked_ptr.
    **
    ** Parameter T is the type of pointed data.
    */
    
    template <class T>
    struct /* HERE: class */ tracked_ptr_proxy
    {

      friend class tracked_ptr<T>;


      /// Attributes.

      T* ptr_;
      std::set<const tracked_ptr<T>*> holders_;


      /// Cpy ctor is not impled.
      tracked_ptr_proxy(const tracked_ptr_proxy&);

      /// Assignmt op is not impled.
      void operator=(const tracked_ptr_proxy&);


      /*! \brief Ctor.
      **
      ** We have a new proxy so we have to make sure that the pointer,
      ** if not null, was not already proxified, meaning that this new
      ** proxy handles 'new' data.
      */
      tracked_ptr_proxy(T* ptr) :
	ptr_(0),
	holders_()
      {
	if (ptr != 0)
	  {
	    invariant(proxy_of()[ptr] == 0);
	    this->ptr_ = ptr;
	    proxy_of()[this->ptr_] = this;
	  }
	else
	  this->ptr_ = 0;
      }


      /*! \brief Change data pointer.
      **
      ** If the change is effective, former data are deleted.
      */
      void set_ptr(T* ptr)
      {
	if (ptr == this->ptr_)
	  {
	    // nothing to be done
	    return;
	  }

	if (this->ptr_ != 0)
	  {
	    invariant(proxy_of()[this->ptr_] == this);
	    proxy_of().erase(this->ptr_);
	    delete this->ptr_;
	    this->ptr_ = 0; // safety
	  };

	// same code as ctor:
	if (ptr != 0)
	  {
	    invariant(proxy_of()[ptr] == 0);
	    this->ptr_ = ptr;
	    proxy_of()[this->ptr_] = this;
	  }
	else
	  this->ptr_ = 0;
      }


      /*! \brief Register a new holder.
      **
      ** \precondition Test that it is effectively a new holder.
      */
      void register_holder(const tracked_ptr<T>* holder)
      {
	precondition(holder != 0);
	precondition(holders_.find(holder) == holders_.end());
	holders_.insert(holder);
      }


      /*! \brief Unregister a holder.
      **
      ** If it is the last holder and if data is not null, data are
      ** deleted.  In that case, the return value (true) allows the
      ** tracked_pointer to know that the proxy has to be deleted.
      **
      ** \precondition Test that it is effectively a holder.
      ** \return True if data is deleted, false otherwise
      */

      bool unregister_holder(const tracked_ptr<T>* holder)
      {
	// "simple" tests:
	precondition(holder != 0);
	precondition(holders_.size() > 0);

	// "main" test:
	// note that the couple of "simple" tests above are subtests of the following one
	typename std::set<const tracked_ptr<T>*>::iterator h = holders_.find(holder);
	precondition(h != holders_.end());

	holders_.erase(h);

	if (holders_.size() == 0 && this->ptr_ != 0)
	  {
	    proxy_of()[this->ptr_] = 0;
	    delete this->ptr_;
	    this->ptr_ = 0; // safety
	    return true;
	  }

	return false;
      }


      // procedures

      typedef std::map<const T*, const tracked_ptr_proxy*> map_type;

      static map_type& proxy_of()
      {
	static map_type* proxy_of_ = new map_type;
	return *proxy_of_;
      }

      friend
      std::ostream& operator<<(std::ostream& ostr, const tracked_ptr_proxy& proxy)
      {
	ostr << "proxy " << &proxy << " [ptr=" << proxy.ptr_ << " holders=(";
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

  } // end of namespace internal





  template <typename T>
  class tracked_ptr
  {
  public:

    /*! \brief Mimics the behavior of op* for a pointer in the const case.
    **
    ** \invariant Pointer proxy exists.
    ** \precondition Data exists (is not null).
    */
    const T& operator*() const
    {
      invariant(proxy_ != 0);
      precondition(proxy_->ptr_ != 0);
      return *(proxy_->ptr_);
    }

    /*! \brief Mimics the behavior of op* for a pointer in the mutable case.
    **
    ** \invariant Pointer proxy exists.
    ** \precondition Data exists (is not null).
    */
    T& operator*()
    {
      invariant(proxy_ != 0);
      precondition(proxy_->ptr_ != 0);
      return *(proxy_->ptr_);
    }

    /*! \brief Mimics the behavior of op-> for a pointer in the const case.
    **
    ** \invariant Pointer proxy exists.
    */
    const T*const operator->() const
    {
      invariant(proxy_ != 0);
      precondition(proxy_->ptr_ != 0);
      return proxy_->ptr_;
    }

    /*! \brief Mimics the behavior of op-> for a pointer in the mutable case.
    **
    ** \invariant Pointer proxy exists.
    */
    T*const operator->()
    {
      invariant(proxy_ != 0);
      precondition(proxy_->ptr_ != 0);
      return proxy_->ptr_;
    }

    /// Coercion towards Boolean (for arithmetical tests).
    operator bool() const
    {
      invariant(proxy_ != 0);
      return proxy_->ptr_ != 0;
    }

    /// Negation (for arithmetical tests).
    bool operator !() const
    {
      invariant(proxy_ != 0);
      return ! bool(*this);
    }

    /// Comparison 'equal to' (for arithmetical tests).
    bool operator==(int b) const
    {
      invariant(proxy_ != 0);
      return bool(*this) == bool(b);
    }

    /// Comparison 'not equal to' (for arithmetical tests).
    bool operator!=(int b) const
    {
      invariant(proxy_ != 0);
      return bool(*this) != bool(b);
    }


    /// Hook that gives access to the data proxy.
    const internal::tracked_ptr_proxy<T>* proxy() const { return this->proxy_; }
          internal::tracked_ptr_proxy<T>* proxy()       { return this->proxy_; }



    /*! \brief Ctor.
    **
    ** Creates a proxy that knows the target data pointer.  This
    ** pointer can be null (0).
    */
    tracked_ptr(T* ptr = 0) :
      proxy_(new internal::tracked_ptr_proxy<T>(ptr))
    {
      this->proxy_->register_holder(this);
      invariant_();
    }


    /*! \brief Cpy ctor performs a shallow copy.
    **
    ** The data proxy of \a rhs is shared with the constructed
    ** tracked_ptr.
    */
    tracked_ptr(const tracked_ptr& rhs) :
      proxy_(rhs.proxy_)
    {
      precondition(rhs.proxy_ != 0);
      this->proxy_->register_holder(this);
      invariant_();
    }


    /*! \brief Assignment operator with rhs being a tracked pointer.
    **
    ** The lhs proxy unregisters its data and registers the rhs data.
    ** In the case of both pointers have the same value, no action is
    ** performed and a warning is produced.  The unregistration
    ** process may desallocate data if the target pointer was the only
    ** access to those data.
    */
    tracked_ptr& operator=(const tracked_ptr& rhs)
    {
      invariant(this->proxy_ != 0);
      precondition(rhs.proxy_ != 0);

      invariant_(); // safety

      if (rhs.proxy_ == this->proxy_)
	{
	  std::cerr << "warning: nothing done cause both objects share the same data!" << std::endl;
	  // FIXME: say more
	  // e.g.: ima1 = ima2; ima2 = ima1;
	  return *this;
	}

      if (this->proxy_->unregister_holder(this))
	{
	  // the current 'tracked pointer' was the only access to
	  // some data (this->proxy_->ptr_); data has been
	  // desallocated, so it goes the same for the proxy:
	  delete this->proxy_;
	}

      this->proxy_ = rhs.proxy_;
      this->proxy_->register_holder(this);

      invariant_();
      return *this;
    }


    /*! \brief Assignment operator with rhs being a data pointer.
    **
    ** This assignment just changes the pointed data.
    */
    tracked_ptr& operator=(T* ptr)
    {
      invariant(this->proxy_ != 0);
      this->proxy_->set_ptr(ptr);
      return *this;
    }


    // Dtor.

    ~tracked_ptr()
    {
      invariant(this->proxy_ != 0);
      if (this->proxy_->unregister_holder(this))
	{
	  // the current 'tracked pointer' was the only access to
	  // some data (this->proxy_->ptr_); data has been
	  // desallocated, so it goes the same for the proxy:
	  delete this->proxy_;
	}
      this->proxy_ = 0; // safety
    }


  private:

    /// Only attribute.

    internal::tracked_ptr_proxy<T>* proxy_;

    void invariant_() const
    {
      if (proxy_ == 0)
	return;
      const std::set<const tracked_ptr<T>*>& holders = proxy_->holders_;
      typename std::set<const tracked_ptr<T>*>::const_iterator i;
      for (i = holders.begin(); i != holders.end(); ++i)
	assert((*i)->proxy_ == this->proxy_);
    }

    friend
    std::ostream& operator<<(std::ostream& ostr, const tracked_ptr& ptr)
    {
      invariant(ptr.proxy_ != 0);
      return ostr << "tracked_ptr " << &ptr << " --> " << *(ptr.proxy_);
    }
  };
  

} // end of namespace mlc



#endif // ! METALIC_TRACKED_PTR_HH
