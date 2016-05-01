#include "mainWindow.h"
#include "matrix.h"
#include <omp.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
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
	parser = new Parser("../obj/diablo3_pose.obj", width/2);
	//parser = new Parser("../obj/Millennium_Falcon.obj", width/2);
	//parser = new Parser("../obj/reconstructed_head.obj");

	polygons = parser->polygons;
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
	verticalBar->setMinimum(0);
	verticalBar->setMaximum(360);
	verticalBar->setValue(180);

	connect(verticalBar, SIGNAL(valueChanged(int)), this, SLOT(redraw()));
	horizontalLayout->addWidget(verticalBar);

	mainLayout->addLayout(horizontalLayout);

	horizontalBar = new QSlider(Qt::Horizontal);
	horizontalBar->setMinimum(0);
	horizontalBar->setMaximum(360);
	horizontalBar->setValue(180);

	connect(horizontalBar, SIGNAL(valueChanged(int)), this, SLOT(redraw()));
	mainLayout->addWidget(horizontalBar);

	setLayout(mainLayout);
	center();
	show();
}

void MainWindow::drawTriangle(Vec3 *v1, Vec3 *v2, Vec3 *v3, QColor color, QImage *image, float *zBuffer)
{
	int minX = max(0.0f, ceil(min(v1->x, min(v2->x, v3->x))));
	int maxX = min(width - 1.0f, floor(max(v1->x, max(v2->x, v3->x))));
	int minY = max(0.0f, ceil(min(v1->y, min(v2->y, v3->y))));
	int maxY = min(height - 1.0f, floor(max(v1->y, max(v2->y, v3->y))));

	float triangleArea = (v1->y - v3->y) * (v2->x - v3->x) + (v2->y - v3->y) * (v3->x - v1->x);

	QRgb *rowData = (QRgb*)image->scanLine(minY);
	int stride = image->bytesPerLine() / 4;
	for (int y = minY; y <= maxY; y++, rowData += stride)
	{
		QRgb *line = rowData + minX;
		for (int x = minX; x <= maxX; x++, line++)
		{
			float b1 = ((y - v3->y) * (v2->x - v3->x) + (v2->y - v3->y) * (v3->x - x)) / triangleArea;
			float b2 = ((y - v1->y) * (v3->x - v1->x) + (v3->y - v1->y) * (v1->x - x)) / triangleArea;
			float b3 = ((y - v2->y) * (v1->x - v2->x) + (v1->y - v2->y) * (v2->x - x)) / triangleArea;

			if (b1 >= 0 && b1 <= 1 && b2 >= 0 && b2 <= 1 && b3 >= 0 && b3 <= 1)
			{
				float depth = b1 * v1->z + b2 * v2->z + b3 * v3->z;
				int zIndex = y * width + x;
				if (zBuffer[zIndex] < depth)
				{
					*(line) = color.rgb();
					zBuffer[zIndex] = depth;
				}
			}
		}
	}
}

void MainWindow::redraw()
{
	QTime time;
	time.start();

	QImage image = QImage(width, height, QImage::Format_RGB32);
	image.fill(QColor(0, 0, 0));

	float y = horizontalBar->value() * M_PI / 180;
	float sine=qSin(y);
	float cosine=qCos(y);
	Mat4 yRotate;
	yRotate.identity();
	yRotate[0 + 4 * 0] = cosine;
	yRotate[2 + 4 * 0] = -sine;
	yRotate[0 + 4 * 2] = sine;
	yRotate[2 + 4 * 2] = cosine;

	float x = verticalBar->value() * M_PI / 180;
	sine=qSin(x);
	cosine=qCos(x);
	Mat4 xRotate;
	xRotate.identity();
	xRotate[1 + 4 * 1] = cosine;
	xRotate[2 + 4 * 1] = -sine;
	xRotate[1 + 4 * 2] = sine;
	xRotate[2 + 4 * 2] = cosine;

	Mat4 rotate = xRotate * yRotate;

	float zBuffer[width * height];
	for(int i = 0; i < width * height; i++)
	{
		zBuffer[i] = numeric_limits<int>::min();
	}

	#pragma omp parallel for num_threads(numCores)
	for(size_t i = 0; i < polygons.size(); i++)
	{
		Polygon polygon = polygons[i];
		Vec3 v1 = rotate * polygon.v1;
		v1 += Vec3(width / 2, height / 2);

		Vec3 v2 = rotate * polygon.v2;
		v2 += Vec3(width / 2, height / 2);

		Vec3 v3 = rotate * polygon.v3;
		v3 += Vec3(width / 2, height / 2);

		Vec3 norm = ((v2 - v1) ^ (v3 - v1)).normalize();

		float intensity = abs(norm.z);
		QColor color(intensity * 255, intensity * 255, intensity * 255);

		drawTriangle(&v1, &v2, &v3, color, &image, zBuffer);

	}
	drawArea->setPixmap(QPixmap::fromImage(image));
	cerr << "Время:" << time.elapsed() << endl;
}

void MainWindow::center()
{
	QRect qr = frameGeometry();
	QPoint cp = QDesktopWidget().availableGeometry().center();
	qr.moveCenter(cp);
	move(qr.topLeft());
}
