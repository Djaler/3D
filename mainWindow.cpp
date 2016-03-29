#include "mainWindow.h"
#include <QtMath>
#include <vector>
#include <iostream>
#include "matrix.h"
#include <omp.h>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
	: QWidget(parent)
{
	width = 700;
	height = 700;
	initUI();

	numCores = omp_get_num_procs();

	initModel();
}

void MainWindow::initModel()
{
	image = QImage(width, height, QImage::Format_RGB32);

	parser = new Parser("../obj/diablo3_pose.obj", width/2);
	//parser = new Parser("../obj/Millennium_Falcon.obj", width/2);
	//parser = new Parser("../obj/reconstructed_head.obj");

	polygons = parser->polygons;
}

void MainWindow::initUI()
{
	QVBoxLayout *mainLayout = new QVBoxLayout();

	QHBoxLayout *horizontalLayout = new QHBoxLayout();

	drawArea = new QWidget();
	drawArea->show();
	horizontalLayout->addWidget(drawArea);

	verticalBar = new QSlider(Qt::Vertical);
	verticalBar->setMinimum(0);
	verticalBar->setMaximum(360);
	verticalBar->setValue(180);

	connect(verticalBar, SIGNAL(valueChanged(int)), this, SLOT(repaint()));
	horizontalLayout->addWidget(verticalBar);

	mainLayout->addLayout(horizontalLayout);

	horizontalBar = new QSlider(Qt::Horizontal);
	horizontalBar->setMinimum(0);
	horizontalBar->setMaximum(360);
	horizontalBar->setValue(180);

	connect(horizontalBar, SIGNAL(valueChanged(int)), this, SLOT(repaint()));
	mainLayout->addWidget(horizontalBar);

	setLayout(mainLayout);
	resize(width,height);
	center();
	show();
}

void MainWindow::paintEvent(QPaintEvent *)
{
	image.fill(QColor(0, 0, 0));

	float heading = qDegreesToRadians((float)horizontalBar->value());
	float sine=qSin(heading);
	float cosine=qCos(heading);
	Matrix headingTransform(new float[9]{cosine, 0, -sine, 0, 1, 0, sine, 0, cosine});

	float pitch = qDegreesToRadians((float)verticalBar->value());
	sine=qSin(pitch);
	cosine=qCos(pitch);
	Matrix pitchTransform(new float[9]{1, 0, 0, 0, cosine, -sine, 0, sine, cosine});

	Matrix transform = headingTransform.multiply(pitchTransform);

	float zBuffer[width * height];
	for(int i = 0; i < width * height; i++)
	{
		zBuffer[i] = numeric_limits<int>::min();
	}

	QTime time;
	time.start();

	#pragma omp parallel for num_threads(numCores)
	for(size_t i = 0; i < polygons.size(); i++)
	{
		Polygon polygon = polygons[i];
		Vertex v1 = polygon.v1.transformed(transform);
		v1.translate(width / 2, height / 2);

		Vertex v2 = polygon.v2.transformed(transform);
		v2.translate(width / 2, height / 2);

		Vertex v3 = polygon.v3.transformed(transform);
		v3.translate(width / 2,height / 2);

		Vertex norm = (v2 - v1) ^ (v3 - v1);

		norm.normalize();

		float intensity = abs(norm.z);

		QColor color(intensity * 255, intensity * 255, intensity * 255);

		int minX = qMax(0, qCeil(qMin(v1.x, qMin(v2.x, v3.x))));
		int maxX = qMin(width - 1, qFloor(qMax(v1.x, qMax(v2.x, v3.x))));
		int minY = qMax(0, qCeil(qMin(v1.y, qMin(v2.y, v3.y))));
		int maxY = qMin(height - 1, qFloor(qMax(v1.y, qMax(v2.y, v3.y))));

		float triangleArea = (v1.y - v3.y) * (v2.x - v3.x) + (v2.y - v3.y) * (v3.x - v1.x);

		QRgb *rowData = (QRgb*)image.scanLine(minY);
		int stride = image.bytesPerLine() / 4;
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
						#pragma omp critical
						{
							zBuffer[zIndex] = depth;
						}
					}
				}
			}
		}
	}
	cerr << "Время:" << time.elapsed() << endl;
	QPainter painter(this);
	painter.drawImage(0,0,image);
	painter.end();
}

void MainWindow::center()
{
	QRect qr = frameGeometry();
	QPoint cp = QDesktopWidget().availableGeometry().center();
	qr.moveCenter(cp);
	move(qr.topLeft());
}
