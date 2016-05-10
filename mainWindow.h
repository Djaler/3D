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

		int width;
		int height;

		Object *object;
		int numCores;

		QLabel *drawArea;
		QLabel *fpsMeter;
		QSlider *verticalBar;
		QSlider *horizontalBar;

	private slots:
		void redraw();
};
