#pragma once

#include <QtWidgets>
#include "parser.h"

class MainWindow : public QWidget
{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);

	private:
		void initUI();
		void center();

		void initModel();
		void drawTriangle(Vec3 *v1, Vec3 *v2, Vec3 *v3, QColor color, QImage *image, float *zBuffer);

		int width;
		int height;
		Parser *parser;
		vector<Polygon> polygons;
		int numCores;

		QLabel *drawArea;
		QSlider *verticalBar;
		QSlider *horizontalBar;

	private slots:
		void redraw();
};
