#ifndef OLENA_CORE_MACROS_HH
# define OLENA_CORE_MACROS_HH

# include <oln/core/cats.hh>
# include <oln/core/props.hh>



// FIXME: some crucial (meta-)work to be done here!
//
// first, introduce a piece of meta-code to get the proper abstraction
// from parameter; e.g., if I is an abstract::image then abstraction
// is abstract::image else etc.
//
// second, in macro below, there should be an exact_type from I somewhere
// cause we cannot be sure that I is an exact type; just think about it...





// for oln::abstract::image
//--------------------------

// unary macros

# define oln_delegated_type(T) typename oln::props<oln_category_type(T),T>::delegated_type

# define oln_size_type(T)  typename oln::props<oln_category_type(T),T>::size_type
# define oln_point_type(T) typename oln::props<oln_category_type(T),T>::point_type
# define oln_value_type(T) typename oln::props<oln_category_type(T),T>::value_type

# define oln_iter_type(T) typename oln::props<oln_category_type(T),T>::iter_type

# define oln_value_storage_type(T)  typename oln::props<oln_category_type(T),T>::value_storage_type
# define oln_value_container_type(T)  typename oln::props<oln_category_type(T),T>::value_container_type

# define oln_piter_type(T) typename oln::props<oln_category_type(T),T>::piter_type
# define oln_fwd_piter_type(T) typename oln::props<oln_category_type(T),T>::fwd_piter_type

// binary macros

# define oln_ch_value_type(I,V) typename oln::props<oln_category_type(I),I>::ch_value_type<V>::ret


// for oln::abstract::data_storage

# define oln_data_type(T)  typename oln::props<oln_category_type(T),T>::data_type



// generic macros
//----------------

# define oln_typeof( FROM_TYPE, WHAT_TYPE ) typename oln::props<oln_category_type(FROM_TYPE),FROM_TYPE>::WHAT_TYPE##_type



#endif // ! OLENA_CORE_MACROS_HH
