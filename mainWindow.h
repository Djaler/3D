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
		void paintEvent(QPaintEvent *);

		void initModel();

		int width;
		int height;
		Parser *parser;
		vector<Polygon> polygons;
		QImage image;
		int numCores;

		QWidget *drawArea;
		QSlider *verticalBar;
		QSlider *horizontalBar;
};
