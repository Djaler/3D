#ifndef MATRIX_H
#define MATRIX_H

#include "vertex.h"

class Matrix3
{
	float values[9];

	public:
		Matrix3(float values_[9])
		{
			for(int i=0; i<9;i++)
			{
				values[i]=values_[i];
			}
		}
		Matrix3 multiply(Matrix3 other)
		{
			float result[9];
			for (int row = 0; row < 3; row++)
			{
				for (int col = 0; col < 3; col++)
				{
					result[row * 3 + col] = 0;
					for (int i = 0; i < 3; i++)
					{
						result[row * 3 + col] += values[row * 3 + i] * other.values[i * 3 + col];
					}
				}
			}
			return Matrix3(result);
		}
		Vertex transform(Vertex in)
		{
			return Vertex(	in.x * values[0] + in.y * values[3] + in.z * values[6],
							in.x * values[1] + in.y * values[4] + in.z * values[7],
							in.x * values[2] + in.y * values[5] + in.z * values[8]);
		}
};

#endif // MATRIX_H
