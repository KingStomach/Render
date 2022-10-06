#ifndef _MARTEX_H_
#define _MARTEX_H_

#include <cmath>
#include <corecrt_math_defines.h>

#include "Vector.h"

template<typename T, size_t ROW, size_t COL>
struct Matrix
{
	Vector<Vector<T, COL>, ROW> data;

	Matrix();
	const Vector<T, COL>& getRow(size_t i) const;
	Vector<T, ROW> getCol(size_t i) const;
	void setRow(size_t i, const Vector<T, COL>& row);
	void setCol(size_t i, const Vector<T, ROW>& col);

	const Vector<T, COL>& operator[] (size_t i) const;
	Vector<T, COL>& operator[] (size_t i);

	Matrix<T, ROW, COL> operator+(const Matrix<T, ROW, COL>& matrix);
	Matrix<T, ROW, COL> operator-(const Matrix<T, ROW, COL>& matrix);
	Matrix<T, ROW, COL> operator*(const Matrix<T, ROW, COL>& matrix) const;
	Vector<T, COL> operator*(const Vector<T, COL>& v) const;
	Matrix<T, ROW, COL> operator/(const T& x) const;

};

template<typename T, size_t ROW, size_t COL>
Matrix<T, ROW, COL>::Matrix() {}

template<typename T, size_t ROW, size_t COL>
inline const Vector<T, COL>& Matrix<T, ROW, COL>::getRow(size_t i) const
{
	return data[i];
}

template<typename T, size_t ROW, size_t COL>
inline Vector<T, ROW> Matrix<T, ROW, COL>::getCol(size_t i) const
{
	Vector<T, ROW> t;
	for (size_t j = 0; j < ROW; j++)
		t[j] = data[j][i];
	return t;
}

template<typename T, size_t ROW, size_t COL>
inline void Matrix<T, ROW, COL>::setRow(size_t i, const Vector<T, COL>& row)
{
	data[i] = row;
}

template<typename T, size_t ROW, size_t COL>
inline void Matrix<T, ROW, COL>::setCol(size_t i, const Vector<T, ROW>& col)
{
	for (size_t j = 0; j < ROW; j++)
		data[j][i] = col[j];
}

template<typename T, size_t ROW, size_t COL>
inline const Vector<T, COL>& Matrix<T, ROW, COL>::operator[] (size_t i) const
{
	assert(i < ROW);
	return data[i];
}

template<typename T, size_t ROW, size_t COL>
inline Vector<T, COL>& Matrix<T, ROW, COL>::operator[] (size_t i)
{
	assert(i < ROW);
	return data[i];
}

template<typename T, size_t ROW, size_t COL>
inline Matrix<T, ROW, COL> Matrix<T, ROW, COL>::operator+(const Matrix<T, ROW, COL>& matrix)
{
	Matrix<T, ROW, COL> M;
	for (size_t i = 0; i < ROW; i++)
		for (size_t j = 0; j < COL; j++)
			M[i][j] = data[i][j] + matrix[i][j];
	return M;
}

template<typename T, size_t ROW, size_t COL>
inline Matrix<T, ROW, COL> Matrix<T, ROW, COL>::operator-(const Matrix<T, ROW, COL>& matrix)
{
	Matrix<T, ROW, COL> M;
	for (size_t i = 0; i < ROW; i++)
		for (size_t j = 0; j < COL; j++)
			M[i][j] = data[i][j] - matrix[i][j];
	return M;
}

template<typename T, size_t ROW, size_t COL>
inline Matrix<T, ROW, COL> Matrix<T, ROW, COL>::operator*(const Matrix<T, ROW, COL>& matrix) const
{
	Matrix<T, ROW, COL> M;
	for (size_t i = 0; i < ROW; i++)
		for (size_t j = 0; j < COL; j++)
			M[i][j] = this->getRow(i) * matrix.getCol(j);
	return M;
}

template<typename T, size_t ROW, size_t COL>
inline Vector<T, COL> Matrix<T, ROW, COL>::operator*(const Vector<T, COL>& v) const
{
	Vector<T, COL> t;
	for (size_t i = 0; i < COL; i++)
		t[i] = this->getRow(i) * v;
	return t;
}

template<typename T, size_t ROW, size_t COL>
inline Matrix<T, ROW, COL> operator*(const Matrix<T, ROW, COL>& matrix, const T& x)
{
	Matrix<T, ROW, COL> M;
	for (size_t i = 0; i < COL; i++)
		for (size_t j = 0; j < COL; j++)
			M[i][j] = matrix[i][j] * x;
	return M;
}

template<typename T, size_t ROW, size_t COL>
inline Matrix<T, ROW, COL> operator*(const T& x, const Matrix<T, ROW, COL>& matrix)
{
	Matrix<T, ROW, COL> M;
	for (size_t i = 0; i < COL; i++)
		for (size_t j = 0; j < COL; j++)
			M[i][j] = matrix[i][j] * x;
	return M;
}

template<typename T, size_t ROW, size_t COL>
Matrix<T, ROW, COL> Matrix<T, ROW, COL>::operator/(const T& x) const
{
	Matrix<T, ROW, COL> M;
	for (size_t i = 0; i < COL; i++)
		for (size_t j = 0; j < COL; j++)
			M[i][j] = data[i][j] / x;
	return M;
}

template<typename T, size_t N>
inline Matrix<T, N, N> identity()
{
	Matrix<T, N, N> M;
	for (size_t i = 0; i < N; i++)
		M[i][i] = 1;
	return M;
}

template<typename T>
inline Matrix<T, 4, 4> Scale(const Vector<T, 3>& vector)
{
	Matrix<T, 4, 4> M = identity<T, 4>();
	M[0][0] = vector.x;
	M[1][1] = vector.y;
	M[2][2] = vector.z;
	return M;
}

template<typename T>
inline Matrix<T, 4, 4> Translate(const Vector<T, 3>& position)
{
	Matrix<T, 4, 4> M = identity<T,4>();
	M.setCol(3, convert<T, 3, 4>(position, 1.0));
	return M;
}

template<typename T>
Matrix<T, 4, 4> LookAt(const Vector<T, 3>& position, const Vector<T, 3>& lookat, const Vector<T, 3>& up)
{
	Matrix<T, 4, 4> M1 = Translate(-position);
	Matrix<T, 4, 4> M2 = identity<T, 4>();
	Vector<T, 3> direction = (lookat - position).normalize();
	M2.setRow(0, convert<T, 3, 4>(direction ^ up, 0.0));
	M2.setRow(1, convert<T, 3, 4>(up, 0.0));
	M2.setRow(2, convert<T, 3, 4>(-direction, 0.0));
	return M2 * M1;
}

template<typename T>
Matrix<T, 4, 4> Ortho(const T& left, const T& right, const T& bottom, const T& top, const T& zNear, const T& zFar)
{
	Matrix<T, 4, 4> M1 = identity<T, 4>();
	Matrix<T, 4, 4> M2 = identity<T, 4>();
	M1[0][0] = 2.0 / (right - left);
	M1[1][1] = 2.0 / (top - bottom);
	M1[2][2] = 2.0 / (-zNear + zFar);
	M2.setCol(3, Vector<T, 4>(-(right + left) / 2.0, -(top + bottom) / 2.0, -(-zNear - zFar) / 2.0, 1.0));
	return M1 * M2;
}

template<typename T>
Matrix<T, 4, 4> Perspective(const T& fov, const T& aspect, const T& zNear, const T& zFar)
{
	T top = zNear * tan(M_PI * fov / 360.0), bottom = -top;
	T left = top * aspect, right = -left;
	Matrix<T, 4, 4> M;
	M[0][0] = -zNear;
	M[1][1] = -zNear;
	M[2][2] = -zNear - zFar;
	M[2][3] = -zNear * zFar;
	M[3][2] = 1.0;
	return Ortho(left, right, bottom, top, zNear, zFar) * M;
}


typedef Matrix<double, 4, 4> Mat4d;

#endif
