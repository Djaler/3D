#pragma once

#include <QtWidgets>
#include "object.h"

class MainWindow : public QWidget
{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);

	private:
		void initUI();
		void center();

		void initModel();
		void drawTriangle(Vec3 v1, Vec3 v2, Vec3 v3, QColor color, QImage *image, float *zBuffer, float translateX, float translateY);

		int width;
		int height;

		Object *object;
		int numCores;

		QLabel *drawArea;
		QSlider *verticalBar;
		QSlider *horizontalBar;
		QLabel *fpsMeter;

	private slots:
		void redraw();
};
