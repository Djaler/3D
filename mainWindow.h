#pragma once

#include <QtWidgets>
#include "tardis.h"
#include "camera.h"

class MainWindow : public QWidget
{
		Q_OBJECT

	public:
		MainWindow(QWidget *parent = 0);

	private:
		void initUI();
		void initCamera();
		void center();
		void updateCameraParams();
		void addObject(Tardis);

		int width;
		int height;

		vector<Tardis> *objects;
		int currentObject;
		Camera *camera;
		int numCores;

		QLabel *drawArea;
		QLabel *fpsMeter;
		QSlider *verticalBar;
		QSlider *horizontalBar;

		QPushButton *addButton, *deleteButton;
		QDoubleSpinBox *eyeXEdit, *eyeYEdit, *eyeZEdit;
		QDoubleSpinBox *centerXEdit, *centerYEdit, *centerZEdit;
		QLabel *fovLabel; QSpinBox *fovSpinBox;
		QListWidget *objectsList;
		QDoubleSpinBox *translateXEdit, *translateYEdit, *translateZEdit;
		QDoubleSpinBox *scaleXEdit, *scaleYEdit, *scaleZEdit;
		QDoubleSpinBox *rotateXEdit, *rotateYEdit, *rotateZEdit;

	private slots:
		void redraw();
		void addDialog();
		void switchCamera(bool);
		void switchProjection(bool);
		void moveCamera();
		void rotateCamera();
		void changeFov(int);
		void selectObject(int);
		void changeModel();
		void editObject(QModelIndex);
		void deleteObj();
};
