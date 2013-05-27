#include "matrix.hh"

Matrix::Matrix(unsigned h, unsigned w)
{
  height = h;
  width = w;
}

void Matrix::initialize()
{
  for (unsigned i = 0; i < height; ++i)
  {
    std::vector<float> line;

    for (unsigned j = 0; j < width; ++j)
      line.push_back(0);

    m.push_back(line);
  }
}

void Matrix::initialize(float *values)
{
  for (unsigned i = 0; i < height; ++i)
  {
    std::vector<float> line;

    for (unsigned j = 0; j < width; ++j)
      line.push_back(values[i * width + j]);

    m.push_back(line);
  }
}

void Matrix::print()
{
  for (unsigned i = 0; i < height; ++i)
  {
    std::cout << "|";

    for (unsigned j = 0; j < width; ++j)
      std::cout << " " << m.at(i).at(j);

    std::cout << " |" << std::endl;
  }
  std::cout << std::endl;
}


float Matrix::determinant()
{
  float d1 = m.at(0).at(0) * m.at(1).at(1) * m.at(2).at(2);
  float d2 = m.at(0).at(1) * m.at(1).at(2) * m.at(2).at(0);
  float d3 = m.at(0).at(2) * m.at(1).at(0) * m.at(2).at(1);

  float d4 = m.at(0).at(2) * m.at(1).at(1) * m.at(2).at(0);
  float d5 = m.at(1).at(2) * m.at(2).at(1) * m.at(0).at(0);
  float d6 = m.at(2).at(2) * m.at(0).at(1) * m.at(1).at(0);

  return (d1 + d2 + d3 - d4 - d5 - d6);
}

bool Matrix::inverse(Matrix& inversed)
{
  bool canInverse = false;

  float det = determinant();

  if (det != 0)
  {
    float a = m.at(0).at(0);
    float b = m.at(0).at(1);
    float c = m.at(0).at(2);

    float d = m.at(1).at(0);
    float e = m.at(1).at(1);
    float f = m.at(1).at(2);

    float g = m.at(2).at(0);
    float h = m.at(2).at(1);
    float i = m.at(2).at(2);

    Matrix comatrix(3, 3);
    comatrix.initialize();

    comatrix.set(0, 0, e*i - f*h);
    comatrix.set(0, 1, c*h - b*i);
    comatrix.set(0, 2, b*f - c*e);

    comatrix.set(1, 0, f*g - d*i);
    comatrix.set(1, 1, a*i - c*g);
    comatrix.set(1, 2, c*d - a*f);

    comatrix.set(2, 0, d*h - e*g);
    comatrix.set(2, 1, b*g - a*h);
    comatrix.set(2, 2, a*e - b*d);

    for (unsigned i = 0; i < comatrix.getWidth(); ++i)
      for (unsigned j = 0; j < comatrix.getHeight(); ++j)
        comatrix.set(i, j, comatrix.get(i,j) / det);

    comatrix.print();
    inversed = comatrix;

    canInverse = true;
  }

  return canInverse;
}

// Operators
Matrix Matrix::operator*(Matrix& right)
{
  Matrix result(height, right.getWidth());
  result.initialize();

  for (unsigned i = 0; i < height; ++i)
    for (unsigned j = 0; j < width; ++j)
      for (unsigned k = 0; k < right.getWidth(); ++k)
        result.add(i, k, m.at(i).at(j) * right.get(j, k));

  return result;
};

// Getters and Setters
unsigned Matrix::getHeight() { return height; }
unsigned Matrix::getWidth() { return width; }
float Matrix::get(unsigned i, unsigned j) { return m.at(i).at(j); }
void Matrix::set(unsigned i, unsigned j, float x) { m.at(i).at(j) = x; }
void Matrix::add(unsigned i, unsigned j, float x) { m.at(i).at(j) += x; }
