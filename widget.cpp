#include "widget.h"
#include <QtMath>
#include <vector>
#include "matrix.h"

using namespace std;

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{
	width = 700;
	height = 700;
	initUI();

	model = new Model("african_head.obj");
}

void Widget::initUI()
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

	connect(verticalBar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
	horizontalLayout->addWidget(verticalBar);

	mainLayout->addLayout(horizontalLayout);

	horizontalBar = new QSlider(Qt::Horizontal);
	horizontalBar->setMinimum(0);
	horizontalBar->setMaximum(360);
	horizontalBar->setValue(180);

	connect(horizontalBar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
	mainLayout->addWidget(horizontalBar);

	setLayout(mainLayout);
	resize(width,height);
	center();
	show();
}


void Widget::paintEvent(QPaintEvent *)
{
	QImage image = QImage(width,height,QImage::Format_RGB32);
	image.fill(QColor(0,0,0));

	float heading = qDegreesToRadians((float)horizontalBar->value());
	float sine=qSin(heading);
	float cosine=qCos(heading);

	Matrix3 headingTransform(new float[9]{cosine,0,-sine,0,1,0,sine,0,cosine});

	float pitch = qDegreesToRadians((float)verticalBar->value());
	sine=qSin(pitch);
	cosine=qCos(pitch);

	Matrix3 pitchTransform(new float[9]{1,0,0,0,cosine,-sine,0,sine,cosine});

	Matrix3 transform = headingTransform.multiply(pitchTransform);


	float zBuffer[width*height];
	for(int i=0; i<width*height; i++)
	{
		zBuffer[i]=	numeric_limits<int>::min();
	}
	for(int i=0; i<model->nfaces(); i++)
	{
		Triangle face=model->face(i);
		Vertex v1 = face.v1;
		v1.scale(width/2);
		v1=transform.transform(v1);
		v1.translate(width/2,height/2);

		Vertex v2 = face.v2;
		v2.scale(width/2);
		v2=transform.transform(v2);
		v2.translate(width/2,height/2);

		Vertex v3 = face.v3;
		v3.scale(width/2);
		v3=transform.transform(v3);
		v3.translate(width/2,height/2);

		Vertex ab(v2.x-v1.x,v2.y-v1.y,v2.z-v1.z);
		Vertex ac(v3.x-v1.x,v3.y-v1.y,v3.z-v1.z);

		Vertex norm(ab.y * ac.z - ab.z * ac.y,
					ab.z * ac.x - ab.x * ac.z,
					ab.x * ac.y - ab.y * ac.x);

		float normalLength = sqrt(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
		norm.x /= normalLength;
		norm.y /= normalLength;
		norm.z /= normalLength;

		float angleCos = abs(norm.z);

		QColor color(angleCos*255,angleCos*255,angleCos*255);

		int minX = (int) qMax(0, qCeil(qMin(v1.x, qMin(v2.x, v3.x))));
		int maxX = (int) qMin(width - 1, qFloor(qMax(v1.x, qMax(v2.x, v3.x))));
		int minY = (int) qMax(0, qCeil(qMin(v1.y, qMin(v2.y, v3.y))));
		int maxY = (int) qMin(height - 1, qFloor(qMax(v1.y, qMax(v2.y, v3.y))));

		float triangleArea = (v1.y - v3.y) * (v2.x - v3.x) + (v2.y - v3.y) * (v3.x - v1.x);

		for (int y = minY; y <= maxY; y++)
		{
			for (int x = minX; x <= maxX; x++)
			{
				float b1 = ((y - v3.y) * (v2.x - v3.x) + (v2.y - v3.y) * (v3.x - x)) / triangleArea;
				float b2 = ((y - v1.y) * (v3.x - v1.x) + (v3.y - v1.y) * (v1.x - x)) / triangleArea;
				float b3 = ((y - v2.y) * (v1.x - v2.x) + (v1.y - v2.y) * (v2.x - x)) / triangleArea;
				if (b1 >= 0 && b1 <= 1 && b2 >= 0 && b2 <= 1 && b3 >= 0 && b3 <= 1)
				{
					float depth = b1*v1.z+b2*v2.z+b3*v3.z;
					int zIndex = y*width+x;
					if (zBuffer[zIndex]<depth)
					{
						image.setPixel(x,y,color.rgb());
						zBuffer[zIndex]=depth;
					}
				}
			}
		}
	}
	QPainter painter(this);
	painter.drawImage(0,0,image);
	painter.end();
}

void Widget::center()
{
	QRect qr = frameGeometry();
	QPoint cp = QDesktopWidget().availableGeometry().center();
	qr.moveCenter(cp);
	move(qr.topLeft());
}
