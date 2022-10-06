#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <assert.h>
#include <cmath>
#include <initializer_list>

template<typename T, size_t N>
struct Vector
{
	T data[N];

	Vector();
	Vector(const Vector<T, N>& u);
	Vector(const std::initializer_list<T>& u);
	const T& operator[] (size_t i) const;
	T& operator[] (size_t i);
};

template <typename T>
struct Vector<T, 2>
{
	union
	{
		struct { T x, y; };    // 元素别名
		struct { T u, v; };    // 元素别名
		T data[2];                // 元素数组
	};

	Vector();
	Vector(const T& X, const T& Y);
	Vector(const Vector<T, 2>& u);
	const T& operator[] (size_t i) const;
	T& operator[] (size_t i);
	double length();
	Vector<T, 2>& normalize();
};

template <typename T>
struct Vector<T, 3> {
	union
	{
		struct { T x, y, z; };    // 元素别名
		struct { T r, g, b; };    // 元素别名
		T data[3];                   // 元素数组
	};

	Vector();
	Vector(const T& X, const T& Y, const T& Z);
	Vector(const Vector<T, 3>& u);
	const T& operator[] (size_t i) const;
	T& operator[] (size_t i);
	double length();
	Vector<T, 3>& normalize();
};

template <typename T>
struct Vector<T, 4> {
	union
	{
		struct { T x, y, z, w; };    // 元素别名
		struct { T r, g, b, a; };    // 元素别名
		T data[4];                      // 元素数组
	};

	Vector();
	Vector(const T& X, const T& Y, const T& Z, const T& W);
	Vector(const Vector<T, 4>& u);
	const T& operator[] (size_t i) const;
	T& operator[] (size_t i);
	double length();
	Vector<T, 4>& normalize();
};


// Definition of Vector<T, N>
template<typename T, size_t N>
Vector<T, N>::Vector()
{
	for (size_t i = 0; i < N; i++)
		data[i] = T();
}

template<typename T, size_t N>
Vector<T, N>::Vector(const Vector<T, N>& u)
{
	for (size_t i = 0; i < N; i++)
		data[i] = u.data;
}

template<typename T, size_t N>
Vector<T, N>::Vector(const std::initializer_list<T>& u)
{
	auto it = u.begin();
	for (size_t i = 0; i < N; i++)
		data[i] = *it++;
}

template<typename T, size_t N>
inline const T& Vector<T, N>::operator[] (size_t i) const
{
	assert(i < N);
	return data[i];
}

template<typename T, size_t N>
inline T& Vector<T, N>::operator[] (size_t i)
{
	assert(i < N);
	return data[i];
}


// Definition of Vector<T, 2>
template<typename T>
Vector<T, 2>::Vector() : x(T()), y(T()) {}

template<typename T>
Vector<T, 2>::Vector(const T& X, const T& Y) : x(X), y(Y) {}

template<typename T>
Vector<T, 2>::Vector(const Vector<T, 2>& u) : x(u.x), y(u.y) {}

template<typename T>
inline const T& Vector<T, 2>::operator[] (size_t i) const
{
	assert(i < 2);
	return data[i];
}

template<typename T>
inline T& Vector<T, 2>::operator[] (size_t i)
{
	assert(i < 2);
	return data[i];
}

template<typename T>
inline double Vector<T, 2>::length()
{
	return std::sqrt((*this) * (*this));
}

template<typename T>
inline Vector<T, 2>& Vector<T, 2>::normalize()
{
	(*this) /= this->length();
	return *this;
}


// Definition of Vector<T, 3>
template<typename T>
Vector<T, 3>::Vector() : x(T()), y(T()), z(T()) {}

template<typename T>
Vector<T, 3>::Vector(const T& X, const T& Y, const T& Z) : x(X), y(Y), z(Z) {}

template<typename T>
Vector<T, 3>::Vector(const Vector<T, 3>& u) : x(u.x), y(u.y), z(u.z) {}

template<typename T>
inline const T& Vector<T, 3>::operator[] (size_t i) const
{
	assert(i < 3);
	return data[i];
}

template<typename T>
inline T& Vector<T, 3>::operator[] (size_t i)
{
	assert(i < 3);
	return data[i];
}

template<typename T>
inline double Vector<T, 3>::length()
{
	return std::sqrt((*this) * (*this));
}

template<typename T>
inline Vector<T, 3>& Vector<T, 3>::normalize()
{
	(*this) /= this->length();
	return *this;
}


// Definition of Vector<T, 4>
template<typename T>
Vector<T, 4>::Vector() : x(T()), y(T()), z(T()), w(T()) {}

template<typename T>
Vector<T, 4>::Vector(const T& X, const T& Y, const T& Z, const T& W) : x(X), y(Y), z(Z), w(W) {}

template<typename T>
Vector<T, 4>::Vector(const Vector<T, 4>& u) : x(u.x), y(u.y), z(u.z), w(u.w) {}

template<typename T>
inline const T& Vector<T, 4>::operator[] (size_t i) const
{
	assert(i < 4);
	return data[i];
}

template<typename T>
inline T& Vector<T, 4>::operator[] (size_t i)
{
	assert(i < 4);
	return data[i];
}

template<typename T>
inline double Vector<T, 4>::length()
{
	return std::sqrt((*this) * (*this));
}

template<typename T>
inline Vector<T, 4>& Vector<T, 4>::normalize()
{
	(*this) /= this->length();
	return *this;
}


// Definition of operator for all template
template <typename T, size_t N>
inline Vector<T, N> operator+(const Vector<T, N>& a, const Vector<T, N>& b)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++)
		c[i] = a[i] + b[i];
	return c;
}

template <typename T, size_t N>
inline Vector<T, N>& operator+=(const Vector<T, N>& a, const Vector<T, N>& b)
{
	for (size_t i = 0; i < N; i++)
		a[i] += b[i];
	return a;
}

template <typename T, size_t N>
inline Vector<T, N> operator-(const Vector<T, N>& a, const Vector<T, N>& b)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++)
		c[i] = a[i] - b[i];
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator-(const Vector<T, N>& a)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++)
		c[i] = -a[i];
	return c;
}

template <typename T, size_t N>
inline Vector<T, N>& operator-=(const Vector<T, N>& a, const Vector<T, N>& b)
{
	for (size_t i = 0; i < N; i++)
		a[i] -= b[i];
	return a;
}

/*
template <typename T, size_t N>
inline Vector<T, N> operator*(const Vector<T, N>& a, const Vector<T, N>& b)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++)
		c[i] = a[i] * b[i];
	return c;
}
*/

template <typename T, size_t N>
inline double operator*(const Vector<T, N>& a, const Vector<T, N>& b)
{
	double result = 0;
	for (size_t i = 0; i < N; i++)
		result += a[i] * b[i];
	return result;
}

template <typename T, size_t N>
inline Vector<T, N> operator*(const T& x, const Vector<T, N>& a)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++)
		c[i] = a[i] * x;
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator*(const Vector<T, N>& a, const T& x)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++)
		c[i] = a[i] * x;
	return c;
}

template <typename T, size_t N>
inline Vector<T, N>& operator*=(const Vector<T, N>& a, const T& x)
{
	for (size_t i = 0; i < N; i++)
		a[i] *= x;
	return a;
}

template <typename T, size_t N>
inline Vector<T, N> operator/(const Vector<T, N>& a, const Vector<T, N>& b)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++)
		c[i] = a[i] / b[i];
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator/(const Vector<T, N>& a, const T& x)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++)
		c[i] = a[i] / x;
	return c;
}

template <typename T, size_t N>
inline Vector<T, N>& operator/=(Vector<T, N>& a, const T& x)
{
	for (size_t i = 0; i < N; i++)
		a[i] /= x;
	return a;
}

template <typename T>
inline Vector<T, 3> operator^(const Vector<T, 3>& a, const Vector<T, 3>& b)
{
	return Vector<T, 3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

template <typename T, size_t N>
inline bool operator==(const Vector<T, N>& a, const Vector<T, N>& b)
{
	for (size_t i = 0; i < N; i++)
		if (a[i] != b[i])
			return false;
	return true;
}

template <typename T, size_t N>
inline bool operator!=(const Vector<T, N>& a, const Vector<T, N>& b)
{
	for (size_t i = 0; i < N; i++)
		if (a[i] == b[i])
			return false;
	return true;
}

template <typename T, size_t N1, size_t N2>
inline Vector<T, N2> convert(const Vector<T, N1>& a, const T& fill = T())
{
	Vector<T, N2> b;
	for (size_t i = 0; i < N2; i++)
	{
		if (i < N1)
			b[i] = a[i];
		else
			b[i] = fill;
	}
	return b;
}

typedef Vector<int, 2> Vec2i;
typedef Vector<float, 2> Vec2f;
typedef Vector<double, 2> Vec2d;
typedef Vector<int, 3> Vec3i;
typedef Vector<float, 3> Vec3f;
typedef Vector<double, 3> Vec3d;
typedef Vector<int, 4> Vec4i;
typedef Vector<float, 4> Vec4f;
typedef Vector<double, 4> Vec4d;

#endif