#pragma once

#include <QtWidgets>
#include "vector.h"

void drawTriangle(Vec3 v1, Vec3 v2, Vec3 v3, QColor color, QImage *image, float *zBuffer, int width, int height)
{
	int minX = max(0.0f, ceil(min(v1.x, min(v2.x, v3.x))));
	int maxX = min(width - 1.0f, floor(max(v1.x, max(v2.x, v3.x))));
	int minY = max(0.0f, ceil(min(v1.y, min(v2.y, v3.y))));
	int maxY = min(height - 1.0f, floor(max(v1.y, max(v2.y, v3.y))));

	float triangleArea = (v1.y - v3.y) * (v2.x - v3.x) + (v2.y - v3.y) * (v3.x - v1.x);

	QRgb *rowData = (QRgb*)image->scanLine(minY);
	int stride = image->bytesPerLine() / 4;
	for (int y = minY; y <= maxY; y++, rowData += stride)
	{
		QRgb *line = rowData + minX;
		for (int x = minX; x <= maxX; x++, line++)
		{
			float b1 = ((y - v3.y) * (v2.x - v3.x) + (v2.y - v3.y) * (v3.x - x)) / triangleArea;
			float b2 = ((y - v1.y) * (v3.x - v1.x) + (v3.y - v1.y) * (v1.x - x)) / triangleArea;
			float b3 = ((y - v2.y) * (v1.x - v2.x) + (v1.y - v2.y) * (v2.x - x)) / triangleArea;

			if (b1 >= 0 && b1 <= 1 && b2 >= 0 && b2 <= 1 && b3 >= 0 && b3 <= 1)
			{
				float depth = b1 * v1.z + b2 * v2.z + b3 * v3.z;
				int zIndex = y * width + x;
				if (zBuffer[zIndex] > depth)
				{
					*line = color.rgb();
					zBuffer[zIndex] = depth;
				}
			}
		}
	}
}
