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

	numCores = omp_get_num_procs();

	objects = new vector<Object>();

	Vec3 eye(0, 0, -600);
	Vec3 center(0, 0, 0);

	camera = new Camera(eye, center, 90, width, height, 0.1, 10000);
	switchCamera(true);

	Tardis tardis(200, 500, 180, 480, 180, 30, 10, 20, 30, 220, 30, 20, 3);
	objects->push_back(tardis.getObject());

	redraw();
}

void MainWindow::initUI()
{
	QHBoxLayout *mainLayout = new QHBoxLayout();

	QVBoxLayout *drawLayout = new QVBoxLayout();

	QHBoxLayout *horizontalLayout = new QHBoxLayout();
	horizontalLayout->setAlignment(Qt::AlignLeft);

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

	connect(verticalBar, SIGNAL(valueChanged(int)), this, SLOT(redraw()));
	horizontalLayout->addWidget(verticalBar);

	drawLayout->addLayout(horizontalLayout);

	horizontalBar = new QSlider(Qt::Horizontal);
	horizontalBar->setMinimum(-180);
	horizontalBar->setMaximum(180);
	horizontalBar->setValue(0);

	connect(horizontalBar, SIGNAL(valueChanged(int)), this, SLOT(redraw()));
	drawLayout->addWidget(horizontalBar);

	mainLayout->addLayout(drawLayout);

	QVBoxLayout *rightPanel = new QVBoxLayout();
	rightPanel->setAlignment(Qt::AlignTop);

	addButton = new QPushButton("Добавить");
	rightPanel->addWidget(addButton);
	connect(addButton, SIGNAL(pressed()), this, SLOT(add()));

	QRadioButton *rotateAroundCenterRadio = new QRadioButton("Поворот камеры вокруг точки зрения");
	rotateAroundCenterRadio->setChecked(true);
	connect(rotateAroundCenterRadio, SIGNAL(toggled(bool)), this, SLOT(switchCamera(bool)));
	rightPanel->addWidget(rotateAroundCenterRadio);
	QRadioButton *rotateAroundEyeRadio = new QRadioButton("Поворот камеры вокруг своей оси");
	rightPanel->addWidget(rotateAroundEyeRadio);

	mainLayout->addLayout(rightPanel);

	setLayout(mainLayout);
	center();
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

	camera->rotate(verticalBar->value(), horizontalBar->value());

	Mat4 view = camera->view();

	Mat4 projectionViewport = camera->projectionViewport();

	for(size_t i = 0; i < objects->size(); i++)
	{
		Mat4 modelView = view * objects->at(i).model();

		vector<Polygon> *buf = new vector<Polygon>();
		volatile bool cutted = false;

		#pragma omp parallel for shared(cutted) num_threads(numCores)
		for(size_t j = 0; j < objects->at(i).polygonsCount(); j++)
		{
			if(cutted)
			{
				continue;
			}
			Polygon polygon = objects->at(i).polygon(j);

			Vec4 v1 = modelView * polygon.v1.toVec4();
			Vec4 v2 = modelView * polygon.v2.toVec4();
			Vec4 v3 = modelView * polygon.v3.toVec4();

			Vec3 norm = Vec3::normal(v1.toVec3(), v2.toVec3(), v3.toVec3());
			float intensity = abs(Vec3::dotProduct(norm, Vec3(0, 0, -1)));

			if(intensity > 1) intensity = 1;

			v1 = projectionViewport * v1;
			v2 = projectionViewport * v2;
			v3 = projectionViewport * v3;

			if(v1.z <= 0.1 || v2.z <= 0.1 || v3.z <= 0.1 ||
			   v1.z >= 10000 || v2.z >= 10000 || v3.z >= 10000)
			{
				cutted = true;
				continue;
			}
			else
			{
				#pragma omp critical
				buf->push_back(Polygon(v1.toVec3(), v2.toVec3(), v3.toVec3(), intensity));
			}
		}
		if(!cutted)
		{
			for(size_t j = 0; j < buf->size(); j++)
			{
				drawTriangle(buf->at(j).v1, buf->at(j).v2, buf->at(j).v3, buf->at(j).intensity, &image, zBuffer, width, height);
			}

		}
	}
	drawArea->setPixmap(QPixmap::fromImage(image.mirrored()));

	fpsMeter->setText(QString("FPS = %1").arg(1000 / time.elapsed()));
}

void MainWindow::add()
{
	float width = 0, height = 0, recessWidth = 0, recessHeight = 0;
	float ledgeWidth = 0, ledgeHeight = 0, pyramidHeight = 0;
	float lanternRadius = 0, lanternHeight = 0;
	float edgeWidth = 0, edgeHeight = 0;
	float septumWidth = 0; int septumCount = 0;
	ParametersWidget *addWidget = new ParametersWidget(&width, &height, &recessWidth, &recessHeight, &ledgeWidth, &ledgeHeight, &pyramidHeight, &lanternRadius, &lanternHeight, &edgeWidth, &edgeHeight, &septumWidth, &septumCount, this);
	addWidget->exec();

	Tardis tardis(width, height, recessWidth, recessHeight, ledgeWidth, ledgeHeight, pyramidHeight, lanternRadius, lanternHeight, edgeWidth, edgeHeight, septumWidth, septumCount);
	objects->push_back(tardis.getObject());
	redraw();

}

void MainWindow::switchCamera(bool rotateAroundCenter)
{
	if(rotateAroundCenter)
	{
		verticalBar->setValue(90 - camera->xRotateArountCenter());
	}
	else
	{
		verticalBar->setValue(camera->xRotateArountEye() - 90);
	}
	camera->setRotateAroundCenter(rotateAroundCenter);
}

void MainWindow::center()
{
	QRect qr = frameGeometry();
	QPoint cp = QDesktopWidget().availableGeometry().center();
	qr.moveCenter(cp);
	move(qr.topLeft());
}
