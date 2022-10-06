#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <array>

#include "Vector.h"

template <typename T>
bool isIntersect(const std::array<Vector<T, 3>, 3>& Triangle, const Vector<T, 3>& point)
{
	std::array<Vector<T, 3>, 3> TriangleEdge;
	TriangleEdge[0] = Triangle[1] - Triangle[0];
	TriangleEdge[1] = Triangle[2] - Triangle[1];
	TriangleEdge[2] = Triangle[0] - Triangle[2];

	std::array<Vector<T, 3>, 3> result;
	for (size_t i = 0; i < 3; i++)
	{
		result[i] = TriangleEdge[i]^(point - Triangle[i]);
	}
	return (result[0].z > 0.0 && result[1].z > 0.0 && result[2].z > 0.0) ||
		(result[0].z < 0.0 && result[1].z < 0.0 && result[2].z < 0.0);
}


template <typename T, typename N>
Vector<N, 3> BarycentricInterpol2D(const std::array<T, 3>& Triangle, const T& point)
{
	N alpha = ((point.y - Triangle[1].y) * (Triangle[2].x - Triangle[1].x) - (point.x - Triangle[1].x) * (Triangle[2].y - Triangle[1].y)) /
		((Triangle[0].y - Triangle[1].y) * (Triangle[2].x - Triangle[1].x) - (Triangle[0].x - Triangle[1]. x) * (Triangle[2].y - Triangle[1].y));
	N beta = ((point.y - Triangle[2].y) * (Triangle[0].x - Triangle[2].x) - (point.x - Triangle[2].x) * (Triangle[0].y - Triangle[2].y)) /
		((Triangle[1].y - Triangle[2].y) * (Triangle[0].x - Triangle[2].x) - (Triangle[1].x - Triangle[2]. x) * (Triangle[0].y - Triangle[2].y));
	N gamma = 1.0 - alpha - beta;
	if (alpha > 0.0 && beta > 0.0)
		return Vector<N, 3>(alpha, beta, gamma);
	else
		return Vector<N, 3>(-1, -1, -1);
}

//              p0 ------x---p1
template <typename T, typename N>
inline N LinearInterpol(const std::pair<T, N>& p0, const std::pair<T, N>& p1, const T& x)
{
	T t = (x - p0.first) / (p1.first - p0.first);
	return t * p0.second + (1 - t) * p1.second;
}
//            p2------------p3
//            |        |    |
//            |        |    |
//            |        p    |
//            |        |    |
//            |        |    |
//            p0------------p1
template <typename T, typename N>
inline N BilinearInterpol(const std::pair<Vector<T, 2>, N>& p0, const std::pair<Vector<T, 2>, N>& p1,
	const std::pair<Vector<T, 2>, N>& p2, const std::pair<Vector<T, 2>, N>& p3, const Vector<T, 2>& p)
{
	N n1 = LinearInterpol(std::pair<T, N>(p0.first.x, p0.second), std::pair<T, N>(p1.first.x, p1.second), p.x);
	N n2 = LinearInterpol(std::pair<T, N>(p2.first.x, p2.second), std::pair<T, N>(p3.first.x, p3.second), p.x);
	return LinearInterpol(std::pair<T, N>(p0.first.y, n1), std::pair<T, N>(p2.first.y, n2), p.y);
}

#endif