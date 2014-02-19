#ifndef __MAT_H__
#define __MAT_H__

//      N
//   ------
// M |    |
//   |    |
//   ------
template <class NumType, ushort M, ushort N>
class Mat {
 public:
 private:
  NumType x[M*N];
};

#endif
