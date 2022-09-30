//=====================================================================
//
// Geometry.h - 该文件参考了 tinyrender 的 Geometry.h 与 RenderHelp 的 RenderHelp.h
//
//=====================================================================

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <assert.h>
#include <initializer_list>

template<typename T, size_t N> struct Vector
{
	T data[N];
	inline Vector() { for (size_t i = 0; i < N; i++) data[i] = T(); }
	inline Vector(const Vector<T, N>& u) { for (size_t i = 0; i < N; i++) data[i] = u.data; }
	inline Vector(const std::initializer_list<T>& u) { auto it = u.begin(); for (size_t i = 0; i < N; i++) data[i] = *it++; }
	inline const T& operator[] (size_t i) const { assert(i < N); return data[i]; }
	inline T& operator[] (size_t i) { assert(i < N); return data[i]; }
};

template <typename T> struct Vector<T, 2>
{
	union
	{
		struct { T x, y; };    // 元素别名
		struct { T u, v; };    // 元素别名
		T data[2];                // 元素数组
	};
	inline Vector() : x(T()), y(T()) {}
	inline Vector(const T& X, const T& Y) : x(X), y(Y) {}
	inline Vector(const Vector<T, 2>& u) : x(u.x), y(u.y) {}
	inline const T& operator[] (size_t i) const { assert(i < 2); return data[i]; }
	inline T& operator[] (size_t i) { assert(i < 2); return data[i]; }
	inline T dot(const Vector<T, 2> &a) { return x * a.x + y * a.y; }
	inline T length() { return std::sqrt(this->dot(*this)); }
	inline const Vector<T, 2>& normalize() { (*this) /= this->length; return *this; }
};

template <typename T> struct Vector<T, 3> {
	union
	{
		struct { T x, y, z; };    // 元素别名
		struct { T r, g, b; };    // 元素别名
		T data[3];                   // 元素数组
	};
	inline Vector() : x(T()), y(T()), z(T()) {}
	inline Vector(const T& X, const T& Y, const T& Z) : x(X), y(Y), z(Z) {}
	inline Vector(const Vector<T, 3>& u) : x(u.x), y(u.y), z(u.z) {}
	inline Vector(const Vector<T, 2>& u, const T& Z) : x(u.x), y(u.y), z(Z) {}
	inline const T& operator[] (size_t i) const { assert(i < 3); return data[i]; }
	inline T& operator[] (size_t i) { assert(i < 3); return data[i]; }
	inline T dot(const Vector<T, 3> &a) { return x * a.x + y * a.y + z * a.z; }
	inline T length() { return std::sqrt(this->dot(*this)); }
	inline const Vector<T, 3>& normalize() { (*this) /= this->length; return *this; }
};

template <typename T> struct Vector<T, 4> {
	union
	{
		struct { T x, y, z, w; };    // 元素别名
		struct { T r, g, b, a; };    // 元素别名
		T data[4];                      // 元素数组
	};
	inline Vector() : x(T()), y(T()), z(T()), w(T()) {}
	inline Vector(const T& X, const T& Y, const T& Z, const T& W) : x(X), y(Y), z(Z), w(W) {}
	inline Vector(const Vector<T, 4>& u) : x(u.x), y(u.y), z(u.z), w(u.w) {}
	inline Vector(const Vector<T, 3>& u, const T& W) : x(u.x), y(u.y), z(u.z), w(W) {}
	inline const T& operator[] (size_t i) const { assert(i < 4); return data[i]; }
	inline T& operator[] (size_t i) { assert(i < 4); return data[i]; }
	inline T dot(const Vector<T, 3> &a) { return x * a.x + y * a.y + z * a.z + w * a.w; }
	inline T length() { return std::sqrt(this->dot(*this)); }
	inline const Vector<T, 4>& normalize() { (*this) /= this->length; return *this; }
};

template <typename T, size_t N>
inline Vector<T, N> operator+(const Vector<T, N>& a, const Vector<T, N>& b)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] + b[i];
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator-(const Vector<T, N>& a, const Vector<T, N>& b)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] - b[i];
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator*(const Vector<T, N>& a, const Vector<T, N>& b)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] * b[i];
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator*(const T& x, const Vector<T, N>& a)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] * x;
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator*(const Vector<T, N>& a, const T& x)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] * x;
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator/(const Vector<T, N>& a, const Vector<T, N>& b)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] / b[i];
	return c;
}

template <typename T, size_t N>
inline Vector<T, N> operator/(const Vector<T, N>& a, const T& x)
{
	Vector<T, N> c;
	for (size_t i = 0; i < N; i++) c[i] = a[i] / x;
	return c;
}

template <typename T, size_t N>
inline Vector<T, N>& operator/=(Vector<T, N>& a, const T &x)
{
	for (size_t i = 0; i < N; i++) a[i] /= x;
	return a;
}

template <typename T, size_t N>
inline bool operator==(const Vector<T, N>& a, const Vector<T, N>& b)
{
	for (size_t i = 0; i < N; i++) if (a[i] != b[i]) return false;
	return true;
}

template <typename T, size_t N>
inline bool operator!=(const Vector<T, N>& a, const Vector<T, N>& b)
{
	for (size_t i = 0; i < N; i++) if (a[i] == b[i]) return false;
	return true;
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