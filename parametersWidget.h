#pragma once

#include <QtWidgets>
#include <iostream>

class ParametersWidget : public QDialog
{
		Q_OBJECT

	float *_width, *_height, *recessWidth, *recessHeight;
	float *ledgeWidth, *ledgeHeight, *pyramidHeight;
	float *lanternRadius, *lanternHeight;
	float *edgeWidth, *edgeHeight;
	float *septumWidth; int *septumCount;

	QLineEdit *widthEdit, *heightEdit, *recessWidthEdit, *recessHeightEdit;
	QLineEdit *ledgeWidthEdit, *ledgeHeightEdit, *pyramidHeightEdit;
	QLineEdit *lanternRadiusEdit, *lanternHeightEdit;
	QLineEdit *edgeWidthEdit, *edgeHeightEdit;
	QLineEdit *septumWidthEdit, *septumCountEdit;

	public:
		ParametersWidget(float *width, float *height, float *recessWidth,float *recessHeight,
				  float *ledgeWidth, float *ledgeHeight, float *pyramidHeight,float *lanternRadius,
				  float *lanternHeight, float *edgeWidth,float *edgeHeight, float *septumWidth,
				  int *septumCount, QWidget *parent = 0) : QDialog(parent)
		{
			this->_width = width;
			this->_height = height;
			this->recessWidth = recessWidth;
			this->recessHeight = recessHeight;
			this->ledgeWidth = ledgeWidth;
			this->ledgeHeight = ledgeHeight;
			this->pyramidHeight = pyramidHeight;
			this->lanternRadius = lanternRadius;
			this->lanternHeight = lanternHeight;
			this->edgeWidth = edgeWidth;
			this->edgeHeight = edgeHeight;
			this->septumWidth = septumWidth;
			this->septumCount = septumCount;
			initUI();
		}

	private:
		void initUI()
		{
			QGridLayout *mainLayout = new QGridLayout();

			mainLayout->addWidget(new QLabel("Ширина будки"), 0, 0);
			widthEdit = new QLineEdit(QString::number(*_width));
			mainLayout->addWidget(widthEdit, 0, 1);

			mainLayout->addWidget(new QLabel("Высота будки"), 1, 0);
			heightEdit = new QLineEdit(QString::number(*_height));
			mainLayout->addWidget(heightEdit, 1, 1);

			mainLayout->addWidget(new QLabel("Ширина углубления"), 2, 0);
			recessWidthEdit = new QLineEdit(QString::number(*recessWidth));
			mainLayout->addWidget(recessWidthEdit, 2, 1);

			mainLayout->addWidget(new QLabel("Высота углубления"), 3, 0);
			recessHeightEdit = new QLineEdit(QString::number(*recessHeight));
			mainLayout->addWidget(recessHeightEdit, 3, 1);

			mainLayout->addWidget(new QLabel("Ширина верхнего выступа"), 4, 0);
			ledgeWidthEdit = new QLineEdit(QString::number(*ledgeWidth));
			mainLayout->addWidget(ledgeWidthEdit, 4, 1);

			mainLayout->addWidget(new QLabel("Высота верхнего выступа"), 5, 0);
			ledgeHeightEdit = new QLineEdit(QString::number(*ledgeHeight));
			mainLayout->addWidget(ledgeHeightEdit, 5, 1);

			mainLayout->addWidget(new QLabel("Высота пирамидального основания фонаря"), 6, 0);
			pyramidHeightEdit = new QLineEdit(QString::number(*pyramidHeight));
			mainLayout->addWidget(pyramidHeightEdit, 6, 1);

			mainLayout->addWidget(new QLabel("Радиус фонаря"), 7, 0);
			lanternRadiusEdit = new QLineEdit(QString::number(*lanternRadius));
			mainLayout->addWidget(lanternRadiusEdit, 7, 1);

			mainLayout->addWidget(new QLabel("Высота фонаря"), 8, 0);
			lanternHeightEdit = new QLineEdit(QString::number(*lanternHeight));
			mainLayout->addWidget(lanternHeightEdit, 8, 1);

			mainLayout->addWidget(new QLabel("Ширина нижней кромки"), 9, 0);
			edgeWidthEdit = new QLineEdit(QString::number(*edgeWidth));
			mainLayout->addWidget(edgeWidthEdit, 9, 1);

			mainLayout->addWidget(new QLabel("Высота нижней кромки"), 10, 0);
			edgeHeightEdit = new QLineEdit(QString::number(*edgeHeight));
			mainLayout->addWidget(edgeHeightEdit, 10, 1);

			mainLayout->addWidget(new QLabel("Ширина горизонтальных разделителей"), 11, 0);
			septumWidthEdit = new QLineEdit(QString::number(*septumWidth));
			mainLayout->addWidget(septumWidthEdit, 11, 1);

			mainLayout->addWidget(new QLabel("Количество горизонтальных разделителей"), 12, 0);
			septumCountEdit = new QLineEdit(QString::number(*septumCount));
			mainLayout->addWidget(septumCountEdit, 12, 1);

			QPushButton *applyButton = new QPushButton("Применить");
			connect(applyButton, SIGNAL(pressed()), this, SLOT(apply()));
			mainLayout->addWidget(applyButton, 13, 0, 1, 2);
			setLayout(mainLayout);
		}

	private slots:
		void apply()
		{
			*_width = widthEdit->text().toFloat();
			*_height = heightEdit->text().toFloat();
			*recessWidth = recessWidthEdit->text().toFloat();
			*recessHeight = recessHeightEdit->text().toFloat();
			*ledgeWidth = ledgeWidthEdit->text().toFloat();
			*ledgeHeight = ledgeHeightEdit->text().toFloat();
			*pyramidHeight = pyramidHeightEdit->text().toFloat();
			*lanternRadius = lanternRadiusEdit->text().toFloat();
			*lanternHeight = lanternHeightEdit->text().toFloat();
			*edgeWidth = edgeWidthEdit->text().toFloat();
			*edgeHeight = edgeHeightEdit->text().toFloat();
			*septumWidth = septumWidthEdit->text().toFloat();
			*septumCount = septumCountEdit->text().toFloat();

			if(*_width <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Ширина должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*_height <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Высота должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*recessWidth <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Ширина углубления должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*recessWidth >= *_width)
			{
				QMessageBox::warning(this, "Ошибка", "Ширина углубления должна меньше ширины будки", QMessageBox::Ok);
				return;
			}
			if(*recessHeight <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Высота углубления должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*recessHeight >= *_height)
			{
				QMessageBox::warning(this, "Ошибка", "Высота углубления должна меньше ширины будки", QMessageBox::Ok);
				return;
			}
			if(*ledgeWidth <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Ширина верхнего выступа должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*ledgeWidth >= *_width)
			{
				QMessageBox::warning(this, "Ошибка", "Ширина верхнего выступа должна меньше ширины будки", QMessageBox::Ok);
				return;
			}
			if(*ledgeHeight <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Высота верхнего выступа должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*pyramidHeight <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Высота пирамидального основания должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*lanternRadius <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Радиус фонаря должен быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*lanternRadius >= *_width)
			{
				QMessageBox::warning(this, "Ошибка", "Радиус фонаря должен быть меньше ширины будки", QMessageBox::Ok);
				return;
			}
			if(*edgeWidth <= *_width)
			{
				QMessageBox::warning(this, "Ошибка", "Ширина нижней кромки должна быть больше ширины будки", QMessageBox::Ok);
				return;
			}
			if(*edgeHeight <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Высота нижней кромки должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*septumWidth <= 0)
			{
				QMessageBox::warning(this, "Ошибка", "Ширина горизонтальных разделителей должна быть положительным числом", QMessageBox::Ok);
				return;
			}
			if(*septumCount < 0)
			{
				QMessageBox::warning(this, "Ошибка", "Количество горизонтальных разделителей не должно быть отрицательным числом", QMessageBox::Ok);
				return;
			}
			if((*septumWidth * *septumCount) >= *_height)
			{
				QMessageBox::warning(this, "Ошибка", "Общая ширина горизонтальных разделителей не должна превышать высоту будки", QMessageBox::Ok);
				return;
			}
			close();
		}
};
