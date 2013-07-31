#ifndef MATRIX_HH
# define MATRIX_HH

# include <iostream>
# include <vector>

class Matrix
{
  public:
    // Constructor
    Matrix(unsigned i, unsigned j);

    void initialize();
    void initialize(float *values);
    void print();
    bool inverse(Matrix& inversed);
    void absolute();
    bool isMajorOffset();
    float determinant();
    Matrix* solve3x3(Matrix& b);
    float dotProduct(Matrix* vec);

    // Operators
    Matrix operator*(Matrix& right);

    // Getters and Setters
    unsigned getHeight();
    unsigned getWidth();
    float get(unsigned i, unsigned j);
    void set(unsigned i, unsigned j, float value);
    void add(unsigned i, unsigned j, float value);

  private:
    std::vector< std::vector<float> > m;
    unsigned height;
    unsigned width;
};

#endif /* ! MATRIX_HH */
