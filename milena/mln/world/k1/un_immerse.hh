

  inline
  point2d
  un_immerse(const point2d& p)
  {
    point2d tmp(p.row() / 2, p.col() / 2);
    return tmp;
  }

  struct is_2_face_t : Function_v2b< is_2_face_t >
  {
    typedef bool result;
    inline bool operator()(const point2d& p) const { return FIXME; } // p.row() % 2 && p.col() % 2
  } is_2_face;

  struct is_not_2_face_t : Function_v2b< is_not_2_face_t >
  {
    typedef bool result;
    inline bool operator()(const point2d& p) const { return FIXME; } // ! (p.row() % 2 && p.col() % 2)
  } is_not_2_face;
