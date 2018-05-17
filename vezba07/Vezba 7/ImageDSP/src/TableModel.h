
#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QVariant>

class TableModel : public QAbstractTableModel {
Q_OBJECT

public:
	explicit TableModel(int columns, QObject *parent = 0);

	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	Qt::ItemFlags flags(const QModelIndex& index) const;
	bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole);
	bool insertRows(int position, int rows, const QModelIndex& index = QModelIndex());
	bool removeRows(int position, int rows, const QModelIndex& index = QModelIndex());


	void appendRow();
	void appendRow(const QVector<QVariant>& rowData);
	QVariant get(int row, int column) const;
	bool set(int row, int column, const QVariant& value);

private:
	int columns;
	QVector<QVector<QVariant> > table; // List of rows.
};

#endif // TABLEMODEL_H
