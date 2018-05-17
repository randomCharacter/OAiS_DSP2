/**
 * @file HoverEditTriggerListView.h
 * @date Apr 22, 2016
 *
 * @author Milos Subotic <milos.subotic.sm@gmail.com>
 * @license MIT
 *
 * @brief
 *
 * @version 1.0
 * Changelog:
 * 1.0 - Initial version.
 *
 */

#ifndef HOVEREDITTRIGGERLISTVIEW_H_
#define HOVEREDITTRIGGERLISTVIEW_H_

#include <QListView>

class HoverEditTriggerListView : public QListView {
	Q_OBJECT
public:
	HoverEditTriggerListView(QWidget* parent = 0);
	~HoverEditTriggerListView(){}

private slots:
	void enteredEvent(const QModelIndex& index);

protected:
	void rowsAboutToBeRemoved(const QModelIndex& parent, int start, int end);

private:
	void closeEditor();


private:
	QModelIndex currentIndex;
	QModelIndex editedIndex;

};


#endif // HOVEREDITTRIGGERLISTVIEW_H_
