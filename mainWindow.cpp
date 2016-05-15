#include <omp.h>
#include "mainWindow.h"
#include "matrix.h"
#include "drawing.h"
#include "tardis.h"
#include "addwidget.h"

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

	camera = new Camera(eye, center, 90, width, height, 0.1, 1000);
	verticalBar->setValue(90 - camera->xRotate());

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

	camera->rotateAroundCenter(verticalBar->value(), horizontalBar->value());

	Mat4 view = camera->view();

	Mat4 projectionViewport = camera->projectionViewport();

	for(size_t i = 0; i < objects->size(); i++)
	{
		Mat4 modelView = view * objects->at(i).model();

		#pragma omp parallel for num_threads(numCores)
		for(size_t j = 0; j < objects->at(i).polygonsCount(); j++)
		{
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

			drawTriangle(v1.toVec3(), v2.toVec3(), v3.toVec3(), intensity, &image, zBuffer, width, height);
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
	AddWidget *addWidget = new AddWidget(&width, &height, &recessWidth, &recessHeight, &ledgeWidth, &ledgeHeight, &pyramidHeight, &lanternRadius, &lanternHeight, &edgeWidth, &edgeHeight, &septumWidth, &septumCount, this);
	addWidget->exec();

	Tardis tardis(width, height, recessWidth, recessHeight, ledgeWidth, ledgeHeight, pyramidHeight, lanternRadius, lanternHeight, edgeWidth, edgeHeight, septumWidth, septumCount);
	objects->push_back(tardis.getObject());
	redraw();

}

void MainWindow::center()
{
	QRect qr = frameGeometry();
	QPoint cp = QDesktopWidget().availableGeometry().center();
	qr.moveCenter(cp);
	move(qr.topLeft());
}
