
#ifndef PROJECT_H_
#define PROJECT_H_

#include <QObject>
#include <QString>
#include <QModelIndex>
#include <QVector>

class QAbstractTableModel;
class TableModel;
class QSortFilterProxyModel;


class Project : public QObject {
	Q_OBJECT

public:
	explicit Project(const QString& fileName, QObject* parent = 0);
	~Project() {}

	QAbstractItemModel* getProgsModel() const;
	QAbstractItemModel* getOutImgsModel() const;
	QAbstractItemModel* getInImgsModel() const;
	QAbstractItemModel* getParamsModel() const;

	QImage* getCurrentOutImg() const;
	QImage* getCurrentInImg() const;

public slots:
	void save();
	void currentProgChanged(const QModelIndex& index);
	void currentInImgChanged(const QModelIndex& index);

private:
	QString fileName;

	TableModel* progsModel; // QString name
	int currentProg;

	TableModel* outImgsModel; // QString name, QImage* image
	int currentOutImg;
	TableModel* inImgsModel; // QString name, QImage* image
	int currentInImg;

	struct ProgSetup {
		// QString name, double min, int current, double max.
		TableModel* paramsModel;
	};
	QVector<ProgSetup> progSetups;

	QSortFilterProxyModel* paramsProxyModel;

	void appendProg(const QString& name);

private slots:
	void dataChanged(
			const QModelIndex& topLeft,
			const QModelIndex& bottomRight);
private:
	void imageProcessing();

signals:
	void imageProcessingDone();
};

#endif // PROJECT_H_
