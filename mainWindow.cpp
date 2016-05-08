#include <omp.h>
#include "mainWindow.h"
#include "matrix.h"

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
	object = new Object("../obj/diablo3_pose.obj");
	//object = Object("../obj/Millennium_Falcon.obj");
	//object = new Object("../obj/reconstructed_head.obj");

	/*object.addVertex(Vec3(0,0,0));
	object.addVertex(Vec3(0,1,0));
	object.addVertex(Vec3(0,0,1));
	object.addVertex(Vec3(1,0,0));

	object.addPolygon(vector<size_t> {0,1,2});
	object.addPolygon(vector<size_t> {0,2,3});*/
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
	verticalBar->setMinimum(-180);
	verticalBar->setMaximum(180);
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
	fpsMeter->setGeometry(0,0,100,50);
	fpsMeter->setStyleSheet("QLabel { background-color : black; color : white; }");
	fpsMeter->show();

	setLayout(mainLayout);
	center();
	show();
}

void MainWindow::drawTriangle(Vec3 v1, Vec3 v2, Vec3 v3, QColor color, QImage *image, float *zBuffer, float translateX, float translateY)
{
	v1 += Vec3(translateX,translateY);
	v2 += Vec3(translateX,translateY);
	v3 += Vec3(translateX,translateY);

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
				if (zBuffer[zIndex] > depth)
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

	Mat4 rotate = Mat4::rotate(verticalBar->value(),horizontalBar->value());

	Mat4 scale = Mat4::scale(width/2,width/2,width/2);

	Mat4 translate = Mat4::identity();

	float zBuffer[width * height];
	for(int i = 0; i < width * height; i++)
	{
		zBuffer[i] = numeric_limits<int>::max();
	}

	Vec3 eye(0,0,500);
	Vec3 center(0,0,0);
	eye = rotate * eye;
	//cerr<<eye.x<<" "<<eye.y<<" "<<eye.z<<endl;

	Mat4 view = Mat4::lookAt(eye, center, Vec3(0,1,0));
	//view.print();
	//cerr<<endl;

	Mat4 projection;
	projection.identity();
	projection[3 + 2 * 4] = -1/(eye-center).length();

	Mat4 model = scale * translate;// * rotate;

	#pragma omp parallel for num_threads(numCores)
	for(size_t i = 0; i < object->polygonsCount(); i++)
	{
		Polygon polygon = object->polygon(i);
		Vec4 v1 = Vec3ToVec4(polygon.v1);
		v1 = model * v1;
		v1 = view * v1;
		//v1 = projection * v1;

		Vec4 v2 = Vec3ToVec4(polygon.v2);
		v2 = model * v2;
		v2 = view * v2;
		//v2 = projection * v2;

		Vec4 v3 = Vec3ToVec4(polygon.v3);
		//cerr<<v3.x<<" "<<v3.y<<" "<<v3.z<<" "<<v3.w<<endl;
		v3 = model * v3;
		//cerr<<v3.x<<" "<<v3.y<<" "<<v3.z<<" "<<v3.w<<endl;
		v3 = view * v3;
		//cerr<<v3.x<<" "<<v3.y<<" "<<v3.z<<" "<<v3.w<<endl;
		//v3 = projection * v3;
		//cerr<<v3.x<<" "<<v3.y<<" "<<v3.z<<" "<<v3.w<<endl;

		Vec3 v1_3 = Vec4ToVec3(v1);
		Vec3 v2_3 = Vec4ToVec3(v2);
		Vec3 v3_3 = Vec4ToVec3(v3);
		//cerr<<v3_3.x<<" "<<v3_3.y<<" "<<v3_3.z<<endl;
		//cerr<<endl;

		Vec3 norm = Vec3::normal(v1_3,v2_3,v3_3);

		float intensity = abs(norm.z);
		QColor color(intensity * 255, intensity * 255, intensity * 255);

		drawTriangle(v1_3, v2_3, v3_3, color, &image, zBuffer, width/2, height/2);

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
