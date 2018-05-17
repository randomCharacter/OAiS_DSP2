
#include "TableModel.h"

TableModel::TableModel(int columns, QObject *parent)
		: QAbstractTableModel(parent), columns(columns) {
}

int TableModel::rowCount(const QModelIndex& parent) const {
	Q_UNUSED(parent);
	return table.size();
}

int TableModel::columnCount(const QModelIndex& parent) const {
	Q_UNUSED(parent);
	return columns;
}

QVariant TableModel::data(const QModelIndex& index, int role) const {
	if(!index.isValid()){
		return QVariant();
	}

	if(index.row() >= table.size() || index.row() < 0){
		return QVariant();
	}

	if(index.column() >= columns || index.column() < 0){
		return QVariant();
	}

	if(role != Qt::DisplayRole){
		return QVariant();
	}

	return table[index.row()][index.column()];
}

Qt::ItemFlags TableModel::flags(const QModelIndex& index) const {
	if(!index.isValid()){
		return Qt::ItemIsEnabled;
	}

	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

bool TableModel::setData(
		const QModelIndex& index,
		const QVariant& value,
		int role) {
	if(!index.isValid()){
		return false;
	}

	if(index.row() >= table.size() || index.row() < 0){
		return false;
	}

	if(index.column() >= columns || index.column() < 0){
		return false;
	}

	if(role != Qt::EditRole){
		return false;
	}

	table[index.row()][index.column()] = value;

	//TODO Does it should be done like this and does it include bounds.
	emit dataChanged(index, index);

	return true;
}


bool TableModel::insertRows(int position, int rows, const QModelIndex& index) {
	Q_UNUSED(index);
	beginInsertRows(QModelIndex(), position, position + rows - 1);

	for(int row = 0; row < rows; ++row){
		table.insert(position, QVector<QVariant>(columns));
	}

	endInsertRows();
	return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex& index) {
	Q_UNUSED(index);
	beginRemoveRows(QModelIndex(), position, position + rows - 1);

	table.remove(position, rows);

	endRemoveRows();
	return true;
}


void TableModel::appendRow() {
	appendRow(QVector<QVariant>(columns));
}

void TableModel::appendRow(const QVector<QVariant>& rowData) {
	int position = table.size();

	beginInsertRows(QModelIndex(), position, position);

	table.insert(position, rowData);

	endInsertRows();
}

QVariant TableModel::get(int row, int column) const {
	return data(index(row, column, QModelIndex()), Qt::DisplayRole);
}

bool TableModel::set(int row, int column, const QVariant& value) {
	return setData(index(row, column, QModelIndex()), value, Qt::EditRole);
}
