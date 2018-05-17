#ifndef UNIVERSALITEMDELEGATE_H_
#define UNIVERSALITEMDELEGATE_H_

#include <QStyledItemDelegate>

#include "ItemWidget.h"

class UniversalItemDelegate : public QStyledItemDelegate {
Q_OBJECT

public:
	explicit UniversalItemDelegate(
			ItemWidget* view,
			ItemWidget* editor,
			QObject* parent = 0);
	~UniversalItemDelegate() {
	}

protected:
	QSize sizeHint(
			const QStyleOptionViewItem& option,
			const QModelIndex& index) const;
	void paint(
			QPainter* painter,
			const QStyleOptionViewItem& option,
			const QModelIndex& index) const;

	QWidget* createEditor(
			QWidget* parent,
			const QStyleOptionViewItem& option,
			const QModelIndex& index) const;
	void updateEditorGeometry(
			QWidget* editor,
			const QStyleOptionViewItem& option,
			const QModelIndex& index) const;
	void setEditorData(QWidget* editor, const QModelIndex& index) const;
	void setModelData(
			QWidget* editor,
			QAbstractItemModel* model,
			const QModelIndex& index) const;

private:
	mutable int editingRow;
	ItemWidget* view;
	ItemWidget* editor;
};

#endif // UNIVERSALITEMDELEGATE_H_
