#ifndef MATRIX_H
#define MATRIX_H

class Matrix
{
	float values[9];

	public:
		Matrix(float *values_)
		{
			for(int i = 0; i < 9; i++)
			{
				values[i] = values_[i];
			}
		}

		Matrix multiply(Matrix other)
		{
			float result[9];
			for (int row = 0; row < 3; row++)
			{
				for (int col = 0; col < 3; col++)
				{
					result[row * 3 + col] = 0;
					for (int i = 0; i < 3; i++)
					{
						result[row * 3 + col] += values[row * 3 + i] * other[i * 3 + col];
					}
				}
			}
			return Matrix(result);
		}

		float& operator [] (int n)
		{
			return values[n];
		}
};

#endif // MATRIX_H
