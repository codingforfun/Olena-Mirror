#ifndef RUBY_STREAM_HH
#define RUBY_STREAM_HH

namespace ruby
{

  struct eval_type {};
  eval_type eval;

  struct stream
  {
    stream() : last_value_(Qnil) {}

    void
    push(const eval_type&)
    {
      eval();
    }

    template <class T>
    void
    push(const T& obj)
    {
      contents_ << obj;
    }

    VALUE
    eval()
    {
      if (contents_.str().size() != 0)
      {
        // std::cout << "Eval (" << contents_.str() << ")" << std::endl;
        last_value_ = rb_eval_string(contents_.str().c_str());
        contents_.str(std::string());
      }
      return last_value_;
    }

    VALUE last_value() { return last_value_; }

    protected:

    std::ostringstream  contents_;
    VALUE               last_value_;
  };

}

template <class T>
ruby::stream& operator<< (ruby::stream& stream, const T& obj)
{
  stream.push(obj);
  return stream;
}

#endif
