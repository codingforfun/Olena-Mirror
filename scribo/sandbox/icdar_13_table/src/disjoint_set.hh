#ifndef DISJOINT_SET_HH
# define DISJOINT_SET_HH

/// \file
/// Implementation of a disjoint-set forest.

# include <limits>
# include <vector>

template <typename T>
struct disjoint_set
{
  disjoint_set(size_t size)
  // The maximum value of T is considered an ``invalid value''.
  : parent(size, std::numeric_limits<T>::max())
  {
    for (unsigned i = 0; i < parent.size(); ++i)
      make_set(i);
  }

  /// Create initial sets (singletons).
  void make_set(T x)
  {
    parent[x] = x;
  }

  bool is_root(T x)
  {
    return parent[x] == x;
  }

  /// Find the root of the set containing \a x.
  T find(T x)
  {
    if (!is_root(x))
      // Path compression.
      parent[x] = find(parent[x]);
    return parent[x];
  }

  /// Merge the set containing \a y into the set containing \a x.
  void make_union(T x, T y)
  {
    T x_root = find(x);
    T y_root = find(y);
    parent[y_root] = x_root;
  }

  /// Parent relationship.  A node with is its own parent is a
  /// root (representative of a component).
  std::vector<T> parent;
};

#endif // ! DISJOINT_SET_HH
