#ifndef METALIC_CONTRACT_HH
# define METALIC_CONTRACT_HH


# include <assert.h>

// FIXME: a lot of code has been removed here
// for simplification purpose only

# define assertion(expr)         assert(expr)
# define invariant(expr)         assert(expr)
# define precondition(expr)      assert(expr)
# define postcondition(expr)     assert(expr)


#endif // ! METALIC_CONTRACT_HH
