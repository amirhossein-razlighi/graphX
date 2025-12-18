#pragma once
#include <cmath>
#include <cassert>
#include <iostream>

template <int n>
struct vec
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

  double operator*(const vec<n> &otherVec) const {
    double dotProduct = 0.0;
    for (int i = 0; i < n; i++) {
      dotProduct += this->data[i] * otherVec.data[i];
    }
    return dotProduct;
  }

  vec<n> operator*(const double scalar) const {
    vec<n> result;
    for (int i = 0; i < n; i++) {
      result.data[i] = this->data[i] * scalar;
    }
    return result;
  }

  double l2norm() const {
    return (this * this);
  }

  double norm() const {
    return std::sqrt(l2norm());
  }
  vec<n> operator+(const vec<n> &otherVec) const {
    vec<n> result;
    for (int i = 0; i < n; i++) {
      result.data[i] = this->data[i] + otherVec.data[i];
    }
    return result; 
  }
  vec<n> operator-(const vec<n> &otherVec) const {
    vec<n> result;
    for (int i = 0; i < n; i++) {
      result.data[i] = this->data[i] - otherVec.data[i];
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
struct vec<3>
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
