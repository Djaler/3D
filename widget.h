#pragma once

#include <QtWidgets>
#include "model.h"

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
		Model *model;

		QWidget *drawArea;
		QSlider *verticalBar;
		QSlider *horizontalBar;
};
