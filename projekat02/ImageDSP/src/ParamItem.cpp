
#include "ParamItem.h"
#include "ui_ParamItem.h"

#include <QDebug>

ParamItem::ParamItem(QWidget* parent)
	: ItemWidget(parent), ui(new Ui_ParamItem) {
	ui->setupUi(this);

	ui->CurrentValueEdit->setValidator(new QDoubleValidator());
	maxValue = ui->paramSlider->maximum();

	connect(
			ui->CurrentValueEdit,
			SIGNAL(editingFinished()),
			this,
			SLOT(currentEditEditingFinished()));
	connect(
			ui->paramSlider,
			SIGNAL(sliderReleased()),
			this,
			SLOT(paramSliderReleased()));
}

ItemWidget* ParamItem::clone(QWidget* parent) {
	return new ParamItem(parent);
}

void ParamItem::setIndex(const QModelIndex& index) {
	currentIndex = index;
	update();
}

void ParamItem::update() {
	double min = getDoubleField(1);
	double current = getDoubleField(2);
	double max = getDoubleField(3);
	Q_ASSERT(min <= max);
	Q_ASSERT(min <= current && current <= max);

	ui->paramName->setText(getStringField(0));

	ui->MinLabel->setText(QString::number(min));

	ui->CurrentValueEdit->setText(QString::number(current));

	ui->MaxLabel->setText(QString::number(max));

	int sliderCurrent = qRound64((current - min) / (max - min) * maxValue);
	Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= maxValue);
	ui->paramSlider->setValue(sliderCurrent);
}

double ParamItem::getDoubleField(int column) {
	const QAbstractItemModel* model = currentIndex.model();
	//TODO Handle errors.
	bool doubleOk;
	double d = model->data(
			model->index(currentIndex.row(), column, QModelIndex())).toDouble(
			&doubleOk);
	Q_ASSERT(doubleOk);
	return d;
}

QString ParamItem::getStringField(int column) {
	const QAbstractItemModel* model = currentIndex.model();
	//TODO Handle errors.
	QString s =
			model->data(
					model->index(currentIndex.row(), column, QModelIndex()))
					.toString();
	return s;
}

void ParamItem::setField(int column, double value) {
	QAbstractItemModel* model
		= const_cast<QAbstractItemModel*>(currentIndex.model());

	model->setData(
			model->index(currentIndex.row(), column, QModelIndex()), value);
}


void ParamItem::currentEditEditingFinished() {
	double min = getDoubleField(1);
	double current = ui->CurrentValueEdit->text().toDouble();
	double max = getDoubleField(3);

	if(current < min){
		current = min;
	}

	if(current > max){
		current = max;
	}

	// Snap it to slider resolution.
	int sliderCurrent = qRound64((current - min) / (max - min) * maxValue);
	current = double(sliderCurrent)/maxValue*(max - min) + min;

	setField(2, current);

	update();
}

void ParamItem::paramSliderReleased() {
	int sliderCurrent = ui->paramSlider->sliderPosition();
	Q_ASSERT(0 <= sliderCurrent && sliderCurrent <= maxValue);

	double min = getDoubleField(1);
	double max = getDoubleField(3);
	double current = double(sliderCurrent)/maxValue*(max - min) + min;

	setField(2, current);

	update();
}

///////////////////////////////////////////////////////////////////////////////
