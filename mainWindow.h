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
		void updateCameraCoords();

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
		QLineEdit *eyeXEdit, *eyeYEdit, *eyeZEdit;
		QLineEdit *centerXEdit, *centerYEdit, *centerZEdit;

	private slots:
		void redraw();
		void add();
		void switchCamera(bool);
		void moveCamera();
		void rotateCamera();
};
