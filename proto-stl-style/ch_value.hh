#ifndef MINIOLN_CH_VALUE_HH
# define MINIOLN_CH_VALUE_HH


// fwd decls of concrete image types

template <class T> struct image1d;
template <class T> struct image2d;

template <class I, class N> struct with_nbh_;
template <class I, class M> struct with_imask_;
template <class I, class S> struct with_isubset_;
template <class I>          struct with_log_;



// ch_value only deals with concrete image types

template <class I, class T>
struct ch_value
{
};

template <class V, class T>
struct ch_value < image1d<V>, T >
{
  typedef image1d<T> ret;
};


template <class V, class T>
struct ch_value < image2d<V>, T >
{
  typedef image2d<T> ret;
};

template <class I, class N, class T>
struct ch_value < with_nbh_<I, N>, T >
{
  typedef with_nbh_<typename ch_value<I,T>::ret, N> ret;
};

template <class I, class M, class T>
struct ch_value < with_imask_<I, M>, T >
{
  typedef with_imask_<typename ch_value<I,T>::ret, M> ret;
};

template <class I, class S, class T>
struct ch_value < with_isubset_<I, S>, T >
{
  typedef with_isubset_<typename ch_value<I,T>::ret, S> ret;
};

template <class I, class T>
struct ch_value < with_log_<I>, T >
{
  typedef with_log_<typename ch_value<I,T>::ret> ret;
};




// safety: prevent calling ch_value on NON concrete image types

template <class I> struct rowof_;
template <class I, class F> struct through_fun_;
template <class I, class F> struct through_bijfun_;
template <class I, class A> struct attr_;

template <class I, class T>
struct ch_value < rowof_<I>, T >;

template <class I, class F, class T>
struct ch_value < through_fun_<I, F>, T >;

template <class I, class F, class T>
struct ch_value < through_bijfun_<I, F>, T >;

template <class I, class A, class T>
struct ch_value < attr_<I, A>, T >;


#endif
