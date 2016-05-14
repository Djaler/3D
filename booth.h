#pragma once

#include "object.h"

class Booth
{
	float width, height, recessWidth, recessHeight;
	Object *booth;

	public:
		Booth(float width, float height, float recessWidth, float recessHeight) : width(width), height(height),
			recessWidth(recessWidth), recessHeight(recessHeight)
		{
			initMainBox();
		}

		Object* getObject()
		{
			return booth;
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

			booth = new Object();
			initBox(t1, t2, t3, t4, b1, b2, b3, b4);

			initCorners(t1, t2, t3, t4, b1, b2, b3, b4);

			initSepta();
		}

		void initSepta()
		{
			float bottom = -recessHeight / 2;
			float top = recessHeight / 2;
			float left = -recessWidth / 2;
			float front = -recessWidth / 2;
			float right = recessWidth / 2;
			float back = recessWidth / 2;

			float corner = width - recessWidth;

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

			float corner = width - recessWidth;

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

			corner = height - recessHeight;

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
			booth->addPolygon(Polygon(t1, t2, t3));
			booth->addPolygon(Polygon(t1, t4, t3));

			booth->addPolygon(Polygon(t3, b3, b4));
			booth->addPolygon(Polygon(t3, t4, b4));

			booth->addPolygon(Polygon(t1, b1, t4));
			booth->addPolygon(Polygon(b4, b1, t4));

			booth->addPolygon(Polygon(t1, b1, t2));
			booth->addPolygon(Polygon(b2, b1, t2));

			booth->addPolygon(Polygon(t2, b2, t3));
			booth->addPolygon(Polygon(b3, b2, t3));

			booth->addPolygon(Polygon(b1, b2, b3));
			booth->addPolygon(Polygon(b1, b4, b3));
		}
};
