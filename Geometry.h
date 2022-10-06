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

#endif