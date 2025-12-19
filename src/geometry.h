#pragma once
#include <cmath>
#include <cassert>
#include <iostream>

template <int n, typename Derived>
struct baseVec
{
  Derived &derived() { return static_cast<Derived &>(*this); }
  const Derived &derived() const { return static_cast<const Derived &>(*this); }

  double norm() const
  {
    double sum = 0.0;
    for (int i = 0; i < n; i++)
      sum += derived()[i] * derived()[i];
    return std::sqrt(sum);
  }
  double l2norm() const { return norm() * norm(); }

  Derived operator/(const double scalar) const
  {
    Derived result;
    for (int i = 0; i < n; i++)
      result[i] = derived()[i] / scalar;
    return result;
  }
  Derived operator*(const double scalar) const
  {
    Derived result;
    for (int i = 0; i < n; i++)
      result[i] = derived()[i] * scalar;
    return result;
  }
  Derived operator+(const Derived &other) const
  {
    Derived result;
    for (int i = 0; i < n; i++)
      result[i] = derived()[i] + other[i];
    return result;
  }
  Derived operator-(const Derived &other) const
  {
    Derived result;
    for (int i = 0; i < n; i++)
      result[i] = derived()[i] - other[i];
    return result;
  }
  double operator*(const Derived &other) const
  {
    double dot = 0.0;
    for (int i = 0; i < n; i++)
      dot += derived()[i] * other[i];
    return dot;
  }
};

template <int n>
struct vec : baseVec<n, vec<n>>
{
  double data[n] = {0};
  double &operator[](const int i)
  {
    assert(i >= 0 && i < n);
    return data[i];
  }
  double operator[](const int i) const
  {
    assert(i >= 0 && i < n);
    return data[i];
  }
};

template <int n, int m>
struct mat
{
  double data[n][m] = {{0}};
  vec<n> operator*(const vec<m> &v) const
  {
    vec<n> result;
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < m; j++)
      {
        result[i] += data[i][j] * v[j];
      }
    }
    return result;
  }
};

template <int n>
std::ostream &operator<<(std::ostream &out, const vec<n> &v)
{
  for (int i = 0; i < n; i++)
    out << v[i] << " ";
  return out;
}

template <>
struct vec<3> : baseVec<3, vec<3>>
{
  double x = 0, y = 0, z = 0;
  double &operator[](const int i)
  {
    assert(i >= 0 && i < 3);
    return i ? (1 == i ? y : z) : x;
  }
  double operator[](const int i) const
  {
    assert(i >= 0 && i < 3);
    return i ? (1 == i ? y : z) : x;
  }
};

typedef vec<3> vec3;
