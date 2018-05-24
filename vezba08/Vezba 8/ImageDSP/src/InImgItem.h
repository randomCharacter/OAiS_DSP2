#ifndef INIMGITEM_H_
#define INIMGITEM_H_

#include <QWidget>

// Forward declaration.
class Ui_InImgItem;

class InImgItem : public QWidget {
	Q_OBJECT

public:
	explicit InImgItem(QWidget* parent = 0);
	~InImgItem() {
	}

private:
	Ui_InImgItem* ui;
};

#endif // INIMGITEM_H_
