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

    inversed = comatrix;

    canInverse = true;
  }

  return canInverse;
}

Matrix* Matrix::solve3x3(Matrix& b)
{
    Matrix* solution = NULL;
    float det = determinant();

    if (det > 0.1 || det < -0.1)
    {
        solution = new Matrix(3, 1);
        solution->initialize();

        float x = b.get(0, 0);
        float y = b.get(1, 0);
        float z = b.get(2, 0);

        Matrix m1(3, 3);
        m1.initialize();
        m1.set(0, 0, x);    m1.set(0, 1, m[0][1]);  m1.set(0, 2, m[0][2]);
        m1.set(1, 0, y);    m1.set(1, 1, m[1][1]);  m1.set(1, 2, m[1][2]);
        m1.set(2, 0, z);    m1.set(2, 1, m[2][1]);  m1.set(2, 2, m[2][2]);
        float d1 = m1.determinant();

        Matrix m2(3, 3);
        m2.initialize();
        m2.set(0, 0, m[0][0]);  m2.set(0, 1, x);    m2.set(0, 2, m[0][2]);
        m2.set(1, 0, m[1][0]);  m2.set(1, 1, y);    m2.set(1, 2, m[1][2]);
        m2.set(2, 0, m[2][0]);  m2.set(2, 1, z);    m2.set(2, 2, m[2][2]);
        float d2 = m2.determinant();

        Matrix m3(3, 3);
        m3.initialize();
        m3.set(0, 0, m[0][0]);  m3.set(0, 1, m[0][1]);  m3.set(0, 2, x);
        m3.set(1, 0, m[1][0]);  m3.set(1, 1, m[1][1]);  m3.set(1, 2, y);
        m3.set(2, 0, m[2][0]);  m3.set(2, 1, m[2][1]);  m3.set(2, 2, z);
        float d3 = m3.determinant();

        solution->set(0, 0, d1 / det);
        solution->set(1, 0, d2 / det);
        solution->set(2, 0, d3 / det);
    }

    return solution;
}

float Matrix::dotProduct(Matrix* vec)
{
    return (m[0][0] * vec->get(0, 0)
          + m[1][0] * vec->get(1, 0)
          + m[2][0] * vec->get(2, 0));
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
}

void Matrix::absolute()
{
  for (unsigned i = 0; i < height; ++i)
    for (unsigned j = 0; j < width; ++j)
      m.at(i).at(j) *= (m.at(i).at(j) >= 0) ? 1 : -1;
}

bool Matrix::isMajorOffset()
{
  unsigned i = 0;
  unsigned j = 0;
  bool isMajorOffset = false;

  while (!isMajorOffset && i < height)
  {
    while (!isMajorOffset && j < width)
    {
      isMajorOffset = m.at(i).at(j) > 0.5 || - m.at(i).at(j) > 0.5;
      ++j;
    }
    ++i;
  }

  return isMajorOffset;
}

// Getters and Setters
unsigned Matrix::getHeight() { return height; }
unsigned Matrix::getWidth() { return width; }
float Matrix::get(unsigned i, unsigned j) { return m.at(i).at(j); }
void Matrix::set(unsigned i, unsigned j, float x) { m.at(i).at(j) = x; }
void Matrix::add(unsigned i, unsigned j, float x) { m.at(i).at(j) += x; }
