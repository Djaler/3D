#include <omp.h>
#include "mainWindow.h"
#include "matrix.h"
#include "drawing.h"
#include "tardis.h"
#include "parametersWidget.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
	width = 600;
	height = 600;
	initUI();
	initCamera();

	numCores = omp_get_num_procs();

	objects = new vector<Tardis>();

	Tardis tardis(200, 500, 180, 480, 180, 30, 10, 20, 30, 220, 30, 20, 3);
	addObject(tardis);

	redraw();
}

void MainWindow::initUI()
{
	QHBoxLayout *mainLayout = new QHBoxLayout();

	QVBoxLayout *drawLayout = new QVBoxLayout();

	QHBoxLayout *horizontalLayout = new QHBoxLayout();

	drawArea = new QLabel();
	drawArea->resize(width, height);
	drawArea->show();
	horizontalLayout->addWidget(drawArea);

	fpsMeter = new QLabel(drawArea);
	fpsMeter->setGeometry(0, 0, 75, 25);
	fpsMeter->setStyleSheet("QLabel {color: white;}");
	fpsMeter->show();

	verticalBar = new QSlider(Qt::Vertical);
	verticalBar->setMinimum(-89);
	verticalBar->setMaximum(89);
	verticalBar->setValue(0);

	connect(verticalBar, SIGNAL(sliderMoved(int)), this, SLOT(rotateCamera()));
	horizontalLayout->addWidget(verticalBar);

	drawLayout->addLayout(horizontalLayout);

	horizontalBar = new QSlider(Qt::Horizontal);
	horizontalBar->setMinimum(-180);
	horizontalBar->setMaximum(180);
	horizontalBar->setValue(0);

	connect(horizontalBar, SIGNAL(sliderMoved(int)), this, SLOT(rotateCamera()));
	drawLayout->addWidget(horizontalBar);

	mainLayout->addLayout(drawLayout);

	QGridLayout *rightPanel = new QGridLayout();
	rightPanel->setAlignment(Qt::AlignTop);

	QRadioButton *rotateAroundCenterRadio = new QRadioButton("Поворот камеры вокруг точки зрения");
	rotateAroundCenterRadio->setChecked(true);
	connect(rotateAroundCenterRadio, SIGNAL(toggled(bool)), this, SLOT(switchCamera(bool)));
	rightPanel->addWidget(rotateAroundCenterRadio, 0, 0, 1, 3);
	QRadioButton *rotateAroundEyeRadio = new QRadioButton("Поворот камеры вокруг своей оси");
	rightPanel->addWidget(rotateAroundEyeRadio, 1, 0, 1, 3);

	rightPanel->addWidget(new QLabel("Координаты камеры"), 2, 0, 1, 3);

	eyeXEdit = new QDoubleSpinBox();
	eyeXEdit->setRange(-99999, 99999);
	eyeXEdit->setSingleStep(10);
	eyeXEdit->setValue(0);
	connect(eyeXEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	rightPanel->addWidget(eyeXEdit, 3, 0, 1, 1);

	eyeYEdit = new QDoubleSpinBox();
	eyeYEdit->setRange(-99999, 99999);
	eyeYEdit->setSingleStep(10);
	eyeYEdit->setValue(0);
	connect(eyeYEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	rightPanel->addWidget(eyeYEdit, 3, 1, 1, 1);

	eyeZEdit = new QDoubleSpinBox();
	eyeZEdit->setRange(-99999, 99999);
	eyeZEdit->setSingleStep(10);
	eyeZEdit->setValue(-600);
	connect(eyeZEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	rightPanel->addWidget(eyeZEdit, 3, 2, 1, 1);

	rightPanel->addWidget(new QLabel("Координаты точки зрения"), 4, 0, 1, 3);

	centerXEdit = new QDoubleSpinBox();
	centerXEdit->setRange(-99999, 99999);
	centerXEdit->setSingleStep(10);
	centerXEdit->setValue(0);
	connect(centerXEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	rightPanel->addWidget(centerXEdit, 5, 0, 1, 1);

	centerYEdit = new QDoubleSpinBox();
	centerYEdit->setRange(-99999, 99999);
	centerYEdit->setSingleStep(10);
	centerYEdit->setValue(0);
	connect(centerYEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	rightPanel->addWidget(centerYEdit, 5, 1, 1, 1);

	centerZEdit = new QDoubleSpinBox();
	centerZEdit->setRange(-99999, 99999);
	centerZEdit->setSingleStep(10);
	centerZEdit->setValue(0);
	connect(centerZEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	rightPanel->addWidget(centerZEdit, 5, 2, 1, 1);

	rightPanel->addWidget(new QLabel("Поле зрения"), 6, 0, 1, 1);

	fovSpinBox = new QSpinBox();
	fovSpinBox->setRange(1, 180);
	fovSpinBox->setValue(90);
	fovSpinBox->setSingleStep(10);
	connect(fovSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeFov(int)));
	rightPanel->addWidget(fovSpinBox, 6, 1, 1, 2);

	addButton = new QPushButton("Добавить");
	rightPanel->addWidget(addButton, 7, 0, 1, 3);
	connect(addButton, SIGNAL(pressed()), this, SLOT(addDialog()));

	objectsList = new QListWidget();
	connect(objectsList, SIGNAL(currentRowChanged(int)), this, SLOT(selectObject(int)));
	connect(objectsList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(editObject(QModelIndex)));
	rightPanel->addWidget(objectsList, 9, 0, 1, 3);

	rightPanel->addWidget(new QLabel("Смещение"), 10, 0, 1, 3);

	translateXEdit = new QDoubleSpinBox();
	translateXEdit->setRange(-99999, 99999);
	translateXEdit->setSingleStep(10);
	connect(translateXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(translateXEdit, 11, 0, 1, 1);

	translateYEdit = new QDoubleSpinBox();
	translateYEdit->setRange(-99999, 99999);
	translateYEdit->setSingleStep(10);
	connect(translateYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(translateYEdit, 11, 1, 1, 1);

	translateZEdit = new QDoubleSpinBox();
	translateZEdit->setRange(-99999, 99999);
	translateZEdit->setSingleStep(10);
	connect(translateZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(translateZEdit, 11, 2, 1, 1);

	rightPanel->addWidget(new QLabel("Масштабирование"), 12, 0, 1, 3);

	scaleXEdit = new QDoubleSpinBox();
	scaleXEdit->setRange(-99999, 99999);
	scaleXEdit->setValue(1);
	connect(scaleXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(scaleXEdit, 13, 0, 1, 1);

	scaleYEdit = new QDoubleSpinBox();
	scaleYEdit->setRange(-99999, 99999);
	scaleYEdit->setValue(1);
	connect(scaleYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(scaleYEdit, 13, 1, 1, 1);

	scaleZEdit = new QDoubleSpinBox();
	scaleZEdit->setRange(-99999, 99999);
	scaleZEdit->setValue(1);
	connect(scaleZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(scaleZEdit, 13, 2, 1, 1);

	rightPanel->addWidget(new QLabel("Поворот"), 14, 0, 1, 3);

	rotateXEdit = new QDoubleSpinBox();
	rotateXEdit->setRange(-180, 180);
	rotateXEdit->setSingleStep(10);
	connect(rotateXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(rotateXEdit, 15, 0, 1, 1);

	rotateYEdit = new QDoubleSpinBox();
	rotateYEdit->setRange(-180, 180);
	rotateYEdit->setSingleStep(10);
	connect(rotateYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(rotateYEdit, 15, 1, 1, 1);

	rotateZEdit = new QDoubleSpinBox();
	rotateZEdit->setRange(-180, 180);
	rotateZEdit->setSingleStep(10);
	connect(rotateZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rightPanel->addWidget(rotateZEdit, 15, 2, 1, 1);

	mainLayout->addLayout(rightPanel);

	setLayout(mainLayout);
	center();
}

void MainWindow::initCamera()
{
	Vec3 eye(eyeXEdit->value(), eyeYEdit->value(), eyeZEdit->value());
	Vec3 center(centerXEdit->value(), centerYEdit->value(), centerZEdit->value());

	camera = new Camera(eye, center, fovSpinBox->value(), width, height, 1, 10000);
	switchCamera(true);
}

void MainWindow::redraw()
{
	QTime time;
	time.start();

	QImage image = QImage(width, height, QImage::Format_RGB32);
	image.fill(QColor(0, 0, 0));

	float zBuffer[width * height];
	for(int i = 0; i < width * height; i++)
	{
		zBuffer[i] = numeric_limits<int>::max();
	}

	Mat4 view = camera->view();

	Mat4 projectionViewport = camera->projectionViewport();

	for(size_t i = 0; i < objects->size(); i++)
	{
		Mat4 modelView = view * objects->at(i).object().model();

		vector<Polygon> *object = new vector<Polygon>();
		bool cutted = false;

		#pragma omp parallel for shared(cutted) num_threads(numCores)
		for(size_t j = 0; j < objects->at(i).object().polygonsCount(); j++)
		{
			if(cutted)
			{
				continue;
			}
			Polygon polygon = objects->at(i).object().polygon(j);

			Vec4 v1 = modelView * polygon.v1.toVec4();
			Vec4 v2 = modelView * polygon.v2.toVec4();
			Vec4 v3 = modelView * polygon.v3.toVec4();

			Vec3 norm = Vec3::normal(v1.toVec3(), v2.toVec3(), v3.toVec3());
			float intensity = abs(Vec3::dotProduct(norm, Vec3(0, 0, -1)));

			if(intensity > 1) intensity = 1;

			v1 = projectionViewport * v1;
			v2 = projectionViewport * v2;
			v3 = projectionViewport * v3;

			if(v1.z <= camera->near() || v2.z <= camera->near() || v3.z <= camera->near() ||
			   v1.z >= camera->far() || v2.z >= camera->far() || v3.z >= camera->far())
			{
				cutted = true;
				continue;
			}
			else
			{
				#pragma omp critical
				object->push_back(Polygon(v1.toVec3(), v2.toVec3(), v3.toVec3(), intensity));
			}
		}
		if(!cutted)
		{
			#pragma omp parallel for num_threads(numCores)
			for(size_t j = 0; j < object->size(); j++)
			{
				drawTriangle(object->at(j).v1, object->at(j).v2, object->at(j).v3,
							 object->at(j).intensity, &image, zBuffer, width, height);
			}

		}
	}
	drawArea->setPixmap(QPixmap::fromImage(image.mirrored()));

	fpsMeter->setText(QString("FPS = %1").arg(1000 / time.elapsed()));
}

void MainWindow::addDialog()
{
	float width = 0, height = 0, recessWidth = 0, recessHeight = 0;
	float ledgeWidth = 0, ledgeHeight = 0, pyramidHeight = 0;
	float lanternRadius = 0, lanternHeight = 0;
	float edgeWidth = 0, edgeHeight = 0;
	float septumWidth = 0; int septumCount = 0;

	ParametersWidget *addWidget = new ParametersWidget(&width, &height, &recessWidth, &recessHeight,
													   &ledgeWidth, &ledgeHeight, &pyramidHeight,
													   &lanternRadius, &lanternHeight,
													   &edgeWidth, &edgeHeight,
													   &septumWidth, &septumCount, this);
	addWidget->exec();

	Tardis tardis(width, height, recessWidth, recessHeight, ledgeWidth, ledgeHeight, pyramidHeight,
				  lanternRadius, lanternHeight, edgeWidth, edgeHeight, septumWidth, septumCount);

	addObject(tardis);
	redraw();
}

void MainWindow::switchCamera(bool rotateAroundCenter)
{
	camera->setRotateAroundCenter(rotateAroundCenter);
	verticalBar->setValue(camera->xRotate());
	horizontalBar->setValue(camera->yRotate());
}

void MainWindow::moveCamera()
{
	camera->setEye(Vec3(eyeXEdit->value(), eyeYEdit->value(), eyeZEdit->value()));
	camera->setCenter(Vec3(centerXEdit->value(), centerYEdit->value(), centerZEdit->value()));

	verticalBar->setValue(camera->xRotate());
	horizontalBar->setValue(camera->yRotate());

	redraw();
}

void MainWindow::rotateCamera()
{
	camera->rotate(verticalBar->value(), horizontalBar->value());
	updateCameraParams();
	redraw();
}

void MainWindow::changeFov(int fov)
{
	camera->setFov(fov);
	redraw();
}

void MainWindow::selectObject(int number)
{
	disconnect(translateXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	translateXEdit->setValue(objects->at(number).object().xTranslate());
	connect(translateXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	disconnect(translateYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	translateYEdit->setValue(objects->at(number).object().yTranslate());
	connect(translateYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	disconnect(translateZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	translateZEdit->setValue(objects->at(number).object().zTranslate());
	connect(translateZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	disconnect(scaleXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	scaleXEdit->setValue(objects->at(number).object().xScale());
	connect(scaleXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	disconnect(scaleYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	scaleYEdit->setValue(objects->at(number).object().yScale());
	connect(scaleYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	disconnect(scaleZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	scaleZEdit->setValue(objects->at(number).object().zScale());
	connect(scaleZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	disconnect(rotateXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rotateXEdit->setValue(objects->at(number).object().xRotate());
	connect(rotateXEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	disconnect(rotateYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rotateYEdit->setValue(objects->at(number).object().yRotate());
	connect(rotateYEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	disconnect(rotateZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));
	rotateZEdit->setValue(objects->at(number).object().zRotate());
	connect(rotateZEdit, SIGNAL(valueChanged(double)), this, SLOT(changeModel()));

	currentObject = number;
}

void MainWindow::changeModel()
{
	objects->at(currentObject).object().setTranslate(translateXEdit->value(),
												  translateYEdit->value(),
												  translateZEdit->value());
	objects->at(currentObject).object().setScale(scaleXEdit->value(),
											  scaleYEdit->value(),
											  scaleZEdit->value());
	objects->at(currentObject).object().setRotate(rotateXEdit->value(),
											   rotateYEdit->value(),
											   rotateZEdit->value());
	objects->at(currentObject).object().updateModel();
	redraw();
}

void MainWindow::editObject(QModelIndex index)
{
	Tardis old = objects->at(index.row());
	float width = old.width, height = old.height,
			recessWidth = old.recessWidth, recessHeight = old.recessHeight;
	float ledgeWidth = old.ledgeWidth, ledgeHeight = old.ledgeHeight,
			pyramidHeight = old.pyramidHeight;
	float lanternRadius = old.lanternRadius, lanternHeight = old.lanternHeight;
	float edgeWidth = old.edgeWidth, edgeHeight = old.edgeHeight;
	float septumWidth = old.septumWidth; int septumCount = old.septumCount;
	ParametersWidget *addWidget = new ParametersWidget(&width, &height, &recessWidth, &recessHeight,
													   &ledgeWidth, &ledgeHeight, &pyramidHeight,
													   &lanternRadius, &lanternHeight,
													   &edgeWidth, &edgeHeight,
													   &septumWidth, &septumCount, this);
	addWidget->exec();

	Tardis tardis(width, height, recessWidth, recessHeight, ledgeWidth, ledgeHeight, pyramidHeight,
				  lanternRadius, lanternHeight, edgeWidth, edgeHeight, septumWidth, septumCount);

	Object oldObj = objects->at(index.row()).object();

	tardis.object().setTranslate(oldObj.xTranslate(), oldObj.yTranslate(), oldObj.zTranslate());
	tardis.object().setRotate(oldObj.xRotate(), oldObj.yRotate(), oldObj.zRotate());
	tardis.object().setScale(oldObj.xScale(), oldObj.yScale(), oldObj.zScale());
	tardis.object().updateModel();

	objects->at(index.row()) = tardis;
	redraw();
}

void MainWindow::center()
{
	QRect qr = frameGeometry();
	QPoint cp = QDesktopWidget().availableGeometry().center();
	qr.moveCenter(cp);
	move(qr.topLeft());
}

void MainWindow::updateCameraParams()
{
	disconnect(eyeXEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	eyeXEdit->setValue(camera->eye().x);
	connect(eyeXEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));

	disconnect(eyeYEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	eyeYEdit->setValue(camera->eye().y);
	connect(eyeYEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));

	disconnect(eyeZEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	eyeZEdit->setValue(camera->eye().z);
	connect(eyeZEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));

	disconnect(centerXEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	centerXEdit->setValue(camera->center().x);
	connect(centerXEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));

	disconnect(centerYEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	centerYEdit->setValue(camera->center().y);
	connect(centerYEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));

	disconnect(centerZEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
	centerZEdit->setValue(camera->center().z);
	connect(centerZEdit, SIGNAL(valueChanged(double)), this, SLOT(moveCamera()));
}

void MainWindow::addObject(Tardis tardis)
{
	int number = objects->size();
	objects->push_back(tardis);
	objectsList->addItem(QString("TARDIS №%1").arg(number + 1));
	objectsList->item(number)->setSelected(true);
	selectObject(number);
}
