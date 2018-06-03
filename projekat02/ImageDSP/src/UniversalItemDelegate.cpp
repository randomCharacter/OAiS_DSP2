#include "UniversalItemDelegate.h"

#include <QPainter>

#include <QDebug>

UniversalItemDelegate::UniversalItemDelegate(
		ItemWidget* view,
		ItemWidget* editor,
		QObject* parent)
		:
				QStyledItemDelegate(parent),
				editingRow(-1),
				view(view),
				editor(editor) {
}

QSize UniversalItemDelegate::sizeHint(
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const {
	Q_UNUSED(option);
	Q_UNUSED(index);

	if(editingRow == index.row()){
		//view->setIndex(index);
		return view->sizeHint();
	}else{
		//editor->setIndex(index);
		return editor->sizeHint();
	}
}

void UniversalItemDelegate::paint(
		QPainter* painter,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const {

	if(editingRow == index.row()){
		return;
	}

	view->setIndex(index);

	QStyleOptionViewItemV4 opt(option);
	initStyleOption(&opt, index);

	QPaintDevice* originalPainterDevice = painter->device();

	if(option.state & QStyle::State_Selected){
		painter->fillRect(option.rect, option.palette.highlight());
	}

	view->setGeometry(option.rect);

	painter->end();

	view->render(
			painter->device(),
			option.rect.topLeft(),
			QRegion(0, 0, option.rect.width(), option.rect.height()),
			QWidget::DrawChildren);

	painter->begin(originalPainterDevice);
}

QWidget* UniversalItemDelegate::createEditor(
		QWidget* parent,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const {
	Q_UNUSED(option);

	editingRow = index.row();

	ItemWidget* newEditor = editor->clone(parent);
	newEditor->setIndex(index);
	return newEditor;
}

void UniversalItemDelegate::updateEditorGeometry(
		QWidget* editor,
		const QStyleOptionViewItem& option,
		const QModelIndex& index) const {
	Q_UNUSED(index);
	editor->setGeometry(option.rect);
}

void UniversalItemDelegate::setEditorData(
		QWidget* editor,
		const QModelIndex& index) const {
	Q_UNUSED(editor);
	Q_UNUSED(index);
	//dynamic_cast<ItemWidget*>(editor)->setIndex(index);
}

void UniversalItemDelegate::setModelData(
		QWidget* editor,
		QAbstractItemModel* model,
		const QModelIndex& index) const {
	Q_UNUSED(editor);
	Q_UNUSED(model);

	editingRow = -1;
}

