#pragma once

#include <QtWidgets>
#include "parser.h"

class Widget : public QWidget
{
		Q_OBJECT

	public:
		Widget(QWidget *parent = 0);
	private:
		void initUI();
		void center();
		void paintEvent(QPaintEvent *);

		int width;
		int height;
		Parser *parser;
		vector<Triangle> faces;
		QImage image;

		QWidget *drawArea;
		QSlider *verticalBar;
		QSlider *horizontalBar;
};
