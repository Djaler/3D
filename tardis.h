#pragma once

#include "object.h"

class Tardis
{
	Object tardis;

	public:
		float width, height, recessWidth, recessHeight;
		float ledgeWidth, ledgeHeight, pyramidHeight;
		float lanternRadius, lanternHeight;
		float edgeWidth, edgeHeight;
		float septumWidth; int septumCount;

		Tardis(float width, float height, float recessWidth, float recessHeight,
			  float ledgeWidth, float ledgeHeight, float pyramidHeight,
			  float lanternRadius, float lanternHeight, float edgeWidth,
			   float edgeHeight, float septumWidth, int septumCount)
		{
			this->width = width;
			this->height = height;
			this->recessWidth = recessWidth;
			this->recessHeight = recessHeight;
			this->ledgeWidth = ledgeWidth;
			this->ledgeHeight = ledgeHeight;
			this->pyramidHeight = pyramidHeight;
			this->lanternRadius = lanternRadius;
			this->lanternHeight = lanternHeight;
			this->edgeWidth = edgeWidth;
			this->edgeHeight = edgeHeight;
			this->septumWidth = septumWidth;
			this->septumCount = septumCount;

			initMainBox();

			initLedge();

			initEdge();
		}

		Object getObject()
		{
			return tardis;
		}

	private:
		void initMainBox()
		{
			float bottom = -recessHeight / 2;
			float top = recessHeight / 2;
			float left = -recessWidth / 2;
			float front = -recessWidth / 2;
			float right = recessWidth / 2;
			float back = recessWidth / 2;
			Vec3 b1(left, bottom,  front);
			Vec3 b2(left, bottom, back);
			Vec3 b3(right, bottom, back);
			Vec3 b4(right, bottom, front);
			Vec3 t1(left, top, front);
			Vec3 t2(left, top, back);
			Vec3 t3(right, top, back);
			Vec3 t4(right, top, front);

			initBox(t1, t2, t3, t4, b1, b2, b3, b4);

			initCorners(t1, t2, t3, t4, b1, b2, b3, b4);

			initVerticalSepta();

			initHorizontalSepta();
		}

		void initHorizontalSepta()
		{
			float freeSpace = recessHeight - septumWidth * septumCount;

			float left = -recessWidth / 2;
			float front = -recessWidth / 2;
			float right = recessWidth / 2;
			float back = recessWidth / 2;

			float corner = (width - recessWidth) / 2;

			float bottom = recessHeight / 2;
			float top;

			for(int i = 0; i < septumCount; i++)
			{
				top = bottom - (freeSpace / (septumCount + 1));
				bottom = top - septumWidth;

				Vec3 t1(left, top, front - corner);
				Vec3 t2(left, top, back + corner);
				Vec3 t3(right, top, back + corner);
				Vec3 t4(right, top, front - corner);
				Vec3 b1(left, bottom, front - corner);
				Vec3 b2(left, bottom, back + corner);
				Vec3 b3(right, bottom, back + corner);
				Vec3 b4(right, bottom, front - corner);

				initBox(t1, t2, t3, t4, b1, b2, b3, b4);

				t1 = Vec3(left - corner, top, front);
				t2 = Vec3(left - corner, top, back);
				t3 = Vec3(right + corner, top, back);
				t4 = Vec3(right + corner, top, front);
				b1 = Vec3(left - corner, bottom, front);
				b2 = Vec3(left - corner, bottom, back);
				b3 = Vec3(right + corner, bottom, back);
				b4 = Vec3(right + corner, bottom, front);

				initBox(t1, t2, t3, t4, b1, b2, b3, b4);
			}
		}

		void initEdge()
		{
			float bottom = -height / 2 - edgeHeight;
			float top = -height / 2;
			float left = -edgeWidth / 2;
			float right = edgeWidth / 2;
			float front = -edgeWidth / 2;
			float back = edgeWidth / 2;

			Vec3 b1(left, bottom, front);
			Vec3 b2(left, bottom, back);
			Vec3 b3(right, bottom, back);
			Vec3 b4(right, bottom, front);
			Vec3 t1(left, top, front);
			Vec3 t2(left, top, back);
			Vec3 t3(right, top, back);
			Vec3 t4(right, top, front);

			initBox(t1, t2, t3, t4, b1, b2, b3, b4);
		}

		void initLedge()
		{
			float bottom = height / 2;
			float top = height / 2 + ledgeHeight;
			float left = -ledgeWidth / 2;
			float right = ledgeWidth / 2;
			float front = -ledgeWidth / 2;
			float back = ledgeWidth / 2;

			Vec3 b1(left, bottom, front);
			Vec3 b2(left, bottom, back);
			Vec3 b3(right, bottom, back);
			Vec3 b4(right, bottom, front);
			Vec3 t1(left, top, front);
			Vec3 t2(left, top, back);
			Vec3 t3(right, top, back);
			Vec3 t4(right, top, front);

			initBox(t1, t2, t3, t4, b1, b2, b3, b4);

			initPyramid(t1, t2, t3, t4, top);

			initCylinder(top + pyramidHeight);

			initHalfSphere(top + pyramidHeight + lanternHeight);
		}

		void initHalfSphere(float bottom)
		{
			Vec3 center(0, bottom, 0);

			int segments = 50;

			float stepF = M_PI / segments;
			float stepO = M_PI / segments;

			for(int i = 0; i < segments; i++)
			{
				for(int j = 0; j < segments; j++)
				{
					float x = lanternRadius * sin(j * stepO) * cos(i * stepF);
					float y = lanternRadius * sin(j * stepO) * sin(i * stepF) + bottom;
					float z = lanternRadius * cos(j * stepO);
					Vec3 v1(x, y, z);

					x = lanternRadius * sin((j + 1) * stepO) * cos(i * stepF);
					y = lanternRadius * sin((j + 1) * stepO) * sin(i * stepF) + bottom;
					z = lanternRadius * cos((j + 1) * stepO);
					Vec3 v2(x, y, z);

					x = lanternRadius * sin(j * stepO) * cos((i + 1) * stepF);
					y = lanternRadius * sin(j * stepO) * sin((i + 1) * stepF) + bottom;
					z = lanternRadius * cos(j * stepO);
					Vec3 v3(x, y, z);

					tardis.addPolygon(Polygon(v1, v2, v3));

					x = lanternRadius * sin((j - 1) * stepO) * cos(i * stepF);
					y = lanternRadius * sin((j - 1) * stepO) * sin(i * stepF) + bottom;
					z = lanternRadius * cos((j - 1) * stepO);
					v2 = Vec3(x, y, z);

					x = lanternRadius * sin(j * stepO) * cos((i - 1) * stepF);
					y = lanternRadius * sin(j * stepO) * sin((i - 1) * stepF) + bottom;
					z = lanternRadius * cos(j * stepO);
					v3 = Vec3(x, y, z);

					tardis.addPolygon(Polygon(v1, v2, v3));
				}
			}
		}

		void initCylinder(float bottom)
		{
			float top = bottom + lanternHeight;
			Vec3 center_t(0, top, 0);
			Vec3 center_b(0, bottom, 0);

			int segments = 100;

			float step = 2 * M_PI / segments;

			for(int i = 0; i < segments; i++)
			{
				float x = lanternRadius * cos(i * step);
				float z = lanternRadius * sin(i * step);
				Vec3 current_t(x, top, z);
				Vec3 current_b(x, bottom, z);
				x = lanternRadius * cos((i+1) * step);
				z = lanternRadius * sin((i+1) * step);
				Vec3 next_t(x, top, z);
				Vec3 next_b(x, bottom, z);
				tardis.addPolygon(Polygon(center_t, current_t, next_t));
				tardis.addPolygon(Polygon(center_b, current_b, next_b));
				tardis.addPolygon(Polygon(current_t, next_t, current_b));
				tardis.addPolygon(Polygon(current_b, next_b, next_t));
			}


		}

		void initPyramid(Vec3 b1, Vec3 b2, Vec3 b3, Vec3 b4, float bottom)
		{
			float left = -lanternRadius;
			float right = lanternRadius;
			float front = -lanternRadius;
			float back = lanternRadius;
			float top = bottom + pyramidHeight;
			Vec3 t1(left, top, front);
			Vec3 t2(left, top, back);
			Vec3 t3(right, top, back);
			Vec3 t4(right, top, front);

			initBox(t1, t2, t3, t4, b1, b2, b3, b4);
		}

		void initVerticalSepta()
		{
			float bottom = -recessHeight / 2;
			float top = recessHeight / 2;
			float left = -recessWidth / 2;
			float front = -recessWidth / 2;
			float right = recessWidth / 2;
			float back = recessWidth / 2;

			float corner = (width - recessWidth) / 2;

			Vec3 sf_t1(-corner / 2, top, front - corner);
			Vec3 sf_t2(-corner / 2, top, front);
			Vec3 sf_t3(corner / 2, top, front);
			Vec3 sf_t4(corner / 2, top, front - corner);
			Vec3 sf_b1(-corner / 2, bottom, front - corner);
			Vec3 sf_b2(-corner / 2, bottom, front);
			Vec3 sf_b3(corner / 2, bottom, front);
			Vec3 sf_b4(corner / 2, bottom, front - corner);

			initBox(sf_t1, sf_t2, sf_t3, sf_t4, sf_b1, sf_b2, sf_b3, sf_b4);

			Vec3 sb_t1(-corner / 2, top, back + corner);
			Vec3 sb_t2(-corner / 2, top, back);
			Vec3 sb_t3(corner / 2, top, back);
			Vec3 sb_t4(corner / 2, top, back + corner);
			Vec3 sb_b1(-corner / 2, bottom, back + corner);
			Vec3 sb_b2(-corner / 2, bottom, back);
			Vec3 sb_b3(corner / 2, bottom, back);
			Vec3 sb_b4(corner / 2, bottom, back + corner);

			initBox(sb_t1, sb_t2, sb_t3, sb_t4, sb_b1, sb_b2, sb_b3, sb_b4);

			Vec3 sl_t1(left - corner, top, -corner / 2);
			Vec3 sl_t2(left - corner, top, corner / 2);
			Vec3 sl_t3(left, top, corner / 2);
			Vec3 sl_t4(left, top, -corner / 2);
			Vec3 sl_b1(left - corner, bottom, -corner / 2);
			Vec3 sl_b2(left - corner, bottom, corner / 2);
			Vec3 sl_b3(left, bottom, corner / 2);
			Vec3 sl_b4(left, bottom, -corner / 2);

			initBox(sl_t1, sl_t2, sl_t3, sl_t4, sl_b1, sl_b2, sl_b3, sl_b4);

			Vec3 sr_t1(right + corner, top, -corner / 2);
			Vec3 sr_t2(right + corner, top, corner / 2);
			Vec3 sr_t3(right, top, corner / 2);
			Vec3 sr_t4(right, top, -corner / 2);
			Vec3 sr_b1(right + corner, bottom, -corner / 2);
			Vec3 sr_b2(right + corner, bottom, corner / 2);
			Vec3 sr_b3(right, bottom, corner / 2);
			Vec3 sr_b4(right, bottom, -corner / 2);

			initBox(sr_t1, sr_t2, sr_t3, sr_t4, sr_b1, sr_b2, sr_b3, sr_b4);
		}

		void initCorners(Vec3 t1, Vec3 t2, Vec3 t3, Vec3 t4, Vec3 b1, Vec3 b2, Vec3 b3, Vec3 b4)
		{
			float bottom = -recessHeight / 2;
			float top = recessHeight / 2;
			float left = -recessWidth / 2;
			float front = -recessWidth / 2;
			float right = recessWidth / 2;
			float back = recessWidth / 2;

			float corner = (width - recessWidth) / 2;

			Vec3 c1_t1(left - corner, top, front - corner);
			Vec3 c1_t2(left - corner, top, front);
			Vec3 c1_t3 = t1;
			Vec3 c1_t4(left, top, front - corner);
			Vec3 c1_b1(left - corner, bottom, front - corner);
			Vec3 c1_b2(left - corner, bottom, front);
			Vec3 c1_b3 = b1;
			Vec3 c1_b4(left, bottom, front - corner);

			initBox(c1_t1, c1_t2, c1_t3, c1_t4, c1_b1, c1_b2, c1_b3, c1_b4);

			Vec3 c2_t1(left - corner, top, back + corner);
			Vec3 c2_t2(left - corner, top, back);
			Vec3 c2_t3 = t2;
			Vec3 c2_t4(left, top, back + corner);
			Vec3 c2_b1(left - corner, bottom, back + corner);
			Vec3 c2_b2(left - corner, bottom, back);
			Vec3 c2_b3 = b2;
			Vec3 c2_b4(left, bottom, back + corner);

			initBox(c2_t1, c2_t2, c2_t3, c2_t4, c2_b1, c2_b2, c2_b3, c2_b4);

			Vec3 c3_t1(right + corner, top, back + corner);
			Vec3 c3_t2(right + corner, top, back);
			Vec3 c3_t3 = t3;
			Vec3 c3_t4(right, top, back + corner);
			Vec3 c3_b1(right + corner, bottom, back + corner);
			Vec3 c3_b2(right + corner, bottom, back);
			Vec3 c3_b3 = b3;
			Vec3 c3_b4(right, bottom, back + corner);

			initBox(c3_t1, c3_t2, c3_t3, c3_t4, c3_b1, c3_b2, c3_b3, c3_b4);

			Vec3 c4_t1(right + corner, top, front - corner);
			Vec3 c4_t2(right + corner, top, front);
			Vec3 c4_t3 = t4;
			Vec3 c4_t4(right, top, front - corner);
			Vec3 c4_b1(right + corner, bottom, front - corner);
			Vec3 c4_b2(right + corner, bottom, front);
			Vec3 c4_b3 = b4;
			Vec3 c4_b4(right, bottom, front - corner);

			initBox(c4_t1, c4_t2, c4_t3, c4_t4, c4_b1, c4_b2, c4_b3, c4_b4);

			corner = (height - recessHeight) / 2;

			Vec3 ct_b1 = c1_t1;
			Vec3 ct_b2 = c2_t1;
			Vec3 ct_b3 = c3_t1;
			Vec3 ct_b4 = c4_t1;
			Vec3 ct_t1 = ct_b1 + Vec3(0, corner, 0);
			Vec3 ct_t2 = ct_b2 + Vec3(0, corner, 0);
			Vec3 ct_t3 = ct_b3 + Vec3(0, corner, 0);
			Vec3 ct_t4 = ct_b4 + Vec3(0, corner, 0);

			initBox(ct_t1, ct_t2, ct_t3, ct_t4, ct_b1, ct_b2, ct_b3, ct_b4);

			Vec3 cb_t1 = c1_b1;
			Vec3 cb_t2 = c2_b1;
			Vec3 cb_t3 = c3_b1;
			Vec3 cb_t4 = c4_b1;
			Vec3 cb_b1 = cb_t1 - Vec3(0, corner, 0);
			Vec3 cb_b2 = cb_t2 - Vec3(0, corner, 0);
			Vec3 cb_b3 = cb_t3 - Vec3(0, corner, 0);
			Vec3 cb_b4 = cb_t4 - Vec3(0, corner, 0);

			initBox(cb_t1, cb_t2, cb_t3, cb_t4, cb_b1, cb_b2, cb_b3, cb_b4);
		}

		void initBox(Vec3 t1, Vec3 t2, Vec3 t3, Vec3 t4, Vec3 b1, Vec3 b2, Vec3 b3, Vec3 b4)
		{
			tardis.addPolygon(Polygon(t1, t2, t3));
			tardis.addPolygon(Polygon(t1, t4, t3));

			tardis.addPolygon(Polygon(t3, b3, b4));
			tardis.addPolygon(Polygon(t3, t4, b4));

			tardis.addPolygon(Polygon(t1, b1, t4));
			tardis.addPolygon(Polygon(b4, b1, t4));

			tardis.addPolygon(Polygon(t1, b1, t2));
			tardis.addPolygon(Polygon(b2, b1, t2));

			tardis.addPolygon(Polygon(t2, b2, t3));
			tardis.addPolygon(Polygon(b3, b2, t3));

			tardis.addPolygon(Polygon(b1, b2, b3));
			tardis.addPolygon(Polygon(b1, b4, b3));
		}
};
