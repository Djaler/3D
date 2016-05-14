#include <omp.h>
#include "mainWindow.h"
#include "matrix.h"
#include "drawing.h"
#include "camera.h"
#include "booth.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
	width = 600;
	height = 600;
	initUI();

	numCores = omp_get_num_procs();

	initModel();

	redraw();
}

void MainWindow::initModel()
{
	/*object = new Object("../obj/diablo3_pose.obj");
	object->setScale(width / 2, width / 2, width / 2);
	object->setRotate(0, 180, 0);*/
	/*object = new Object("../obj/Millennium_Falcon.obj");
	object->setTranslate(0, -50, 0);
	object->setRotate(0, 180, 0);*/
	/*object = new Object("../obj/reconstructed_head.obj");
	object->setRotate(0, 180, 0);*/

	Booth booth(200, 500, 180, 480);
	object = booth.getObject();
}

void MainWindow::initUI()
{
	QVBoxLayout *mainLayout = new QVBoxLayout();

	QHBoxLayout *horizontalLayout = new QHBoxLayout();

	drawArea = new QLabel();
	drawArea->resize(width, height);
	drawArea->show();
	horizontalLayout->addWidget(drawArea);

	verticalBar = new QSlider(Qt::Vertical);
	verticalBar->setMinimum(-89);
	verticalBar->setMaximum(89);
	verticalBar->setValue(0);

	connect(verticalBar, SIGNAL(valueChanged(int)), this, SLOT(redraw()));
	horizontalLayout->addWidget(verticalBar);

	mainLayout->addLayout(horizontalLayout);

	horizontalBar = new QSlider(Qt::Horizontal);
	horizontalBar->setMinimum(-180);
	horizontalBar->setMaximum(180);
	horizontalBar->setValue(0);

	connect(horizontalBar, SIGNAL(valueChanged(int)), this, SLOT(redraw()));
	mainLayout->addWidget(horizontalBar);

	fpsMeter = new QLabel(drawArea);
	fpsMeter->setGeometry(0, 0, 75, 25);
	fpsMeter->setStyleSheet("QLabel {color: white;}");
	fpsMeter->show();

	setLayout(mainLayout);
	center();
	show();
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

	Vec3 light(0, 0, -1);
	Vec3 eye(0, 0, -600);
	Vec3 center(0, 0, 0);

	Camera camera(eye, center, light);
	camera.rotateAroundCenter(verticalBar->value(), horizontalBar->value());

	Mat4 view = camera.view();

	Mat4 projection = Mat4::perspective(90, width / height, 0.1, 1000);
	Mat4 viewport = Mat4::viewport(width, height);

	#pragma omp parallel for num_threads(numCores)
	for(size_t i = 0; i < object->polygonsCount(); i++)
	{
		Polygon polygon = object->polygon(i);
		Vec4 v1 = polygon.v1.toVec4();
		v1 = object->model() * v1;
		v1 = view * v1;

		Vec4 v2 = polygon.v2.toVec4();
		v2 = object->model() * v2;
		v2 = view * v2;

		Vec4 v3 = polygon.v3.toVec4();
		v3 = object->model() * v3;
		v3 = view * v3;

		Vec3 norm = Vec3::normal(v1.toVec3(), v2.toVec3(), v3.toVec3());
		float intensity = abs(Vec3::dotProduct(norm, camera.light()));

		if(intensity > 1)
		{
			intensity = 1;
		}
		QColor color(intensity * 255, intensity * 255, intensity * 255);

		v1 = projection * v1;
		v1 = viewport * v1;
		v2 = projection * v2;
		v2 = viewport * v2;
		v3 = projection * v3;
		v3 = viewport * v3;

		drawTriangle(v1.toVec3(), v2.toVec3(), v3.toVec3(), color, &image, zBuffer, width, height);
	}
	drawArea->setPixmap(QPixmap::fromImage(image.mirrored()));

	fpsMeter->setText(QString("FPS = %1").arg(1000 / time.elapsed()));
}

void MainWindow::center()
{
	QRect qr = frameGeometry();
	QPoint cp = QDesktopWidget().availableGeometry().center();
	qr.moveCenter(cp);
	move(qr.topLeft());
}
