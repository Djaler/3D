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
	//parser = new Parser("../obj/diablo3_pose.obj", width/2);
	parser = new Parser("../obj/Millennium_Falcon.obj");
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
	horizontalBar->setMinimum(-180);
	horizontalBar->setMaximum(180);
	horizontalBar->setValue(0);

	connect(horizontalBar, SIGNAL(valueChanged(int)), this, SLOT(redraw()));
	mainLayout->addWidget(horizontalBar);

	setLayout(mainLayout);
	center();
	show();
}

void MainWindow::drawTriangle(Vec3 v1, Vec3 v2, Vec3 v3, QColor color, QImage *image, float *zBuffer)
{
	int minX = max(0.0f, ceil(min(v1.x, min(v2.x, v3.x))));
	int maxX = min(width - 1.0f, floor(max(v1.x, max(v2.x, v3.x))));
	int minY = max(0.0f, ceil(min(v1.y, min(v2.y, v3.y))));
	int maxY = min(height - 1.0f, floor(max(v1.y, max(v2.y, v3.y))));

	float triangleArea = (v1.y - v3.y) * (v2.x - v3.x) + (v2.y - v3.y) * (v3.x - v1.x);

	QRgb *rowData = (QRgb*)image->scanLine(minY);
	int stride = image->bytesPerLine() / 4;
	for (int y = minY; y <= maxY; y++, rowData += stride)
	{
		QRgb *line = rowData + minX;
		for (int x = minX; x <= maxX; x++, line++)
		{
			float b1 = ((y - v3.y) * (v2.x - v3.x) + (v2.y - v3.y) * (v3.x - x)) / triangleArea;
			float b2 = ((y - v1.y) * (v3.x - v1.x) + (v3.y - v1.y) * (v1.x - x)) / triangleArea;
			float b3 = ((y - v2.y) * (v1.x - v2.x) + (v1.y - v2.y) * (v2.x - x)) / triangleArea;

			if (b1 >= 0 && b1 <= 1 && b2 >= 0 && b2 <= 1 && b3 >= 0 && b3 <= 1)
			{
				float depth = b1 * v1.z + b2 * v2.z + b3 * v3.z;
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

Mat4 lookat(Vec4 eye, Vec4 center, Vec4 up) {
	Vec4 z = (center-eye).normalize();
	Vec4 x = (up ^ z);
	cerr<<x.x<<" "<<x.y<<" "<<x.z<<" "<<x.w<<endl;
	x = x.normalize();
	cerr<<x.x<<" "<<x.y<<" "<<x.z<<" "<<x.w<<endl;
	Vec4 y = (z ^ x);
	y=y.normalize();
	Mat4 orientation;
	orientation.identity();
	Mat4 translation;
	translation.identity();
	for (int i=0; i<3; i++)
	{
		orientation[0 + i * 4] = x[i];
		orientation[1 + i * 4] = y[i];
		orientation[2 + i * 4] = z[i];
		translation[i + 3 * 4] = -center[i];
	}
	Mat4 view = orientation * translation;
	return view;
}

void MainWindow::redraw()
{
	QTime time;
	time.start();

	QImage image = QImage(width, height, QImage::Format_RGB32);
	image.fill(QColor(0, 0, 0));

	Mat4 rotate = Mat4::rotate(verticalBar->value(),horizontalBar->value());

	Mat4 translate = Mat4::translate(width/2, height/2);

	float zBuffer[width * height];
	for(int i = 0; i < width * height; i++)
	{
		zBuffer[i] = numeric_limits<int>::min();
	}

	Vec4 eye(0,100,700,1);
	Vec4 center(0,0,0,1);

	Mat4 view  = lookat(eye, center, Vec4(0,1,0));

	Mat4 projection;
	projection.identity();
	projection[3 + 2 * 4] = -1/(eye-center).length();

	#pragma omp parallel for num_threads(numCores)
	for(size_t i = 0; i < polygons.size(); i++)
	{
		Polygon polygon = polygons[i];
		Vec4 v1 = Vec3ToVec4(polygon.v1);
		v1 = rotate * v1;
		v1 = view * v1;
		v1 = projection * v1;
		v1 = translate * v1;

		Vec4 v2 = Vec3ToVec4(polygon.v2);
		v2 = rotate * v2;
		v2 = view * v2;
		v2 = projection * v2;
		v2 = translate * v2;

		Vec4 v3 = Vec3ToVec4(polygon.v3);
		v3 = rotate * v3;
		v3 = view * v3;
		v3 = projection * v3;
		v3 = translate * v3;

		Vec4 norm = ((v2 - v1) ^ (v3 - v1)).normalize();

		float intensity = abs(norm.z);
		QColor color(intensity * 255, intensity * 255, intensity * 255);

		drawTriangle(Vec4ToVec3(v1), Vec4ToVec3(v2), Vec4ToVec3(v3), color, &image, zBuffer);

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
