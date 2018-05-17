
#include "HoverEditTriggerListView.h"

HoverEditTriggerListView::HoverEditTriggerListView(QWidget* parent)
		: QListView(parent) {

	setMouseTracking(true);

	connect(
			this,
			SIGNAL(entered(QModelIndex)),
			this,
			SLOT(enteredEvent(QModelIndex)));

	setEditTriggers(QAbstractItemView::AllEditTriggers);
}

void HoverEditTriggerListView::enteredEvent(const QModelIndex& index) {

	// Do not allow to enter twice here with same index.
	if(currentIndex == index){
		return;
	}

	currentIndex = index;

	closeEditor();

	openPersistentEditor(index);
	editedIndex = index;
	setCurrentIndex(index);
}

void HoverEditTriggerListView::rowsAboutToBeRemoved(
		const QModelIndex& parent,
		int start,
		int end) {
	Q_UNUSED(parent);
	Q_UNUSED(start);
	Q_UNUSED(end);

	closeEditor();
}

void HoverEditTriggerListView::closeEditor() {
	if(editedIndex != QModelIndex()){
		commitData(indexWidget(editedIndex));
		closePersistentEditor(editedIndex);
		editedIndex = QModelIndex();
		setCurrentIndex(editedIndex);
	}
}

