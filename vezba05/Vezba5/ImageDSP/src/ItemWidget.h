
#ifndef ITEMWIDGET_H_
#define ITEMWIDGET_H_

#include <QWidget>
#include <QModelIndex>

class ItemWidget : public QWidget {
	Q_OBJECT

public:
	ItemWidget(QWidget* parent = 0);
	virtual ~ItemWidget(){}

	virtual ItemWidget* clone(QWidget* parent) = 0;
	virtual void setIndex(const QModelIndex& index) = 0;

};

#endif // ITEMWIDGET_H_
