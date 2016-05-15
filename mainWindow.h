#pragma once

#include <QtWidgets>
#include "object.h"
#include "camera.h"

class MainWindow : public QWidget
{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);

	private:
		void initUI();
		void center();

		int width;
		int height;

		vector<Object> *objects;
		Camera *camera;
		int numCores;

		QLabel *drawArea;
		QLabel *fpsMeter;
		QSlider *verticalBar;
		QSlider *horizontalBar;

		QPushButton *addButton;

	private slots:
		void redraw();
		void add();
};
